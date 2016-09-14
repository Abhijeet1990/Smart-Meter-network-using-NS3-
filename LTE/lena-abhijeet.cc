#include <iostream>
#include <fstream>

//recreation of desired scenario topology/architecture
#include "ns3/lte-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/lte-module.h"
#include "ns3/network-module.h"
#include "ns3/core-module.h"
//specifiy configuration parameters of the object that are used for simulation
#include "ns3/config-store.h"
#include "ns3/mobility-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/tap-bridge-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-routing-table-entry.h"
#include "ns3/ipv4-static-routing.h"
#include "ns3/ipv4-list-routing.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/applications-module.h"
#include "ns3/config-store.h"
//#include "ns3/gtk-config-store.h"
#include "ns3/flow-monitor-module.h"



using namespace ns3;

int main(int argc,char *argv[])
{
std::string mode = "UseBridge";
std::string tapName = "mytap0";
//std::string tapName2 = "mytap1";
uint16_t numberOfMeters = 1;

GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::RealtimeSimulatorImpl"));
GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

CommandLine cmd;
cmd.AddValue("numberOfMeters", "Number of Smart meters", numberOfMeters);
cmd.AddValue ("mode", "Mode setting of TapBridge", mode);
cmd.AddValue ("tapName", "Name of the OS tap device on left", tapName);
//cmd.AddValue ("tapName2","Name of the OS tap device on right", tapName2);
cmd.Parse (argc, argv);
ConfigStore inputConfig;
inputConfig.ConfigureDefaults ();
// parse again so you can override default values from the command line



//Create an Lte helper
//This will initate common objects such as Channel object and provide
//methods to add eNBs and UEs and configure them
Ptr<LteHelper> lteHelper = CreateObject<LteHelper>();

//we will consider PointToPointEpcHelper, which implements an EPC based on point-to-point links. 
Ptr<PointToPointEpcHelper>  epcHelper = CreateObject<PointToPointEpcHelper> ();
/*
1.LTE helper will trigger the appropriate EPC configuration in correspondance with some important configuration, such as when a new eNB or UE is added to the simulation, or an EPS bearer is created. The EPC helper will automatically take care of the necessary setup, such as S1 link creation and S1 bearer setup. All this will be done without the intervention of the user.
2. EpcHelper will also automatically create the PGW node and configure it so that it can properly handle traffic from/to the LTE radio access network.
*/
lteHelper->SetEpcHelper (epcHelper);

//Log Distance Propogation Loss Model
//lteHelper->SetAttribute("PathlossModel",StringValue("ns3::FriisSpectrumPropagationLossModel "));

//Create Node objects for eNBs(base stations) and UEs(smart meters)
NodeContainer smartMeters;
smartMeters.Create(numberOfMeters);
NodeContainer baseStation;
baseStation.Create(1);
//configure the mobility model for all the nodes created
MobilityHelper mobility_SM;
mobility_SM.SetMobilityModel("ns3::ConstantPositionMobilityModel");
mobility_SM.Install(smartMeters);

MobilityHelper mobility_BS;
mobility_BS.SetMobilityModel("ns3::ConstantPositionMobilityModel");
mobility_BS.Install(baseStation);// Base Station is by default (0,0,0) position

Ptr<ConstantPositionMobilityModel> SM0 = smartMeters.Get (0)->GetObject<ConstantPositionMobilityModel> ();
//Ptr<ConstantPositionMobilityModel> SM1 = smartMeters.Get (1)->GetObject<ConstantPositionMobilityModel> ();
SM0->SetPosition (Vector (5.0, 5.0, 0));
//SM1->SetPosition (Vector (30.0, 40.0, 0));

//Install LTE protocol on the BaseStations(eNBs)
NetDeviceContainer BSDevices;
BSDevices = lteHelper->InstallEnbDevice(baseStation);

//Install LTE protocol on the Smart Meters(UEs)
NetDeviceContainer SMDevices;
SMDevices = lteHelper->InstallUeDevice(smartMeters);

//EPC Portion
//Still, we need to add some explicit code to connect the PGW to other IPv4 networks (e.g., the internet). Here is a very simple example about how to connect a single remote host to the PGW via a point-to-point link

Ptr<Node> PGW = epcHelper->GetPgwNode ();
NodeContainer testNode;
testNode.Create(1);

// Create nodes in the RemoteHost side in 11.11.11.0/24 network
NodeContainer controlCenter;
controlCenter.Create (2);

Ptr<Node> ControlCenterTap = controlCenter.Get(0);
Ptr<Node> remoteHost = controlCenter.Get(1);//one of the interfaces is in the 11.11.11.0 network

CsmaHelper csma11;
csma11.SetChannelAttribute ("DataRate", StringValue ("10Mbps"));
csma11.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (0)));

NetDeviceContainer CCDevices;
CCDevices = csma11.Install (controlCenter);
 
InternetStackHelper internet;
internet.Install (controlCenter);
internet.Install (testNode);

Ipv4AddressHelper ipv4CC;
ipv4CC.SetBase("11.11.11.0","255.255.255.0");
Ipv4InterfaceContainer CCIpIfaces = ipv4CC.Assign (CCDevices);




// Create the internet
PointToPointHelper p2ph;
p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Gb/s")));
p2ph.SetDeviceAttribute ("Mtu", UintegerValue (1500));
p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (0.001)));
NetDeviceContainer internetDevices = p2ph.Install (testNode.Get(0),PGW);
Ipv4AddressHelper ipv4h;
ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);

// Create the internet
PointToPointHelper p2ph2;
p2ph2.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Gb/s")));
p2ph2.SetDeviceAttribute ("Mtu", UintegerValue (1500));
p2ph2.SetChannelAttribute ("Delay", TimeValue (Seconds (0.001)));
NetDeviceContainer internetDevices2 = p2ph.Install (remoteHost,testNode.Get(0));
Ipv4AddressHelper ipv4h2;
ipv4h2.SetBase ("2.0.0.0", "255.0.0.0");
Ipv4InterfaceContainer internetIpIfaces2 = ipv4h2.Assign (internetDevices2);

// interface 0 is localhost, 1 is the p2p device
//Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress (1);

//here the remote host has the IP address of 1.0.0.0 network... if it does not function...we need to create the host in 11.11.11.0 network
//important to specify routes so that the remote host can reach LTE UEs. One way of doing this is by exploiting the fact that the     PointToPointEpcHelper will by default assign to LTE UEs an IP address in the 7.0.0.0 network. 

Ipv4StaticRoutingHelper ipv4RoutingHelper;


// we install the IP stack on the smart meters
internet.Install (smartMeters);




// assign IP address to smart meters..
for(uint32_t i =0; i < smartMeters.GetN();++i)
{       
        Ptr<Node> ue= smartMeters.Get(i);
        Ptr<NetDevice> ueLteDevice = SMDevices.Get(i);
	Ipv4InterfaceContainer smartMeterIpIface = epcHelper->AssignUeIpv4Address (ueLteDevice);
	// set the default gateway for the UE
	//Ptr<Ipv4StaticRouting> smartMeterStaticRouting = ipv4RoutingHelper.GetStaticRouting (ue->GetObject<Ipv4> ());
	//smartMeterStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
}



//Attach the smart meters to the base stations. This will configure the meters acc to Base Station configuration
//and Create a RRC connection between them
for (uint16_t j = 0; j < numberOfMeters; j++)
      {
        lteHelper->Attach (SMDevices.Get(j), BSDevices.Get(0));
        // side effect: the default EPS bearer will be activated
      }


TapBridgeHelper tapBridge;
tapBridge.SetAttribute ("Mode", StringValue (mode));
tapBridge.SetAttribute ("DeviceName", StringValue (tapName));
tapBridge.Install (ControlCenterTap,CCDevices.Get (0));


/*
//Activate a data radio bearer between each smart meter and the base-station it is attached to
enum EpsBearer::Qci q=EpsBearer::GBR_CONV_VOICE;
EpsBearer bearer(q);
lteHelper->ActivateDataRadioBearer(SMDevices,bearer);
//This method also activates two saturation traffic generators for that bearer,one is uplink another is downlink
*/
//Ipv4GlobalRoutingHelper::PopulateRoutingTables ();



/*****Monitor the IP address of the Smart meter  and Remote Host device***********************/

Ptr<Node> SM_zero = smartMeters.Get(0);
Ptr<Ipv4> ipv4 = SM_zero->GetObject<Ipv4> ();
Ipv4Address interface_one_addr = ipv4->GetAddress(1,0).GetLocal();
std::cout << "smart meter interface one: "<<interface_one_addr << std::endl;


Ptr<Ipv4> ipv4_remote = remoteHost->GetObject<Ipv4> ();
Ipv4Address interface_one = ipv4_remote->GetAddress(1,0).GetLocal();
std::cout <<"remote host interface one: "<< interface_one << std::endl;
Ipv4Address interface_two = ipv4_remote->GetAddress(2,0).GetLocal();
std::cout << "remote host interface two: "<<interface_two << std::endl;

Ptr<Ipv4> PGW_addr = PGW->GetObject<Ipv4> ();
Ipv4Address PGW_int_one = PGW_addr->GetAddress(1,0).GetLocal();
std::cout <<"PGW interface one:"<< PGW_int_one << std::endl;
Ipv4Address PGW_int_two = PGW_addr->GetAddress(2,0).GetLocal();
std::cout << "PGW interface two:"<<PGW_int_two << std::endl;

Ipv4Address PGW_int_three = PGW_addr->GetAddress(3,0).GetLocal();
std::cout << "PGW interface three:"<<PGW_int_three << std::endl;

Ptr<Ipv4> testNode_addr = testNode.Get(0)->GetObject<Ipv4> ();
Ipv4Address testNode_int_one = testNode_addr->GetAddress(1,0).GetLocal();
std::cout <<"Testnode interface one:"<< testNode_int_one << std::endl;
Ipv4Address testNode_int_two = testNode_addr->GetAddress(2,0).GetLocal();
std::cout << "Testnode interface two:"<<testNode_int_two << std::endl;
/*****************have to build the static routing******************************/

Ipv4Address gateway = epcHelper->GetUeDefaultGatewayAddress ();
std::cout << "Gateway address: "<<gateway << std::endl;


Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4>());
//in the remoteHost there are two interfaces...one to the csma 11.11.11.0 network and other to the point to point network (1.0.0.0/8)
remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 2);
remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("1.0.0.0"), Ipv4Mask ("255.0.0.0"), 2);
// we reach the 7.0.0.0 LTE network through the second interface of the remoteHost
//remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("10.10.10.0"), Ipv4Mask ("255.255.255.0"), gateway, 2);
//we reach the 10.10.10.0 network that acts as the smart meter device through the same second interface of the remote Host
remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("11.11.11.0"), Ipv4Mask ("255.255.255.0"), 1);
//we reach the 11.11.11.0 network that acts as the Remote Host device throgh the first interface of the remote host which is connected to the bridge in the OS
//remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("1.0.0.0"), Ipv4Mask ("255.0.0.0"), 2);


Ptr<Ipv4StaticRouting> testNodeStaticRouting = ipv4RoutingHelper.GetStaticRouting (testNode.Get(0)->GetObject<Ipv4>());
testNodeStaticRouting-> AddNetworkRouteTo(Ipv4Address("11.11.11.0"),Ipv4Mask ("255.255.255.0"),2);
//testNodeStaticRouting-> AddNetworkRouteTo(Ipv4Address("10.10.10.0"),Ipv4Mask ("255.255.255.0"),1);
testNodeStaticRouting-> AddNetworkRouteTo(Ipv4Address("7.0.0.0"),Ipv4Mask ("255.0.0.0"),1);

Ptr<Ipv4StaticRouting> PGWStaticRouting = ipv4RoutingHelper.GetStaticRouting (PGW->GetObject<Ipv4>());
//PGWStaticRouting->AddHostRouteTo(Ipv4Address("7.0.0.2"),Ipv4Address ("7.0.0.1"),1);
//PGWStaticRouting->AddNetworkRouteTo(Ipv4Address("10.10.10.0"),Ipv4Mask ("255.255.255.0"),1);
PGWStaticRouting->AddNetworkRouteTo(Ipv4Address("11.11.11.0"),Ipv4Mask ("255.255.255.0"),3);
//PGWStaticRouting->AddHostRouteTo(Ipv4Address("7.0.0.2"),Ipv4Address("10.0.0.5"),2);

Ptr<Ipv4StaticRouting> smStaticRouting=ipv4RoutingHelper.GetStaticRouting (smartMeters.Get(0)->GetObject<Ipv4>());
//smStaticRouting-> AddNetworkRouteTo(Ipv4Address("10.10.10.0"),Ipv4Mask ("255.255.255.0"),1);
smStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
smStaticRouting-> AddNetworkRouteTo(Ipv4Address("11.11.11.0"),Ipv4Mask ("255.255.255.0"),1);

Simulator::Stop (Seconds (600.0));
Simulator::Run();
Simulator::Destroy();

return 0;

}
