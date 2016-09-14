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

#include <iostream>
#include <fstream>

#include "ns3/ipv4-global-routing-helper.h"

#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-routing-table-entry.h"

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/tap-bridge-module.h"
#include "ns3/ipv4-static-routing.h"
#include "ns3/ipv4-list-routing.h"
#include "ns3/ipv4-list-routing-helper.h"



using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("CC_p2p_SS_ieee13_tap");

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  //uint32_t i,j;
  //2 nodes for each network...so that we have a PMU and a PDC
  uint32_t nCsmaCC = 2;
  uint32_t nCsma645 = 2;
  uint32_t nCsma646 = 2;
  uint32_t nCsma632 = 2;
  uint32_t nCsma671 = 2;
  uint32_t nCsma684 = 2;
  uint32_t nCsma611 = 2;
  uint32_t nCsma633 = 2;
  uint32_t nCsma634 = 2;
  uint32_t nCsma692 = 2;
  uint32_t nCsma675 = 2;
  uint32_t nCsma680 = 2;
  uint32_t nCsma652 = 2;
  uint32_t nCsma650 = 2;

  

  //uint32_t np2p9 = 2;
  bool tracing = true;
  std::string mode = "UseBridge";
  std::string tapName ="mytap0";
  std::string tapName1 = "one";
  std::string tapName2 = "two";
  std::string tapName3 = "three";
  std::string tapName4 = "four";
  std::string tapName5 = "five";
  std::string tapName6 = "six";
  std::string tapName7 = "seven";
  std::string tapName8 = "eight";
  std::string tapName9 = "nine";
  std::string tapName10 = "ten";
  std::string tapName11= "eleven";
  std::string tapName12= "twelve";
  std::string tapNameCC = "mytap1";
  


  CommandLine cmd;
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);
  cmd.AddValue ("mode", "Mode setting of TapBridge", mode);
  cmd.AddValue ("tapName", "Name of the OS tap device", tapName);
  cmd.AddValue ("tapNameCC","Name of the OS second tap device", tapNameCC);
  cmd.AddValue ("tapName1","Name of the OS third tap device", tapName1);
  cmd.AddValue ("tapName2","Name of the OS fourth tap device", tapName2);
  cmd.AddValue ("tapName3","Name of the OS fivth tap device", tapName3);
  cmd.AddValue ("tapName4","Name of the OS sixth tap device", tapName4);
  cmd.AddValue ("tapName5","Name of the OS seventh tap device", tapName5);
  cmd.AddValue ("tapName6","Name of the OS eighth tap device", tapName6);
  cmd.AddValue ("tapName7","Name of the OS ninth tap device", tapName7);
  cmd.AddValue ("tapName8","Name of the OS tenth tap device", tapName8);
  cmd.AddValue ("tapName9","Name of the OS eleventh tap device", tapName9);
  cmd.AddValue ("tapName10","Name of the OS twelveth tap device", tapName10);
  cmd.AddValue ("tapName11","Name of the OS thirteenth tap device", tapName11);
  cmd.AddValue ("tapName12","Name of the OS forteenth tap device", tapName12);
  cmd.Parse (argc,argv);

  // Check for valid number of csma or wifi nodes
  // 250 should be enough, otherwise IP addresses 
  // soon become an issue

  GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::RealtimeSimulatorImpl"));
  GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));
  
//Buses: 611,632,633,634,645,646,650,652,671,675,680,684,692  
  NodeContainer csmaNodes611;
  csmaNodes611.Create (nCsma611);

  NodeContainer csmaNodes632;
  csmaNodes632.Create (nCsma632);

  NodeContainer csmaNodes633;
  csmaNodes633.Create (nCsma633);

  NodeContainer csmaNodes634;
  csmaNodes634.Create (nCsma634);


  NodeContainer csmaNodes645;
  csmaNodes645.Create (nCsma645);

  NodeContainer csmaNodes646;
  csmaNodes646.Create (nCsma646);

  NodeContainer csmaNodes650;
  csmaNodes650.Create (nCsma650);

  NodeContainer csmaNodes652;
  csmaNodes652.Create (nCsma652);

  NodeContainer csmaNodes671;
  csmaNodes671.Create (nCsma671);

  NodeContainer csmaNodes675;
  csmaNodes675.Create (nCsma675);

  NodeContainer csmaNodes680;
  csmaNodes680.Create (nCsma680);

  NodeContainer csmaNodes684;
  csmaNodes684.Create (nCsma684);

  NodeContainer csmaNodes692;
  csmaNodes692.Create (nCsma692);

  NodeContainer csmaNodesCC;
  csmaNodesCC.Create (nCsmaCC);

//Buses devices: 611,632,633,634,645,646,650,652,671,675,680,684,692,CC 
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (10560)));

  NetDeviceContainer csmaDevices611;
  csmaDevices611 = csma.Install (csmaNodes611);

  NetDeviceContainer csmaDevices632;
  csmaDevices632 = csma.Install (csmaNodes632);

  NetDeviceContainer csmaDevices633;
  csmaDevices633 = csma.Install (csmaNodes633);

  NetDeviceContainer csmaDevices634;
  csmaDevices634 = csma.Install (csmaNodes634);

  NetDeviceContainer csmaDevices645;
  csmaDevices645 = csma.Install (csmaNodes645);

  NetDeviceContainer csmaDevices646;
  csmaDevices646 = csma.Install (csmaNodes646);

  NetDeviceContainer csmaDevices650;
  csmaDevices650 = csma.Install (csmaNodes650);

  NetDeviceContainer csmaDevices652;
  csmaDevices652 = csma.Install (csmaNodes652);

  NetDeviceContainer csmaDevices671;
  csmaDevices671 = csma.Install (csmaNodes671);

  NetDeviceContainer csmaDevices675;
  csmaDevices675 = csma.Install (csmaNodes675);

  NetDeviceContainer csmaDevices680;
  csmaDevices680 = csma.Install (csmaNodes680);

  NetDeviceContainer csmaDevices684;
  csmaDevices684 = csma.Install (csmaNodes684);

  NetDeviceContainer csmaDevices692;
  csmaDevices692 = csma.Install (csmaNodes692);

  NetDeviceContainer csmaDevicesCC;
  csmaDevicesCC = csma.Install (csmaNodesCC);

  
  InternetStackHelper stack;
  stack.Install(csmaNodes611);
  stack.Install(csmaNodes632);
  stack.Install(csmaNodes633);
  stack.Install(csmaNodes634);
  stack.Install(csmaNodes645);
  stack.Install(csmaNodes646);
  stack.Install(csmaNodes650);
  stack.Install(csmaNodes652);
  stack.Install(csmaNodes671);
  stack.Install(csmaNodes675);
  stack.Install(csmaNodes680);
  stack.Install(csmaNodes684);
  stack.Install(csmaNodes692);
  stack.Install(csmaNodesCC);




    //PointToPointHelper and set the associated default Attributes so that we create a 5 Mbps transmitter on devices created using the helper and a 2 msec delay on channels created by the helper.

// p2p12_2,p2p2_3,p2p2_6,p2p3_6,p2p3_4,p2p3_5,p2p6_9,p2p6_10,p2p6_8,p2p6_4,p2p9_10, p2p6_8,p2p6_4,p2p9_10,p2p10_11,p2p4_11,p2p8_7,p2p7_4,p2p5_4
  PointToPointHelper p2p12_2;
  p2p12_2.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p2p12_2.SetChannelAttribute ("Delay", StringValue ("0.2ms"));
  NodeContainer p2pNode12_2 = NodeContainer (csmaNodesCC.Get (1), csmaNodes611.Get (1));
  NetDeviceContainer p2pDevices12_2 = p2p12_2.Install (p2pNode12_2);


  PointToPointHelper p2p2_3;
  p2p2_3.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p2p2_3.SetChannelAttribute ("Delay", StringValue ("0.609ms"));
  NodeContainer p2pNode2_3 = NodeContainer (csmaNodes611.Get (1), csmaNodes632.Get (1));
  NetDeviceContainer p2pDevices2_3 = p2p2_3.Install (p2pNode2_3);

PointToPointHelper p2p2_6;
  p2p2_6.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p2p2_6.SetChannelAttribute ("Delay", StringValue ("1.147ms"));
  NodeContainer p2pNode2_6 = NodeContainer (csmaNodes611.Get (1), csmaNodes645.Get (1));
  NetDeviceContainer p2pDevices2_6 = p2p2_6.Install (p2pNode2_6);

PointToPointHelper p2p3_6;
  p2p3_6.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p2p3_6.SetChannelAttribute ("Delay", StringValue ("0.5ms"));
  NodeContainer p2pNode3_6 = NodeContainer (csmaNodes632.Get (1), csmaNodes645.Get (1));
  NetDeviceContainer p2pDevices3_6 = p2p3_6.Install (p2pNode3_6);

PointToPointHelper p2p3_4;
  p2p3_4.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p2p3_4.SetChannelAttribute ("Delay", StringValue ("0.906ms"));
  NodeContainer p2pNode3_4 = NodeContainer (csmaNodes632.Get (1), csmaNodes634.Get (1));
  NetDeviceContainer p2pDevices3_4 = p2p3_4.Install (p2pNode3_4);

PointToPointHelper p2p3_5;
  p2p3_5.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p2p3_5.SetChannelAttribute ("Delay", StringValue ("1.018ms"));
  NodeContainer p2pNode3_5 = NodeContainer (csmaNodes632.Get (1), csmaNodes633.Get (1));
  NetDeviceContainer p2pDevices3_5 = p2p3_5.Install (p2pNode3_5);

PointToPointHelper p2p6_9;
  p2p6_9.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p2p6_9.SetChannelAttribute ("Delay", StringValue ("0.123ms"));
  NodeContainer p2pNode6_9 = NodeContainer (csmaNodes645.Get (1), csmaNodes680.Get (1));
  NetDeviceContainer p2pDevices6_9 = p2p6_9.Install (p2pNode6_9);

PointToPointHelper p2p6_10;
  p2p6_10.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p2p6_10.SetChannelAttribute ("Delay", StringValue ("0.062ms"));
  NodeContainer p2pNode6_10 = NodeContainer (csmaNodes645.Get (1), csmaNodes684.Get (1));
  NetDeviceContainer p2pDevices6_10 = p2p6_10.Install (p2pNode6_10);

PointToPointHelper p2p6_8;
  p2p6_8.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p2p6_8.SetChannelAttribute ("Delay", StringValue ("0.095ms"));
  NodeContainer p2pNode6_8 = NodeContainer (csmaNodes645.Get (1), csmaNodes675.Get (1));
  NetDeviceContainer p2pDevices6_8 = p2p6_8.Install (p2pNode6_8);

PointToPointHelper p2p6_4;
  p2p6_4.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p2p6_4.SetChannelAttribute ("Delay", StringValue ("0.216ms"));
  NodeContainer p2pNode6_4 = NodeContainer (csmaNodes645.Get (1), csmaNodes634.Get (1));
  NetDeviceContainer p2pDevices6_4 = p2p6_4.Install (p2pNode6_4);

PointToPointHelper p2p9_10;
  p2p9_10.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p2p9_10.SetChannelAttribute ("Delay", StringValue ("0.096ms"));
  NodeContainer p2pNode9_10 = NodeContainer (csmaNodes680.Get (1), csmaNodes684.Get (1));
  NetDeviceContainer p2pDevices9_10 = p2p9_10.Install (p2pNode9_10);

PointToPointHelper p2p10_11;
  p2p10_11.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p2p10_11.SetChannelAttribute ("Delay", StringValue ("0.167ms"));
  NodeContainer p2pNode10_11 = NodeContainer (csmaNodes684.Get (1), csmaNodes692.Get (1));
  NetDeviceContainer p2pDevices10_11 = p2p10_11.Install (p2pNode10_11);

PointToPointHelper p2p4_11;
  p2p4_11.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p2p4_11.SetChannelAttribute ("Delay", StringValue ("0.129ms"));
  NodeContainer p2pNode4_11 = NodeContainer (csmaNodes634.Get (1), csmaNodes692.Get (1));
  NetDeviceContainer p2pDevices4_11 = p2p4_11.Install (p2pNode4_11);


PointToPointHelper p2p8_7;
  p2p8_7.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p2p8_7.SetChannelAttribute ("Delay", StringValue ("0.092ms"));
  NodeContainer p2pNode8_7 = NodeContainer (csmaNodes675.Get (1), csmaNodes671.Get (1));
  NetDeviceContainer p2pDevices8_7 = p2p8_7.Install (p2pNode8_7);

PointToPointHelper p2p7_4;
  p2p7_4.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p2p7_4.SetChannelAttribute ("Delay", StringValue ("0.04ms"));
  NodeContainer p2pNode7_4 = NodeContainer (csmaNodes671.Get (1), csmaNodes634.Get (1));
  NetDeviceContainer p2pDevices7_4 = p2p7_4.Install (p2pNode7_4);


PointToPointHelper p2p5_4;
  p2p5_4.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p2p5_4.SetChannelAttribute ("Delay", StringValue ("0.879ms"));
  NodeContainer p2pNode5_4 = NodeContainer (csmaNodes633.Get (1), csmaNodes634.Get (1));
  NetDeviceContainer p2pDevices5_4 = p2p5_4.Install (p2pNode5_4);





 
  //
  // The topology has a Wifi network of four nodes on the left side.  We'll make
  // node zero the AP and have the other three will be the STAs.
  //


Ipv4AddressHelper address;

//// p2pDevices12_2,p2pDevices2_3,p2pDevices2_6,p2pDevices3_6,p2pDevices3_4,p2pDevices3_5,p2pDevices6_9,p2pDevices6_10,p2pDevices6_8,p2pDevices6_4,p2pDevices9_10, //p2pDevices10_11,p2pDevices4_11,p2pDevices8_7,p2pDevices7_4,p2pDevices5_4

  address.SetBase ("1.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces12_2;
  p2pInterfaces12_2 = address.Assign (p2pDevices12_2);

   address.SetBase ("2.2.2.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces2_3;
  p2pInterfaces2_3 = address.Assign (p2pDevices2_3);

   address.SetBase ("3.3.3.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces2_6;
  p2pInterfaces2_6 = address.Assign (p2pDevices2_6);

 address.SetBase ("4.4.4.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces3_6;
  p2pInterfaces3_6 = address.Assign (p2pDevices3_6);

 address.SetBase ("5.5.5.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces3_4;
  p2pInterfaces3_4 = address.Assign (p2pDevices3_4);

 address.SetBase ("6.6.6.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces3_5;
  p2pInterfaces3_5 = address.Assign (p2pDevices3_5);

 address.SetBase ("7.7.7.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces6_9;
  p2pInterfaces6_9 = address.Assign (p2pDevices6_9);

 address.SetBase ("8.8.8.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces6_10;
  p2pInterfaces6_10 = address.Assign (p2pDevices6_10);

 address.SetBase ("9.9.9.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces6_8;
  p2pInterfaces6_8 = address.Assign (p2pDevices6_8);

 address.SetBase ("12.12.12.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces6_4;
  p2pInterfaces6_4 = address.Assign (p2pDevices6_4);

 address.SetBase ("13.13.13.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces9_10;
  p2pInterfaces9_10 = address.Assign (p2pDevices9_10);

 address.SetBase ("14.14.14.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces10_11;
  p2pInterfaces10_11 = address.Assign (p2pDevices10_11);

 address.SetBase ("15.15.15.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces4_11;
  p2pInterfaces4_11 = address.Assign (p2pDevices4_11);

 address.SetBase ("16.16.16.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces8_7;
  p2pInterfaces8_7 = address.Assign (p2pDevices8_7);

 address.SetBase ("17.17.17.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces7_4;
  p2pInterfaces7_4 = address.Assign (p2pDevices7_4);

 address.SetBase ("18.18.18.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces5_4;
  p2pInterfaces5_4 = address.Assign (p2pDevices5_4);

//ip address for LANs

  //Bus 1 :611
  address.SetBase("192.168.2.0","255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces2;
  csmaInterfaces2 = address.Assign (csmaDevices611);
  //Bus 2: 632
  address.SetBase("192.168.3.0","255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces3;
  csmaInterfaces3 = address.Assign (csmaDevices632);
  //Bus 4: 634
  address.SetBase("192.168.4.0","255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces4;
  csmaInterfaces4 = address.Assign (csmaDevices634);
  //Bus 7: 650
  address.SetBase("192.168.4.0","255.255.255.0","0.0.0.10");
  Ipv4InterfaceContainer csmaInterfaces4_1;
  csmaInterfaces4_1 = address.Assign (csmaDevices650);
  //Bus 8: 652
  address.SetBase("192.168.4.0","255.255.255.0","0.0.0.20");
  Ipv4InterfaceContainer csmaInterfaces4_2;
  csmaInterfaces4_2 = address.Assign (csmaDevices652);
  //Bus 3: 633
  address.SetBase("192.168.5.0","255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces5;
  csmaInterfaces5 = address.Assign (csmaDevices633);
  //Bus 5: 645
  address.SetBase("192.168.6.0","255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces6;
  csmaInterfaces6 = address.Assign (csmaDevices645);
  //Bus 6: 646
  address.SetBase("192.168.6.0","255.255.255.0","0.0.0.10");
  Ipv4InterfaceContainer csmaInterfaces6_1;
  csmaInterfaces6_1 = address.Assign (csmaDevices646);
  //Bus 9: 671
  address.SetBase("192.168.7.0","255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces7;
  csmaInterfaces7 = address.Assign (csmaDevices671);
  //Bus 10: 675
  address.SetBase("192.168.8.0","255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces8;
  csmaInterfaces8 = address.Assign (csmaDevices675);
  //Bus 11: 680
  address.SetBase("192.168.9.0","255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces9;
  csmaInterfaces9 = address.Assign (csmaDevices680);
  //Bus 12: 684
  address.SetBase("192.168.10.0","255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces10;
  csmaInterfaces10 = address.Assign (csmaDevices684);
  //Bus 13: 692
  address.SetBase("11.11.11.0","255.255.255.0","0.0.0.3");
  Ipv4InterfaceContainer csmaInterfaces11;
  csmaInterfaces11 = address.Assign (csmaDevices692);
  //CC
  address.SetBase("10.10.10.0","255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces12;
  csmaInterfaces12 = address.Assign (csmaDevicesCC);

  
  TapBridgeHelper tapBridge;
  tapBridge.SetAttribute ("Mode", StringValue (mode));
  tapBridge.SetAttribute ("DeviceName", StringValue (tapName));
  tapBridge.Install (csmaNodes692.Get(0),csmaDevices692.Get (0));
  
  TapBridgeHelper tapBridge1;
  tapBridge1.SetAttribute("Mode", StringValue (mode));
  tapBridge1.SetAttribute ("DeviceName", StringValue (tapName1));
  tapBridge1.Install (csmaNodes671.Get(0),csmaDevices671.Get (0));
  
  TapBridgeHelper tapBridge2;
  tapBridge2.SetAttribute("Mode", StringValue (mode));
  tapBridge2.SetAttribute ("DeviceName", StringValue (tapName2));
  tapBridge2.Install (csmaNodes611.Get(0),csmaDevices611.Get (0));

  TapBridgeHelper tapBridge3;
  tapBridge3.SetAttribute("Mode", StringValue (mode));
  tapBridge3.SetAttribute ("DeviceName", StringValue (tapName3));
  tapBridge3.Install (csmaNodes632.Get(0),csmaDevices632.Get (0));

  TapBridgeHelper tapBridge4;
  tapBridge4.SetAttribute("Mode", StringValue (mode));
  tapBridge4.SetAttribute ("DeviceName", StringValue (tapName4));
  tapBridge4.Install (csmaNodes634.Get(0),csmaDevices634.Get (0));

  TapBridgeHelper tapBridge5;
  tapBridge5.SetAttribute("Mode", StringValue (mode));
  tapBridge5.SetAttribute ("DeviceName", StringValue (tapName5));
  tapBridge5.Install (csmaNodes650.Get(0),csmaDevices650.Get (0));

  TapBridgeHelper tapBridge6;
  tapBridge6.SetAttribute("Mode", StringValue (mode));
  tapBridge6.SetAttribute ("DeviceName", StringValue (tapName6));
  tapBridge6.Install (csmaNodes652.Get(0),csmaDevices652.Get (0));

  TapBridgeHelper tapBridge7;
  tapBridge7.SetAttribute("Mode", StringValue (mode));
  tapBridge7.SetAttribute ("DeviceName", StringValue (tapName7));
  tapBridge7.Install (csmaNodes633.Get(0),csmaDevices633.Get (0));

  TapBridgeHelper tapBridge8;
  tapBridge8.SetAttribute("Mode", StringValue (mode));
  tapBridge8.SetAttribute ("DeviceName", StringValue (tapName8));
  tapBridge8.Install (csmaNodes645.Get(0),csmaDevices645.Get (0));

  TapBridgeHelper tapBridge9;
  tapBridge9.SetAttribute("Mode", StringValue (mode));
  tapBridge9.SetAttribute ("DeviceName", StringValue (tapName9));
  tapBridge9.Install (csmaNodes646.Get(0),csmaDevices646.Get (0));

  TapBridgeHelper tapBridge10;
  tapBridge10.SetAttribute("Mode", StringValue (mode));
  tapBridge10.SetAttribute ("DeviceName", StringValue (tapName10));
  tapBridge10.Install (csmaNodes675.Get(0),csmaDevices675.Get (0));

  TapBridgeHelper tapBridge11;
  tapBridge11.SetAttribute("Mode", StringValue (mode));
  tapBridge11.SetAttribute ("DeviceName", StringValue (tapName11));
  tapBridge11.Install (csmaNodes680.Get(0),csmaDevices680.Get (0));

  TapBridgeHelper tapBridge12;
  tapBridge12.SetAttribute("Mode", StringValue (mode));
  tapBridge12.SetAttribute ("DeviceName", StringValue (tapName12));
  tapBridge12.Install (csmaNodes684.Get(0),csmaDevices684.Get (0));

  TapBridgeHelper tapBridgeCC;
  tapBridgeCC.SetAttribute("Mode", StringValue (mode));
  tapBridgeCC.SetAttribute ("DeviceName", StringValue (tapNameCC));
  tapBridgeCC.Install (csmaNodesCC.Get(0),csmaDevicesCC.Get (0));

  

 
 /* 
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  
  Ptr<Node> node1 = p2pNodes9.Get(0);
  Ptr<Node> node2 = p2pNodes9.Get(1);

  Ptr<Ipv4> node1_addr = node1->GetObject<Ipv4> ();
  Ipv4Address node1_if1 = node1_addr->GetAddress(1,0).GetLocal();
  Ipv4Address node1_if2 = node1_addr->GetAddress(2,0).GetLocal();
  std::cout << "p2p node 1 interface one: "<<node1_if1 << "p2p node 1 interface two:"<<node1_if2<<std::endl;

  Ptr<Ipv4> node2_addr = node2->GetObject<Ipv4> ();
  Ipv4Address node2_if1 = node2_addr->GetAddress(1,0).GetLocal();
  Ipv4Address node2_if2 = node2_addr->GetAddress(2,0).GetLocal();
  std::cout << "p2p node 2 interface one: "<<node2_if1 << "p2p node 2 interface two:"<<node2_if2<<std::endl;

  Ptr<Ipv4StaticRouting> p2pnode1 = ipv4RoutingHelper.GetStaticRouting (node1->GetObject<Ipv4>());
  p2pnode1->AddNetworkRouteTo (Ipv4Address ("10.10.10.0"), Ipv4Mask ("255.255.255.0"), 1);
  
  Ptr<Ipv4StaticRouting> p2pnode2 = ipv4RoutingHelper.GetStaticRouting (node2->GetObject<Ipv4>());
  p2pnode2->AddNetworkRouteTo (Ipv4Address ("11.11.11.0"), Ipv4Mask ("255.255.255.0"), 1);
  */
  /*The above static route in the p2pNodes.Get(1) allowed 11.11.11.32 PC to communicate with 9.9.9.0 network*/

//  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
//have to define static routes...because PopulateRoutingTables() would not work for such complex topology

//// p2pDevices12_2,p2pDevices2_3,p2pDevices2_6,p2pDevices3_6,p2pDevices3_4,p2pDevices3_5,p2pDevices6_9,p2pDevices6_10,p2pDevices6_8,p2pDevices6_4,p2pDevices9_10, //p2pDevices10_11,p2pDevices4_11,p2pDevices8_7,p2pDevices7_4,p2pDevices5_4
Ipv4StaticRoutingHelper ipv4RoutingHelper;
  
  Ptr<Node> CC = p2pNode12_2.Get(0);
  Ptr<Node> N0 = p2pNode2_3.Get(0);
  Ptr<Node> N1 = p2pNode3_6.Get(0);
  Ptr<Node> N2 = p2pNode5_4.Get(0);
  Ptr<Node> N3 = p2pNode4_11.Get(0);
  Ptr<Node> N4 = p2pNode6_10.Get(0);
  Ptr<Node> N5 = p2pNode7_4.Get(0);
  Ptr<Node> N6 = p2pNode8_7.Get(0);
  Ptr<Node> N7 = p2pNode9_10.Get(0);
  Ptr<Node> N8 = p2pNode10_11.Get(0);
  Ptr<Node> N9 = p2pNode10_11.Get(1);
  

  Ptr<Ipv4> N0_addr = N0->GetObject<Ipv4> ();
  Ipv4Address N0_if1 = N0_addr->GetAddress(1,0).GetLocal();
  Ipv4Address N0_if2 = N0_addr->GetAddress(2,0).GetLocal();
  Ipv4Address N0_if3 = N0_addr->GetAddress(3,0).GetLocal();
  Ipv4Address N0_if4 = N0_addr->GetAddress(4,0).GetLocal();
  std::cout << "N0 interface one: "<<N0_if1 << "N0 interface two: "<<N0_if2 <<"N0 interface three: "<<N0_if3<<"N0 interface four: "<<N0_if4<<std::endl;

  Ptr<Ipv4> N1_addr = N1->GetObject<Ipv4> ();
  Ipv4Address N1_if1 = N1_addr->GetAddress(1,0).GetLocal();
  Ipv4Address N1_if2 = N1_addr->GetAddress(2,0).GetLocal();
  Ipv4Address N1_if3 = N1_addr->GetAddress(3,0).GetLocal();
  Ipv4Address N1_if4 = N1_addr->GetAddress(4,0).GetLocal();
  Ipv4Address N1_if5 = N1_addr->GetAddress(5,0).GetLocal();
  std::cout << "N1 interface one: "<<N1_if1 << "N1 interface two: "<<N1_if2 <<"N1 interface three: "<<N1_if3<<"N1 interface four: "<<N1_if4<<"N1 interface five: "<<N1_if5<<std::endl;

  Ptr<Ipv4> N2_addr = N2->GetObject<Ipv4> ();
  Ipv4Address N2_if1 = N2_addr->GetAddress(1,0).GetLocal();
  Ipv4Address N2_if2 = N2_addr->GetAddress(2,0).GetLocal();
  Ipv4Address N2_if3 = N2_addr->GetAddress(3,0).GetLocal();
  std::cout << "N2 interface one: "<<N2_if1 << "N2 interface two: "<<N2_if2 <<"N2 interface three: "<<N2_if3<<std::endl;

  Ptr<Ipv4> N3_addr = N3->GetObject<Ipv4> ();
  Ipv4Address N3_if1 = N3_addr->GetAddress(1,0).GetLocal();
  Ipv4Address N3_if2 = N3_addr->GetAddress(2,0).GetLocal();
  Ipv4Address N3_if3 = N3_addr->GetAddress(3,0).GetLocal();
  Ipv4Address N3_if4 = N3_addr->GetAddress(4,0).GetLocal();
  Ipv4Address N3_if5 = N3_addr->GetAddress(5,0).GetLocal();
  Ipv4Address N3_if6 = N3_addr->GetAddress(6,0).GetLocal();
  std::cout << "N3 interface one: "<<N3_if1 << "N3 interface two: "<<N3_if2 <<"N3 interface three: "<<N3_if3<<"N3 interface four: "<<N3_if4<<"N3 interface five: "<<N3_if5<<"N3 interface six: "<<N3_if6<<std::endl;
  
  Ptr<Ipv4> N4_addr = N4->GetObject<Ipv4> ();
  Ipv4Address N4_if1 = N4_addr->GetAddress(1,0).GetLocal();
  Ipv4Address N4_if2 = N4_addr->GetAddress(2,0).GetLocal();
  Ipv4Address N4_if3 = N4_addr->GetAddress(3,0).GetLocal();
  Ipv4Address N4_if4 = N4_addr->GetAddress(4,0).GetLocal();
  Ipv4Address N4_if5 = N4_addr->GetAddress(5,0).GetLocal();
  Ipv4Address N4_if6 = N4_addr->GetAddress(6,0).GetLocal();
  Ipv4Address N4_if7 = N4_addr->GetAddress(7,0).GetLocal();
  std::cout << "N4 interface one: "<<N4_if1 << "N4 interface two: "<<N4_if2 <<"N4 interface three: "<<N4_if3<<"N4 interface four: "<<N4_if4<<"N4 interface five: "<<N4_if5<<"N4 interface six: "<<N4_if6<<"N4 interface seven: "<<N4_if7<<std::endl;

  Ptr<Ipv4> N5_addr = N5->GetObject<Ipv4> ();
  Ipv4Address N5_if1 = N5_addr->GetAddress(1,0).GetLocal();
  Ipv4Address N5_if2 = N5_addr->GetAddress(2,0).GetLocal();
  Ipv4Address N5_if3 = N5_addr->GetAddress(3,0).GetLocal();
  std::cout << "N5 interface one: "<<N5_if1 << "N5 interface two: "<<N5_if2 <<"N5 interface three: "<<N5_if3<<std::endl;

  Ptr<Ipv4> N6_addr = N6->GetObject<Ipv4> ();
  Ipv4Address N6_if1 = N6_addr->GetAddress(1,0).GetLocal();
  Ipv4Address N6_if2 = N6_addr->GetAddress(2,0).GetLocal();
  Ipv4Address N6_if3 = N6_addr->GetAddress(3,0).GetLocal();
  std::cout << "N6 interface one: "<<N6_if1 << "N6 interface two: "<<N6_if2 <<"N5 interface three: "<<N6_if3<<std::endl;

  Ptr<Ipv4> N7_addr = N7->GetObject<Ipv4> ();
  Ipv4Address N7_if1 = N7_addr->GetAddress(1,0).GetLocal();
  Ipv4Address N7_if2 = N7_addr->GetAddress(2,0).GetLocal();
  Ipv4Address N7_if3 = N7_addr->GetAddress(3,0).GetLocal();
  std::cout << "N7 interface one: "<<N7_if1 << "N7 interface two: "<<N7_if2 <<"N7 interface three: "<<N7_if3<<std::endl;

  Ptr<Ipv4> N8_addr = N8->GetObject<Ipv4> ();
  Ipv4Address N8_if1 = N8_addr->GetAddress(1,0).GetLocal();
  Ipv4Address N8_if2 = N8_addr->GetAddress(2,0).GetLocal();
  Ipv4Address N8_if3 = N8_addr->GetAddress(3,0).GetLocal();
  Ipv4Address N8_if4 = N8_addr->GetAddress(4,0).GetLocal();
  std::cout << "N8 interface one: "<<N8_if1 << "N8 interface two: "<<N8_if2 <<"N8 interface three: "<<N8_if3<<"N8 interface four: "<<N8_if4<<std::endl;

  Ptr<Ipv4> N9_addr = N9->GetObject<Ipv4> ();
  Ipv4Address N9_if1 = N9_addr->GetAddress(1,0).GetLocal();
  Ipv4Address N9_if2 = N9_addr->GetAddress(2,0).GetLocal();
  Ipv4Address N9_if3 = N9_addr->GetAddress(3,0).GetLocal();
  std::cout << "N9 interface one: "<<N9_if1 << "N9 interface two: "<<N9_if2 <<"N9 interface three: "<<N9_if3 <<std::endl;

//defining static route for each nodes created above
  Ptr<Ipv4StaticRouting> CC_RT = ipv4RoutingHelper.GetStaticRouting (CC->GetObject<Ipv4>());
  CC_RT->AddNetworkRouteTo (Ipv4Address ("11.11.11.0"), Ipv4Mask ("255.255.255.0"), 1);
  CC_RT->AddNetworkRouteTo (Ipv4Address ("192.168.2.0"), Ipv4Mask ("255.255.255.0"), 1);
  CC_RT->AddNetworkRouteTo (Ipv4Address ("192.168.3.0"), Ipv4Mask ("255.255.255.0"), 1);
  CC_RT->AddNetworkRouteTo (Ipv4Address ("192.168.4.0"), Ipv4Mask ("255.255.255.0"), 1);
  CC_RT->AddNetworkRouteTo (Ipv4Address ("192.168.5.0"), Ipv4Mask ("255.255.255.0"), 1);
  CC_RT->AddNetworkRouteTo (Ipv4Address ("192.168.6.0"), Ipv4Mask ("255.255.255.0"), 1);
  CC_RT->AddNetworkRouteTo (Ipv4Address ("192.168.7.0"), Ipv4Mask ("255.255.255.0"), 1);
  CC_RT->AddNetworkRouteTo (Ipv4Address ("192.168.8.0"), Ipv4Mask ("255.255.255.0"), 1);
  CC_RT->AddNetworkRouteTo (Ipv4Address ("192.168.9.0"), Ipv4Mask ("255.255.255.0"), 1);
  CC_RT->AddNetworkRouteTo (Ipv4Address ("192.168.10.0"), Ipv4Mask ("255.255.255.0"), 1);
  
  Ptr<Ipv4StaticRouting> N0_RT = ipv4RoutingHelper.GetStaticRouting (N0->GetObject<Ipv4>());
  N0_RT->AddNetworkRouteTo (Ipv4Address ("11.11.11.0"), Ipv4Mask ("255.255.255.0"), 3);
  N0_RT->AddNetworkRouteTo (Ipv4Address ("192.168.3.0"), Ipv4Mask ("255.255.255.0"), 2);
  N0_RT->AddNetworkRouteTo (Ipv4Address ("192.168.4.0"), Ipv4Mask ("255.255.255.0"), 3);
  N0_RT->AddNetworkRouteTo (Ipv4Address ("192.168.5.0"), Ipv4Mask ("255.255.255.0"), 2);
  N0_RT->AddNetworkRouteTo (Ipv4Address ("192.168.6.0"), Ipv4Mask ("255.255.255.0"), 3);
  N0_RT->AddNetworkRouteTo (Ipv4Address ("192.168.7.0"), Ipv4Mask ("255.255.255.0"), 3);
  N0_RT->AddNetworkRouteTo (Ipv4Address ("192.168.8.0"), Ipv4Mask ("255.255.255.0"), 3);
  N0_RT->AddNetworkRouteTo (Ipv4Address ("192.168.9.0"), Ipv4Mask ("255.255.255.0"), 3);
  N0_RT->AddNetworkRouteTo (Ipv4Address ("192.168.10.0"), Ipv4Mask ("255.255.255.0"), 3);
  N0_RT->AddNetworkRouteTo (Ipv4Address ("10.10.10.0"), Ipv4Mask ("255.255.255.0"), 1);

  Ptr<Ipv4StaticRouting> N1_RT = ipv4RoutingHelper.GetStaticRouting (N1->GetObject<Ipv4>());
  N1_RT->AddNetworkRouteTo (Ipv4Address ("11.11.11.0"), Ipv4Mask ("255.255.255.0"), 3);
  N1_RT->AddNetworkRouteTo (Ipv4Address ("192.168.2.0"), Ipv4Mask ("255.255.255.0"), 1);
  N1_RT->AddNetworkRouteTo (Ipv4Address ("192.168.4.0"), Ipv4Mask ("255.255.255.0"), 3);
  N1_RT->AddNetworkRouteTo (Ipv4Address ("192.168.5.0"), Ipv4Mask ("255.255.255.0"), 4);
  N1_RT->AddNetworkRouteTo (Ipv4Address ("192.168.6.0"), Ipv4Mask ("255.255.255.0"), 2);
  N1_RT->AddNetworkRouteTo (Ipv4Address ("192.168.7.0"), Ipv4Mask ("255.255.255.0"), 2);
  N1_RT->AddNetworkRouteTo (Ipv4Address ("192.168.8.0"), Ipv4Mask ("255.255.255.0"), 2);
  N1_RT->AddNetworkRouteTo (Ipv4Address ("192.168.9.0"), Ipv4Mask ("255.255.255.0"), 2);
  N1_RT->AddNetworkRouteTo (Ipv4Address ("192.168.10.0"), Ipv4Mask ("255.255.255.0"), 2);
  N1_RT->AddNetworkRouteTo (Ipv4Address ("10.10.10.0"), Ipv4Mask ("255.255.255.0"), 1);
 
  Ptr<Ipv4StaticRouting> N2_RT = ipv4RoutingHelper.GetStaticRouting (N2->GetObject<Ipv4>());
  N2_RT->AddNetworkRouteTo (Ipv4Address ("11.11.11.0"), Ipv4Mask ("255.255.255.0"), 2);
  N2_RT->AddNetworkRouteTo (Ipv4Address ("192.168.2.0"), Ipv4Mask ("255.255.255.0"), 2);
  N2_RT->AddNetworkRouteTo (Ipv4Address ("192.168.3.0"), Ipv4Mask ("255.255.255.0"), 1);
  N2_RT->AddNetworkRouteTo (Ipv4Address ("192.168.4.0"), Ipv4Mask ("255.255.255.0"), 2);
  N2_RT->AddNetworkRouteTo (Ipv4Address ("192.168.6.0"), Ipv4Mask ("255.255.255.0"), 2);
  N2_RT->AddNetworkRouteTo (Ipv4Address ("192.168.7.0"), Ipv4Mask ("255.255.255.0"), 2);
  N2_RT->AddNetworkRouteTo (Ipv4Address ("192.168.8.0"), Ipv4Mask ("255.255.255.0"), 2);
  N2_RT->AddNetworkRouteTo (Ipv4Address ("192.168.9.0"), Ipv4Mask ("255.255.255.0"), 2);
  N2_RT->AddNetworkRouteTo (Ipv4Address ("192.168.10.0"), Ipv4Mask ("255.255.255.0"), 2);
  N2_RT->AddNetworkRouteTo (Ipv4Address ("10.10.10.0"), Ipv4Mask ("255.255.255.0"), 1);

  Ptr<Ipv4StaticRouting> N3_RT = ipv4RoutingHelper.GetStaticRouting (N3->GetObject<Ipv4>());
  N3_RT->AddNetworkRouteTo (Ipv4Address ("11.11.11.0"), Ipv4Mask ("255.255.255.0"), 3);
  N3_RT->AddNetworkRouteTo (Ipv4Address ("192.168.2.0"), Ipv4Mask ("255.255.255.0"), 2);
  N3_RT->AddNetworkRouteTo (Ipv4Address ("192.168.3.0"), Ipv4Mask ("255.255.255.0"), 1);
  N3_RT->AddNetworkRouteTo (Ipv4Address ("192.168.5.0"), Ipv4Mask ("255.255.255.0"), 5);
  N3_RT->AddNetworkRouteTo (Ipv4Address ("192.168.6.0"), Ipv4Mask ("255.255.255.0"), 2);
  N3_RT->AddNetworkRouteTo (Ipv4Address ("192.168.7.0"), Ipv4Mask ("255.255.255.0"), 4);
  N3_RT->AddNetworkRouteTo (Ipv4Address ("192.168.8.0"), Ipv4Mask ("255.255.255.0"), 4);
  N3_RT->AddNetworkRouteTo (Ipv4Address ("192.168.9.0"), Ipv4Mask ("255.255.255.0"), 2);
  N3_RT->AddNetworkRouteTo (Ipv4Address ("192.168.10.0"), Ipv4Mask ("255.255.255.0"), 3);
  N3_RT->AddNetworkRouteTo (Ipv4Address ("10.10.10.0"), Ipv4Mask ("255.255.255.0"), 2);

  Ptr<Ipv4StaticRouting> N4_RT = ipv4RoutingHelper.GetStaticRouting (N4->GetObject<Ipv4>());
  N4_RT->AddNetworkRouteTo (Ipv4Address ("11.11.11.0"), Ipv4Mask ("255.255.255.0"), 4);
  N4_RT->AddNetworkRouteTo (Ipv4Address ("192.168.2.0"), Ipv4Mask ("255.255.255.0"), 1);
  N4_RT->AddNetworkRouteTo (Ipv4Address ("192.168.3.0"), Ipv4Mask ("255.255.255.0"), 2);
  N4_RT->AddNetworkRouteTo (Ipv4Address ("192.168.4.0"), Ipv4Mask ("255.255.255.0"), 6);
  N4_RT->AddNetworkRouteTo (Ipv4Address ("192.168.5.0"), Ipv4Mask ("255.255.255.0"), 6);
  N4_RT->AddNetworkRouteTo (Ipv4Address ("192.168.7.0"), Ipv4Mask ("255.255.255.0"), 5);
  N4_RT->AddNetworkRouteTo (Ipv4Address ("192.168.8.0"), Ipv4Mask ("255.255.255.0"), 5);
  N4_RT->AddNetworkRouteTo (Ipv4Address ("192.168.9.0"), Ipv4Mask ("255.255.255.0"), 3);
  N4_RT->AddNetworkRouteTo (Ipv4Address ("192.168.10.0"), Ipv4Mask ("255.255.255.0"), 4);
  N4_RT->AddNetworkRouteTo (Ipv4Address ("10.10.10.0"), Ipv4Mask ("255.255.255.0"), 1);

  Ptr<Ipv4StaticRouting> N5_RT = ipv4RoutingHelper.GetStaticRouting (N5->GetObject<Ipv4>());
  N5_RT->AddNetworkRouteTo (Ipv4Address ("11.11.11.0"), Ipv4Mask ("255.255.255.0"), 2);
  N5_RT->AddNetworkRouteTo (Ipv4Address ("192.168.2.0"), Ipv4Mask ("255.255.255.0"), 1);
  N5_RT->AddNetworkRouteTo (Ipv4Address ("192.168.3.0"), Ipv4Mask ("255.255.255.0"), 2);
  N5_RT->AddNetworkRouteTo (Ipv4Address ("192.168.4.0"), Ipv4Mask ("255.255.255.0"), 2);
  N5_RT->AddNetworkRouteTo (Ipv4Address ("192.168.5.0"), Ipv4Mask ("255.255.255.0"), 2);
  N5_RT->AddNetworkRouteTo (Ipv4Address ("192.168.6.0"), Ipv4Mask ("255.255.255.0"), 1);
  N5_RT->AddNetworkRouteTo (Ipv4Address ("192.168.8.0"), Ipv4Mask ("255.255.255.0"), 1);
  N5_RT->AddNetworkRouteTo (Ipv4Address ("192.168.9.0"), Ipv4Mask ("255.255.255.0"), 1);
  N5_RT->AddNetworkRouteTo (Ipv4Address ("192.168.10.0"), Ipv4Mask ("255.255.255.0"), 1);
  N5_RT->AddNetworkRouteTo (Ipv4Address ("10.10.10.0"), Ipv4Mask ("255.255.255.0"), 1);

  Ptr<Ipv4StaticRouting> N6_RT = ipv4RoutingHelper.GetStaticRouting (N6->GetObject<Ipv4>());
  N6_RT->AddNetworkRouteTo (Ipv4Address ("11.11.11.0"), Ipv4Mask ("255.255.255.0"), 2);
  N6_RT->AddNetworkRouteTo (Ipv4Address ("192.168.2.0"), Ipv4Mask ("255.255.255.0"), 1);
  N6_RT->AddNetworkRouteTo (Ipv4Address ("192.168.3.0"), Ipv4Mask ("255.255.255.0"), 1);
  N6_RT->AddNetworkRouteTo (Ipv4Address ("192.168.4.0"), Ipv4Mask ("255.255.255.0"), 2);
  N6_RT->AddNetworkRouteTo (Ipv4Address ("192.168.5.0"), Ipv4Mask ("255.255.255.0"), 2);
  N6_RT->AddNetworkRouteTo (Ipv4Address ("192.168.6.0"), Ipv4Mask ("255.255.255.0"), 1);
  N6_RT->AddNetworkRouteTo (Ipv4Address ("192.168.7.0"), Ipv4Mask ("255.255.255.0"), 2);
  N6_RT->AddNetworkRouteTo (Ipv4Address ("192.168.9.0"), Ipv4Mask ("255.255.255.0"), 1);
  N6_RT->AddNetworkRouteTo (Ipv4Address ("192.168.10.0"), Ipv4Mask ("255.255.255.0"), 1);
  N6_RT->AddNetworkRouteTo (Ipv4Address ("10.10.10.0"), Ipv4Mask ("255.255.255.0"), 1);

  Ptr<Ipv4StaticRouting> N7_RT = ipv4RoutingHelper.GetStaticRouting (N7->GetObject<Ipv4>());
  N7_RT->AddNetworkRouteTo (Ipv4Address ("11.11.11.0"), Ipv4Mask ("255.255.255.0"), 2);
  N7_RT->AddNetworkRouteTo (Ipv4Address ("192.168.2.0"), Ipv4Mask ("255.255.255.0"), 1);
  N7_RT->AddNetworkRouteTo (Ipv4Address ("192.168.3.0"), Ipv4Mask ("255.255.255.0"), 1);
  N7_RT->AddNetworkRouteTo (Ipv4Address ("192.168.4.0"), Ipv4Mask ("255.255.255.0"), 2);
  N7_RT->AddNetworkRouteTo (Ipv4Address ("192.168.5.0"), Ipv4Mask ("255.255.255.0"), 1);
  N7_RT->AddNetworkRouteTo (Ipv4Address ("192.168.6.0"), Ipv4Mask ("255.255.255.0"), 1);
  N7_RT->AddNetworkRouteTo (Ipv4Address ("192.168.7.0"), Ipv4Mask ("255.255.255.0"), 1);
  N7_RT->AddNetworkRouteTo (Ipv4Address ("192.168.8.0"), Ipv4Mask ("255.255.255.0"), 1);
  N7_RT->AddNetworkRouteTo (Ipv4Address ("192.168.10.0"), Ipv4Mask ("255.255.255.0"), 2);
  N7_RT->AddNetworkRouteTo (Ipv4Address ("10.10.10.0"), Ipv4Mask ("255.255.255.0"), 1);

  Ptr<Ipv4StaticRouting> N8_RT = ipv4RoutingHelper.GetStaticRouting (N8->GetObject<Ipv4>());
  N8_RT->AddNetworkRouteTo (Ipv4Address ("11.11.11.0"), Ipv4Mask ("255.255.255.0"), 3);
  N8_RT->AddNetworkRouteTo (Ipv4Address ("192.168.2.0"), Ipv4Mask ("255.255.255.0"), 1);
  N8_RT->AddNetworkRouteTo (Ipv4Address ("192.168.3.0"), Ipv4Mask ("255.255.255.0"), 1);
  N8_RT->AddNetworkRouteTo (Ipv4Address ("192.168.4.0"), Ipv4Mask ("255.255.255.0"), 3);
  N8_RT->AddNetworkRouteTo (Ipv4Address ("192.168.5.0"), Ipv4Mask ("255.255.255.0"), 1);
  N8_RT->AddNetworkRouteTo (Ipv4Address ("192.168.6.0"), Ipv4Mask ("255.255.255.0"), 1);
  N8_RT->AddNetworkRouteTo (Ipv4Address ("192.168.7.0"), Ipv4Mask ("255.255.255.0"), 3);
  N8_RT->AddNetworkRouteTo (Ipv4Address ("192.168.8.0"), Ipv4Mask ("255.255.255.0"), 1);
  N8_RT->AddNetworkRouteTo (Ipv4Address ("192.168.9.0"), Ipv4Mask ("255.255.255.0"), 2);
  N8_RT->AddNetworkRouteTo (Ipv4Address ("10.10.10.0"), Ipv4Mask ("255.255.255.0"), 1);

  Ptr<Ipv4StaticRouting> N9_RT = ipv4RoutingHelper.GetStaticRouting (N9->GetObject<Ipv4>());
  N9_RT->AddNetworkRouteTo (Ipv4Address ("192.168.2.0"), Ipv4Mask ("255.255.255.0"), 1);
  N9_RT->AddNetworkRouteTo (Ipv4Address ("192.168.3.0"), Ipv4Mask ("255.255.255.0"), 2);
  N9_RT->AddNetworkRouteTo (Ipv4Address ("192.168.4.0"), Ipv4Mask ("255.255.255.0"), 2);
  N9_RT->AddNetworkRouteTo (Ipv4Address ("192.168.5.0"), Ipv4Mask ("255.255.255.0"), 2);
  N9_RT->AddNetworkRouteTo (Ipv4Address ("192.168.6.0"), Ipv4Mask ("255.255.255.0"), 1);
  N9_RT->AddNetworkRouteTo (Ipv4Address ("192.168.7.0"), Ipv4Mask ("255.255.255.0"), 2);
  N9_RT->AddNetworkRouteTo (Ipv4Address ("192.168.8.0"), Ipv4Mask ("255.255.255.0"), 1);
  N9_RT->AddNetworkRouteTo (Ipv4Address ("192.168.9.0"), Ipv4Mask ("255.255.255.0"), 1);
  N9_RT->AddNetworkRouteTo (Ipv4Address ("192.168.10.0"), Ipv4Mask ("255.255.255.0"), 1);
  N9_RT->AddNetworkRouteTo (Ipv4Address ("10.10.10.0"), Ipv4Mask ("255.255.255.0"), 1);


  Simulator::Stop (Seconds (60000.0));

  if (tracing == true)
    {
      //pointToPoint.EnablePcap("csma-csma-tap", p2pDevices9.Get (0), true);
      csma.EnablePcap ("csma-csma-tap1", csmaDevices671.Get (1), true);
      csma.EnablePcap ("csma-csma-tap2", csmaDevicesCC.Get (1), true);
    }

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
