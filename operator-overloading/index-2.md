# 运算符重载-2
上一章最后有提到了**前缀运算符**，某些运算符比较特殊，既有前缀又有后缀，比如 ++，--
## 自增，自减运算
```
class Yori {
public:
    friend int operator++(const Yori& yori, int);
    Yori(int val): val_(val) {}

    int operator++() {
        // 前缀
        return ++val_;
    }

    int operator++(int) {
        // 后缀
        int temp = val_;
        val_++;
        return temp;
    }

    inline int val() const {
        return val_;
    }
private:
    int val_ = 0;
};

// 如果采用非成员变量的方式，后缀运算法也需要注意下
int operator++(const Yori& yori, int) {
    int temp = yori.val_;
    yori.val_++;
    return temp;
}

int main(int argc, char* argv[]) {
    Yori y1(1);
    Yori y2(1);

    printf("operator++ result: %d \n", y1++);
    printf("++operator result: %d \n", ++y2);
    return 0;
}
```
用非成员函数的方式，写法也是类似的，后缀运算法需要增加一个类型。  

## 重载()，使其成为函数对象
这个在 callable-object 一文中有讲解，想详细了解可以移步去[callable-object](../callable-object/index.md)。  

## 比较运算符
代码很简单，可以看这里:  
[./code/example_3.cc](./code/example_3.cc)  
C++20 支持了 <=> 运算法，但是需要 gcc10 以上，感兴趣的同学可以尝试一下。  

## 逻辑运算符
代码很简单，可以看这里:  
[./code/example_4.cc](./code/example_4.cc)  
不过这里需要注意一下，重载了 && 和 || 运算符的话，会失去**短路求值**的特性，可以看如下代码:  
```
class Yori {
public:
    friend int operator||(bool b, const Yori& other);
    Yori(int val): val_(val) {}
private:
    int val_ = 0;
};

bool test() {
    printf("test run \n");
    return true;
}

int operator||(bool b, const Yori& other) {
    printf("|| run \n");
    return b && other.val_;
}

int main(int argc, char* argv[]) {
    if (true || test()) {
        printf("Hello World \n");
    }

    if (true || Yori(1)) {
        printf("Hello World \n");
    }
    return 0;
}
```
main 里面有两个 if 判断，第一个就走了短路求值的特性，并没有执行 test 方法，就认为结果为 true。而第二个 if 条件命中了我们的重载逻辑，所以执行了 operator||。  

## 赋值运算符
```
class Yori {
public:
    Yori() = default;
    Yori(int val): val_(val) {}

    Yori& operator=(const Yori& other) {
        printf("copy assignment \n");
        if (this == &other) {
            return *this;
        }
        val_ = other.val_;

        return *this;
    }

    Yori& operator=(Yori&& other) noexcept {
        printf("move assignment \n");
        if (this == &other) {
            return *this;
        }
        val_ = other.val_;

        return *this;
    }
private:
    int val_ = 0;
};

int main(int argc, char* argv[]) {
    Yori y1(2);

    Yori y2;
    y2 = y1;
    y2 = Yori(3);
    
    return 0;
}
```
赋值运算符需要考虑是 copy 还是 move 语义，然后也不要忘记证同测试哦。  

最后还有一些**数组下标运算符**、**指针运算符**等，基本上处理起来也大差不差的，这里就不细说了。