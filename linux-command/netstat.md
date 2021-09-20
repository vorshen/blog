# netstat
一个很重要并且很常用的命令。

## debian 下没有 netstat 时
```
apt-get install net-tools
// 包含 arp, ifconfig, netstat, rarp, nameif, route
```

## 查看端口情况
```
netstat -atunp
```
查看端口时，主要用后面这些参数  
-a 显示所有连线中的 Socket  
-t tcp 协议  
-u udp 协议  
-n 直接使用IP地址  
-p 显示对应的程序