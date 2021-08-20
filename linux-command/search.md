水个文章总结一下 Linux 下的一些查询方式。  
主要是我用的不够多，不够熟练，每次需要用的时候都得现查，显得自己很憨，所以总结了一点自己常用的。  

# 查询文件 —— find

## 通过文件名查询
这个应该是用的场景很多，比如有的时候 nginx 没有装在默认的地方，想通过搜索 nginx.conf 找到目录。  
```
sudo find / -name nginx.conf
```
**注意**：-name 之后的名称，必须完全匹配，比如如果是 -name nginx.c，这样就找不到配置了。  
不过我们可以通过*来模糊匹配:
```
// 找到所有后缀是 .conf 的文件
sudo find / -name *.conf

// 找到所有文件名是 nginx 的文件
sudo find / -name nginx.*

// 别骚了
sudo find / -name *.*
```

## 通过文件类型查询
这里说的文件类型并不是什么 png 格式、txt 格式之类的**文件格式**，主要有以下几类:
>d: 目录
>c: 字型装置文件
>b: 区块装置文件
>p: 具名贮列
>f: 一般文件
>l: 符号连结
>s: socket

配合 type 用起来也很方便:
```
// 可以查询到 nginx 相关功能的目录都在哪些地方
sudo find / -type d -name nginx
```

## 通过时间查询
当我们需要查询最近一段时间修改的文件的时候可以用，有两个区分的唯独，一个是按时间、还有一个按读取还是修改:
>amin 分钟 读取
>atime 天 读取
>cmin 分钟 状态修改
>ctime 天 状态修改
>mmin 分钟 内容修改
>mtime 天 内容修改

- 当时间参数是 - 时，意味着查询的是最近这几天以内的。
- 当时间参数是 + 时，意味着查询的是最近这几天以外的。

```
// 查询 /var/log 目录下，10分钟以内的被读取过的文件
sudo find /var/log -amin -101

// 查询 /var/log 目录下，10分钟以前的被修改过的文件
sudo find /var/log -mmin +10
```

## 嵌套层级目录查询
我们前面说到可以查询目录，但如果想查询嵌套层级的目录，用 name + type 是不行的，会提示报错：  
```
root@bf2fce72ef7d:/# find / -type d -name "systemd/system"
find: warning: '-name' matches against basenames only, but the given pattern contains a directory separator ('/'), thus the expression will evaluate to false all the time.  Did you mean '-wholename'?
```
主要是 -name 不支持 directory separator，我们要用如下的方式：
```
root@bf2fce72ef7d:/# find / -type d -wholename "*/systemd/system"
/usr/lib/systemd/system
/etc/systemd/system
```

# 查询命令在哪 —— which
因为 which 查询的是 $PATH 路径下的各个目录，所以它可以用来查询我们全局执行的命令究竟来自哪里。  
比如一般的多 gcc 版本我们可能采用 devtoolset 来共存切换版本(docker 的方式更佳)，此时我们想确认 gcc 来自于哪里就可以:
```
which gcc
```
像我机器上就并不是系统自带的 /bin/gcc，而是 devtoolset 的路径。  

# 特定的查询文件 —— whereis
这节主要是配合上一节，上一节我们用了 which gcc 为例，可以看到全局下执行的 gcc 命令来自于哪里，哪怕有多个。但是当有多个情况下，如果我们把路径全部输出出来，那就得用 whereis 了:
```
whereis gcc

结果
gcc: /usr/bin/gcc /usr/lib/gcc /usr/libexec/gcc /opt/rh/devtoolset-8/root/usr/bin/gcc /usr/share/man/man1/gcc.1.gz
```
这里还查到了一个 /usr/share/man/man1/gcc.1.gz 结果，因为 whereis 不仅仅可以查找二进制执行文件，也可以查找 man 说明文件，当然我们可以通过参数指定类型。  
- -b 只查找二进制文件
- -m 只查找说明文件

# 查找文件内容 —— grep
刚刚我们说的，都是按文件名、或者按文件路径查询，用的还很多的，就是查找文件里面的内容，也就是 grep，比如查询一个服务是否开启:
```
ps -aux | grep nginx
```
grep 可以查找文件或标准输入流中匹配的内容，一般来说有如下常用的参数:
- -i 忽略大小写  
- -c 统计匹配的行数
- -E 采取正则的方式
- -C <number> 不仅仅展示匹配的行，并且额外展示前后 number 行，方便查看上下文
- -r 对目录采取递归检测