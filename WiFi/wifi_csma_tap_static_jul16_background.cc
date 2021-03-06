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
//                                     LAN 10.10.10.0

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("wifi_csma_tap");

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  //uint32_t i,j;
  uint32_t nWifi10 = 29,duration = 500;
  uint32_t nCsma11 = 2;
  uint32_t np2p9 = 2;
  bool tracing = true;
  std::string mode = "UseBridge";
  std::string tapName = "mytap0";
  std::string tapName2 = "mytap1";


  CommandLine cmd;
  cmd.AddValue ("nWifi10", "Number of \"extra\" CSMA nodes/devices in LAN 11", nWifi10);
  cmd.AddValue ("nCsma11", "Number of \"extra\" CSMA nodes/devices in LAN 10", nCsma11);
  cmd.AddValue ("nCsma9", "Number of \"extra\" CSMA nodes/devices in LAN 9", np2p9);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);
  cmd.AddValue ("mode", "Mode setting of TapBridge", mode);
  cmd.AddValue ("tapName", "Name of the OS tap device", tapName);
  cmd.AddValue ("tapName2","Name of the OS second tap device", tapName2);

  cmd.Parse (argc,argv);

  // Check for valid number of csma or wifi nodes
  // 250 should be enough, otherwise IP addresses 
  // soon become an issue
  if (nWifi10 > 250 ||  np2p9 != 2 || nCsma11 > 250)
    {
      std::cout << "Too many csma nodes, no more than 250 each." << std::endl;
      return 1;
    }

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

  GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::RealtimeSimulatorImpl"));
  GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));
  
  
  NodeContainer csmaNodes11;
  csmaNodes11.Create (nCsma11);

  NodeContainer wifiNodes10;
  wifiNodes10.Create (nWifi10);

    //PointToPointHelper and set the associated default Attributes so that we create a 5 Mbps transmitter on devices created using the helper and a 2 msec delay on channels created by the helper. 
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("8ms"));
  //We then Install the devices on the nodes and the channel between them
  NodeContainer p2pNodes9 = NodeContainer (csmaNodes11.Get (1), wifiNodes10.Get (1));
  NetDeviceContainer p2pDevices9 = pointToPoint.Install (p2pNodes9);


  CsmaHelper csma11;
  csma11.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma11.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (10560)));

  NetDeviceContainer csmaDevices11;
  csmaDevices11 = csma11.Install (csmaNodes11);
 
  //
  // The topology has a Wifi network of four nodes on the left side.  We'll make
  // node zero the AP and have the other three will be the STAs.
  //
  

  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  wifiPhy.Set ("TxGain", DoubleValue (0) ); 
  wifiPhy.Set ("RxGain", DoubleValue (0) ); 

/*senses the status of the channel (free or busy) through a Clear Channel Assessment (CCA) procedure;
if the channel is free, transmits;
        else, if the channel is busy, then increments
                BE by one, up to a maximum BE level;
if the retransmission number (NB) 
    overpasses a pre-assigned  maximum  threshold,  then  signals  failure, else restarts from the beginning.
        The CCA procedure can be carried out in three different modes:
1.CCA mode 1:  medium is assumed busy if the measured  power  level  (Received  Signal  Strength  Indication,  RSSI)is  higher  than  a       prefixed  threshold,CCA threshold
2.CCA mode 2:medium is assumed busy if at least one signal with the modulation and spreading characteristics of IEEE 802.15.4 is detected.
3.CCA mode 3:medium is assumed busy if both the above assumptions are detected.*/

  wifiPhy.Set ("CcaMode1Threshold", DoubleValue (0.0) );

  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss ("ns3::LogDistancePropagationLossModel");
  wifiPhy.SetChannel (wifiChannel.Create ());

//MAC Layer configuration of WiFi

  Ssid ssid = Ssid ("left");
  WifiHelper wifi;
//The default state is defined as being an Adhoc MAC layer with an ARF rate control algorithm and both objects using their default attribute values. By default, configure MAC and PHY for 802.11a.
//WIFI standards
/*WIFI_PHY_STANDARD_80211a 
  WIFI_PHY_STANDARD_80211b 
  WIFI_PHY_STANDARD_80211g 
  WIFI_PHY_STANDARD_80211_10MHZ 
  WIFI_PHY_STANDARD_80211_5MHZ
  WIFI_PHY_STANDARD_holland 
  WIFI_PHY_STANDARD_80211n_2_4GHZ 
  WIFI_PHY_STANDARD_80211n_5GHZ 
  WIFI_PHY_STANDARD_80211ac */ 

  wifi.SetStandard(WIFI_PHY_STANDARD_80211ac);
  WifiMacHelper wifiMac;

/* There are various rate control Algorithms like AARF,ARF,CARA,ONOE,RRAA,ConstantRate and minstrel:
ARF: sender modifies the transmission rate based on succesful transmission of previous packets
AARF: the threshold on number of succesful transmission triggering fast transission rate is adaptively varied
CARA: collision aware rate adaptation.The key idea of CARA is that the transmitter station combines adaptively
     the Request-to-Send/Clear-to-Send (RTS/CTS) exchange with the Clear  Channel  Assessment  (CCA)  functionality  to  differentiate
     frame  collisions  from  frame  transmission  failures  caused  by channel errors. Therefore, compared with other open-loop rate-
     adaptation  schemes,  CARA  is  more  likely  to  make  the  correct rate adaptation decisions. 
ONOE: Onoe is a credit based RCA where the value of the credit is determined by
      the frequency of successful, erroneous and retransmissions accumulated during
      a fixed invocation period of 1000 ms. If less than 10% of the packets need to be
      retransmitted at a particular rate, Onoe keeps increasing its credit point till the
      threshold value of 10 is reached. At this point, the current transmission rate is
      increased to the next available higher rate and the process repeated with credit score of zero
RRAA:  RRAA uses short-term loss ratio to opportunistically guide its rate change decisions, and
      an adaptive RTS filter to prevent collision losses from triggering rate decrease. 
Minstrel: https://wireless.wiki.kernel.org/en/developers/documentation/mac80211/ratecontrol/minstrel
Constant Rate is a fixed rate control
*/
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager");
//ApWifiMac: 
  wifiMac.SetType ("ns3::ApWifiMac",
                   "Ssid", SsidValue (ssid));
  NetDeviceContainer wifiDevices10 = wifi.Install (wifiPhy, wifiMac, wifiNodes10.Get (0));

//StaWifiMac: The Wifi MAC high model for a non-AP STA in a BSS. 
  wifiMac.SetType ("ns3::StaWifiMac",
                   "Ssid", SsidValue (ssid),
                   "ActiveProbing", BooleanValue (false));
  for(int i=1;i<=28;i++)
 { wifiDevices10.Add (wifi.Install (wifiPhy, wifiMac, NodeContainer (wifiNodes10.Get (i))));}

  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
    "MinX", DoubleValue (0.0),
    "MinY", DoubleValue (0.0),
    "DeltaX", DoubleValue (0.0),
    "DeltaY", DoubleValue (0.0),
    "GridWidth", UintegerValue (3),
    "LayoutType", StringValue ("RowFirst"));
  mobility.Install (wifiNodes10.Get(0));

for(int i = 1 ; i<=28;i++)
  {mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
    "MinX", DoubleValue (0.0),
    "MinY", DoubleValue (0.0),
    "DeltaX", DoubleValue (10*i),
    "DeltaY", DoubleValue (10*i),
    "GridWidth", UintegerValue (3),
    "LayoutType", StringValue ("RowFirst"));
  mobility.Install (wifiNodes10.Get(i));}

  
  InternetStackHelper stack;
  stack.Install(wifiNodes10);
  stack.Install(csmaNodes11);
  

  Ipv4AddressHelper address;

  address.SetBase ("9.9.9.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces9;
  p2pInterfaces9 = address.Assign (p2pDevices9);

  //address.SetBase ("11.11.11.0", "255.255.255.0","0.0.0.7");
  address.SetBase ("10.10.10.0","255.255.255.0","0.0.0.20");
  Ipv4InterfaceContainer WifiInterfaces10;
  WifiInterfaces10 = address.Assign (wifiDevices10);

  address.SetBase ("11.11.11.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces11;
  csmaInterfaces11 = address.Assign (csmaDevices11);
  
  
  TapBridgeHelper tapBridge;
  tapBridge.SetAttribute ("Mode", StringValue (mode));
  tapBridge.SetAttribute ("DeviceName", StringValue (tapName));
  tapBridge.Install (csmaNodes11.Get(0),csmaDevices11.Get (0));
  
  TapBridgeHelper tapBridge2;
  tapBridge2.SetAttribute("Mode", StringValue (mode));
  tapBridge2.SetAttribute ("DeviceName", StringValue (tapName2));
  tapBridge2.Install (wifiNodes10.Get (0), wifiDevices10.Get (0));
  
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
  /*The above static route in the p2pNodes.Get(1) allowed 11.11.11.32 PC to communicate with 9.9.9.0 network*/

  //Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
   /*background traffic*/
  UdpServerHelper* udpServer = new UdpServerHelper[17];
  ApplicationContainer* serverApps = new ApplicationContainer[17];
  UdpClientHelper* udpClient = new UdpClientHelper[17];
  ApplicationContainer* clientApps = new ApplicationContainer[17];

  for (int i = 0; i <  13; i++)
    {
      // set server port to 100+(i*10)
      udpServer[i] = UdpServerHelper (100 + (i * 10));
      serverApps[i] = udpServer[i].Install (wifiNodes10.Get (i+2));
      serverApps[i].Start (Seconds (6));
      serverApps[i].Stop (Seconds (duration));

      udpClient[i] = UdpClientHelper (WifiInterfaces10.GetAddress (i+2), 100 + (i * 10));
      udpClient[i].SetAttribute ("MaxPackets", UintegerValue (1200));
      udpClient[i].SetAttribute ("Interval", TimeValue (Seconds (2)));
      udpClient[i].SetAttribute ("PacketSize", UintegerValue (800));

      clientApps[i] = udpClient[i].Install (wifiNodes10.Get (i + 15));
      clientApps[i].Start (Seconds (6));
      clientApps[i].Stop (Seconds (duration));
    }
  Simulator::Stop (Seconds (600.0));

  if (tracing == true)
    {
      //pointToPoint.EnablePcap("wifi-csma-tap-jul11", p2pDevices9.Get (0), true);
      csma11.EnablePcap ("34nodescsma", csmaDevices11.Get (0), true);
      
      wifiPhy.EnablePcap ("34nodesap", wifiDevices10.Get (0), true);
      wifiPhy.EnablePcap ("34nodessta", wifiDevices10.Get (1), true);
    }

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
