# 函数重载与缺省参数
看到谷歌代码规范关于这块的学习整理。  
具体代码规范可以看，这里不再展开:  
[中文版本](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/functions/)  
[英文版本](https://google.github.io/styleguide/cppguide.html#Functions)  

## 函数重载的使用
一般来说，大家想到使用函数重载的可能是以下三种场景:  
1. **不同的参数 导致函数逻辑本身有差异**  
```cpp
struct Cmd1 {
  // ...
};

struct Cmd2 {
  // ...
};

struct Cmd3 {
  // ...
};

void DoSomething(const Cmd1& a, const Cmd2& b) {}
void DoSomething(const Cmd1& a, const Cmd3& b) {}
// ...
```
只要实现了 Cmd 之间两两参数的全部函数组合，上层就可以随意去调用。

2. **支持不同的参数类型**
```cpp
void HandleString(const string& str) {}
void HandleString(const char* text, size_t textlen) {}
void HandleString(const string_view& sv) {}
```
可以看出来这里是为了支持不同的字符串类型参数，可以通过 cpp 形式的 `string`，也可以通过 c 形式的 `char* + size` 的方式。  

**不过上面的代码也是有问题的**，因为当我们参数是字符串字面量的时候，编译器会报错，因为它也不清楚改选择哪一种函数重载。(一般来说更推荐使用 `string_view`)  

这里说一下函数重载在官方中的缺点:  
- 如果函数单靠不同的参数类型而重载(这意味着参数数量不变)，读者就得十分熟悉 C++ 五花八门的匹配规则，以了解匹配过程具体到底如何。
- 如果派生类只重载了某个函数的部分变体，继承语义就容易令人困惑。

针对第1点，匹配过程，直接上代码:  
```cpp
void HandleString(const std::string& str) {
  printf("const std::string& \n");
}

void HandleString(std::string&& str) {
  printf("std::string&& \n");
}

void HandleString(const std::string&& str) {
  printf("const std::string&& \n");
}

int main(int argc, char* argv[]) {
  HandleString("123");

  std::string s1("456");
  HandleString(s1);

  const std::string s2("789");
  HandleString(std::move(s2));
  return 0;
}

// 结果
// std::string&& 
// const std::string& 
// const std::string&&
```
至少我在读代码的时候，如果遇到了这种形式的函数重载，我会脑壳疼……

针对第2点，派生类只重载了某个函数的部分，会导致基类所有重载失效，我们看代码:  
```cpp
class Base {
 public:
  virtual void DoSomething(int t) {}
  virtual void DoSomething(const std::string& str) {}
};

class Derived : public Base {
 public:
  void DoSomething(int t) {}
};

int main(int argc, char* argv[]) {
  Derived d;
  d.DoSomething(1);
  d.DoSomething("123");  // 编译报错
  return 0;
}
```
这里虽然 Derived 只重载了 `int` 类型的虚函数，但是 DoSomething 会全部失效，如果不谨慎，就容易出现诡异的 bug。  
需要牢记: **派生类要么不覆盖基类重载函数的实例，覆盖就全部覆盖**。*（要么全都带走，要么一无所有）*

## 缺省参数的使用
缺省参数的场景稍微一致了些，主要是为了优化「有些函数一般情况下使用缺省参数，但有时需要又使用非缺省的参数」这样的场景，这时候用缺省参数的方式，**写法上很清晰**。  

然而在谷歌的代码规范中指明，一般情况下更推荐使用函数重载，那是为什么呢？  

因为**缺省参数**对比**函数重载**的的优势只有一个写法上清晰，但是它会带来如下的问题:  
- 缺省参数实际上是函数重载语义的另一种实现方式，所以上面说的函数重载的问题，缺省参数也有会存在。
- 虚函数调用的缺省参数取决于目标对象的静态类型。
- 缺省参数是在每个调用点都要进行重新求值的，这会造成生成的代码迅速膨胀。
- 缺省参数会干扰函数指针，导致函数签名与调用点的签名不一致。

这里重点说一下2，4点。  

先说虚函数调用缺省参数的，用一段代码为例。  
```cpp
class Base {
 public:
  virtual void DoSomething(int a = 0) {
    printf("Base DoSomething \n");
  }
  virtual void SayHi(const std::string& str = "hello world") {
    printf("Base Say: %s \n", str.c_str());
  }
};

class Derived : public Base {
 public:
  void DoSomething(int a) {
    printf("Derived DoSomething \n");
  }
  void SayHi(const std::string& str = "Yori drink") {
    printf("Derived Say: %s \n", str.c_str());
  }
};

int main(int argc, char* argv[]) {
  Derived derived;
  // 问题1
  // 派生类重新定义了虚函数 DoSomething，但是派生类上并没有使用缺省参数
  // 由于缺省参数本质上也是函数重载实现的（看函数重载的缺点2），所以寄！
  // derived.DoSomething(); // 编译会报错
  derived.SayHi();  // 可以正常执行

  std::shared_ptr<Base> pderived = std::make_shared<Derived>();
  // 问题2
  // 派生类重新定义了虚函数 SayHi，并且派生类复写了缺省参数
  // 但由于虚函数是动态绑定，缺省参数是静态绑定
  // 导致这里缺省参数仍然是 Base 中的
  pderived->SayHi();  // 结果 Derived Say: hello world
  return 0;
}
```
注释中写明了两个问题的情况，这里就不再展开说了，重点需要牢记问题的前因后果:  
**原因**: 虚函数是动态绑定的，而缺省参数是静态绑定的  
**结果**: 绝对不要重新定义继承而来的缺省参数值

再说一下缺省参数会干扰函数指针，继续上代码: 
```cpp
typedef void(*func)();

void DoSomething(int value = 3) {
  printf("DoSomething value: %d\n", value);
}

void Execute(func f) {
  f();
}

int main(int argc, char* argv[]) {
  DoSomething();  // 可以执行
  Execute(DoSomething);  // 编译报错
}
```
这里对于使用者来说 DoSomething 它的签名参数可以是空，也可以是 `int`，但是对于编译器来说，DoSomething 由于缺省参数的存在函数签名和 func 对不上。  

## 抉择
知道了函数重载和缺省参数的情况，缺省参数还是有很多风险的，不过人家也有读写更方便的优点。  
所以按谷歌的代码规范来说:  
1. 对于虚函数，明确禁止使用缺省参数。原因上面也解释了。
2. 对于缺省值不一样的情况，明确禁止使用缺省参数。比如`void f(int n = counter++);`，这里我相信无论是写代码的人，还是看代码的人，应该都不会认同这种写法吧……

除了上面两种情况，一般情况下还是建议用函数重载。但如果真的很喜欢缺省函数的话，只要能避开缺省参数的那些问题，还是可以使用的。
