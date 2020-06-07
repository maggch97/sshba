# sshba

## 方便使用ssh连接服务器的命令行工具

Wdinwos10 + Windows Terminal + sshba 0.0.1

![image](https://raw.githubusercontent.com/maggch97/sshba/master/img/GIF2020-6-7%208-01-10.gif)

Ubuntu20.04 + Terminator + sshba 0.0.2

![image](https://raw.githubusercontent.com/maggch97/sshba/master/img/GIF2020-06-07-15-28.gif)

### 使用方法：

将.sshba目录移动到用户目录 

Windows下使用 `echo %USERPROFILE%` , Linux/Mac使用 `echo $HOME` 输出用户目录

系统 PATH ( Windows 下是 Path ) 环境变量增加 sshba 程序所在目录

默认e向上，d向下，回车连接。其他所有按键都是退出。

### 配置：

`.sshba/config` 文件是按键配置，注意冒号和按键中间需要有一个空格

`.sshba/ssh_info` 文件是所有服务器信息，格式 `名称 地址 端口 用户名` 


### 编译:
```
g++ ./sshba.cpp --std=c++17 -o sshba
```


