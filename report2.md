#第二次ns3软件培训报告
##曹金超
###过程及内容
####1.拓扑结构图
在third.cc的拓扑基础上增加了一个point2point信道和一个csma信道，连接分别如下<br>
![拓扑图1](http://t3.qpic.cn/mblogpic/1b588eb2c04032126c90/2000)<br>
![拓扑图2](http://t3.qpic.cn/mblogpic/47ddc6c7cd3452a96b04/2000)<br>
####2.server与client通信结果截图
设置了4个客户机，1个服务器，运行结果如下图<br>
![结果1](http://t3.qpic.cn/mblogpic/4d8b4c89053753c70f52/2000)<br>
![结果2](http://t3.qpic.cn/mblogpic/f65729b496648964324e/2000)<br>
####3.STA移动模型部分代码截图
要STA节点线性远离AP，则要选择ConstantVelocityMobilityModel的移动模型，对其进行移动速度设置<br>
![代码截图1](http://t3.qpic.cn/mblogpic/78d6e169e4cde93cebfc/2000)<br>
![代码截图2](http://t3.qpic.cn/mblogpic/2d13be2449a334238712/2000)<br>
###问题及总结
1.在拓扑2中，出现了不知道如何将两个P2P信道连接在一个节点上的情况。因为同时将两个P2P节点赋值给一个csma节点会使其覆盖掉前一个，所以应该将其中一个p2p节点赋值给csma节点后，再将该csma节点赋值给另一个p2p节点，这样便达到了效果。<br>
2.移动模型中ConstantVelocityMobilityModel如何设置移动速度的问题，解决如上图所示。