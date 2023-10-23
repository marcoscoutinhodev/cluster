#!/bin/bash

# $1 -> Network ID (/24)

apt install sshpass -y

ssh-keygen -q -t rsa -N '' -f ~/.ssh/id_rsa <<<y >/dev/null 2>&1 && \
        ssh-copy-id -i ~/.ssh/id_rsa.pub master@master && \
        ssh-copy-id -i ~/.ssh/id_rsa.pub node01@node01 && \
        ssh-copy-id -i ~/.ssh/id_rsa.pub node02@node02

timedatectl set-timezone "America/Sao_Paulo"

apt install chrony -y

if [ "$(hostname | grep master)" == "master" ]
then
        sed -i '/^pool /{s/.*/server 0.br.pool.ntp.org iburst/; :a;n;ba}' /etc/chrony/chrony.conf
        sed -i '/^pool /{s/.*/server 1.br.pool.ntp.org iburst/; :a;n;ba}' /etc/chrony/chrony.conf
        sed -i '/^pool /{s/.*/server 2.br.pool.ntp.org iburst/; :a;n;ba}' /etc/chrony/chrony.conf
        sed -i '/^pool /{s/.*/server 3.br.pool.ntp.org iburst/; :a;n;ba}' /etc/chrony/chrony.conf

        echo "allow $1.0/24" >> /etc/chrony/chrony.conf
else
        sed -i "/^pool /{s/.*/server $1.10 iburst/; :a;n;ba}" /etc/chrony/chrony.conf
        sed -i '/^pool /d' /etc/chrony/chrony.conf
fi

systemctl restart chrony
