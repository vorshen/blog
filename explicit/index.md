# explicit
`explicit` 是 C++11 新增的特性，这个关键字主要出现在函数的前面，英文直译是「显性的、明确的」，真实作用和直译也是完全一样。  

## 介绍
有明确的，当然也有不明确的，或者说含糊其辞的，在代码中主要指的是类型的不明确，比如下:
```
class Yori {
public:
    Yori(int val): val_(val) {}

    void add(const Yori& yori) {
        val_ += yori.val();
    }

    inline int val() const {
        return val_;
    }
private:
    int val_;
};

int main(int argc, char* argv[]) {
    Yori y1(2);
    Yori y2(3);

    y1.add(y2); // 明确对应上了参数类型
    y1.add(3); // 并没有明确对应上参数类型

    printf("%d\n", y1.val());

    return 0;
}
```
可以看到，这里第一次调用，参数类型是匹配的。但是第二次调用，只传入了一个 `int` 类型，也可以正常执行，因为编译器帮我们进行了隐式的类型转换，执行了 `Yori` 的构造函数，所以此时我们只需要给 `Yori` 的构造函数上加上 `explicit` 修饰符，就会编译报错。
```
class Yori {
public:
    explicit Yori(int val): val_(val) {} // 增加了 explicit
```
编译报错如下  
![](./assets/1.png)  

`explicit` 不仅仅可以用在构造函数上，还可以用在**类的类型转换函数上**  
**注意：_类的普通函数无法使用_**  
```
class Yori {
public:
    void add(int t) {
        val_ += t;
    }

    inline int val() const {
        return val_;
    }

    operator int() {
        return val_;
    }
private:
    int val_ = 1;
};

int main(int argc, char* argv[]) {
    Yori y;
    y.add(Yori()); // Yori() 当作参数的时候，会执行类型转换函数，转为 int

    return 0;
}
```
此时是正常的，如果我们增加 `explicit` 就不行了  
```
#include <stdio.h>

class Yori {
public:
    void add(int t) {
        val_ += t;
    }

    inline int val() const {
        return val_;
    }

    explicit operator int() {
        return val_;
    }
private:
    int val_ = 1;
};

int main(int argc, char* argv[]) {
    Yori y;
    y.add(Yori()); // 编译报错
    y.add(static_cast<int>(Yori())); // 显式强制类型转换可以

    return 0;
}
```
这样则会编译报错，报错如下:  
![](./assets/2.png)  

不过有一个特殊点需要注意的，就是当满足
1. 转换类型为 `bool` 时
2. 在各种条件语句中（`if else`，`for` 等逻辑运算符）  
可以不用显式的类型转换，如下:  
```
class Yori {
public:
    explicit operator bool() {
        return val_ > 0;
    }
private:
    int val_ = 1;
};

int main(int argc, char* argv[]) {
    Yori y;

    if (Yori()) {
        printf("正常执行\n");
    }

    return 0;
}
```

简单总结一下，也就是说:  
1. 针对类构造函数增加 `explicit` 修饰，并不是要求该构造函数的参数明确，而且**要求使用该 `class` 前必须进行明确的构造**。
2. 针对类转换函数增加 `explicit` 修饰，这样类不能隐式转换到对应的类型，必须显示（一些特例看上面）。

## 进一步理解
看了上面的介绍，我们来结合谷歌的代码规范 [https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/classes/#implicit-conversions](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/classes/#implicit-conversions) 深入的理解一下 `explicit`。

谷歌的代码规范是这样的:  
>不要定义隐式类型转换. 对于转换运算符和单参数构造函数, 请使用 explicit 关键字.

分析一下有三点信息:  
1. 不要定义隐式类型转换
2. 单参数构造函数请使用 `explicit`
3. 类型转换运算符请使用 `explicit`

首先第一点**不要定义隐式类型转换**，这个应该是比较好认知的，隐式带来方便的同时，可能伴随着风险。

单参数构造函数明确使用 `explicit` 是因为，一旦没有 `explicit` 可能就会被使用者无意进行了隐式类型转换。

类型转换运算符稍微复杂一点点，其实正常来说，一个类很少会有类型转换运算符。但是，`bool` 是一个比较特殊的存在，很多时候会直接用类的实例当作 `bool` 类型在条件判断中使用。  
但是 `bool` 本身是一种数值类型，**所以在一些计算类型的上下文下也会有效**，这可就操蛋了，比如如下的例子:  
```
class Yori {
public:
    void Add() {
        value_++;
    }

    void Reduce() {
        value_--;
    }

    operator bool() {
        return value_ > 0;
    }
private:
    int value_ = 0;
};

int main(int argc, char* argv[]) {
    Yori y;
    y.Add();
    y.Add();

    // 正确的使用方式
    if (y) {
        printf("Success\n");
    }

    // 被误用的场景
    y + 2;
}
```
虽然只提供了 `bool` 的类型转换，但是也可以做加法，但这里就属于非期望使用场景了。此时只需要给 `operator` 加上 `explicit` 就可以了，编译器可以直接给出报错。
> error: no match for ‘operator+’ (operand types are ‘Yori’ and ‘int’)

## 总结
`explicit` 是很重要的一个关键字，了解其作用，然后实际编码中就按谷歌的编码规范来即可~
