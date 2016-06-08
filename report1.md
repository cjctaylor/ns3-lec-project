#第一次NS3软件仿真报告
##曹金超
###1. 修改程序
在函数printHello函数中增加学号和延时频率两个参数,在main函数里同样用cmd.AddValue来通过终端对其进行赋值。<br>
![图1](http://ww3.sinaimg.cn/mw1024/edd82795jw1f4o6bvcpiqj20p20kogqk.jpg)<br>
###2. 终端设置不同输出频率
在终端设置输出间隔时间delay分别为0.5，1，2得到不同结果。<br>
![图2](http://ww3.sinaimg.cn/mw1024/edd82795jw1f4ny24q7lcj20k20cugsz.jpg)<br>
![图3](http://ww3.sinaimg.cn/mw1024/edd82795jw1f4ny24n7fvj20k709mjxl.jpg)<br>
由于程序在第5秒就运行完毕，所以对应不同输出频率的输出结果也会有所不同。<br>
###3. wc计数
统计输出的总次数就是统计输出的行数，所以我在之前的命令后增加了wc -l，即统计输出行数。<br>
![图4](http://ww2.sinaimg.cn/mw1024/edd82795jw1f4ny24kd1lj20kc09iq8z.jpg)<br>
可以看出，输出频率为0.5s时，输出为10次；输出频率为1s时，输出为5次；输出频率为2s时，输出为3次。理论与实际运行的结果相吻合。<br>
###4. grep查找特定信息
在0.5s输出频率之下进行查找操作，在其命令之后添加如下图代码，结果如图所示：<br>
![图5](http://ww1.sinaimg.cn/mw1024/edd82795jw1f4ny24uidbj20k20duqcl.jpg)<br>
可以看出，grep查找的时候是查找的输出文本的内容而不是实际时间，如输入“+4”时，它会将4s时刻和4.5s时刻的两行内容都保留了下来。所以我只有输入“+40”才能将4.5s时刻的那行内容过滤掉，只剩下4s时刻的输出内容。<br>