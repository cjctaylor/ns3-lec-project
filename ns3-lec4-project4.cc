#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FourthScriptExample");		//定义记录组件

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  uint32_t nWifi0 = 6;
  uint32_t nWifi1 = 6;				//wifi节点数量
  bool tracing = false;

  CommandLine cmd;
  cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi0);
  cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi1);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);

  cmd.Parse (argc,argv);

  // Check for valid number of csma or wifi nodes
  // 250 should be enough, otherwise IP addresses 
  // soon become an issue		//判断是否超过了250个，超过报错 , 原因？
  if (nWifi0 > 250 || nWifi1 > 250)
    {
      std::cout << "Too many wifi or csma nodes, no more than 250 each." << std::endl;
      return 1;
    }

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);	//启动记录组件
    }

  //创建2个节点，p2p链路两端
  NodeContainer p2pNodes;
  p2pNodes.Create (2);

  //创建信道，设置信道参数，在设备安装到节点上
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);

//创建wifista无线终端，AP接入点
  NodeContainer wifiStaNodes0;
  wifiStaNodes0.Create (nWifi0);
  NodeContainer wifiApNode0 = p2pNodes.Get (0);

  //创建无线设备于无线节点之间的互联通道，并将通道对象与物理层对象关联
  //确保所有物理层对象使用相同的底层信道，即无线信道
  YansWifiChannelHelper channel0 = YansWifiChannelHelper::Default ();
  channel0.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");

  YansWifiPhyHelper phy0 = YansWifiPhyHelper::Default ();
  phy0.SetChannel (channel0.Create ());

  //配置速率控制算法，AARF算法
  WifiHelper wifi0 = WifiHelper::Default ();
  wifi0.SetRemoteStationManager ("ns3::AarfWifiManager");

  NqosWifiMacHelper mac0 = NqosWifiMacHelper::Default ();

 //配置mac类型为sta模式，不发送探测请求
  Ssid ssid0 = Ssid ("ns-3-ssid0");
  mac0.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid0),
               "ActiveProbing", BooleanValue (false));

 //创建无线设备，将mac层和phy层安装到设备上
  NetDeviceContainer staDevices0;
  staDevices0 = wifi0.Install (phy0, mac0, wifiStaNodes0);

  //配置AP节点的mac层为AP模式，创建AP设备
  mac0.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid0));

  NetDeviceContainer apDevices0;
  apDevices0 = wifi0.Install (phy0, mac0, wifiApNode0);

  //配置移动模型，起始位置
  MobilityHelper mobility0;
/*
  mobility0.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (-25.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (10.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));
*/
  Ptr<ListPositionAllocator> positionAlloc0 = CreateObject<ListPositionAllocator> ();
  positionAlloc0->Add (Vector (-25.0, 0.0, 0.0));
  positionAlloc0->Add (Vector (-35.0, 0.0, 0.0));
  positionAlloc0->Add (Vector (-30.0, 8.660254, 0.0));
  positionAlloc0->Add (Vector (-20.0, 8.660254, 0.0));
  positionAlloc0->Add (Vector (-15.0, 0.0, 0.0));
  positionAlloc0->Add (Vector (-20.0, -8.660254, 0.0));
  positionAlloc0->Add (Vector (-30.0, -8.660254, 0.0));
  mobility0.SetPositionAllocator (positionAlloc0);
//配置AP移动方式，ConstantPositionMobilityModel，固定位置模型
  mobility0.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility0.Install (wifiApNode0);

  //配置STA移动方式，RandomWalk2dMobilityModel，随机游走模型
  mobility0.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Bounds", RectangleValue (Rectangle (-250, 250, -250, 250)));
  mobility0.Install (wifiStaNodes0);



//创建wifista无线终端，AP接入点
  NodeContainer wifiStaNodes1;
  wifiStaNodes1.Create (nWifi1);
  NodeContainer wifiApNode1 = p2pNodes.Get (1);

  //创建无线设备于无线节点之间的互联通道，并将通道对象与物理层对象关联
  //确保所有物理层对象使用相同的底层信道，即无线信道
  YansWifiChannelHelper channel1 = YansWifiChannelHelper::Default ();
  channel1.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");

  YansWifiPhyHelper phy1 = YansWifiPhyHelper::Default ();
  phy1.SetChannel (channel1.Create ());

  //配置速率控制算法，AARF算法
  WifiHelper wifi1 = WifiHelper::Default ();
  wifi1.SetRemoteStationManager ("ns3::AarfWifiManager");

  NqosWifiMacHelper mac1 = NqosWifiMacHelper::Default ();

 //配置mac类型为sta模式，不发送探测请求
  Ssid ssid1 = Ssid ("ns-3-ssid1");
  mac1.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid1),
               "ActiveProbing", BooleanValue (false));

 //创建无线设备，将mac层和phy层安装到设备上
  NetDeviceContainer staDevices1;
  staDevices1 = wifi1.Install (phy1, mac1, wifiStaNodes1);

  //配置AP节点的mac层为AP模式，创建AP设备
  mac1.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid1));

  NetDeviceContainer apDevices1;
  apDevices1 = wifi1.Install (phy1, mac1, wifiApNode1);

  //配置移动模型，起始位置
  MobilityHelper mobility1;
/*
  mobility1.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (25.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (10.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));

*/

  Ptr<ListPositionAllocator> positionAlloc1 = CreateObject<ListPositionAllocator> ();
  positionAlloc1->Add (Vector (25.0, 0.0, 0.0));
  positionAlloc1->Add (Vector (35.0, 0.0, 0.0));
  positionAlloc1->Add (Vector (30.0, 8.660254, 0.0));
  positionAlloc1->Add (Vector (20.0, 8.660254, 0.0));
  positionAlloc1->Add (Vector (15.0, 0.0, 0.0));
  positionAlloc1->Add (Vector (20.0, -8.660254, 0.0));
  positionAlloc1->Add (Vector (30.0, -8.660254, 0.0));
  mobility1.SetPositionAllocator (positionAlloc1);
//配置AP移动方式，ConstantPositionMobilityModel，固定位置模型
  mobility1.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility1.Install (wifiApNode1);

  //配置STA移动方式，RandomWalk2dMobilityModel，随机游走模型
  mobility1.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Bounds", RectangleValue (Rectangle (-250, 250, -250, 250)));
  mobility1.Install (wifiStaNodes1);


  //已经创建了节点，设备，信道和移动模型，接下来配置协议栈
  InternetStackHelper stack;
  stack.Install (wifiApNode0);
  stack.Install (wifiStaNodes0);
  stack.Install (wifiApNode1);
  stack.Install (wifiStaNodes1);

  //分配IP地址
  Ipv4AddressHelper address;
 //P2P信道
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);
 //wifi信道
  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer wifiAp0Interface;
  address.Assign (staDevices0);
  wifiAp0Interface = address.Assign (apDevices0);
 //wifi信道
  address.SetBase ("10.1.3.0", "255.255.255.0");
  address.Assign (staDevices1);
  address.Assign (apDevices1);
  //放置echo服务端程序在一个AP节点,端口为9
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (wifiApNode0);
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  //回显客户端放在最后的STA节点，指向CSMA网络的服务器，上面的节点地址，端口为9
  UdpEchoClientHelper echoClient (wifiAp0Interface.GetAddress(0), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (5));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.1)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (512));

  //安装其他节点应用程序
  ApplicationContainer clientApps = 
    echoClient.Install (wifiStaNodes1.Get (nWifi1 - 1));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));
  //启动互联网络路由
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Stop (Seconds (10.0));

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

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}





