'''
Ethernet learning switch in Python.

Note that this file currently has the code to implement a "hub"
in it, not a learning switch.  (I.e., it's currently a switch
that doesn't learn.)
'''
from switchyard.lib.userlib import *

# need to check if the source is within the switch mac list. If it isn't, add it and continue. once continued then see if the destination is in the mac addresses. If it is not, then flood out to all hosts barring the current address.
def main(net):
    my_interfaces = net.interfaces() 
    mymacs = [intf.ethaddr for intf in my_interfaces]
    mac_to_intf = dict()

    while True:
        try:
            timestamp,input_port,packet = net.recv_packet()
        except NoPackets:
            continue
        except Shutdown:
            return

        log_debug ("In {} received packet {} on {}".format(net.name, packet, input_port))
        #if the packet destination has already been logged by me, check that the source has as well then send it to the destination
     
        print("Received a packet")

        #Check to see if the source of the packet has been logged as a key of the dictionary which is mapping mac addresses to their interface port. If it has not, then add the entry to the dictionary  
        if not packet[0].src in mac_to_intf.keys():
           print("New source address")
           mac_to_intf[packet[0].src] = input_port
           print("Added source address")
        
        #store the destination in a variable for convenience
        pkt_dest = packet[0].dst
        
        #If the packet is meant for the current switch, drop it as per instructions in switchyard documentation
        if pkt_dest in mymacs:
          print("Received a packet for me, dropping it")
          log_debug ("Packet is meant for me")
          
        #Else if the packet mac is already known, then send the packet along to the correct interface for that mac address
        elif pkt_dest in mac_to_intf.keys():
          print("Mac address known, passing on to another interface")
          intf_name = mac_to_intf[pkt_dest]
          log_debug("Sending packet {} to {}".format(packet, intf_name))
          net.send_packet(intf_name, packet)
          print("Packet sent")
          
        #if the mac address of the destination is not known, then flood the packet out on all the interfaces to find out where it is.
        else:
          print("New destination")
          for intf in my_interfaces:
            if input_port != intf.name:
              log_debug ("Flooding packet {} to {}".format(packet, intf.name))
              net.send_packet(intf.name, packet)
    net.shutdown()
