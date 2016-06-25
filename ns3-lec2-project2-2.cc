/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"

// Default Network Topology
//默认网络拓扑
// Number of wifi or csma nodes can be increased up to 250
//                          |
//                 Rank 0   |   Rank 1
// -------------------------|----------------------------
//
//
//                                            10.1.2.0      
//   Wifi 10.1.5.0                      --------------------- n8   n9   n10  n11
//                AP                    |   point-to-point    |    |    |    |
// *    *    *    *                     |                     ================
// |    |    |    |      10.1.1.0       |                       LAN 10.1.4.0
// n5   n6   n7   n0 ------------------ n1   n2   n3   n4
//                     point-to-point   |    |    |    |
//                                      ================
//                                        LAN 10.1.3.0

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");		//定义记录组件

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  uint32_t nCsma = 3;			//csma节点数量
  uint32_t nWifi = 3;				//wifi节点数量
  uint32_t nCsma1 = 3;
   bool tracing = false;


  CommandLine cmd;
  cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);

  cmd.Parse (argc,argv);

  // Check for valid number of csma or wifi nodes
  // 250 should be enough, otherwise IP addresses 
  // soon become an issue		//判断是否超过了250个，超过报错 , 原因？
  if (nWifi > 250 || nCsma > 250)
    {
      std::cout << "Too many wifi or csma nodes, no more than 250 each." << std::endl;
      return 1;
    }

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);	//启动记录组件
    }

  //第一个p2p信道
  //创建2个节点，p2p链路两端
  NodeContainer p2pNodes;
  p2pNodes.Create (2);

  //创建信道，设置信道参数，在设备安装到节点上
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);
  
  //第一个csma信道
  //创建csma节点，包含一个p2p节点

  NodeContainer csmaNodes;
  csmaNodes.Add (p2pNodes.Get (1));
  csmaNodes.Create (nCsma);

  //创建信道，设置信道参数，在设备安装到节点上
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

  //第二个p2p信道
  //创建2个节点，p2p链路两端
  NodeContainer p2pNodes1;
  p2pNodes1.Add (csmaNodes.Get (0));
  p2pNodes1.Create(1);

  //创建信道，设置信道参数，在设备安装到节点上
  PointToPointHelper pointToPoint1;
  pointToPoint1.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint1.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer p2pDevices1;
  p2pDevices1 = pointToPoint.Install (p2pNodes1);
  
  //第二个csma信道
  //创建csma节点，包含一个p2p节点

  NodeContainer csmaNodes1;
  csmaNodes1.Add (p2pNodes1.Get (1));
  csmaNodes1.Create (nCsma1);

  //创建信道，设置信道参数，在设备安装到节点上
  CsmaHelper csma1;
  csma1.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma1.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  NetDeviceContainer csmaDevices1;
  csmaDevices1 = csma1.Install (csmaNodes1);

//创建wifista无线终端，AP接入点
  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (nWifi);
  NodeContainer wifiApNode = p2pNodes.Get (0);

  //创建无线设备于无线节点之间的互联通道，并将通道对象与物理层对象关联
  //确保所有物理层对象使用相同的底层信道，即无线信道
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

  //配置速率控制算法，AARF算法
  WifiHelper wifi = WifiHelper::Default ();
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager");

  NqosWifiMacHelper mac = NqosWifiMacHelper::Default ();

  //配置mac类型为sta模式，不发送探测请求
  Ssid ssid = Ssid ("ns-3-ssid");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));

  //创建无线设备，将mac层和phy层安装到设备上
  NetDeviceContainer staDevices;
  staDevices = wifi.Install (phy, mac, wifiStaNodes);

  //配置AP节点的mac层为AP模式，创建AP设备
  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));

  NetDeviceContainer apDevices;
  apDevices = wifi.Install (phy, mac, wifiApNode);

  //配置移动模型，起始位置
  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));

  //配置STA移动方式，RandomWalk2dMobilityModel，随机游走模型
  mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  mobility.Install (wifiStaNodes);
  for(uint n=0;n<wifiStaNodes.GetN();n++)
  {
        Ptr<ConstantVelocityMobilityModel>haha =
                                 wifiStaNodes.Get(n)->GetObject<ConstantVelocityMobilityModel>();
        haha->SetVelocity(Vector(8,0,0));
  }


//配置AP移动方式，ConstantPositionMobilityModel，固定位置模型
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNode);

  //已经创建了节点，设备，信道和移动模型，接下来配置协议栈
  InternetStackHelper stack;
  stack.Install (csmaNodes);
  stack.Install (csmaNodes1);
  stack.Install (wifiApNode);
  stack.Install (wifiStaNodes);

  //分配IP地址
  Ipv4AddressHelper address;
 //P2P信道
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);
 //第二个P2P信道
  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces1;
  p2pInterfaces1 = address.Assign (p2pDevices1);
 //csma信道
  address.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign (csmaDevices);
 //csma1信道
  address.SetBase ("10.1.4.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces1;
  csmaInterfaces1 = address.Assign (csmaDevices1);
 //wifi信道
  address.SetBase ("10.1.5.0", "255.255.255.0");
  address.Assign (staDevices);
  address.Assign (apDevices);

  //放置echo服务端程序在最右边的csma节点,端口为9
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (csmaNodes1.Get (nCsma1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  //回显客户端放在最后的STA节点，指向CSMA网络的服务器，上面的节点地址，端口为9
  UdpEchoClientHelper echoClient (csmaInterfaces1.GetAddress (nCsma1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  //安装其他节点应用程序
  ApplicationContainer clientApps = 
    echoClient.Install (wifiStaNodes.Get (nWifi - 1));
  clientApps.Start (Seconds (5.0));
  clientApps.Stop (Seconds (10.0));
//第二个客户端
  //回显客户端放在最后的STA节点，指向CSMA网络的服务器，上面的节点地址，端口为9
  UdpEchoClientHelper echoClient0 (csmaInterfaces1.GetAddress (nCsma1), 9);
  echoClient0.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient0.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient0.SetAttribute ("PacketSize", UintegerValue (1024));

  //安装其他节点应用程序
  ApplicationContainer clientApps0 = 
    echoClient0.Install (csmaNodes.Get (nCsma - 1));
  clientApps0.Start (Seconds (6.0));
  clientApps0.Stop (Seconds (10.0));
//第三个客户端
  //回显客户端放在最后的STA节点，指向CSMA网络的服务器，上面的节点地址，端口为9
  UdpEchoClientHelper echoClient1 (csmaInterfaces1.GetAddress (nCsma1), 9);
  echoClient1.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient1.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient1.SetAttribute ("PacketSize", UintegerValue (1024));

  //安装其他节点应用程序
  ApplicationContainer clientApps1 = 
    echoClient1.Install (csmaNodes1.Get (0));
  clientApps1.Start (Seconds (7.0));
  clientApps1.Stop (Seconds (10.0));
//第四个客户端
  //回显客户端放在最后的STA节点，指向CSMA网络的服务器，上面的节点地址，端口为9
  UdpEchoClientHelper echoClient2 (csmaInterfaces1.GetAddress (nCsma1), 9);
  echoClient2.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient2.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient2.SetAttribute ("PacketSize", UintegerValue (1024));

  //安装其他节点应用程序
  ApplicationContainer clientApps2 = 
    echoClient2.Install (wifiApNode);
  clientApps2.Start (Seconds (8.0));
  clientApps2.Stop (Seconds (10.0));
  //启动互联网络路由
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Stop (Seconds (10.0));

  if (tracing == true)
    {
      pointToPoint.EnablePcap ("ns3-lec2-project2-2",p2pDevices.Get(0));
      pointToPoint.EnablePcap ("ns3-lec2-project2-2",p2pDevices.Get(1));
      phy.EnablePcap ("ns3-lec2-project2-2", apDevices.Get (0));
      csma.EnablePcap ("ns3-lec2-project2-2", csmaDevices.Get (0), true);
    }

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
