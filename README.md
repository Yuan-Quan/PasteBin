# 树梅派软路由 运维说明

系统 Ubuntu Server 22.04 (LTS)

## 访问

用户名: `nahida`  
HostName: `akasha`  

```sh
ssh nahida@192.168.220.1
```

clash控制面板yacd由clash host在了`192.168.220.1:9090/ui`  
用于更换节点, 切换模式等  

## 网络架构

低层后端是 NetworkManager  
主要通过 NetPlan 管理  

网线接口的静态ip是由netplan设置的  
`静态ip是: 192.168.220.1`  
netplan会自动连接校园网路WiFi

网线接口的DHCP服务是由dnsmasq提供  
`ip池与租期写在dnsmasq的配置文件里'  

> dnsmasq设置的上游服务器并非为clash的fake-ip服务器, 因为clash的tun会自动劫持DNS请求

clash会作为一个systemd服务启用, 运作在TUN Mode下, 创建utun网络设备.  

iptables创建一个NAT以及相应的规则, 将网线接口(eth0)與utun桥接起来, 将流量交给clash  

iptable是由iptables-persistent加载的, 规则存在 `/etc/iptables/`  
用于建立ip表的指令:

```sh
sudo iptables -t nat -A POSTROUTING -o utun -j MASQUERADE
sudo iptables -A FORWARD -i utun -o eth0 -m state --state RELATED,ESTABLISHED -j ACCEPT  
sudo iptables -A FORWARD -i eth0 -o utun -j ACCEPT
```

clash使用的outbound interface是wlan0, 链接到外部网路.
