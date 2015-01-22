#!/bin/sh
# Initialize all the chains by removing all rules
iptables --flush
iptables -t nat --flush
iptables -t mangle --flush

# Delete any user-defined chains
iptables --delete-chain
iptables -t nat --delete-chain
iptables -t mangle --delete-chain

# Set default policies
iptables --policy INPUT DROP
iptables --policy OUTPUT ACCEPT
iptables --policy FORWARD DROP

# Accept all traffic on the loopback (lo) device
iptables -A INPUT -i lo -p all -j ACCEPT
iptables -A OUTPUT -o lo -p all -j ACCEPT

# Accept internally-requested input
iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT

# Accept user-specified traffic
iptables -A INPUT -i em1 -p tcp --dport 80 -j ACCEPT
iptables -A INPUT -i em1 -p tcp --dport 81 -j ACCEPT

iptables -A INPUT -i em2 -p tcp --dport 22 -j ACCEPT
iptables -A INPUT -i em2 -p tcp --dport 80 -j ACCEPT
iptables -A INPUT -i em2 -p tcp --dport 81 -j ACCEPT
iptables -A INPUT -i em2 -p tcp --dport 3306 -j ACCEPT
# ping
iptables -A INPUT -p icmp -m state --state ESTABLISHED,RELATED -j ACCEPT
iptables -A INPUT -i em2 -p icmp -j ACCEPT
