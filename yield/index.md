# 导语
最近看的代码 yield 比较多，上次看到这么多 function* 还是在 koa1 时代，脑子中满是 yield 和 next，而我自己用这个用的较少，就水个文章学习一下。

# Prerequisites
yield 英文直译有着「提供」、「退让」的意思，先了解直译，对后面内容理解有帮助。  

本文主要谈一些我的思想理解，不会详细描述 api 怎么用，所以完全不了解 yield 的同学，可以先看一下 api 使用文档稍微了解一下。  
- javascript yield [https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Operators/yield](https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Operators/yield)
- c++ yield [https://zh.cppreference.com/w/cpp/thread/yield](https://zh.cppreference.com/w/cpp/thread/yield)

# yield 在 javascript 中
我最早在项目中接触 yield 是因为项目用了 koa，但后面 koa2 将 yield 全面替换为 async、await，当时我以为 yield 是 async、await 的中间临时方案，或者说 async、await 是 yield 的语法糖，其实并不能这么简单的去看。

我们先看一下 yield 在 javascript 中的定义:  
**yield 关键字用来暂停和恢复一个生成器函数。**  
这句话里面有两个概念需要了解:  
1. 生成器函数
2. 暂停和恢复  

1. 
生成器函数就是 function*，（普通的 function 关键字后面增加了一个星号）。那它有什么用呢？  
**生成器函数在执行时能暂停，后面又能从暂停处继续执行。**  
好家伙，这直接让人想到协程了。不过我们不要去想着底层是如何实现的，还是把注意力放在暂停和恢复这两个行为上。  

2. 
在前端代码中，我们让程序暂停和恢复的次数非常多，最大的点就是发送请求，等待 ajax 返回。  
```
const response = await axios.get('/drink?id=yori');
// balabala 操作 response
```
当执行到如上代码的时候，需要发送 IO 请求，在 response 没有回来的时候，cpu 没啥事干，就去其他应用程序里打工去了，此时相当于整个函数执行变成了**暂停**状态，然后 response 回来，整个函数**恢复**执行。  

### yield 与异步
这样一看，首先 yield 能解决异步问题，我们可以写一个代码感受一下。  
```
function* main() {
    yield console.log("利利准备");
    yield* drink();
    yield console.log("利利喝不动了");
}

function* drink() {
    yield console.log("利利吨吨吨");
    yield new Promise(function(resolve, reject) {
        setTimeout(function() {
            console.log('过了 3s');
            resolve();
        }, 3000);
    });
}

function run(gen) { // 类似 co
    const t = gen.next();
    const { value, done } = t;
    if (done) {
        console.log('End');
        return;
    }
    
    if (value instanceof Promise) {
        value.then((e) => run(gen))
    } else {
        run(gen)
    }
}

const gen = main();
run(gen);
```
异步出现在 drink 函数里面那个封装成 Promise 的 setTimeout。问题来了！yield 本身和 Promise 并没有什么火花，**对于 yield 来说它只是把 Promise 当作一个普通的 expression**。  
此时，反倒是 yield 配套的 next 起到了关键性的作用，虽然需要我们自己调用 next 很繁琐，但这同是将操作权给了我们，可以创造无限可能。  

我们可以通过一个 if 条件，判断 yield 结果是否是一个 Promise，如果是 Promise，那就可以就地进行 then 等待，而并非立刻继续 next。  
*这便是 co 函数的核心，但真正的 co 函数还有很多细节，感兴趣同学自行查阅。*  

理解了上面这个示例代码，我们便知道了，**用 yield 来将异步回调函数的写法转为同步的能力，是一种取巧的方案，必须依赖 co 函数进行辅助**。所以相比较 async、await，yield 确实是一种临时方案，koa2 进行全面替换也无可厚非。   
*谁要是异步代码不用 async、await，用 yield，头都给捶烂！！*

# yield 与迭代  
上面一章已经说了，yield 本身并不是给异步用的，那 yield 有自己存在的价值么？当然还是有的！
从网上关于 function* 的示例，基本上都是作为迭代使用，比如下:  
```
function* getValue() {
    let list = [1, 2, 3, 4, 5];
    for (let i = 0; i < list.length; i++) {
        yield list[i];
    }
}

// 自己调用 next 的方式
const gen = getValue();
let t;
while (t = gen.next(), !t.done) {
    console.log(t.value);
}

// 采用 for of 的方式
for (t of getValue()) {
    console.log(t);
}
```
你可能觉得就这？？遍历数据我 for 循环不行么？当然可以，但是这违背了泛型编程的思想。  
数组的 for 循环是一种写法，链表的 for 循环是一种写法，二叉树的 for 循环也是一种写法，自定义的数据结构迭代又会是一种写法。  
*不过对于前端而言，也不一定有那么强烈的泛型编程场景，还是得结合实际考虑。*  

需要注意几点坑或者简易:  
1. 生成器一定要相互独立，切勿随意复用
```
function* getValue() {
    let list = [1, 2, 3, 4, 5];
    for (let i = 0; i < list.length; i++) {
        yield list[i];
    }
}

// 没问题，generator 都相互独立
const gen = getValue();
let t;
while (t = gen.next(), !t.done) {
    for (d of getValue()) {
        console.log(d);
    }
    console.log(t.value);
}

// 有问题
const gen = getValue();
let t;
while (t = gen.next(), !t.done) {
    for (d of gen) { // ⚠️这里用了同一个 gen
        console.log(d);
    }
    console.log(t.value);
}
```
这个不用多说，输出的结果一定不会是你想要的。  

2. 不要在迭代过程中修改迭代的元素
```
let list = [1, 2, 3, 4, 5];
for (let v of list) {
    console.log(v);
    if (Math.random() < 0.5) {
        list.unshift(6);
    }
}
```
看代码应该就能理解，会有元素被输出多次，而6这个元素永远不会出现。当然，如果你觉得自己可以控制好修改元素的位置，语法上并不会拒绝，但一定要记得，**v8 don't know what you want**。  

3. 避免上层对 next 的调用  
next 是一把双刃剑，它的灵活性让 yield 可以胜任很多骚操作（前面说的异步就是一种），但是我的建议是不要将这个东西给上层使用，请封装好，就像 co 函数，就像 for of。  

再回到最开始说的 yield 英文直译，可以理解「提供」和「退让」的语义在哪了么？

# yield 在 C++ 中
这一节就简单了，先直接上代码。  
```
void wait() {
    while (true) {
        this_thread::yield(); // 进行 yield 调用，让出 cpu
    }
}

void calc() {
    int d = 0;
    for (int n = 0; n < 10000; ++n) {
        for (int m = 0; m < 100000; ++m) {
            d += d * n * m;
        }
    }
}

int main(int argc, char* argv[]) {
    thread t1(wait);
    thread t2(calc);

    t1.detach();
    t2.join();

    return 0;
}
```
calc 是一个耗时的函数，在我机器上大概耗时 4s，此时如果我不在 wait 函数中加 yield 执行，**注意执行时 taskset 要设置成单 cpu 执行。**  
```
real    0m7.920s
user    0m7.916s
sys     0m0.000s
``` 
这里 user 的时间不止 4s，可以看出 while true 占据了不少 cpu 时间片。  
然后我们加上 yield 的后结果就不一样了:  
```
real    0m7.910s
user    0m5.012s
sys     0m2.896s
```
这里 user 时间下降到 5s，wait 函数线程占据的 cpu 时间明显下降，但是 sys 时间上升，因为我们通过 yield 将时间交还给了内核，所以可以看到 sys 的时间有所增长。  

再回到最开始说的 yield 英文直译，应该可以很轻易的理解「退让」语义在 C++ 中。

# 总结
javascript 中不要使用 yield 去处理异步，请老老实实的使用 async/await。  
当使用迭代场合的时候，提供了三点建议。

c++ 中 yield 比较简单，也没啥好总结的。

有问题什么的可以一起讨论～