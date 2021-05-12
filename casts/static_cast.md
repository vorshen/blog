# static_cast
好家伙，static_cast 用的可就多了，它是用来进行强迫隐式转换，可以这样理解，针对  
static_cast < *新类型* > ( **表达式** )  
只要表达式可以合法的解析为新类型，那就可以用 static_cast，下面举几个可能常用点的栗子。  

- int 转 double
```
int main(int argc, char* argv[]) {
    double t = 6.66;
    int i = static_cast<int>(t);
    printf("i: %d\n", i);
    return 0;
}
```
没啥好说的，不用 static_cast 也没问题，会走默认的隐式转换，但是用了 static_cast 语义好一些。

- enum 转 int
```
int main() {
    enum Drink { beer, wine };
    int yori = static_cast<int>(Drink::wine);

    printf("%d \n", yori);
    return 0;
}
```
也没啥好说的。

- 左值到右值
```
class Yori {
public:
    Yori(std::string v): value(v) {}

    std::string value;
};

int main(int argc, char* argv[]) {
    Yori yori("123");

    auto a = yori;
    auto b = static_cast<Yori&&>(yori);

    printf("yori address: %p\n", &yori);
    printf("yori value: %s\n", yori.value.c_str());

    printf("a address: %p\n", &a);
    printf("a value: %s\n", a.value.c_str());

    printf("b address: %p\n", &b);
    printf("b value: %s\n", b.value.c_str());

    return 0;
}
```
这里左值到右值的转换，就和调用了 std::move 是一样的。

- 向上转型
```
class Base {
public:
    int a = 0;

    virtual void out() {
        printf("This is Base out.\n");
        printf("a: %d\n", a);
    }
};

class Derive : public Base {
public:
    int a = 1;

    virtual void out() {
        printf("This is Derive out.\n");
        printf("a: %d\n", a);
    }
};

int main(int argc, char* argv[]) {
    Derive d1;
    static_cast<Base>(d1).out();
}
```
注意，这里虽然代码可以执行，但是理论上应该不会如此使用的场景，因为可以说该操作毫无意义，还会有风险。举例如下代码:  
```
class Base {
public:
    int a = 0;

    virtual void add() {
        a++;
        printf("This is Base add.\n");
    }
};

class Derive : public Base {
public:
    int a = 1;

    virtual void add() {
        a += 2;
        printf("This is Derive add.\n");
    }
};

int main(int argc, char* argv[]) {
    Derive d1;

    static_cast<Base>(d1).add();
    printf("a: %d\n", d1.a);
}

// 最终结果
This is Base add.
a: 1
```
如果按我们代码的想法，这里a不是+1就应该+2，但为什么没有发生变化呢？因为我们将 Derive 转型为 Base 的时候，编译器其实是生成了一个临时的对象。  
此时风险就很高了，你想调用的 no-const 方法，其实本质上并没有在你想执行的函数上执行。

- 引用、指针向下转型
```
class Base {
public:
    int a = 0;

    void out() {
        printf("This is Base out.\n");
    }
};

class Derive : public Base {
public:
    int a = 1;

    void out() {
        printf("This is Derive out.\n");
    }
};

int main(int argc, char* argv[]) {
    Derive d1;
    Base& b1 = d1;
    b1.out();
    static_cast<Derive&>(d1).out();
}

// 结果
This is Base out.
This is Derive out.
```
指针也是同理，也比较好理解。注意不能是 virtual 方法；直接用值类型来转换也会报错，比如下:
```
    Base b1;
    static_cast<Derive>(b1).out();
```

以上列举了几个常用的、有意思一点的场景，完整的 static_cast 使用，可以看 [https://zh.cppreference.com/w/cpp/language/static_cast](https://zh.cppreference.com/w/cpp/language/static_cast)。