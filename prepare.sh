#!/bin/bash

# $1 -> Network ID (/24)
# $2 -> Host ID
# $3 -> Ethernet interface

apt update && \
        apt upgrade -y

apt install net-tools -y

cat > /etc/netplan/99_config.yaml <<EOF
network:
  ethernets:
    $3:
      addresses:
        - $1.$2/24
      nameservers:
        addresses: [8.8.8.8, 8.8.4.4]
      routes:
        - to: default
          via: $1.1
  version: 2
EOF

netplan apply

cat >> /etc/hosts <<EOF

$1.10 master
$1.11 node01
$1.12 node02
$1.13 node03

EOF
