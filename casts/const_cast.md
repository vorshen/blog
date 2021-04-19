# const_cast
const_cast 可以移除掉常量特性，但是这里并不是说让一个 const 变量变成非 const（这是不可能的），而是**让引用 const 常量或者指针变成非常量的**，下面举个栗子。
```
int main(int argc, char* argv[]) {
    int a = 0;
    const int& ref1 = a;
    // 如下报错，无法修改常量引用
    // ref1 = 3; 

    const_cast<int&>(ref1) = 1;
    printf("a: %d\n", a);

    const int b = 0;
    const int& ref2 = b;

    const_cast<int&>(ref2) = 1;
    printf("b: %d\n", b);
}

```
这里用引用为例，ref1 本身是一个 int 常量引用，如果我们直接修改编译器会报错，所以我们可以通过 const_cast 进行转换。  

上面的代码有一个特点是 a 本身是非 const，我们的赋值操作可以真实修改 a 的值；但是 b 本身是 const 的，赋值操作无法真正应用到 b 上。*（有点绕，推荐一定要看明白。）*  
注意：可能有同学好奇这里是怎么做到的，是否是有一个新的非 const 对象出现了？我们从代码上来看并没有:
```
class Yori {
public:
    Yori() {
        printf("Constructor Yori\n");
    }

    void Add() {
        val_++;
    }
    void Add() const {

    }

    int Value() const {
        return val_;
    }
private:
    int val_ = 0;
};

int main(int argc, char* argv[]) {
    const Yori y1;
    Yori& y2 = const_cast<Yori&>(y1);
    y2.Add(); // 执行了 const 的 Add
    printf("y1, value: %d, address: %p\n", y1.Value(), &y1);
    printf("y2, value: %d, address: %p\n", y2.Value(), &y2);
}

// 输出
Constructor Yori
y1, value: 1, address: 0x7ffdee7d5424
y2, value: 1, address: 0x7ffdee7d5424
```
无论是构造函数的执行还是地址的情况，都能发现其实只有一个 Yori 对象构造，之所以可以短暂的突破 const 限制，只能高呼编译器 yyds，这里就不详细展开了。  