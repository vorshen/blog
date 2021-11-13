# 结构化绑定
水一水关于结构化绑定的知识。作为 C++ 菜鸡一枚，最早接触这个还是在刷 leetcode 的时候。  
当时发现，诶，这种写法不是很像 js 里面的解构赋值吗，好用！

注意：结构化绑定是 C++17 的特性，gcc 版本记得跟上。

## 背景
C++ 没有 javascript 中的匿名 Object 类型变量，我们描述一个 Object 需要用一个 Struct 来包一下，比如如下代码:  
```
struct Result {
    int code;
    std::string reason;
};

Result GetResult() {
    return {0, "success"};
};

int main(int argc, char* argv[]) {
    auto result = GetResult();

    printf("code: %d\n", result.code);
    printf("reason: %s\n", result.reason.c_str());
}
```
在不讨论正规与否的前提下，其实还是**比较麻烦的，特别是当 Result 的复用程度低的时候**，可以看 javascript 的处理方式:  
```
function getResult() {
    return {
        code: 0,
        reason: 'success'
    };
}

const result = getResult();
console.log('code: ', result.code);
console.log('reason: ', result.reason);
```
可以看到 js 可以直接省略 Result 这样一个结构声明，其实 C++ 也能省略，可以用如下的方式:  
```
std::pair<int, string> GetResult() {
    return {0, "success"};
}

int main(int argc, char* argv[]) {
    auto result = GetResult();

    printf("code: %d\n", result.first);
    printf("reason: %s\n", result.second.c_str());
}
```
**其实这已经是一种解决方式了**，也不用质疑说 pair 只能有两个元素，pair 本身就是 tuple 的一种特殊情况（仅仅有两个元素）。 

但是这里一个比较致命的问题是，**first 和 second 的语义太差了，无法对应上 code 和 reason**。这才是 pair 的致命问题，怎么解决可以继续往下看，这里先不提。 

这一节我们通过了问题+例子，引出了一种解决方案：tuple，那我们来详细看一下。

*当然，也可以用类似 map 这样的 STL 来解决上面的问题，不过这里不如 pair 方便，所以我们就不提了。*

## tuple
我们把上一节 pair 的写法改成 tuple 来看一看:  
```
std::tuple<string, int, string> GetPerson() {
    return {
        "yori",
        28,
        "drink"
    };
}

int main(int argc, char* argv[]) {
    auto man = GetPerson();

    // 写法1
    printf("name: %s\n", std::get<0>(man).c_str());
    printf("age: %d\n", std::get<1>(man));
    printf("interest: %s\n", std::get<2>(man).c_str());

    // 写法2
    string name;
    int age;
    string interest;
    std::tie(name, age, interest) = GetPerson();
    printf("name: %s\n", name.c_str());
    printf("age: %d\n", age);
    printf("interest: %s\n", interest.c_str());
}
```
提供了两种写法，说实话两种都很有异味……并没有感觉比 pair 的 first、second 好到哪里去。  

这次再吐槽一下第二种，tie 这样的写法，提前把变量声明好，就意味着还需要执行默认的构造函数+无法 auto 自动推导，这谁顶得住？

那设想一下，能不能像 javascript 这般处理:  
```
function getPersonByObject() {
    return {
        name: 'yori',
        age: 28,
        interest: 'drink'
    };
}

function getPersonByArray() {
    return ['yori', 28, 'drink'];
}

// 只看语法，别管变量重复声明
const { name, age, interest } = getPersonByObject();
const [name, age, interest] = getPersonByArray();
```

当然是有的，这就要请出我们的主角 structured-binding 来了！

## structured-binding
直接上最终的写法:  
```
std::tuple<string, int, string> GetPerson() {
    return {
        "yori",
        28,
        "drink"
    };
};

int main(int argc, char* argv[]) {
    auto [name, age, interest] = GetPerson();
    printf("name: %s\n", name.c_str());
    printf("age: %d\n", age);
    printf("interest: %s\n", interest.c_str());
}
```
不多说，直接🛫️。  
*而且结构化绑定不仅仅可以只用在 tuple 上，也可以用在数组、struct 上都可以~*

看到 auto 这种写法的同学们，肯定立刻会想到可以用修饰符进行修饰，比如 const auto 之类的，我们来试一试。
```
struct Person {
    int age;
};

int main(int argc, char* argv[]) {
    Person y{28};

    const auto& [age1] = y;
    // 通过修改结构体，来影响到绑定后的值
    printf("age before: %d\n", age1);
    y.age = 29;
    printf("age after: %d\n", age1);

    printf("/////////////////\n");

    // 通过修改绑定后的值，来影响结构体
    auto& [age2] = y;
    printf("age before: %d\n", age2);
    age2 = 30;
    printf("age after: %d\n", age2);
}

// 结果
age before: 28
age after: 29
/////////////////
age before: 29
age after: 30
```
加上修饰符之后的结果和认知上一致，只有有引用，无论修改哪边，另一边都会有所变化。  

关于结构化绑定的基本上也就是这些内容啦，详细可见:  
[https://zh.cppreference.com/w/cpp/language/structured_binding](https://zh.cppreference.com/w/cpp/language/structured_binding)