# dynamic_cast
dynamic_cast 使用的场景没那么广泛，主要就是针对于**向下转型**。这里我们先回忆一下使用 static_cast 时候的向下转型。
```
class Base {
public:
    int a = 0;
    void say() {
        printf("Base\n");
    }
};

class Derived : public Base {
public:
    int b = 1;
    void say() {
        printf("Derived\n");
    }
};

int main(int argc, char* argv[]) {
    // 正常情况
    Derived d1;
    Base& b1 = d1;
    (static_cast<Derived*>(&b1))->say();
    printf("b: %d\n", (static_cast<Derived*>(&b1))->b);

    printf("-----------\n");
    // 骚起来了
    Base b2;
    (static_cast<Derived*>(&b2))->say();
    printf("b: %d\n", (static_cast<Derived*>(&b2))->b);

    return 0;
}

// 输出结果
Derived
b: 1
-----------
Derived
b: 0
```
对于正常情况来说，这也是大多数的使用场景，我只有一个指向基类的指针或者引用，但是我需要调用的是派生类的方法。  
但是，下面那种骚写法通过了编译，并且也输出了结果，这里就很诡异，因为 b2 并非一个真正的 Derived 类型，而我们居然可以按 Derived 来进行操作，这意味着无尽的风险。  
究其根本，其实是编译器并没有对这种转型进行严格的校验，当然，例子中的情况开发可以一眼看出异常，**但非良构的转型情况远不止如此**。
```
class Base {
public:
    int a = 0;
};

class Derived : public Base {
public:
    int b = 1;

    virtual void test() {
        printf("Test\n");
    }
};

int main(int argc, char* argv[]) {
    Base b2;
    (static_cast<Derived*>(&b2))->test();
    printf("b: %d\n", (static_cast<Derived*>(&b2))->b);

    return 0;
}

// 输出结果
Segmentation fault
```
注意 Derived 没有 final 关键字，可能也会是别人的基类，这时候骚写法直接就运行时报错了。  
具体报错本质是虚函数表导致，这里就不展开，后面关于继承相关的文章会详细说到。

简单总结一下就是: static_cast 无法帮我们真实 check 转型的对象真实性，那么就需要我们此篇的主角 dynamic_cast 登场了。  
先看一下上面有问题的 case 使用 dynamic_cast 的情况:  
```
#include <stdio.h>

class Base {
public:
    int a = 0;
};

class Derived : public Base {
public:
    int b = 1;

    virtual void test() {
        printf("Test\n");
    }
};

int main(int argc, char* argv[]) {
    Base b2;
    (dynamic_cast<Derived*>(&b2))->test();
    printf("b: %d\n", (dynamic_cast<Derived*>(&b2))->b);

    return 0;
}

// 编译结果
error: cannot dynamic_cast ‘& b2’ (of type ‘class Base*’) to type ‘class Derived*’ (source type is not polymorphic)
     (dynamic_cast<Derived*>(&b2))->test();
```
可以看到直接编译时就报错了，而不会在运行时带来未知性。

dynamic_cast 除了有这样形式的安全向下转型，还有侧向转型的能力，看如下代码:
```
class Base1 {
public:
    virtual ~Base1() {}
};

class Base2 {
public:
    virtual ~Base2() {}
};

class Derived : public Base1, public Base2 {

};

int main(int argc, char* argv[]) {
    Base1* b1 = new Derived();
    // 以下转型操作合法
    dynamic_cast<Base2*>(b1);
    return 0;
}
```
此时 static_cast 也是万万代替不了 dynamic_cast 的，cppreference 中针对这一现象的官方表述是:  
>>> 若 表达式 指向/指代最终派生对象的公开基类，而同时最终派生对象拥有 Derived 类型的无歧义公开基类，则转型结果指向/指代该 Derived。（“侧向转型（sidecast）”）  

最后总结一下:  
dynamic_cast 主要用作安全的向下转型和侧向转型，虽然使用点比较明确，但还是要尽量减少、避免使用。《Effective C++》中指出其可能有性能缺陷，更重要的是**转型这个行为本身就要尽量的在代码中减少出现**，通过有良好的设计来规避转型。  