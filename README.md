# linux_c_mysql_backup
使用linuxC实现的mysql数据库备份
目标：通过alarm信号定时备份数据库
备注：目前是第一个版，本身不能定时备份可以结合linux自动化实现定时备份。
运行平台：Linux或类unix
测试平台：ubuntu server 14.04 x64
文件信息：
main.c：数据库备份程序
db_list：待备份的数据库信息，一行一个文件。
不足：
文件的读取方式感觉还不到位，使用的是fgetc一个个字符读取然后过滤和组合来读取相关的数据库信息；开始使用的是fgets函数处理的时候，遇到了很多的麻烦，比如读取到回车字符\r,读取到换行字符\n,读取到不可见字符等，后面信心大受伤所以改用了fgetc实现了。目前测试还是挺良好的，也欢迎大家提出更好的解决方案。

编译和运行：
首先把要备份的数据库名写进db_list文件里面一行一个数据库，如
admin
book
然后编译和运行
$gcc -o mian main.c
$./main

如果有需要shell版的可以访问我的linux_shell_mysql_backup
