# 类的构造函数
这篇简单记录一下关于类的构造函数。  

在正式开始之前，先简单的说一下编译器的默认行为，因为本文说的是构造函数，但有些类大家会不写构造函数，其实本质上是编译器帮你声明了。  
下面用 《Effective C++》 中的例子也表明一下:  
```cpp
////////////////// 真实编写的代码
class Empty {};

Empty e1;

Empty e2(e1);
e2 = e1;
///////////////////////////////

// 编译器帮你补充成如下的代码:  
class Empty {
public:
    Empty() { ... }
    Empty(const Empty& rhs) { ... }
    ~Empty() { ... }

    Empty& operator=(const Empty& rhs) { ... }
};
```

## 构造函数
构造函数是最基本的了，其实能说的也不多，主要需要注意如下几点吧:  
1. 使用初始化列表而非在构造函数中赋值。
2. 单参数构造函数加上 `explicit`。
3. 成员变量初始化的注意事项。

简单展开说一下:  
使用初始化列表代替赋值也算一个基本的常识了，不了解的同学看下下面这段代码应该就了解了:  
```cpp
class Foo {
public:
    Foo() {
        printf("Default Constructor\n");
    }

    Foo(const Foo& foo) {
        printf("Copy Constructor\n");
    }

    Foo& operator=(const Foo& foo) {
        printf("Copy Operator\n");
        return *this;
    }
};

class Yori1 {
public:
    Yori1(const Foo& foo): foo_(foo) {} // 直接列表初始化

private:
    Foo foo_;
};

class Yori2 {
public:
    Yori2(const Foo& foo) {
        foo_ = foo;
    }

private:
    Foo foo_;
};

int main(int argc, char* argv[]) {
    Foo foo;

    printf("Yori1 Run Log: \n");
    Yori1 y1(foo);
    printf("\n");

    printf("Yori2 Run Log: \n");
    Yori2 y2(foo);

    return 0;
}

// 结果
Default Constructor
Yori1 Run Log: 
Copy Constructor

Yori2 Run Log: 
Default Constructor
Copy Operator
```
可以看到 `Yori1` 是直接调用拷贝构造函数的，而 `Yori2` 是先走了默认构造函数，再进行了一次拷贝赋值。  
原因也很简单，因为在执行构造函数的时候，要保证类里面的成员变量都已经执行了构造。

单参数的构造函数要用 `explicit` 修饰，这个可以看 [explicit 介绍](../../../explicit/index.md) 中的介绍。

至于成员初始化的事项，我们只需要牢记在 **`class` 中的默认初始化，和函数中变量的默认模式化是一样的**:  
- 对于原生类型或者指针类型，它们不会被初始化，也就是它们会使用内存中的野值作为自己的值，直接使用有很大的风险。所以**原生类型必须要明确的初始化**。
- 对于对象类型，会调用其默认构造函数。

## 拷贝构造函数
拷贝构造函数需要拆开看，有两个关键词「拷贝」、「构造」，其中构造刚刚我们说了，所以这里重点讲一下拷贝。

拷贝理解起来很简单，就是一种 `clone` 的处理，把一个东西按同样的组成再整一份。  

很简单，而且也比 `clone` 这样的处理轻便很多，但这带来的问题就是，**拷贝容易被忽视**。  
很容易出现的情况就是:  
- 不应该支持拷贝或者有特定拷贝行为的对象，开发没有写，但是编译器默认给你整上了。
- 如果派生类的拷贝，别忘了基类。

编译器给你生成的代码，一般都是最基本的，我们看下面这个例子:  
```cpp
class Foo {};

class Yori {
public:
    Yori(const Foo& foo): foo_ptr(&foo) {}
    const Foo* foo_ptr;
};


int main(int argc, char* argv[]) {
    Foo foo;

    Yori y1(foo);
    Yori y2(y1);

    printf("Address: %p \n", y1.foo_ptr);
    printf("Address: %p \n", y2.foo_ptr);

    return 0;
}

// 结果
Address: 0x7fff682dc25f
Address: 0x7fff682dc25f
```
可以看到这里地址是一样的，也就意味着指针类型在**默认拷贝构造函数下，执行的是浅拷贝，它们会指向同一块内存**。  
不管你需要的是否如此，这种行为带来了安全隐患，可能会出现悬挂指针。所以如果存在指针类型的成员变量，要自己实现拷贝构造函数，避免编译器默认实现。  

派生类的拷贝不要忘记基类就很简单了，时刻牢记，**如果要拷贝一个对象，务必将对象中的所有东西都进行拷贝**。

刚刚我们说的是本身可以拷贝，还有可能是无法拷贝的对象，这里的代表就是 `unique_ptr` 指针。  
```cpp
class Foo {};

int main(int argc, char* argv[]) {
    std::unique_ptr<Foo> p = std::make_unique<Foo>();
    auto t = p;
    return 0;
}

// 编译报错
error: use of deleted function ‘std::unique_ptr<_Tp, _Dp>::unique_ptr(const std::unique_ptr<_Tp, _Dp>&)
```
一般来说我们有两种方式主动禁止拷贝:  
1. 将拷贝构造函数和拷贝运算符进行 `delete`（比如 `unique_ptr` 做法）。
2. 将拷贝构造函数和拷贝运算符进行 `private` 私有化（推荐可以有一个 `Uncopyable` 基类）。

## 移动构造函数
移动构造函数和拷贝有一点点像，「构造」就不说了，核心就是理解下「移动」的语义即可。移动的语义也就是移动，将某个元素对象直接交给另一个，不存在同时有两份。

所以 `unique_ptr` 支持移动。

其他的也没啥好说的，至于移动行为是否达到预期、如何禁止移动行为，这些都和上一节拷贝的处理一样，所以就不说了。

## 拷贝构造函数和移动构造函数的规范写法
拷贝构造函数
```cpp
class Foo {
public:
    Foo(const Foo& foo) {}
};
```
标准的拷贝构造函数实参必须要是引用，因为不是引用的话就出现了无限拷贝死循环。  
其次最好加上 const，因为避免修改到实参。

移动构造函数
```cpp
class Foo {
public:
    Foo(Foo&& foo) {}
};
```
标准的移动构造函数采用的是右值引用，这个没啥好说的。  
需要注意的是，移动构造函数不要用 `const` 修饰，因为移动构造函数，其实相当于是操作了实参（将实参内的东西进行了转移），所以实参不能是 `const` 的。
