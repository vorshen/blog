# 命名空间
namespace 其实也比较简单，这里简单记录下。

## 关于 namespace 的思考
代码一定是由各个 Package 所组成的，好处大家自然都知道。  
伴随着功能越来越复杂，Package 肯定也会越多，所以任何变成语言，都逃不过如何使用 Package 这个内容。  
可以把编程语言对于 Package 的处理看成两种:  

### 文件编译自带隔离
这里用 go 举例子好了，因为 go 算是做的比较好的。可以看如下的例子:  
```
///////// test/src/pkg1/index.go
package pkg1

import (
    "fmt"
)

func Echo() {
    fmt.Println("pkg1 echo")
}
//////////////////////////

///////// test/src/pkg2/index.go
package pkg2

import (
    "fmt"
)

func Echo() {
    fmt.Println("pkg2 echo")
}
//////////////////////////

///////// test/src/main.go
package main

import (
    "test/pkg1"
    "test/pkg2"
)

func main() {
    pkg1.Echo()
    pkg2.Echo()
}
//////////////////////////

```
同名函数 Echo 通过 文件 + package 天然分隔开了。  

### 文件不带隔离
这里用 C++ 举例子，哪怕没有指明使用，也会出现异常，编译阶段就把你安排的明明白白。
```
///////// test/src/pkg1.h
#pragma once

#include <iostream>

void echo() {
    std::cout << "pkg1 echo" << std::endl;
}
//////////////////////////

///////// test/src/pkg2.h
#pragma once

#include <iostream>

void echo() {
    std::cout << "pkg2 echo" << std::endl;
}
//////////////////////////

///////// test/src/main.cc
#include "./pkg1.h"
#include "./pkg2.h"

int main() {
    // echo ?????
}
//////////////////////////

// 报错
In file included from main.cc:2:
./pkg2.h:5:6: error: redefinition of ‘void echo()’
 void echo() {
      ^~~~
In file included from main.cc:1:
./pkg1.h:5:6: note: ‘void echo()’ previously defined here
 void echo() {
      ^~~~
```
所以这里我们就需要本文主角 namespace 了，上面的代码大概改动如下，pkg2 同 pkg1。
```
///////// test/src/pkg1.h
#pragma once

#include <iostream>

namespace pkg1 {

void echo() {
    std::cout << "pkg1 echo" << std::endl;
}

}  // namespace pkg1
//////////////////////////

///////// test/src/main.cc
#include "./pkg1.h"
#include "./pkg2.h"

int main() {
    ::pkg1::echo();
    ::pkg2::echo();
}
//////////////////////////
```

这里多嘴提一句 javascript，**作为最灵活的一门语言，它的玩法就很花了**，靠浏览器的全局上下文一把梭，write once, WTF anywhere；也可以用 webpack 等编译成 modules 的模式。

基本上 namespace 很好理解，这里我们多提两个，内联 namespace 和 匿名 namespace。

### 匿名 namespace
一句话总结就是匿名 namespace 和 static 能力是一样的，目前来说也更推荐用匿名 namespace 来代替 static。  
对比 static 的优势的话有如下:  
1. 用匿名 namespace 可以有一组变量，而不用每一个都用 static 标识。
2. namespace 本身也支持嵌套。
3. static 不能修饰 class。（*这点感觉使用场景也比较有限，正常人谁会这样 class 的场景？*）
4. static 在 C 下的语义更加纯粹，只关注函数内修饰变量。（*这点倒是一般，因为 static 还有 C++ 的语义，没法做到绝对的纯粹*）

所以也推荐大家用匿名 namespace 代替 static。  
至于网上也有一些说匿名 namespace 缺点的，主要是 debug 的场景为主，目前也都有一些解决方案了，所以大胆的用吧~

### 内联 namespace
我们知道正常的 namespace 需要增加命名空间的写法来用，比如前面的_::pkg1::echo_，当然也可以通过 using 来优化。  
内联 namespace 可以省略这一步，比如如下代码:  
```
///////// test/src/foo.h
#pragma once

inline namespace Test {

struct Foo {
public:
    int a = 2;
};

}  // namespace Yori
//////////////////////////

///////// test/src/main.cc
#include <iostream>

#include "./foo.h"

int main() {
    Foo foo;
    std::cout << foo.a << std::endl;
}
//////////////////////////
```
上面的代码可以正常的编译执行，可以对比一下把 inline 关键字去掉，就会报错。  
其实结合 inline function 也能比较好理解 namespace 下的 inline 含义，展开就是了。

但是，我们仔细思考一下，那用 inline namespace 的意义在哪？所以这里关键是，什么场景会使用 inline namespace？

我接触有限，没有真正用过 inline namespace，但是我了解后知道，它比较适合一些大型的代码更新，具体可以参见这片文章，说的还是比较清晰的。  
[内联命名空间(inline namespace)](https://blog.csdn.net/craftsman1970/article/details/82872497)

但是说实话，就算有这种代码更新的场景，用内联命名空间可能也会有混乱的风险，需要万分谨慎。