/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright (c) 2007,2008, 2009 INRIA, UDcast
 *
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
 *
 * Author: Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 *                              <amine.ismail@udcast.com>
 */

//
// Default network topology includes a base station (BS) and some number of
// subscriber station (SS) specified by the variable nbSS (defaults to six).
// The SSs are grouped into two groups: senders and receivers. SSs from 0 to
// nbSS/2 are designed as senders and SSs from (nbSS/2 + 1) to nbSS-1 will
// designed as receivers.
// Each SS creates 3 transport connection with the BS. the fist one has as QoS
// scheduling type UGS, the second one rtPS and the third one BE.
// Senders SSs send some stamped IP packets with variable bitrate to receiver SSs
// through the BS station. receiver SSs receive the IP packets analyze them and
// based on a sequnece number decide if all the packets are coorectly received

//                       +-----+    +-----+    +-----+
//                       | SS0 |    | SS1 |    | SS2 |
//                       +-----+    +-----+    +-----+
//                       10.10.10.1   10.10.10.2   10.10.10.3
//                       --------  --------    -------
//                        ((*))    ((*))       ((*))
//
//                                 10.10.10.7
//                          p2p    +------------+
// CSMA 11.11.11.0 ================|Base Station| ==((*))
//                       9.9.9.0   +------------+
//
//                        ((*))    ((*))       ((*))
//                        -------   --------   --------
//                        10.10.10.4   10.10.10.5   10.10.10.6
//                         +-----+    +-----+    +-----+
//                         | SS3 |    | SS4 |    | SS5 |
//                         +-----+    +-----+    +-----+

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/wimax-module.h"
#include "ns3/internet-module.h"
#include "ns3/global-route-manager.h"
#include "ns3/tap-bridge-module.h"
#include <iostream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("wimaxIpV4Simulation");

int main (int argc, char *argv[])
{
  // default values
  int nbSS = 6, duration = 299, schedType = 0,nCsma11=2;
  bool verbose = false;
  WimaxHelper::SchedulerType scheduler = WimaxHelper::SCHED_TYPE_SIMPLE;
  std::string mode = "UseBridge";
  std::string tapName = "mytap0";
  std::string tapName2 = "mytap1";
  LogComponentEnable ("UdpClient", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpServer", LOG_LEVEL_INFO);


  CommandLine cmd;
  cmd.AddValue ("nbSS", "number of subscriber station to create", nbSS);
  cmd.AddValue ("scheduler", "type of scheduler to use with the network devices", schedType);
  cmd.AddValue ("duration", "duration of the simulation in seconds", duration);
  cmd.AddValue ("verbose", "turn on all WimaxNetDevice log components", verbose);
  cmd.AddValue ("mode", "Mode setting of TapBridge", mode);
  cmd.AddValue ("tapName", "Name of the OS tap device", tapName);
  cmd.AddValue ("tapName2", "Name of the OS tap device", tapName2);
  cmd.Parse (argc, argv);

  GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::RealtimeSimulatorImpl"));
  GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

  switch (schedType)
    {
    case 0:
      scheduler = WimaxHelper::SCHED_TYPE_SIMPLE;
      break;
    case 1:
      scheduler = WimaxHelper::SCHED_TYPE_MBQOS;
      break;
    case 2:
      scheduler = WimaxHelper::SCHED_TYPE_RTPS;
      break;
    default:
      scheduler = WimaxHelper::SCHED_TYPE_SIMPLE;
    }


  NodeContainer ssNodes;
  NodeContainer bsNodes;

  ssNodes.Create (nbSS);
  bsNodes.Create (1);

  WimaxHelper wimax;

  NetDeviceContainer ssDevs, bsDevs;

  /*type of schedulers
  The  WiMAX  MAC  layer  is  designed  to  support  different types  of  applications  and  services  having  very  different  QoS requirements
The  adaptive  rtPS (realtime Polling Service)  scheduler is  used  only  for  the  rtPS  QoS  class.  It  is  based  on  the prediction of the rtPS packets arrival. As defined in the IEEE 802.16  standard,  the  BS  allocates  bandwidth  for  rtPS  traffic after  receiving  a  bandwidth  request.  When  the  request  is granted  by  the  BS,  the  subscriber  may  receive  from  upper layers new rtPS packets. These packets will wait for the next
grant  to  be  sent  and,  therefore,  suffer  from  extra  delay.  The basic idea of the adaptive rtPS scheduler is to propose an rtPS
bandwidth  request  process  in  which  the  subscriber  requests time  slots  for  the  data  present  in  the  rtPS  queue  and  also
for the data which will arrive. To estimate the data arrival, a stochastic prediction algorithm is used
  */

  ssDevs = wimax.Install (ssNodes,
                          WimaxHelper::DEVICE_TYPE_SUBSCRIBER_STATION,
                          WimaxHelper::SIMPLE_PHY_TYPE_OFDM,
                          scheduler);
  bsDevs = wimax.Install (bsNodes, WimaxHelper::DEVICE_TYPE_BASE_STATION, WimaxHelper::SIMPLE_PHY_TYPE_OFDM, scheduler);

  Ptr<SubscriberStationNetDevice>* ss = new Ptr<SubscriberStationNetDevice>[nbSS];

  for (int i = 0; i < nbSS; i++)
    {
      ss[i] = ssDevs.Get (i)->GetObject<SubscriberStationNetDevice> ();
      ss[i]->SetModulationType (WimaxPhy::MODULATION_TYPE_QAM16_12);
    }

  Ptr<BaseStationNetDevice> bs;
  bs = bsDevs.Get (0)->GetObject<BaseStationNetDevice> ();

  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
    "MinX", DoubleValue (0.0),
    "MinY", DoubleValue (0.0),
    "DeltaX", DoubleValue (0.0),
    "DeltaY", DoubleValue (0.0),
    "GridWidth", UintegerValue (3),
    "LayoutType", StringValue ("RowFirst"));
  mobility.Install (bsNodes);
for(int i=1;i<=nbSS;i++)
  {
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
    "MinX", DoubleValue (0.0),
    "MinY", DoubleValue (0.0),
    "DeltaX", DoubleValue (200.0*i),
    "DeltaY", DoubleValue (200.0*i),
    "GridWidth", UintegerValue (3),
    "LayoutType", StringValue ("RowFirst"));
  mobility.Install (ssNodes.Get(i-1));
  }

  InternetStackHelper stack;
  stack.Install (bsNodes);
  stack.Install (ssNodes);

  if (verbose)
    {
      wimax.EnableLogComponents ();  // Turn on all wimax logging
    }

  NodeContainer csmaNodes11;
  csmaNodes11.Create (nCsma11);

  CsmaHelper csma11;
  csma11.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma11.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (10560)));

  NetDeviceContainer csmaDevices11;
  csmaDevices11 = csma11.Install (csmaNodes11);

  stack.Install(csmaNodes11);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("8ms"));
  //We then Install the devices on the nodes and the channel between them
  NodeContainer p2pNodes9 = NodeContainer (csmaNodes11.Get (1),bsNodes.Get(0));
  NetDeviceContainer p2pDevices9 = pointToPoint.Install (p2pNodes9);


  NodeContainer csmaNodes10;
  csmaNodes10.Create (1);
  csmaNodes10.Add(ssNodes.Get(nbSS-2));

  CsmaHelper csma10;
  csma10.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma10.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (0)));

  NetDeviceContainer csmaDevices10;
  csmaDevices10 = csma10.Install (csmaNodes10);

  stack.Install(csmaNodes10.Get(0));

  Ipv4AddressHelper address;


  address.SetBase ("9.9.9.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces9;
  p2pInterfaces9 = address.Assign (p2pDevices9);

  address.SetBase ("10.10.10.0", "255.255.255.0");

  Ipv4InterfaceContainer SSinterfaces = address.Assign (ssDevs);
  Ipv4InterfaceContainer BSinterface = address.Assign (bsDevs);

  address.SetBase ("11.11.11.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces11;
  csmaInterfaces11 = address.Assign (csmaDevices11);

  address.SetBase ("10.10.10.0", "255.255.255.0","0.0.0.25");

  Ipv4InterfaceContainer csmaInterfaces10 = address.Assign (csmaDevices10);
  


  TapBridgeHelper tapBridge;
  tapBridge.SetAttribute ("Mode", StringValue (mode));
  tapBridge.SetAttribute ("DeviceName", StringValue (tapName));
  tapBridge.Install (csmaNodes11.Get(0),csmaDevices11.Get (0));

  TapBridgeHelper tapBridge2;
  tapBridge2.SetAttribute ("Mode", StringValue (mode));
  tapBridge2.SetAttribute ("DeviceName", StringValue (tapName2));
  tapBridge2.Install (csmaNodes10.Get(0),csmaDevices10.Get (0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  

  /*background traffic*/
  UdpServerHelper* udpServer = new UdpServerHelper[nbSS / 2];
  ApplicationContainer* serverApps = new ApplicationContainer[nbSS / 2];
  UdpClientHelper* udpClient = new UdpClientHelper[nbSS / 2];
  ApplicationContainer* clientApps = new ApplicationContainer[nbSS / 2];

  for (int i = 0; i < nbSS / 2; i++)
    {
      // set server port to 100+(i*10)
      udpServer[i] = UdpServerHelper (100 + (i * 10));
      serverApps[i] = udpServer[i].Install (ssNodes.Get (i));
      serverApps[i].Start (Seconds (duration-1));
      serverApps[i].Stop (Seconds (duration));

      udpClient[i] = UdpClientHelper (SSinterfaces.GetAddress (i), 100 + (i * 10));
      udpClient[i].SetAttribute ("MaxPackets", UintegerValue (1200));
      udpClient[i].SetAttribute ("Interval", TimeValue (Seconds (2)));
      //udpClient[i].SetAttribute ("Interval", TimeValue (Seconds (y->GetValue())));
      udpClient[i].SetAttribute ("PacketSize", UintegerValue (800));
      //udpClient[i].SetAttribute ("PacketSize", UintegerValue(x_val));

      clientApps[i] = udpClient[i].Install (ssNodes.Get (i + (nbSS / 2)));
      clientApps[i].Start (Seconds (duration-1));
      clientApps[i].Stop (Seconds (duration));
    }

  /*
   * Setup 1 transport connections between each SS and the BS
   */
  for (int i = 0; i < nbSS / 2; i++)
    {
      IpcsClassifierRecord DlClassifierBe (Ipv4Address ("0.0.0.0"),
                                           Ipv4Mask ("0.0.0.0"),
                                           SSinterfaces.GetAddress (i),
                                           Ipv4Mask ("255.255.255.255"),
                                           0,
                                           65000,
                                           100 + (i * 10),
                                           100 + (i * 10),
                                           17,
                                           1);
      ServiceFlow DlServiceFlowBe = wimax.CreateServiceFlow (ServiceFlow::SF_DIRECTION_DOWN,
                                                             ServiceFlow::SF_TYPE_BE,
                                                             DlClassifierBe);
      ss[i]->AddServiceFlow (DlServiceFlowBe);
      IpcsClassifierRecord ulClassifierBe (SSinterfaces.GetAddress (i + (nbSS / 2)),
                                           Ipv4Mask ("255.255.255.255"),
                                           Ipv4Address ("0.0.0.0"),
                                           Ipv4Mask ("0.0.0.0"),
                                           0,
                                           65000,
                                           100 + (i * 10),
                                           100 + (i * 10),
                                           17,
                                           1);
      ServiceFlow ulServiceFlowBe = wimax.CreateServiceFlow (ServiceFlow::SF_DIRECTION_UP,
                                                             ServiceFlow::SF_TYPE_BE,
                                                             ulClassifierBe);
      ss[i + (nbSS / 2)]->AddServiceFlow (ulServiceFlowBe);

    }

  Ptr<Ipv4> SS_node = ssNodes.Get(nbSS-2)->GetObject<Ipv4> ();
  Ipv4Address nodeif1 = SS_node->GetAddress(1,0).GetLocal();
  Ipv4Address nodeif2 = SS_node->GetAddress(2,0).GetLocal();
  std::cout << "SS node interface one: "<<nodeif1 << " SS node interface two:"<<nodeif2<<std::endl;



  Simulator::Stop (Seconds (300.0));
  pointToPoint.EnablePcap("wimax-bs-jul26", p2pDevices9.Get (1), true);
  csma11.EnablePcap("csma-wimax-jul26",csmaDevices11.Get(1), true);
  csma10.EnablePcap("wimax-ss-jul26 ",csmaDevices10.Get(1),true);
  NS_LOG_INFO ("Starting simulation.....");
  Simulator::Run ();

  for (int i = 0; i < nbSS; i++)
    {
      ss[i] = 0;
    }
  delete[] ss;

  bs = 0;
  
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");

  return 0;
}
