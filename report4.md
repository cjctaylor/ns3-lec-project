#第四次NS3作业报告
##曹金超
###1. 仿真拓扑
![图片1](http://ww3.sinaimg.cn/mw690/edd82795gw1f5fkbszqwuj20ja068q3x.jpg)<br>
###2. PyViz
终端窗口的状态：<br>
![图片2](http://ww2.sinaimg.cn/mw690/edd82795gw1f5fdr5ilz3j20k50g1ak1.jpg)<br>
初始状态：<br>
![图片3](http://ww3.sinaimg.cn/mw690/edd82795gw1f5fdqo8q9lj20kk0g10uh.jpg)<br>
仿真过程中：<br>
![图片4](http://ww2.sinaimg.cn/mw690/edd82795gw1f5fdqn26acj20ke0fwabu.jpg)<br>
###3. Tracing产生文件
```cpp
  if (tracing == true)
    {
      pointToPoint.EnablePcap ("ns3-lec4-project4",p2pDevices.Get(0));
      pointToPoint.EnablePcap ("ns3-lec4-project4",p2pDevices.Get(1));
      phy0.EnablePcap ("ns3-lec4-project4", apDevices0.Get (0));
      phy1.EnablePcap ("ns3-lec4-project4", apDevices1.Get (0));
      
      AsciiTraceHelper ascii;
      pointToPoint.EnableAsciiAll(ascii.CreateFileStream("ns3-lec4-project4-p2p.tr"));

      AsciiTraceHelper ascii0;
      phy0.EnableAsciiAll(ascii0.CreateFileStream("ns3-lec4-project4-wifi0.tr"));

      AsciiTraceHelper ascii1;
      phy1.EnableAsciiAll(ascii1.CreateFileStream("ns3-lec4-project4-wifi1.tr"));
    }
```
###4. Matlab处理后的图表
####客户机发报文
![图片5](http://ww4.sinaimg.cn/mw690/edd82795gw1f5fdum9ahfj20b905n74t.jpg)<br>
![图片6](http://ww3.sinaimg.cn/mw690/edd82795gw1f5fdqqn1eej20fl0bo74s.jpg)<br>
####客户机收报文
![图片7](http://ww1.sinaimg.cn/mw690/edd82795gw1f5fdqxlslfj20bq05o0tb.jpg)<br>
![图片8](http://ww4.sinaimg.cn/mw690/edd82795gw1f5fdqp7xokj20fl0bowf2.jpg)<br>

