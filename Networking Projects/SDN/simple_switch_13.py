# Copyright (C) 2011 Nippon Telegraph and Telephone Corporation.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
# implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from ryu.base import app_manager
from ryu.controller import ofp_event
from ryu.controller.handler import CONFIG_DISPATCHER, MAIN_DISPATCHER, DEAD_DISPATCHER
from ryu.controller.handler import set_ev_cls
from ryu.ofproto import ofproto_v1_3
from ryu.lib.packet import packet
from ryu.lib.packet import ethernet
from ryu.lib.packet import ether_types

from ryu.lib import hub
from operator import attrgetter


# For this lab, most of the understanding of code was done through help from the tutor and from the Ryubook credited below. Other information was taken from the links in the lab handout:
# https://ecs.victoria.ac.nz/foswiki/pub/Courses/NWEN302_2018T2/Lab4/RYU-Controller-Tutorial.pdf
# https://ryu.readthedocs.io/en/latest/ofproto_v1_3_ref.html
# https://ryu.readthedocs.io/en/latest/library_packet.html
# Other websites were also researched for information but no code was taken from them.

#Encoded the MAC addresses of the three hosts as variables in order to compare them for Task 1
h1_MAC = '00:00:00:00:00:01'
h2_MAC = '00:00:00:00:00:02'
h3_MAC = '00:00:00:00:00:03'

#Max count variable if number is exceeded will block traffic from host for Task 3
MAX_COUNT = 20
#Variable which stores the time period for which a host will be blocked when packets sent exceeds max count for part three, set to a minute for convenience, could be set to 24 hours if necessary
#timeout_sec = 86400
timeout_sec = 60


class SimpleSwitch13(app_manager.RyuApp):
    OFP_VERSIONS = [ofproto_v1_3.OFP_VERSION]

    def __init__(self, *args, **kwargs):
        super(SimpleSwitch13, self).__init__(*args, **kwargs)
        self.mac_to_port = {}
        #code taken from the RyuBook for Part Two, full credit given below
        self.datapaths = {}
        self.monitor_thread = hub.spawn(self._monitor)
        #Below is my code to help for Part Three
		#The two dictionaries are used to track the number of packets sent from a particular port, and to track the number of times that the port has been blocked from exceeding max count
		#Both dictionaries have the port as the key. The block count is used to compare with the max count, increased each time the port is blocked to change the max number of packets without
		#impacting on the other ports and their counts. Set to 1 when initialised.
        self.port_packet_counts = {}
        self.block_count = {}

    '''
    The code below (and also further on) is taken from the RyuBook, from the link provided in the assignment handout, to handle the monitor thread which runs at the same time as the thread
	which handles the rest of the code. It checks to see if datapaths are stored in the dictionary initialised with the program, adding if they are not and deleting the datapath if it is no
	longer viable. The monitor method below it checks through each of the datapaths, calling request stats on the datapaths which are handled in a later method, then sleeping for 10 seconds before
	calling it again.
    The Ryubook can be accessed through the link below:
    https://osrg.github.io/ryu-book/en/Ryubook.pdf
    And the most relevant chapter from this book which was used in the code for this lab is Chapter 3.2 implemention of traffic monitor
    '''
    @set_ev_cls(ofp_event.EventOFPStateChange,[MAIN_DISPATCHER, DEAD_DISPATCHER])
    def _state_change_handler(self, ev):
        datapath = ev.datapath
        if ev.state == MAIN_DISPATCHER:
            if datapath.id not in self.datapaths:
                self.logger.debug('register datapath: %016x', datapath.id)
                self.datapaths[datapath.id] = datapath

        elif ev.state == DEAD_DISPATCHER:
            if datapath.id in self.datapaths:
                self.logger.debug('unregister datapath: %016x', datapath.id)
                del self.datapaths[datapath.id]

    def _monitor(self):
        while True:
            for dp in self.datapaths.values():
                self._request_stats(dp)
            hub.sleep(10)


    @set_ev_cls(ofp_event.EventOFPSwitchFeatures, CONFIG_DISPATCHER)
    def switch_features_handler(self, ev):
        datapath = ev.msg.datapath
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser


        ''' Task One:
		The code below handles the implementation of Task One, blocking traffic between H2 and H3.
		It does this by matching the eth_src and eth_dest through an OFPMatch, then if they match the pairing of h2-h3 or h3-h2 adding an empty list of actions which allows the packet to be dropped before
		calling add flow to handle the rest of the logic. It has a higher priority than the other packets to ensure that it will take precedence and be dropped due to the empty actions
		'''
        actions = []
        match = parser.OFPMatch(eth_src=h2_MAC, eth_dst=h3_MAC)
        self.add_flow(datapath, 1, match, actions)
        match = parser.OFPMatch(eth_src =h3_MAC, eth_dst=h2_MAC)
        self.add_flow(datapath, 1, match, actions)


		# install table-miss flow entry
        #
        # We specify NO BUFFER to max_len of the output action due to
        # OVS bug. At this moment, if we specify a lesser number, e.g.,
        # 128, OVS will send Packet-In with invalid buffer_id and
        # truncated packet data. In that case, we cannot output packets
        # correctly.  The bug has been fixed in OVS v2.1.0.
        match = parser.OFPMatch()
        actions = [parser.OFPActionOutput(ofproto.OFPP_CONTROLLER,
                                          ofproto.OFPCML_NO_BUFFER)]
        self.add_flow(datapath, 0, match, actions)

    def add_flow(self, datapath, priority, match, actions, buffer_id=None):
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser


        inst = [parser.OFPInstructionActions(ofproto.OFPIT_APPLY_ACTIONS,
                                             actions)]
        if buffer_id:
            mod = parser.OFPFlowMod(datapath=datapath, buffer_id=buffer_id,
                                    priority=priority, match=match,
                                    instructions=inst)
        else:
            mod = parser.OFPFlowMod(datapath=datapath, priority=priority,
                                    match=match, instructions=inst)
        datapath.send_msg(mod)

    @set_ev_cls(ofp_event.EventOFPPacketIn, MAIN_DISPATCHER)
    def _packet_in_handler(self, ev):
        # If you hit this you might want to increase
        # the "miss_send_length" of your switch
        if ev.msg.msg_len < ev.msg.total_len:
            self.logger.debug("packet truncated: only %s of %s bytes",
                              ev.msg.msg_len, ev.msg.total_len)
        msg = ev.msg
        datapath = msg.datapath
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser
        in_port = msg.match['in_port']

        pkt = packet.Packet(msg.data)
        eth = pkt.get_protocols(ethernet.ethernet)[0]

        if eth.ethertype == ether_types.ETH_TYPE_LLDP:
            # ignore lldp packet
            return
        dst = eth.dst
        src = eth.src

        dpid = datapath.id
        self.mac_to_port.setdefault(dpid, {})

        self.logger.info("packet in %s %s %s %s", dpid, src, dst, in_port)

        # learn a mac address to avoid FLOOD next time.
        self.mac_to_port[dpid][src] = in_port

        if dst in self.mac_to_port[dpid]:
            out_port = self.mac_to_port[dpid][dst]
        else:
            out_port = ofproto.OFPP_FLOOD

        actions = [parser.OFPActionOutput(out_port)]

        # install a flow to avoid packet_in next time
        if out_port != ofproto.OFPP_FLOOD:
            match = parser.OFPMatch(in_port=in_port, eth_dst=dst)
            # verify if we have a valid buffer_id, if yes avoid to send both
            # flow_mod & packet_out
            if msg.buffer_id != ofproto.OFP_NO_BUFFER:
                self.add_flow(datapath, 1, match, actions, msg.buffer_id)
                return
            else:
                self.add_flow(datapath, 1, match, actions)
        data = None
        if msg.buffer_id == ofproto.OFP_NO_BUFFER:
            data = msg.data

        out = parser.OFPPacketOut(datapath=datapath, buffer_id=msg.buffer_id,
                                  in_port=in_port, actions=actions, data=data)
        datapath.send_msg(out)

    '''The code below is a continuation of the code taken from RyuBook Chapter 3, implementing the traffic monitor. An adjustment made from the code provided
	in that book is that this implementation does not check flow statistics, merely checking the number of packets sent to or from a port without needing to know which host
	is being sent to or received from (e.g. from h1 to ?, from ? to h1). Other than that the code is mostly taken as is, with additions later for Tasks Two and Three
	'''
    def _request_stats(self, datapath):
        self.logger.debug('send stats request: %016x', datapath.id)
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser
        req = parser.OFPPortStatsRequest(datapath, 0, ofproto.OFPP_ANY)
        datapath.send_msg(req)


    @set_ev_cls(ofp_event.EventOFPPortStatsReply, MAIN_DISPATCHER)
    def _port_stats_reply_handler(self, ev):
        body = ev.msg.body
        datapath = ev.msg.datapath
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser
        self.logger.info('port '
        'rx-pkts'
        'tx-pkts')
        self.logger.info('-------- '
        '--------'
        '--------')
        #Create a stat object which is iterated through, pulling out the port no and the received and sent packets, ignoring other possible fields due to their irrelevance to this lab
        for stat in sorted(body, key=attrgetter('port_no')):
            self.logger.info('%8x %8d %8d',
            stat.port_no,
            stat.rx_packets, stat.tx_packets)
            input_port = stat.port_no
            '''
			Code for Task Two, which is counting traffic too and from h1. It does this by extracting the rx_packets and tx_packets from the stat object created above, printing out each before
			combining them together to give a total traffic number in regards to h1. The extra print statement is used to clarify that work was done for task two, rather than including it in a general
			stats print out as is needed for task three.
            '''
            if(stat.port_no == 1):
                self.logger.info('Printing stats for h1:  '
                                  'Total packets sent: %d  '
                                  'Total packets received: %d  '
                                  'Total packets for host 1: %d  ',
                                  stat.tx_packets,
                                  stat.rx_packets,
                                  stat.rx_packets + stat.tx_packets)
            '''
			Code for Task Three, which is counting traffic from a port/host and blocking if this number exceeds the max count
            First there is an initialisation for each port as it comes through to check that the block count dictionary has at least
			a value of 1 for use in the check later on.
			After this the number of packets sent from a host is updated to the current count over the previous one
			After these two steps, there is a comparison between the number of packets from the input port and a max count variable which is multiplied
			by the number of times that a port has been blocked. this is to ensure that the max count variable is not changed by one host which would affect all the others
			and also to ensure that the number of packets is never changed either. Instead it is compared to an increase of the max count proportional to the number of times a port
			has been blocked, and if this is exceeded it will carry on.
			if the max count is exceeded, then create an empty list of actions with a hard timeout proportional to the timeout seconds variable set above.
            The empty list of actions ensures that the packets will be dropped as long as the timeout period is kept, the priority is higher than any other in the program so that the
            packets will always be dropped, and the block count is increased by one.
            '''
            if input_port not in self.block_count:
                self.block_count[input_port] = 1

            self.port_packet_counts[input_port] = stat.tx_packets

            if self.port_packet_counts[input_port] >= (MAX_COUNT * self.block_count[input_port]):
                self.logger.info('Exceeded output for this port, blocking traffic')
                match = parser.OFPMatch(in_port = input_port)
                actions = []
                inst = [parser.OFPInstructionActions(ofproto.OFPIT_APPLY_ACTIONS, actions)]
                mod = parser.OFPFlowMod(datapath=datapath, priority =2, match = match, instructions=inst, hard_timeout=timeout_sec)
                self.block_count[input_port] = self.block_count[input_port] + 1
                datapath.send_msg(mod)
