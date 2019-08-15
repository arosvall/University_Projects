#!/usr/bin/env python3

from switchyard.lib.userlib import *

class EthernetProcessor():

    def __init__(self, mac, iface):
        self.mac_address = mac
        self.interface = iface
        self.ipv6 = None
        self.physical = None

    def setstack(self, ip, physical):
        self.ipv6 = ip
        self.physical = physical

    def accept_packet(self, packet_data):
        
        # remove first header
        eth = packet_data.get_header_by_name("Ethernet")
        #get the ethernet type from the ethernet header. look into switchyard to find the type
        eth_type = eth.ethertype # will be the extracted ethernet header type from the eth, number corresponding. 
        del packet_data[0]  
        # then compare all the different ether types to find the correct pa look for the object corresponding to the object that #is being checked against, look in the switchyard to find an ipv6 header type to compare with the eth type. From there, #then look into the Switchyard to find the correct documentation.          
        
        if(eth_type == EtherType.IPv4):
          self.ipv4.accept_packet(packet_data)
        
        elif(eth_type == EtherType.ARP):
          self.arp.accept_packet(packet_data)
        
        elif(eth_type == EtherType.IPv6):
          self.ipv6.accept_packet(packet_data)
        
        
    def send_packet(self, packet_data, dst_mac):
        eth_type = 0xffff 

        #using the same type of structure as when accepting a packet, attempt to find the header type with the checks and assign the correct ethertype based on that. Keep placeholder in case of any errors
        if(packet_data.has_header(Arp)):
          print("The packet data type is ARP")
          eth_type = EtherType.ARP 
  
        elif(packet_data.has_header(IPv6)):
          #This takes care of both the ipv6 and the NDP (as the ICMPv6) header
          print("The packet data type is IPv6")
          eth_type = EtherType.IPv6
        
        elif(packet_data.has_header(IPv4)):
          print("The packet data type is ARP")
          eth_type = EtherType.IPv4
        
          # replace placeholder with the correct ethertype, will be an ARP or a NDP, or an ipv4 or an ipv6. 
      
        eth = Ethernet(src=self.mac_address, dst=dst_mac, ethertype=eth_type)
        self.physical.send_packet(self.interface, eth+packet_data)

    def __str__(self):
        return "Ethernet link layer ({})".format(self.ipv6_address)
