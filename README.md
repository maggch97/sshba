# sshba

## 方便使用ssh连接服务器的命令行工具

Wdinwos10 + Windows Terminal + sshba 0.0.1

![image](https://raw.githubusercontent.com/maggch97/sshba/master/img/GIF2020-6-7%208-01-10.gif)

Ubuntu20.04 + Terminator + sshba 0.0.2

![image](https://raw.githubusercontent.com/maggch97/sshba/master/img/GIF2020-06-07-15-28.gif)


### 编译:
```
g++ ./sshba.cpp --std=c++17 -o sshba
```


### 使用方法：

Windows用户向Path环境变量增加 sshba.exe 程序所在目录，Linux/Mac用户把sshba二进制文件移动到`/usr/local/bin`目录

使用`sshba key` 命令配置上下按键(目前只保证字母，数字，符号以及方向键可用)

默认e向上，d向下，回车连接。其他所有按键都是退出。

命令行输入`sshba` 首次使用会提示hosts为空，需要编辑hosts文件增加至少一条服务器信息

### 配置：

使用`sshba config`命令输出配置文件目录

`.sshba/config` 文件是按键配置，二进制文件无法编辑

`.sshba/hosts` 文件是所有服务器信息，格式 `名称 地址 端口 用户名` 



