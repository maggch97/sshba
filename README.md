# sshba

## 方便使用ssh连接服务器的命令行工具

![image](https://raw.githubusercontent.com/maggch97/sshba/master/img/GIF2020-6-7%208-01-10.gif)

### 使用方法：

将.sshba目录移动到用户目录 

Windows下使用 `echo %USERPROFILE%` , Linux/Mac使用 `echo $HOME` 输出用户目录

系统 PATH ( Windows 下是 Path ) 环境变量增加 sshba 程序所在目录

默认e向上，d向下，回车连接。其他所有按键都是退出。


### 编译:
```
g++ .\sshba.cpp --std=c++17
```
