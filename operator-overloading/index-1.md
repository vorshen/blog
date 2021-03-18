# 运算符重载-1
相比较 javascript 这种脚本语言，C++ 可以进行操作符重载，很酷，可以实现下面这样的效果:  
```
// 方案1，类成员函数重载运算符
class Yori {
public:
    Yori(int val): val_(val) {}

    int operator+(const Yori& other) {
        printf("operator+ \n");
        return val_ + other.val_;
    }
private:
    int val_ = 0;
};

// 方法2，非成员函数重载运算符
int operator+(const Yori& lhs, const Yori& rhs) {
    return lhs.val_ + rhs.val_; // 注意，需要 friend 声明一下，这里省略了
}

int main(int argc, char* argv[]) {
    int t = Yori(3) + Yori(4);
    printf("Result: %d \n", t);
    return 0;
}
```
直接让两个对象相加，so cool。当然，能做到的远不止于此。  
## 四则运算
四则运算整体来说比较简单，代码🌰就不贴在这了，可以看这里:  
[./code/example_1.cc](./code/example_1.cc)  
**需要注意的是，这里四则运算是有顺序的。**  
比如 A + B 的执行，编译器有两种选择
1. A 进行了 operator+ 成员函数的重载，优先级高
2. 该环境下，有 operator+ 的针对 A、B 的重载，优先级低  
如果你只重载了 B 的 operator+ 成员函数，是会报错的；还有一种办法，就是给 B 增加类型转换函数，比如:  
```
class Yori {
public:
    operator int() {
        // ...
    }
}
```
但如果是这样的代码遇到了如下的调用，依然会有问题:  
```
3 + Yori(1) // 正常
Yori(1) + 3 // 报错
```
因为此时编译器有两条路:  
1. 将 Yori(1) 进行转换函数，最后变成 int + int
2. 将 3 进行隐式的构造成 Yori，最后变成 Yori + Yori  
编译器无法帮你做决定，编译器决定报错。  
*给 Yori 构造函数增加 explicit 修饰符可以解决，具体可以看[这里](../explicit/index.md)*

## 位运算
代码也比较简单，可以看这里:  
[./code/example_2.cc](./code/example_2.cc)  
也没啥特别要说的，这里的取反是位取反，并非逻辑取反，并且因为取反**单个前缀运算符**，所以如果采用非成员变量的方式，注意一下参数就行。  
```
class Yori {
public:
    friend int operator~(const Yori& yori);
    Yori(int val): val_(val) {}
private:
    int val_;
};
// 采用非成员变量的方式
int operator~(const Yori& yori) {
    return ~yori.val_;
}
```

四则运算和位运算，都是支持带上=进行自赋值操作，比如这样:  
```
class Yori {
public:
    Yori(int val): val_(val) {}

    int operator&=(const Yori& yori) {
        val_ &= yori.val_;
        return val_;
    }

    int operator*=(const Yori& yori) {
        val_ *= yori.val_;
        return val_;
    }

    void operator+=(const Yori& yori) {
        val_ += yori.val_;
    }

    inline int val() {
        return val_;
    }
private:
    int val_;
};

int main(int argc, char* argv[]) {
    Yori y(1);
    y &= Yori(2);
    y += Yori(2);
    printf("Result: %d \n", y *= 2);

    return 0;
}
```
这里需要注意的是，基础类型中默认有返回值的，我们可以将其强行指定为 void 返回，不过不推荐如此。  

未完待续。