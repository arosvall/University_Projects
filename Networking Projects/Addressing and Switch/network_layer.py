#!/usr/bin/env python3
#https://cs.colgate.edu/~jsommers/switchyard/2017.01/writing_a_program.html#introduction-to-packet-parsing-and-construction
#https://cs.colgate.edu/~jsommers/switchyard/2017.01/reference.html#pktlib

from switchyard.lib.userlib import *

class IpProcessor():
    #Can make a static number of hosts for the restricted table size. Drop entries by looking at their ttl, or can check if the host sends more often than the others. can waste time doing requests, could have intelligence if theres a need to check how the hosts are going. 
    #Could determine the host number from preferences.py

    def __init__(self, inet6, inet4):
        self.ipv6_address = inet6
        self.ipv4_address = inet4
        self.stopandwait = None
        self.ethernet = None
        
        # TODO -- resolve addresses from network, not hard coded
        self.ipv6_2mac = dict()
        self.ipv6_2mac[IPv6Address("fe80::200:ff:fe00:1")] = EthAddr("00:00:00:00:00:01")
        self.ipv6_2mac[IPv6Address("fe80::200:ff:fe00:2")] = EthAddr("00:00:00:00:00:02")
        self.ipv6_2mac[IPv6Address("fe80::200:ff:fe00:3")] = EthAddr("00:00:00:00:00:03")

        self.ipv4_2mac = dict()
        self.ipv4_2mac[IPv4Address("10.0.0.1")] = EthAddr("00:00:00:00:00:01")
        self.ipv4_2mac[IPv4Address("10.0.0.2")] = EthAddr("00:00:00:00:00:02")
        self.ipv4_2mac[IPv4Address("10.0.0.3")] = EthAddr("00:00:00:00:00:03")

    def setstack(self, ethernet, stopandwait):
        self.ethernet = ethernet
        self.stopandwait = stopandwait

    def accept_packet(self, packet_data):
        # log_debug("IpProcessor received packet from below")
        if(packet_data.has_header(IPv4)):
          print("Received an IPv4 packet")
          ipv4_packet = packet_data.get_header_by_name("IPv4")
          #if the ipv4 packet is directed at the current host then pass it up
          if(ipv4_packet.dst == self.ipv4_address):
            print("IPv4 packet directed at current host, passing packet to the transport layer as stop and wait")
            #packet is for the current host so delete the packet wrapping it and pass it up the layer
            del packet_data[0]
            self.stopandwait.accept_packet(packet_data, self.ipv4_address)
        
        #if the packet was not for the host, then do nothing
    
    
        #if the packet has an ipv6 header
        elif(packet_data.has_header(IPv6)):
          print("Received an IPv6 packet")
          #store the header in a packet variable
          ipv6_packet = packet_data.get_header_by_name("IPv6")
          #if the destination of the packet is the current host, then delete the packet header as it has been processed and 
          #then pass the next layer up to the transport layer
          if(ipv6_packet.dst == self.ipv6_address):
            print("IPv6 packet directed at current host, passing packet to the transport layer as stop and wait")
            del packet_data[0]
            self.stopandwait.accept_packet(packet_data, self.ipv6_address)
            #look into the ipv6 to find the icmpv6 protocol to see if neighbour request/reply equivalence
            
            #if the packet was not for the host, then do nothing
            
        #if the packet has an arp header
        elif(packet_data.has_header(Arp)):
            print("Received an ARP packet")
            arp_packet = packet_data.get_header_by_name("Arp")
        #if the arp is from a new person, regardless of the arp operation type, then add to the dictionary
        if(arp_packet.senderprotoaddr not in self.ipv4_2mac):
            print("ARP was from a new neighbour, adding to the dictionary")
            #add the dictionary entry with the ip address as the key, and the MAC address as the value
            self.ipv4_2mac[IPv4Address(arp_packet.senderprotoaddr)] = EthAddr(arp_packet.senderhwaddr)
        
        #if the arp operation is a request, then need to send a reply. 
        if(arp_packet.operation == ArpOperation.Request):
            print("Operation was a request, preparing a reply")
            #create an arp header for the data then send it back to the MAC address that sent the request  
            arp_reply = Arp(operation=ArpOperation.Reply, senderhwaddr = self.ethernet.mac_address, targethwaddr = arp_packet.senderhwaddr, senderprotoaddr = self.ipv4_address, targetprotoaddr = arp_packet.senderprotoaddr)
            print("ARP reply created, being sent now")
            self.ethernet.send_packet(arp_reply, arp_packet.senderhwaddr)
        
        #if this is a reply, have already stored the address in the lookup mac-ip dictionary so don't need to do anything else 
        elif(arp_packet.operation == ArpOperation.Reply):
            print("Operation was a reply, no need to process more")	
        
        #self.stopandwait.accept_packet(packet_data, YOU_NEED_TO_FIX_THIS)

    def send_packet(self, packet_data, dst_ip):
        # use add_header to add the header to the packet data when there is information being pa
 
        # log_debug("IpProcessor received packet to be sent")

        print("YOU NEED TO CHANGE THIS")
        #if the recipeient is an unknown (not in the ip map dict), then send with an ARP or a NDP depending on the type of ipv address that the neighbour has. Otherwise if the neighbour is known then just send the packet down with the correct MAC address to ensure it gets to the neighbour. If there are no known neighbours, then need to broadcast to the switch to get an idea of who is alive in the area then move on from there.     
        if type(dst_ip) == type(IPv6Address("0::0")):
          if dst_ip not in self.ipv6_2mac:
              #create the header and add it yourself
              #create the icmpv6 header and then the ipv6 header, add both and send it on. 
            self.ethernet.send_packet(packet_data, "00:00:00:00:00:00")
          else:
              #just create an ipv6 header and add that to the packet before sending it on as the ethernet 
            self.ethernet.send_packet(packet_data, self.ipv6_2mac[dst_ip])


        if type(dst_ip) == type(IPv4Address("0.0.0.0")):
          if dst_ip not in self.ipv4_2mac:
            print("Making an ARP request from an IPv4 packet")
            #make an ARP request, and send it back down to the ethernet layer to be sent out. 
            arp_request = Arp(operation=ArpOperation.Request, senderhwaddr = self.ethernet.mac_address, targethwaddr = "ff:ff:ff:ff:ff:ff", senderprotoaddr = self.ipv4_address, targetprotoaddr = dst_ip)
            print("Request has been created, is being sent now")        
            self.ethernet.send_packet(arp_request, "00:00:00:00:00:00")
            #can try to store the data until the request has a reply, then send the datapacket, could store in a dictionary by ip address then delete them as you go. 
          else:
            self.ethernet.send_packet(packet_data, self.ipv4_2mac[dst_ip])

    def __str__(self):
        return "IP network layer ({} & {})".format(self.ipv6_address, self.ipv4_address)

