# 如何查看系统版本

## 查看内核版本
```
cat /proc/version
```
注意：容器中查看内核，显示的会是母机的信息。  
记得这个就够了，一般来说业务开发不怎么需要查看内核的情况。

## 查看系统版本
其实主要为了区分是 debian 系的还是 RedHat 系的。毕竟对于上层来说，它们是不同的包管理体系，会有一些兼容性的考虑。

1. 如果是 RedHat 系
```
cat /etc/redhat-release
```
RedHat 系在 etc 下会有对应的文件，里面是详细的版本，比如 **CentOS Linux release 8.2.2.2004 (Core)** 这种。

2. 如果是 Debian 系
```
cat /etc/debian_version
```
debian 系就没有上面那个文件，取而代之的会有一个 debian_version 文件。

3. lsb_release
```
lsb_release -a
```
LSB是Linux Standard Base的缩写，-a 意味着完整输出信息，下面是一个例子：
```
root@bf2fce72ef7d:/# lsb_release -a
No LSB modules are available.
Distributor ID:	Ubuntu
Description:	Ubuntu 20.04.2 LTS
Release:	20.04
Codename:	focal
```
虽然很好用，但是注意，可能某些系统没有 lsb_release，需要安装一下。
```
// RedHat
yum install -y redhat-lsb

// debian
apt-get install lsb_release
```