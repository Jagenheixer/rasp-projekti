#!/bin/bash

### Tyhjennetään taulut ###
iptables -F
iptables -X
iptables -t nat -F
iptables -t nat -X

### Oletuspolitiikat: kaikki DROP ###
iptables -P INPUT DROP
iptables -P FORWARD DROP
iptables -P OUTPUT ACCEPT

### 1. Salli loopback ###
iptables -A INPUT -i lo -j ACCEPT

### 2. Salli jo muodostuneet yhteydet ###
iptables -A INPUT -m conntrack --ctstate ESTABLISHED,RELATED -j ACCEPT

### 3. Salli SSH (22/tcp) ###
iptables -A INPUT -p tcp --dport 22 -j ACCEPT

### 4. Salli HTTP (80/tcp) ###
iptables -A INPUT -p tcp --dport 80 -j ACCEPT

### 5. Takaovi (students.oamk.fi IP: 193.167.100.97) ###
iptables -A INPUT -s 193.167.100.97 -j ACCEPT

### 6. ICMP (ping)
iptables -A INPUT -p icmp -j ACCEPT

echo "Palomuuri otettu käyttöön."
