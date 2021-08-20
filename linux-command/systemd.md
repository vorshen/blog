# systemd
最近 docker 里面 os 起 sshd service 的时候，出现了一个问题，如下:
```
[root@5593d4716fe5 /]# systemctl status sshd
System has not been booted with systemd as init system (PID 1). Can't operate.
Failed to connect to bus: Host is down
```
就针对这个问题的前因后果来整个文章记录一下。

## systemd 介绍
systemd 是一系列的工具合集，可以管理所有的系统资源，系统资源统称 Unit。  
Unit 文件统一了过去各种不同系统资源配置格式，例如服务的启/停、定时任务、设备自动挂载、网络配置、虚拟内存配置等。而 Systemd 通过不同的文件后缀来区分这些配置文件。  
- .automount：用于控制自动挂载文件系统，相当于 SysV-init 的 autofs 服务
- .device：对于 /dev 目录下的设备，主要用于定义设备之间的依赖关系
- .mount：定义系统结构层次中的一个挂载点，可以替代过去的 /etc/fstab 配置文件
- .path：用于监控指定目录或文件的变化，并触发其它 Unit 运行
- .scope：这种 Unit 文件不是用户创建的，而是 Systemd 运行时产生的，描述一些系统服务的分组信息
- **.service：封装守护进程的启动、停止、重启和重载操作，是最常见的一种 Unit 文件**
- .slice：用于表示一个 CGroup 的树，通常用户不会自己创建这样的 Unit 文件
- .snapshot：用于表示一个由 systemctl snapshot 命令创建的 Systemd Units 运行状态快照
- .socket：监控来自于系统或网络的数据消息，用于实现基于数据自动触发服务启动
- .swap：定义一个用户做虚拟内存的交换分区
- .target：用于对 Unit 文件进行逻辑分组，引导其它 Unit 的执行。它替代了 SysV-init 运行级别的作用，并提供更灵活的基于特定设备事件的启动方式
- .timer：用于配置在特定时间触发的任务，替代了 Crontab 的功能

## service 结构介绍
我们主要说一下 .service 后缀，它是最常见的一种 Unit 文件，并且也和本文开篇的问题相关。我们挑个 .service 看一下。  
```
[root@5593d4716fe5 /]# cat /usr/lib/systemd/system/sshd.service
[Unit]
Description=OpenSSH server daemon
Documentation=man:sshd(8) man:sshd_config(5)
After=network.target sshd-keygen.target
Wants=sshd-keygen.target

[Service]
Type=notify
EnvironmentFile=-/etc/crypto-policies/back-ends/opensshserver.config
EnvironmentFile=-/etc/sysconfig/sshd
ExecStart=/usr/sbin/sshd -D $OPTIONS $CRYPTO_POLICY
ExecReload=/bin/kill -HUP $MAINPID
KillMode=process
Restart=on-failure
RestartSec=42s

[Install]
WantedBy=multi-user.target
```
这里分成了三段，分别是[Unit], [Service], [Install]。  
Unit、Install 段不细说了，是所有 Unit 文件通用的，是文件的描述、依赖、启动方式等的说明。详细字段可以自信查阅。

Service 段比较重要，这里依次解析一下出现的字段:  
- Type：启动时的进程行为，notify 意味着当前服务启动完毕，会通知Systemd，再继续往下执行。
- EnvironmentFile：加载一个文件，文件中都是环境变量，比如下面出现的 $CRYPTO_POLICY。
- ExecStart：如何启动该服务。
- ExecReload：如何重启该服务。
- killMode：结束进程的方式，process 意味着仅杀死主进程。
- Restart：何种情况下 Systemd 会重启该服务，on-failure 表示非正常退出时（退出状态码非0），包括被信号终止和超时，才会重启。
- RestartSec：重启服务前，需要等待的时间。

## service 目录
systemd 相关，一般会存放在两个地方：
```
[root@5593d4716fe5 /]# find / -type d -wholename "*/systemd/system"
/usr/lib/systemd/system
/etc/systemd/system
```
这里有什么区别呢?  
**/usr/lib/systemd/system** 是系统或第三方软件安装时添加的 Unit 文件。  
**/etc/systemd/system** 是系统或用户自定义的 Unit 文件。  

/etc/systemd/system 优先级更高，并且如果想开机启动，就必须要将 Unit 文件链接到 /etc/systemd/system 中。

### systemctl
systemd 拥有一系列的工具集，这里说个我们经常用的 —— systemctl。

systemctl 用于检查和控制各种系统服务和资源的状态，它可以进行一些系统级别的操作:  
```
# 重启系统
sudo systemctl reboot

# 暂停系统
sudo systemctl suspend
```
而我们更多的是用其管理 service，就好比开篇的使用，那为什么在 docker 中会报错呢？

## systemd 进程
回看一下开篇的报错:  
> System has not been booted with systemd as init system (PID 1). Can't operate.

因为正常来说 systemd 进程必须是 PID1 的：
```
[vorshen@VM-4-48-centos ~]$ ps -ef
UID          PID    PPID  C STIME TTY          TIME CMD
root           1       0  0  2020 ?        00:50:16 /usr/lib/systemd/systemd --system --deserialize 17
```
上面这是正常的情况，而我们如果用 docker 启动 os，一般来说都会这样这些的:  
```
[vorshen@VM-4-48-centos ~]$ sudo docker run -it 830c7f085a77 /bin/bash
[root@d7b1de78d946 /]# ps -ef
UID          PID    PPID  C STIME TTY          TIME CMD
root           1       0  1 15:14 ?        00:00:00 /bin/bash
root          31       1  0 15:14 ?        00:00:00 ps -ef
```
并没有启动 systemd 进程，并且 PID1 也被占用了，所以就出现了上面的问题，那该如何解决呢？
```
[vorshen@VM-4-48-centos ~]$ sudo docker run -itd --privileged 830c7f085a77 /usr/lib/systemd/systemd
135e34357685e40852c4d8817515035719360554c9f3b7737c1d40be00254f74
[vorshen@VM-4-48-centos ~]$ sudo docker exec -it 135e34357685 /bin/bash
[root@135e34357685 /]# systemctl status sshd
● sshd.service - OpenSSH server daemon
   Loaded: loaded (/usr/lib/systemd/system/sshd.service; enabled; vendor preset: enabled)
   Active: inactive (dead)
     Docs: man:sshd(8)
           man:sshd_config(5)
```
其实就是注意一下改动点
1. 不能用 /bin/bash 启动 os，而要用 /usr/lib/systemd/systemd 或者 /usr/sbin/init  
2. 启动参数加 -d 和 --privileged  
3. exec 进入容器的时候，再用 /bin/bash 覆盖初始化的命令  

参考资料:  
[https://cloud.tencent.com/developer/article/1516125](https://cloud.tencent.com/developer/article/1516125)