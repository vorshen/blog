# 函数返回值那些事
最近看到了谷歌的 `C++` 代码规范关于函数输入参数和输出参数那里，和我们的代码规范略有不同，就来水一水相关的知识。

## 函数的输出
一般我们认为函数签名有三个部分组成，函数名、参数、返回值。  

所以提到函数的输出，一般都认为是返回值代表着输出，也确实如此。不过，我们也可以通过参数进行函数的输出，用 Javascript 举例，比如有一个类似的方法:  
```javascript
function calculate(data) {
    // ...
    // data.xxx = xxx
} 
```
`data` 是一个对象，传递的是引用，我们在一些逻辑之后修改了 `data` 的属性，对于外层来说，函数也算是起到了输出的作用。

不过可能有同学质疑，有些规范要求尽量不要去改变函数的输入，目的是**保证函数是无副作用的**。

对于这点，我的看法是在特定场景下认同，某些场合下并不赞同，主要原因有如下:  
1. **无副作用的函数并不代表是幂等的函数**  
因为函数可能不是纯函数，而是某个对象的方法。那哪怕不修改参数，里面可能（也大概率可能）会操作 `this`。  
所以无副作用只是让上一层的调用者放心传入的东西没有变化，并不是整个系统状态可以是无副作用的，这里如果理解不清楚可能会有使用偏差。

2. **可能会降低性能，并且外层调用的地方不好写**
如果我们都是走返回值的方式进行函数输出，意味着我们必须要花一部分开销进行拷贝返回，而更骚的是，可能在上层，我们还需要再进行一次拷贝:  
```javascript
function calculate(data) {
    const cloneData = clone(data);  // 第一次拷贝
    // ...
    return cloneData;
}

// 上下文中存在 data
const result = calculate(data);
Object.assign(data, result);  // 第二次拷贝
```
可以看到，为了保证参数不变，外层的代码可能也会写的很奇怪。

所以我认为不能把「修改参数」这个行为看作洪水猛兽，虽然要尽量避免，但是不能一棒子拍死。其实在浏览器中，我们很常见的阻止函数冒泡的行为，就是一种参数修改的行为:  
```javascript
dom.addEventListener('click', function(e) {
    // 只是通过封装避免直接修改属性，其实也是对参数进行了写行为
    e.stopPropagation();
});
```

包括各大 `node server` 中中间件的写法:  
```javascript
app.use(async (ctx, next) => {
    await next();
    ctx.response.type = 'xml';
    ctx.response.body = fs.createReadStream('really_large.xml');
});
```
所以是否要保证参数是否修改，还是要结合具体的场景来看待。

## 函数输出的规范
### 参数输出
先说通过参数进行输出的规范，谷歌推崇的是:  
> 输入参数是值参或 `const` 引用, 输出参数为指针. 

看谷歌的说法，主要是考虑到 C 中没有引用，所以就延续了输出参数用指针。

这点可选也可不选吧~毕竟我是觉得指针的操作成本比引用高一些，区分好 `const` 即可。

然后需要注意的一点是:  
**函数参数列中中，输入参数在前，输出参数在后。新增参数不要无脑往后加。**

这点对于可读性还是比较重要的。

### 返回值输出
返回值输出是绝大部分场景，其中也有一些需要注意的地方。

1. **多返回值**  
遇到多返回值的时候，一般有几种做法，可以有 `struct`、可以是 `tuple`。  
为了语义，使用 `struct`；追求方便，可以使用 `tuple`，但是请一定要用结构化绑定。

2. **返回值+返回状态**  
这里用 Go 的语法来举个例子:  
```go
func Test() (int, error) {
    return 0, nil
}

func Run() {
    code, err := Test()
    // ...
}
```
上面这种场景，也能认为是多返回值，只不过一个是真实结果，还有一个是返回状态。  
但其实这两种某些场景下是可以合并的，比如下面用 Javascript 来举例。

```javascript
function test() {
    if (!xxx) {
        return null;
    }

    // ...
    return { /* some data */ };
}
```
这种情况，只有一个返回值，但是它是通过类型来决定状态的，之前 C++ 不好处理这种情况，而我们有了 `std::optional` 就好搞了，看如下的例子:  
```cpp
struct Result {
  std::string msg;
};

std::tuple<bool, Result> Test1(int t) {
  Result result;
  if (t < 0) {
    return std::make_tuple(false, result);
  }

  result.msg = "some message";
  return std::make_tuple(true, result);
}

std::optional<Result> Test2(int t) {
  if (t < 0) {
    return std::nullopt;
  }

  Result result;
  result.msg = "some message";
  return std::make_optional<Result>(result);
} 

int main(int argc, char* argv[]) {
  // 写法1
  auto [status, result1] = Test1(1);
  if (status) {
    printf("Test1 %s \n", result1.msg.c_str());
  }

  // 写法2
  auto result2 = Test2(1);
  if (result2) {
    printf("Test2 %s \n", result2->msg.c_str());
  }
  return 0;
}
```
可以明显感觉到写法2有 Javascript 那味了，不管说好不好，至少很方便不是……