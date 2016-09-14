//simple Loop test

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

// Default Network Topology
//
// Number of csma and wifi nodes in each can be increased up to 250
//                          |
//                 Rank 0   |   Rank 1
// -------------------------|----------------------------
//   WLAN 11.11.11.0
//                 
//  ================
//  |    |    |    |   
// n5   n6   n7   n0 ---------------- n1   n2 
//                   LAN 9.9.9.0      |    |    
//                                   ========
// ================ ---------------- LAN 10.10.10.0
// LAN 12.12.12.0     LAN 6.6.6.0
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("wifi_csma_tap");

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  //uint32_t i,j;
  uint32_t nCsma11 = 2;
  uint32_t nCsma10 = 3;
  uint32_t nCsma12 = 2;
  uint32_t np2p9 = 2;
  uint32_t np2p6 = 2;
  bool tracing = true;
  std::string mode="UseBridge";
  std::string tapName = "mytap0";
  std::string tapName1="mytap0_0";
  std::string tapName2 = "mytap1";


  CommandLine cmd;
  cmd.AddValue ("nCsma11", "Number of \"extra\" CSMA nodes/devices in LAN 11", nCsma11);
  cmd.AddValue ("nCsma10", "Number of \"extra\" CSMA nodes/devices in LAN 10", nCsma10);
  cmd.AddValue ("nCsma9", "Number of \"extra\" CSMA nodes/devices in LAN 9", np2p9);
  cmd.AddValue ("nCsma6", "Number of \"extra\" CSMA nodes/devices in LAN 6", np2p6);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);
  cmd.AddValue ("mode", "Mode setting of TapBridge", mode);
  cmd.AddValue ("tapName", "Name of the OS tap device", tapName);
  cmd.AddValue ("tapName1", "Name of the OS tap device", tapName1);
  cmd.AddValue ("tapName2","Name of the OS second tap device", tapName2);

  cmd.Parse (argc,argv);

  // Check for valid number of csma or wifi nodes
  // 250 should be enough, otherwise IP addresses 
  // soon become an issue
 

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

  GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::RealtimeSimulatorImpl"));
  GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));
  
  
  NodeContainer csmaNodes10;
  csmaNodes10.Create (nCsma10);

  NodeContainer csmaNodes11;
  csmaNodes11.Create (nCsma11);

  NodeContainer csmaNodes12;
  csmaNodes12.Create (nCsma12);

    //PointToPointHelper and set the associated default Attributes so that we create a 5 Mbps transmitter on devices created using the helper and a 2 msec delay on channels created by the helper. 
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("14ms"));
  //We then Install the devices on the nodes and the channel between them
  NodeContainer p2pNodes9 = NodeContainer (csmaNodes11.Get (1), csmaNodes10.Get (0));
  NetDeviceContainer p2pDevices9 = pointToPoint.Install (p2pNodes9);

  PointToPointHelper pointToPoint2;
  pointToPoint2.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  pointToPoint2.SetChannelAttribute ("Delay", StringValue ("4ms"));
  //We then Install the devices on the nodes and the channel between them
  NodeContainer p2pNodes6 = NodeContainer (csmaNodes12.Get (1), csmaNodes10.Get (1));
  NetDeviceContainer p2pDevices6 = pointToPoint2.Install (p2pNodes6);

  CsmaHelper csma11;
  csma11.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma11.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (10560)));

  NetDeviceContainer csmaDevices11;
  csmaDevices11 = csma11.Install (csmaNodes11);

  CsmaHelper csma10;
  csma10.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma10.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (10560)));

  NetDeviceContainer csmaDevices10;
  csmaDevices10 = csma10.Install (csmaNodes10);

  CsmaHelper csma12;
  csma12.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma12.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (10560)));

  NetDeviceContainer csmaDevices12;
  csmaDevices12 = csma12.Install (csmaNodes12);
 
  //
  // The topology has a Wifi network of four nodes on the left side.  We'll make
  // node zero the AP and have the other three will be the STAs.
  //
  
  InternetStackHelper stack;
  stack.Install(csmaNodes11);
  stack.Install(csmaNodes10);
  stack.Install(csmaNodes12);
  

  Ipv4AddressHelper address;



  address.SetBase ("6.6.6.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces6;
  p2pInterfaces6 = address.Assign (p2pDevices6);

  
  address.SetBase ("9.9.9.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces9;
  p2pInterfaces9 = address.Assign (p2pDevices9);
  //address.SetBase ("11.11.11.0", "255.255.255.0","0.0.0.7");
  address.SetBase ("11.11.11.0","255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces11;
  csmaInterfaces11 = address.Assign (csmaDevices11);

  address.SetBase ("12.12.12.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces12;
  csmaInterfaces12 = address.Assign (csmaDevices12);

  address.SetBase ("10.10.10.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces10;
  csmaInterfaces10 = address.Assign (csmaDevices10);
  
 
  TapBridgeHelper tapBridge;
  tapBridge.SetAttribute ("Mode", StringValue (mode));
  tapBridge.SetAttribute ("DeviceName", StringValue (tapName));
  tapBridge.Install (csmaNodes11.Get(0),csmaDevices11.Get (0));

  TapBridgeHelper tapBridge1;
  tapBridge1.SetAttribute ("Mode", StringValue (mode));
  tapBridge1.SetAttribute ("DeviceName", StringValue (tapName1));
  tapBridge1.Install (csmaNodes12.Get(0),csmaDevices12.Get (0));
  
  TapBridgeHelper tapBridge2;
  tapBridge2.SetAttribute("Mode", StringValue (mode));
  tapBridge2.SetAttribute ("DeviceName", StringValue (tapName2));
  tapBridge2.Install (csmaNodes10.Get (2), csmaDevices10.Get (2));

  
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  
  Ptr<Node> node1 = p2pNodes9.Get(0);
  Ptr<Node> node2 = p2pNodes9.Get(1);

  Ptr<Node> nodeA = p2pNodes6.Get(0);
  Ptr<Node> nodeB = p2pNodes6.Get(1);

  Ptr<Ipv4> node1_addr = node1->GetObject<Ipv4> ();
  Ipv4Address node1_if1 = node1_addr->GetAddress(1,0).GetLocal();
  Ipv4Address node1_if2 = node1_addr->GetAddress(2,0).GetLocal();
  std::cout << "p2p node 1 interface one: "<<node1_if1 << "p2p node 1 interface two:"<<node1_if2<<std::endl;

  Ptr<Ipv4> node2_addr = node2->GetObject<Ipv4> ();
  Ipv4Address node2_if1 = node2_addr->GetAddress(1,0).GetLocal();
  Ipv4Address node2_if2 = node2_addr->GetAddress(2,0).GetLocal();
  //Ipv4Address node2_if3 = node2_addr->GetAddress(3,0).GetLocal();
  std::cout << "p2p node 2 interface one: "<<node2_if1 << "p2p node 2 interface two:"<<node2_if2<<std::endl;

  Ptr<Ipv4> nodeA_addr = nodeA->GetObject<Ipv4> ();
  Ipv4Address nodeA_if1 = nodeA_addr->GetAddress(1,0).GetLocal();
  Ipv4Address nodeA_if2 = nodeA_addr->GetAddress(2,0).GetLocal();
  std::cout << "p2p node A interface one: "<<nodeA_if1 << "p2p node A interface two:"<<nodeA_if2<<std::endl;

  Ptr<Ipv4> nodeB_addr = nodeB->GetObject<Ipv4> ();
  Ipv4Address nodeB_if1 = nodeB_addr->GetAddress(1,0).GetLocal();
  Ipv4Address nodeB_if2 = nodeB_addr->GetAddress(2,0).GetLocal();
  //Ipv4Address nodeB_if3 = nodeB_addr->GetAddress(3,0).GetLocal();
  std::cout << "p2p node B interface one: "<<nodeB_if1 << "p2p node B interface two:"<<nodeB_if2<<std::endl;

  Ptr<Ipv4StaticRouting> p2pnode1 = ipv4RoutingHelper.GetStaticRouting (node1->GetObject<Ipv4>());
  p2pnode1->AddNetworkRouteTo (Ipv4Address ("10.10.10.0"), Ipv4Mask ("255.255.255.0"), 1);
  
  Ptr<Ipv4StaticRouting> p2pnode2 = ipv4RoutingHelper.GetStaticRouting (node2->GetObject<Ipv4>());
  p2pnode2->AddNetworkRouteTo (Ipv4Address ("11.11.11.0"), Ipv4Mask ("255.255.255.0"), 1);

  Ptr<Ipv4StaticRouting> p2pnodeA = ipv4RoutingHelper.GetStaticRouting (nodeA->GetObject<Ipv4>());
  p2pnodeA->AddNetworkRouteTo (Ipv4Address ("10.10.10.0"), Ipv4Mask ("255.255.255.0"), 1);
  
  Ptr<Ipv4StaticRouting> p2pnodeB = ipv4RoutingHelper.GetStaticRouting (nodeB->GetObject<Ipv4>());
  p2pnodeB->AddNetworkRouteTo (Ipv4Address ("12.12.12.0"), Ipv4Mask ("255.255.255.0"), 1);
  
  /*The above static route in the p2pNodes.Get(1) allowed 11.11.11.32 PC to communicate with 9.9.9.0 network*/

  //Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Stop (Seconds (600.0));

  if (tracing == true)
    {
      pointToPoint.EnablePcap("csma-csma9-tap", p2pDevices9.Get (0), true);
      pointToPoint2.EnablePcap("csma-csma6-tap", p2pDevices6.Get (0), true);
      csma10.EnablePcap ("csma-csma10-tap", csmaDevices10.Get (0), true);
      csma11.EnablePcap ("csma11-csma10-tap", csmaDevices11.Get (0), true);
      csma12.EnablePcap ("csma12-csma10-tap", csmaDevices12.Get (0), true);
    }

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
