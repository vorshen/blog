# 转型 casts
来简单水一水转型相关的知识，转型一般来说可能用得少，但是一定都知道，我们先简单总结一下。

# 隐式转换
首先是隐式转换，编译器 yyds，这个也比较简单，场景有限，直接上代码。
```
int main(int argc, char* argv[]) {
    float a = 2.6;

    int result1 = a * 2;
    printf("result1: %d \n", result1);

    int result2 = (int)a * 2;
    printf("result2: %d \n", result2);
}

// 输出
result1: 5 
result2: 4
```
result1 的计算中，a 看作是 float，常量2也看作为 float，此时计算出来的结果是 5.2，然后再隐式转换为 int，舍弃了小数部分，结果为 5。  

result2 主要是作为一个对比，a 显式转换为了 int，舍弃了小数，后续就全部按 int 计算，答案为 4。

# 旧式转型
上一节已经提到了显式转换，所以本节和后面都说的是显式的情况。  

先说旧式转换，也就是上一节 result2 计算中采用了的方式。  
旧式转型主要指的式 C 风格的语法，比如这样的:  
```
(T)expression

T(expression)
```
主要就是括号位置的区别，旧式转型目前来说用的很少了，因为看起来语法简单，但是带来的问题就是不方便从语法层面判断转型动作。
```
double sum1(double a1, double a2) {
    return a1 + a2;
}

double sum2(double& a1, double a2) {
    a1 *= 2;
    return a1 + a2;
}

int main(int argc, char* argv[]) {
    int a = 2;
    double result1 = sum1((double)a, 2.2);
    printf("result1: %f\n", result1);

    const double b = 2;
    double result2 = sum2((double&)b, 2.2);
    printf("result2: %f\n", result2);
}

// 输出
result1: 4.200000
result2: 6.200000
```
result1 的计算中，我们将 int 显式的转换为了 double 类型，没什么好说的。  

result2 就比较特别了，我们这里 b 本身是一个 const double 类型，然后 sum2 的第一个参数类型是 double&，是一个引用类型，并且在函数体中有进行修改处理。  
如果直接 sum2(b, 2.2) 会报错的，所以我们进行了转型操作，就可以正常执行了。  

注意：Effective C++ 中，关于旧式转型提到了一个**对象生成**的场景，比如这样的代码:  
```
class Yori {
public:
    explicit Yori(int val): limit_(val) {
        printf("Yori \n");
    }

    int limit() const {
        return limit_;
    }
private:
    int limit_ = 0;
};

void drink(const Yori& y) {
    printf("drink %d \n", y.limit());
}

int main(int argc, char* argv[]) {
    drink(Yori(3)); // 转型？
}
```
这里我个人更倾向的理解是:  
并没有将 int 通过转型动作创建 Yori 对象，可以说是将 Yori 隐式转换为了 const Yori。  

# 新式转型
新式转型就是 C++ 的语法了，并且区分了四种类型，我们一一来说下。  
## ![const_cast](./const_cast.md)

## reinterpret_cast 
