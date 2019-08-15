/*
 * sniffer.c
 * Main method provided by David C Harrison (david.harrison@ecs.vuw.ac.nz) July 2015
 * Use as-is, modification, and/or inclusion in derivative works is permitted only if 
 * the original author is credited. 
 *
 * To compile: gcc -o sniffer sniffer.c -l pcap 
 * Code tutorials taken from: 
 * http://yuba.stanford.edu/~casado/pcap/section2.html
 * https://www.tcpdump.org/pcap.html
 * https://www.tcpdump.org/manpages/pcap.3pcap.html
 * included header file descriptions came from sources such as below
 * https://unix.superglobalmegacorp.com/Net2/newsrc/netinet/ip.h.html
 * 
 * And code help for printing data taken heavily from: (full creditation down below and explanation of adaption above relevant method)
 * https://www.binarytides.com/packet-sniffer-code-c-libpcap-linux-sockets/
 * 
 *  To run: tcpdump -s0 -w - | ./sniffer -
 *     Or: ./sniffer <some file captured from tcpdump or wireshark>
 */
 
#include <stdio.h>
#include <pcap.h>     
#include <arpa/inet.h>         //for the ntohs translation part of the assignment, converting addresses
#include <netinet/if_ether.h>  //ethernet header import
#include <netinet/ip.h>        //ipv4 header import
#include <netinet/ip6.h>       //ipv6 header import
#include <netinet/tcp.h>       //tcp header import
#include <netinet/udp.h>       //udp header import
#include <netinet/ip_icmp.h>   //icmp for ipv4 header import
#include <netinet/icmp6.h>     //icmp for ipv6 header import
#include <sys/socket.h>


/* Signatures for all the methods in the program */
void process_IPv4(const u_char *, int);
void process_IPv6(const u_char *, int);
void process_TCP(const u_char *, int);
void process_UDP(const u_char *, int);
void process_ICMP(const u_char *, int);
void process_ICMPv6(const u_char *, int);
void print_Data(const u_char *, int);
void got_packet(u_char *, const struct pcap_pkthdr *, const u_char *);

static int packetNum = 0;  // track how many packets have been processed by the program

/* Method taken from the lab1 handout, credited above */
int main(int argc, char **argv)
{
  if (argc < 2) {
    printf("Must have an argument, either a file name or '-'\n");
    return -1;
  }
  
  pcap_t *handle = pcap_open_offline(argv[1], NULL);
  pcap_loop(handle, 1024*1024, got_packet, NULL);
  pcap_close(handle); 
  
  return 0;
}

/* Method which extracts the ethernet protocol and finds out what the type of the packet is, passing the packet to another method afterwards */
void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet){
  //Keep track of the number of packets processed and find the header length
  printf("Header Length: %d\n", header->len);
  printf("     Packet #: %d \n\n", ++packetNum);

  //assign the packet to the structure created and imported through an netinet header, using the standard length of a ethernet header
  int eth_Length = 14;
  const struct ether_header *eth;
  eth = (struct ether_header*)(packet);
  int eth_Protocol = (int) eth->ether_type;

  //extract the protocol, print a message that the sniffer does not handle this type or pass to ipv4 or ipv6 header for processing.  
  if(eth_Protocol == 8) {
    printf("     Protocol: IPv4\n");
    process_IPv4(packet, eth_Length);
  }
  else if(eth_Protocol == 34525 ) {
    printf("     Protocol: IPv6\n");
    process_IPv6(packet, eth_Length);
  }
  else {
    printf("This sniffer does not handle this Ethernet Protocol: %d \n\n", eth_Protocol);
  }
}
/*Method which handles the IPv4 header, extracting source and desination hosts and extracting the protocol to know which header
will follow from this one, i.e. TCP / UDP...
*/
void process_IPv4(const u_char * packet, int length){
  //create the ip header based on the struct provided in <netinet/ip.h, assigning from the current point in the packet into the fields >
  const struct ip *ip_header; 
  ip_header = (struct ip*)(packet + length);
  
  //create arrays to store the chars from the addresses (source and dest), using the INET_ADDRSTRLEN which is also used in inet_ntop
  char source_address[INET_ADDRSTRLEN];
  char dest_address[INET_ADDRSTRLEN];
  
  //extract header, get the size and add to the current length to know where the next header should start. Extract the protocol to get the next header type
  unsigned char ip_h_size = ip_header->ip_hl;
  int header_Size = (int) ip_header->ip_hl*4; 
  int protocol = (int) ip_header->ip_p;
  int len = length + header_Size;
  
  //convert the source and destination addresses to text form instead of bit form, then store in the previously created char arrays. then print the array, taking advantage of the fact that a string = an array of chars to print all chars in the string immediately.
  inet_ntop(AF_INET, &(ip_header->ip_src), source_address, INET_ADDRSTRLEN);
  inet_ntop(AF_INET, &(ip_header->ip_dst), dest_address, INET_ADDRSTRLEN);
  printf("         From: %s \n", source_address);
  printf("           To: %s \n", dest_address);
  
  //based on the ip's next protocol, continue to process the data packet or exit early if not handled by sniffer.
  if(protocol == 1) {
	  printf("     Protocol: ICMP \n");
	  process_ICMP(packet, len);
	}
	else if(protocol == 6) {
	  printf("     Protocol: TCP \n");
	  process_TCP(packet, len);
	}
	else if(protocol == 17) {
	  printf("     Protocol: UDP \n");
	  process_UDP(packet, len);
	}
  else {
	  printf("     Protocol: %d is not handled within this sniffer \n", protocol);
  }
}

/* Method which handles the IPv6 header, extracting source and desination hosts and extracting the protocol to know which header
will follow from this one, i.e. TCP / UDP...
*/
void process_IPv6(const u_char * packet, int length ){
  //create a header from the netinet header import, use the general size of an ipv6 header and assign the packet to it with the adjustment for the length
  int ipv6_header_size = 40;
  const struct ip6_hdr *ip_header;
  ip_header = (struct ip6_hdr*)(packet + length);
	
  //extract the source and destination addresses using the inet_ntop format to convert the addresses to understandable english
  char source_address[INET6_ADDRSTRLEN];
  char dest_address[INET6_ADDRSTRLEN];
  inet_ntop(AF_INET6, &(ip_header->ip6_src), source_address, INET6_ADDRSTRLEN);
  inet_ntop(AF_INET6, &(ip_header->ip6_dst), dest_address, INET6_ADDRSTRLEN);
  printf("      From: %s \n", source_address);
  printf("        To: %s \n", dest_address);

  //find the length of the headers and the protocol to know which header follows, following the if statement to find the next method to call
  int protocol = ip_header->ip6_nxt;
  int len = length + ipv6_header_size;

  if(protocol == 1) {
	  printf("     Protocol: ICMP \n");
	  process_ICMPv6(packet, len);
  }
	else if(protocol == 6) {
	  printf("     Protocol: TCP \n");
	  process_TCP(packet, len);
	}
	else if(protocol == 17) {
	  printf("     Protocol: UDP \n");
	  process_UDP(packet, len);
	}
	//process outside of the bounds of my understanding, print out the protocol number and move to the next packet
	else {
	  printf("Protocol: %d is not handled within this sniffer \n\n", protocol);
	}
}


/* Method that processes the TCP part of the header, then prints out the data contained once the header has been broken down 
*/
void process_TCP(const u_char * packet, int length){
  //create the pointer for the header, and assign variable names for easy use later.
  const struct tcphdr* tcp_header;
  unsigned short source_port;
  unsigned short destination_port;
  unsigned char *payload;
	
  tcp_header = (struct tcphdr*)(packet + length);
  int len = length + (tcp_header->doff*4);
  //extract the source and destination ports from the header and interpret using the network to host conversion
  source_port = ntohs(tcp_header->source);
  destination_port = ntohs(tcp_header->dest);
  printf("     Src port: %hu\n", source_port);
  printf("    Dest port: %hu\n", destination_port);

  //assign the payload to a u char* adjusting for length of all headers and print out the data
  payload = (u_char*)(packet + len);
  int payload_length = (int) sizeof(payload);
  printf("      Payload: (%d bytes) \n\n", payload_length);
  print_Data(payload, payload_length);
}

/* Method that processes the UDP part of the header, then prints out the data contained once the header has been broken down 
*/

void process_UDP(const u_char * packet, int length ){
  //create a pointer for a header, variables for use in the method
  const struct udphdr* udp_header;
  unsigned short source_port;
  unsigned short destination_port;
  unsigned char *payload;
  int udp_header_size = 8;

  //assign the data from the payload to the structure, adjusting the length of the header
  udp_header = (struct udphdr*)(packet + length);
  
  //extract and print the source and destination sources from the header using network to host conversion  
  source_port = ntohs(udp_header->source);
  destination_port = ntohs(udp_header->source);
  printf("     Src port: %hu \n", source_port);
  printf("    Dest port: %hu \n", destination_port);


  //get payload data
  int len = length + udp_header_size;
  payload = (u_char*)(packet + len);
  int payload_length = sizeof(payload);
  
  //print out protocol details
  printf("Payload: (%d bytes) \n\n", payload_length);
    
  //print out payload data
  print_Data(payload, payload_length);
}

void process_ICMP(const u_char * packet, int length){
  //create the ICMP header with the structure provided in netinet header, using the length of prior headers 
  const struct icmphdr* icmp_header;
  icmp_header = (struct icmphdr*)(packet + length);
  
  //extract the code and type
  int code = (int) icmp_header->code;
  int type = (int) icmp_header->type;
  unsigned char *payload;
  int icmp_header_size = 8;
	
  //print out common type-code messages in English, or default to the general statement of type and code
  if (type == 0){ 
	  printf("     Echo Reply \n");
	}
	else if (type == 1 || type == 2) {
	  printf("     Unassigned/Reserved Type \n");
	}
	else if (type == 3) {
	  printf("     Destination Unreachable\n");
  	printf("     Code was:  %d \n", code);
	}
	else if (type == 5) {
	  printf("     Redirect Message \n");
	  printf("     Code was: %d \n", code);
	}
	else if (type == 8) { 
	  printf("     Echo Request\n");	
	}
	else{ 
    printf("     The type of the control message was: %d \n", type);
    printf("    The code of the control message was: %d \n", code);			
  }
	 
  int len = length + icmp_header_size;
  //find the payload size using the length of the headers total
  payload = (u_char*)(packet + len);
  int payload_length = sizeof(payload);
    
  //print out protocol details
  printf("Payload: (%d bytes) \n \n", payload_length);
    
  //print out payload data
  print_Data(payload, payload_length);
}

/* Method which processes the ICMPv6, understanding what is in the header before printing out data contained in the packet
*/
void process_ICMPv6(const u_char * packet, int length){
  //create a icmp header using the <netinet>header, assigning the next step of the packet to this structure
  const struct icmp6_hdr* icmp6_header;
  icmp6_header = (struct icmp6_hdr*)(packet + length);
  
  //extract type and code from the header, casting to an int for easy comparisons
  int code = (int)icmp6_header->icmp6_code;
  int type = (int)icmp6_header->icmp6_type;
    
  unsigned char *payload;
  int icmp_header_size = 8;
  //understand code and type, using if statements to check. if the code or type is common, then print out an English translation of what the code means
  //otherwise just print the type of message with the code as well. 
  if (type < 128) {
    printf ("    Error message contained \n");
    if (type == 1) {
      printf ("          Destination Unreachable \n");
      printf ("          Code was: %d \n", code);
    }
    else if(type == 2) {
      printf ("          Packet too big\n");
    }
    else if(type == 3) {
      printf("           Time exceeded with code: %d \n", code);	
    }
    else {
      printf("           Error message contained type: %d \n", type);
      printf("           The code of the message was: %d \n", code);
    }
  }
  else{
    printf("     Informational Message contained\n");		
    if (type == 128) {
      printf("    Echo Request\n");
    }
    else if (type == 129) {
      printf("    Echo Reply\n");
    }
    else if (type == 130) {
      printf("    Multicast Listener Query\n");
    }
    else if (type == 131) {
      printf("    Multicast Listener Report\n");
    }
    else if (type == 132) {
      printf("     Multicast Listener Done \n");
    }
    else if (type == 133) {
      printf("     Router Solicitation\n");
    }
    else if (type == 138) {
      printf("     Router Renumbering \n");
    }
    else {
      printf("     Informational message contained type: %d \n", type);
      printf("     The code of the message is: %d \n", code);
    }
  }
	
  int len = length + icmp_header_size;
  payload = (u_char*)(packet + len);
  int payload_length = sizeof(payload);
  
  //print out protocol details
  printf("Payload: (%d bytes) \n", payload_length);
  printf("\n");
  
  //print out payload data
  print_Data(payload, payload_length);
}


/*The following method is taken from the website:
* https://www.binarytides.com/packet-sniffer-code-c-libpcap-linux-sockets/ 
* the code has been adapted slightly to fit the style of this program, adjusting variable names and removing the printing to a file
* as in this assignment we do not print out to a file. There have also been comments added to show the understanding i have over what this
* code is doing  
*/
void print_Data(const u_char * packet, int length) {
  int i , j;
  //step through every character in the data packet
  for(i = 0 ; i < length ; i++){
	  //if a line of hex printing has been completed, then print a space to denote this
    if( i!=0 && i%16==0){ 
      printf("         ");
	  //for each character in the line of hex printing (16 all up)
      for(j=i-16 ; j<i ; j++){
		  //check to see if the character is a number or a letter by comparing to the char value
        if(packet[j]>=32 && packet[j]<=128){
          printf("%c",(unsigned char)packet[j]);
		}
		//if not a number or letter, then print a dot to denote an unknown character
        else {
		  printf("."); 
        }
	  }
	  //print a new line 
      printf("\n");
    } 
         
	//if the end of a line print a space	 
    if(i%16==0){
	  printf("   ");
	}
	//otherwise print the value contained in the data at array position i
    printf(" %02X",(unsigned int)packet[i]);
    
    //at the end of the payload print the last spaces	
    if( i == length-1) { 
      for(j=0;j<15-i%16;j++) {
        printf("   "); 
      }
             
      printf("         ");
      //from point j up to point i in a hex line, check to see if there is a letter or a number based on the char value and print accordingly.       
      for(j = i-i%16 ; j <= i ; j++){
        if(packet[j]>=32 && packet[j]<=128) {
          printf("%c",(unsigned char)packet[j]);
        }
        else {
	      //dot denotes an unknown non-letter or number character
          printf(".");
        }
      }
	  //print a new line 
      printf("\n" );
    }
  }
}