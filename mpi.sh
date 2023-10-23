#!/bin/bash

ssh-keygen -q -t rsa -N '' -f ~/.ssh/id_rsa <<<y >/dev/null 2>&1 && \
  ssh-copy-id -i ~/.ssh/id_rsa.pub root@master && \
  ssh-copy-id -i ~/.ssh/id_rsa.pub root@node01 && \
  ssh-copy-id -i ~/.ssh/id_rsa.pub root@node02

apt-get install build-essential -y
apt-get install mpich -y

if [ "$(hostname | grep master)" == "master" ]
then
cat > /mirror/machinefile <<EOF
node01:4
node02:4
EOF
fi
