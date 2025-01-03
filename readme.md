### OS课程设计
基于linux平台实现的一些windows cmd指令

#### commands:
1. CLS
```
- 清除屏幕
参数	说明
/?	在命令提示符下显示帮助。
```

2. DATE,
```
- date [/t | <month-day-year>]
参数 	描述
<month-day-year> 	
设置指定的日期，其中 month 是月（一位或两位数，包括值 1 - 12），
day 是日（一位或两位数，包括值 1 - 31），
year 是年（两位或四位数，包括值 00 - 99 或 1980 - 2099）。 
必须用句点 (.) 、连字符 () 或斜杠 (/) 分隔月、日和年的值。

注意：请注意，如果使用 2 位数来表示年，则值 80-99 对应于 1980-1999。
/t 	显示当前日期，而不提示输入新日期。
/? 	在命令提示符下显示帮助。
```

3. TIME,
```
time [/t | [<HH>[:<MM>[:<SS>]] [am|pm]]]
参数	说明
<HH>[:<MM>[:<SS>[.<NN>]]] [am | pm]	将系统时间设置为指定的新时间，其中 HH 以小时为单位（必需），MM 以分钟为单位，SS 以秒为单位。 NN 可用于指定百分之一秒。 必须使用冒号 (:) 分隔 HH、MM 和 SS 的值。 SS 和 NN 必须用句点 (.) 分隔。
如果未指定 am 或 pm，则默认情况下时间使用 24 小时格式。

/t	显示当前时间，而不提示输入新时间。
/?	在命令提示符下显示帮助。
```

4. DOSKEY,  
```
目前实现了两个功能，显示历史记录和编辑命令行宏
DOSKEY [/history] [-mirco -exename] [/reinstall] [/micros] 

/history[:n] 显示历史记录,可指定显示的最大历史数,默认10
micro=exename 编辑命令行宏
/reinstall 清除所有宏
/micros 显示所有宏
```

5. FIND,
```
FIND [/V] [/C] [/N] [/I]  -[string] +[file path]

  /V         显示所有未包含指定字符串的行。
  /C         仅显示包含字符串的行数。
  /N         显示行号。
  /I         搜索字符串时忽略大小写。

  -[string] 指定要搜索的文本字符串。
  +[file path] 文件路径
```


6. FINDSTR,
```
1-在FIND的基础上添加了多匹配模式与多文件支持
    如 FINDSTR -pattern1 -pattern2 +path1 +path2
2-添加了表达式的支持和当前路径的支持
    如 FINDSTR -regexp +filename (为了防止终端转义请在给表达式加上" ") /R
3.添加文件搜索
    如 FINDSTR -filename.fix /d:dir path

参数说明:

文件内容搜索(支持多pattern、多文件)
/i 不区分大小写搜索
/v 反转匹配
/c 仅显示匹配的行数
/n 显示匹配行的行号
/r 使用正则表达式匹配

目录搜索  FINDSTR -pattern /d:dir [/i] [/f]
/i 不区分大小写搜索
/f 显示完整路径
(目录搜索只支持一个pattern)
```

7. COMP,
```
用于比较两个文件是否相同
COMP  +data1 +data2 [/n] [/c] [/l] [/d] [/a]

一个匹配结果如:
offset / line 
file1 = diff 
file2 = diff
(如果没指定/n则文件大小必须相同，如果找到10个不同项则停止匹配)

匹配模式
 /n=num 如果文件大小不匹配，启用n来指定匹配的行数, 并启用 /l
 /c 匹配的ASCII字符不区别大小

显示模式
 /l 显示行号 
 /d 用十进制显示dff
 /a 用ASCII显示diff
```


8. FC
```
用于比较两个文件并显示不同（逐行比较）
FC  +data1 +data2 [/c] [/n] [/a] [/b]

匹配显示:
*****file1
previous line
line with diff
next line
*****file2
previous line
line with diff
next line
*****

匹配模式:
/c 不区别大小写进行比较。
/b 执行字节比较,显示如
offset : byte1 byte2 

显示模式:
/n 显示行数
/a 只显示每个不同处的第一行和最后一行。
```


9. EXIT,
```
退出终端
```


10. HELP, 
```
显示帮助
HELP 获取指令概述
HELP command 某个指令的具体帮助
```


11. MORE，
```
用于显示文件内容
如cat file | MORE
或者 more < file
回车显示下一行,空格显示下一页,q退出
```
