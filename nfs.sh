#!/bin/bash

# $1 -> Network ID (/24)

if [ "$(hostname | grep master)" == "master" ]
then
        apt-get install nfs-server -y

        mkdir /mirror
        echo "/mirror $1/24(rw,sync,no_root_squash)" >> /etc/exports
        service nfs-kernel-server restart
else
        apt-get install nfs-common -y

        mkdir /mirror
        mount master:/mirror /mirror
        echo "master:/mirror /mirror nfs" | tee -a /etc/fstab
fi
