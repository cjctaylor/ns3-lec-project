#第三次NS3培训报告
##曹金超
###1. 系统日志信息
通过在终端执行export 'NS_LOG=UdpEchoClientApplication=level_all|prefix_func:UdpEchoServerApplication=level_all|prefix_func则表示启动了客户机和服务器的所有日志信息。通过./waf --run执行程序后得到以下结果：<br>
![图片1](http://ww1.sinaimg.cn/mw690/edd82795gw1f5fdq79vi1j20k30cj46m.jpg)<br>
![图片2](http://ww2.sinaimg.cn/mw690/edd82795gw1f5fdq5x6qfj20k20euaiz.jpg)<br>
![图片3](http://ww1.sinaimg.cn/mw690/edd82795gw1f5fdq24pffj20k20euthb.jpg)<br>
再在终端中执行export NS_LOG=ThirdScriptExample=info表示恢复为原来的记录程序进展信息。通过./waf --run执行程序后得到以下结果：<br>
![图片4](http://ww1.sinaimg.cn/mw690/edd82795gw1f5fdpxrtkjj20k20dc10q.jpg)<br>
###2. Tracing记录
将tracing设置为true后，便能将信道中的数据传送等信息通过pcap和tr文件记录下来<br>
```cpp
  if (tracing == true)
    {
      pointToPoint.EnablePcap ("ns3-lec3-project3",p2pDevices.Get(0));
      pointToPoint.EnablePcap ("ns3-lec3-project3",p2pDevices.Get(1));
      phy.EnablePcap ("ns3-lec3-project3", apDevices.Get (0));
      csma.EnablePcap ("ns3-lec3-project3", csmaDevices.Get (0), true);
      
      AsciiTraceHelper ascii;
      pointToPoint.EnableAsciiAll(ascii.CreateFileStream("ns3-lec3-project3-p2p.tr"));

      AsciiTraceHelper ascii1;
      csma.EnableAsciiAll(ascii1.CreateFileStream("ns3-lec3-project3-csma.tr"));
    }
```
###3. PyViz截图
####第一种拓扑截图
![图片5](http://ww4.sinaimg.cn/mw690/edd82795gw1f5fdpu8bsmj20k20cuahs.jpg)<br>
![图片6](http://ww2.sinaimg.cn/mw690/edd82795gw1f5fdprucz4j20jd0fdgn1.jpg)<br>
####第二种拓扑截图
![图片7](http://ww2.sinaimg.cn/mw690/edd82795gw1f5fdpr4j13j20k20cuahm.jpg)<br>
![图片8](http://ww3.sinaimg.cn/mw690/edd82795gw1f5fdpp83g1j20jd0fdwfs.jpg)<br>
###4. wireshark查看文件
![图片9](http://ww3.sinaimg.cn/mw690/edd82795gw1f5fdpnhniwj20kx0jntj5.jpg)<br>
###5. Matlab处理数据
将四个客户机的收发报文时延拿出来做比较。<br>
####客户机发报文
![图片10](http://ww1.sinaimg.cn/mw690/edd82795gw1f5fdqa2oebj209304wq39.jpg)<br>
![图片11](http://ww4.sinaimg.cn/mw690/edd82795gw1f5fdpvm4twj20gk0c8dg9.jpg)<br>
####客户机收报文
![图片12](http://ww4.sinaimg.cn/mw690/edd82795gw1f5fdq7fhxtj209a0550t5.jpg)<br>
![图片13](http://ww3.sinaimg.cn/mw690/edd82795gw1f5fdpujzosj20gk0c8t99.jpg)<br>
