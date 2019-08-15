#!/usr/bin/env python3

'''
sniffer.py -- Python packet sniffer

By Jordan Ansell (jordan.ansell@ecs.vuw.ac.nz) July 2017

Use as-is, modification, and/or inclusion in derivative works is permitted only if the original author is credited.

Requires the pcapy library.
To install: python3 -m pip install pcapy -t ./pcapy/

To run:     python3 sniffer.py packets_file.pcap
Or:         tcpdump -w - | python3 ./sniffer.py -

note: awesome website: https://www.binarytides.com/code-a-packet-sniffer-in-python-with-pcapy-extension/
http://simplestcodings.blogspot.com/2010/10/tcp-header-format.html?q=ip+header+format
http://mars.netanya.ac.il/~unesco/cdrom/booklet/HTML/NETWORKING/node020.html

find each header size, ipv4 is variable
pass data to each constructor, data being passed need to start where the last header finished. Can either manipulate packet data variable, or slice off data in python
a[:3] give first three numbers 

can use switches as well, common processes get factored out i.e.process ipv4 then process tcp, use the if statements in those methods then can just call that for the question, rather than always writing nested code. 
'''

from pcapy import pcapy  

import binascii
import struct
import sys
import ipaddress
import socket

def mac2str(mac_bytes):
    mac_string = binascii.hexlify(mac_bytes).decode('ascii')
    mac_pairs = [i+j for i,j in zip(mac_string[0::2], mac_string[1::2])]
    return ':'.join(mac_pairs)

class PacketHeaderBase:
    ''' Base class for packet headers. '''

    def __init__(self, hdr_format, field_names, data):
        ''' Creates a header object from the packet data given.
            Field names are given as an array of strings.
            For more information on struct format strings see:
              https://docs.python.org/3/library/struct.html '''

        self.data = data
        self.hdr_length = struct.calcsize(hdr_format)
        self.field_values = struct.unpack(
            hdr_format, 
            data[:self.hdr_length])

        pkt_dict = dict(zip(field_names, self.field_values))
        for k, v in pkt_dict.items():
            setattr(self, k, v)

class Ethernet(PacketHeaderBase):
    ''' Ethernet header class. '''

    # _fmt and _fields define the structure of an Ethernet packet
    fmt = '!6s6sH'
    fields = ['Destination_Address', 'Source_Address', 'Protocol'] 

    def __init__(self, data):
        super().__init__(Ethernet.fmt, Ethernet.fields, data)

    def __str__(self):
        return "Ethernet payload {}".format( binascii.hexlify(self.payload) )  # what information needs to be printed?

class IPv4_Datagram(PacketHeaderBase):
    ''' Ethernet header class. '''

    # need to find correct IP datagram size
    fmt = '!BBHHHBBH4s4s'
    fields = ['Version_Header_Length', 'Service_Type', 'Total_Length', 'Identification', 'Flags_Fragment_Offset', 'TTL', 'Protocol', 'Header_Checksum', 'Source_Address', 'Destination_Address'] 

    def __init__(self, data):
        super().__init__(IPv4_Datagram.fmt, IPv4_Datagram.fields, data)

    def __str__(self):
        return "IPv4_Datagram payload {}".format( binascii.hexlify(self.payload) )  # what information needs to be printed?

class IPv6_Datagram(PacketHeaderBase):
    ''' Ethernet header class. '''

    # need to find correct IP datagram size
    fmt = '!LHBB16s16s'
    fields = ['Version_TrafficClass_Flow_Label', 'Payload_Length', 'Next_Header', 'Hop_Limit', 'Source_Address', 'Destination_Address'] 

    def __init__(self, data):
        super().__init__(IPv4_Datagram.fmt, IPv4_Datagram.fields, data)

    def __str__(self):
        return "IPv4_Datagram payload {}".format( binascii.hexlify(self.payload) )  # what information needs to be printed?
    
class TCP(PacketHeaderBase):
    ''' TCP header class. '''

    # _fmt and _fields define the structure of an Ethernet packet
    fmt = '!HHLLBBHHH'
    fields = ['Source_Port', 'Destination_Port', 'Sequence_Number', 'Ack_Number', 'Header_Length', 'Reserved', 'Flags', 'Window', 'Checksum', 'Urgent_Pointer', 'Options', 'Padding'] 

    def __init__(self, data):
        super().__init__(TCP.fmt, TCP.fields, data)

    def __str__(self):
        return "TCP payload {}".format( binascii.hexlify(self.payload) )  # what information needs to be printed?

class UDP(PacketHeaderBase):
    ''' UDP header class. '''

    # _fmt and _fields define the structure of an Ethernet packet
    fmt = '!HHHH'
    fields = ['Source_Port', 'Destination_Port', 'Length', 'Checksum'] 

    def __init__(self, data):
        super().__init__(UDP.fmt, UDP.fields, data)

    def __str__(self):
        return "UDP payload {}".format( binascii.hexlify(self.payload) )  # what information needs to be printed?

class ICMP(PacketHeaderBase):
    ''' ICMP header class. '''
    #Need to check here that the ipv4 or ipv6 will have a difference in the main body, check what the procotol numbers mean. The numbers, Mason, what do they mean? 
    
    # _fmt and _fields define the structure of an Ethernet packet
    fmt = '!BBH'
    fields = ['Message_Type', 'Code', 'Checksum'] 

    def __init__(self, data):
        super().__init__(ICMP.fmt, ICMP.fields, data)

    def __str__(self):
        return "ICMP payload {}".format( binascii.hexlify(self.payload) )  # what information needs to be printed?

class IPv6Exten(PacketHeaderBase):
    ''' IPv6Exten header class. '''

    # _fmt and _fields define the structure of an Ethernet packet
    fmt = '!HHHH'
    fields = ['Source_Port', 'Destination_Port', 'Length', 'Checksum'] 
       
    def __init__(self, data):
        super().__init__(IPv6Exten.fmt, IPv6Exten.fields, data)

    def __str__(self):
        return "IPv6Exten payload {}".format( binascii.hexlify(self.payload) )  # what information needs to be printed?
    
def process_IPv4(datagram):
    print ('    Ether type: IPv4')
    fromAdd = socket.inet_ntoa(datagram.Source_Address)
    toAdd = socket.inet_ntoa(datagram.Destination_Address)
    print('        From: ' + str(fromAdd))
    print('        To: ' + str(toAdd))
        
    
def process_IPv6(datagram):
    print('      Ether type: IPv6')
    #need to use mac2str here to find the correct bytes for the 
    fromAdd = mac2str(datagram.Source_Address)
#    toAdd = socket.inet_ntoa(datagram.Destination_Address)
    toAdd = mac2str(datagram.Destination_Address)

    print('        From: ' + str(fromAdd))
    print('        To: ' + str(toAdd))
    
def process_TCP(datagram, length):
    print('         Protocol: TCP')
    print('         Src Port: ' + str(datagram.Source_Port))
    print('         Destination Port: ' + str(datagram.Destination_Port))
    datalength = len(datagram.data) - length
    print('         Payload: (' + str(datalength) + ') bytes')
    print_data(datagram.data[length:])
        
def process_ICMP(datagram, version, length):
    print('         Protocol: ICMP')
    type = socket.ntohs(datagram.Message_Type)
    code = socket.ntohs(datagram.Code)
    if version == 4:
        if type == 0:
            print ('          Echo Reply')
        elif type == 1 or type == 2:
            print ('          Unassigned/Reserved Type')
        elif type == 3:
            print ('          Destination Unreachable')
            print ('          Code was: ' + str(code))
        elif type == 5:
            print ('          Redirect Message')
            print ('          Code was: ' + str(code))
        elif type == 8: 
            print ('           Echo Request')	
        else:
            print ('           The type of the control message was: ' + str(type))
            print ('           The code of the control message was: ' + str(code))			
    else:
        if type < 128: 
            print ('    Error message contained')
            if type == 1:
                print ('          Destination Unreachable')
                print ('          Code was: ' + str(code))
            elif type == 2:
                print ('          Packet too big')
            elif type == 3:
                print('           Time exceeded with code: ' + str(code))	
            else:
                print('           Error message contained type: ' + str(type))
                print('           The code of the message was: ' + str(code))
        else:
            print('     Informational Message contained')		
            if type == 128:
                print ('          Echo Request')
            elif type == 129:
                print ('          Echo Reply')
            elif type == 130:
                print ('          Multicast Listener Query')
            elif type == 131:
                print ('          Multicast Listener Report')
            elif type == 132:
                print ('          Multicase Listener Done')
            elif type == 133:
                print ('          Router Solicitation')
            elif type == 138:
                print ('          Router Renumbering')
            else:
                print('          Informational message contained type: ' + str(type) + ' with code' + str(code))
    print_data(datagram.data[length:])
                      
def process_UDP(datagram, length):
    print('         Protocol: UDP')
    print('         Src Port: ' + str(datagram.Source_Port))
    print('         Destination Port: ' + str(datagram.Destination_Port))
    datalength = len(datagram.data) - length
    print ('Payload: (' + str(datalength) + ') bytes')
    print_data(datagram.data[length:])

def process_IPv6_Exten(datagram, length):
    print('         Protocol: IPv6 Extension')
    #Need to decide what to print for the IPv^ Extension, also check the length
    datalength = len(data) - length
    print ('Payload: (' + str(datalength) + ') bytes')
    print_data(datagram.data[length:])

def print_data(data):
    print (str(data))
    #hexed = binascii.hexlify(data) 
    hexed = data.hex()
    print ('hex:'  + str(hexed))
#    tex = format(b
   # dehexed = codecs.decode(hexed, "hex")
    text = bytearray.fromhex(str(data)).decode()
    #current getting non-hexadecimal number found in fromhex() arg at position 1 error, need to fix this. 
    #TODO

def process_packet(packet_data):
    ''' Function for processing a single packet '''
    parsed_pkt = dict()

    # print( binascii.hexlify(packet_data) )
    # process the datalink header
    parsed_pkt['ethernet'] = Ethernet(packet_data)
    
    ethheaderlength = 14
    totallength = ethheaderlength
    
    #now print the relevant fields
    protocolNum = socket.ntohs(parsed_pkt['ethernet'].Protocol)
    print(str(protocolNum))
    if protocolNum == 8 :
        
        parsed_pkt['ip4 datagram'] = IPv4_Datagram(parsed_pkt['ethernet'].data[totallength:])
        process_IPv4(parsed_pkt['ip4 datagram'])        
        headerlength = ((parsed_pkt['ip4 datagram'].Version_Header_Length) & 0xF)       
    
        IPProtocol = parsed_pkt['ip4 datagram'].Protocol
        print(str(IPProtocol))
        totallength += headerlength
        
        if IPProtocol == 1:
            icmpheaderLength = 4
            parsed_pkt['ICMP'] = ICMP(parsed_pkt['ip4 datagram'].data[totallength:])
            #CheckHeaderLength
            process_ICMP(parsed_pkt['ICMP'], 4, icmpheaderLength)
            
        elif IPProtocol == 6:
            parsed_pkt['TCP'] = TCP(parsed_pkt['ip4 datagram'].data[totallength:])
            #CheckHeaderLength
            tcpHeaderLength = 20
            process_TCP(parsed_pkt['TCP'], tcpHeaderLength)           
        
        elif IPProtocol == 17:
            #CheckHeaderLength
            parsed_pkt['UDP'] = UDP(parsed_pkt['ip4 datagram'].data[totallength:])
            udpheaderlength = 8
            #totallength += udpheaderlength
            process_UDP(parsed_pkt['UDP'],udpheaderlength)
    
        elif IPProtocol == 41:
            parsed_pkt['IPv6 Exten'] = IPv6Exten(parsed_pkt['ip4 datagram'].data[totallength:])
            process_IPv6_Exten(parsed_pkt['IPv6 Exten'],totallength)
            #TODO
        else :
            print('      Ether Type: IPv4')
            print('      Protocol number: ' + str(IPProtocol) +  ' cannot be handled within this packet sniffer')
     
    elif protocolNum == 34525 :
        ipv6HeadLength = 40
        #check TCP, UDP same as IPv4, also check the ICMP to see what is different 
        parsed_pkt['IP6'] = IPv6_Datagram(parsed_pkt['ethernet'].data[totallength:])
        process_IPv6(parsed_pkt['IP6'])
        protocolNum = socket.ntohs(parsed_pkt['IP6'].Next_Header)	
        totallength += ipv6HeadLength
        
        if protocolNum == 1:
            parsed_pkt['ICMP'] = ICMP(parsed_pkt['IP6'].data[totallength:])
            icmpheaderLength = 4
            process_ICMP(parsed_pkt['ICMP'], 6, icmpheaderLength)
        
        elif IPProtocol == 6:
            parsed_pkt['TCP'] = TCP(parsed_pkt['IP6'].data[totallength:])
            tcpHeaderLength = 20
            process_TCP(parsed_pkt['TCP'],tcpHeaderLength)
    
        elif IPProtocol == 17:
            parsed_pkt['UDP'] = UDP(parsed_pkt['IP6'].data[totallength:])
            udpheaderlength = 8
            process_UDP(parsed_pkt['UDP'],udpheaderlength)

        else:
            print('      Ether Type: IPv6')
            print('      Protocol number: ' + str(IPProtocol) +  ' cannot be handled within this packet sniffer')
            size = len(data) - totallength
            print('      The payload size was ' + str(size))
    else:
        print ('    Ether type: ' + str(protocolNum) + ' cannot be handled within this packet sniffer')
        

def main(pcap_filename):
    ''' Main function, loops over packets in the given file, counting and processing each. '''
    print( "Opening file: '{}'".format(pcap_filename) )
    pcap_reader = pcapy.open_offline( pcap_filename )

    count = 0
    while True:
        meta, data = pcap_reader.next()
        if len(data) < 1:
            break  # no more packets 
        
        count += 1
        print("#### Packet {}:".format(count))
        process_packet( data )

    print("Complete. {} packets processed.".format(count))

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Please supply pcap file argument: python3 sniffer.py packets.pcap")
        exit()

    main(sys.argv[1])
