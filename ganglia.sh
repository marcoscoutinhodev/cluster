#!/bin/bash

# $1 -> Network ID (/24)

if [ "$(hostname | grep master)" == "master" ]
then
        apt-get install apache2 -y

        systemctl start apache2.service
        systemctl enable apache2.service

        apt-get install software-properties-common && add-apt-repository ppa:ondrej/php
        apt-get update

        apt-get install php7.2 libapache2-mod-php7.2 php7.2-common php7.2-gmp php7.2-curl php7.2-intl php7.2-mbstring php7.2-xmlrpc php7.2-mysql php7.2-gd php7.2-xml php7.2-cli php7.2-zip -y

        sed -i "/^short_open_tag = Off/{s/.*/short_open_tag = On/; :a;n;ba}" /etc/php/7.2/apache2/php.ini
        sed -i "/^memory_limit = 128M/{s/.*/memory_limit = 256M/; :a;n;ba}" /etc/php/7.2/apache2/php.ini
        sed -i "/^upload_max_filesize = 2M/{s/.*/upload_max_filesize = 100M/; :a;n;ba}" /etc/php/7.2/apache2/php.ini
        sed -i "/^max_execution_time = 30/{s/.*/max_execution_time = 360/; :a;n;ba}" /etc/php/7.2/apache2/php.ini
        sed -i "/^;max_input_vars = 1000/{s/.*/max_input_vars = 1500/; :a;n;ba}" /etc/php/7.2/apache2/php.ini
        sed -i "/^;date.timezone =/{s/.*/date.timezone = America\/Sao_Paulo/; :a;n;ba}" /etc/php/7.2/apache2/php.ini

        systemctl restart apache2.service

        apt-get update
        apt-get install ganglia-monitor rrdtool gmetad ganglia-webfrontend -y

        systemctl start ganglia-monitor.service
        systemctl enable ganglia-monitor.service

        sed -i "/^data_source \"this host\"/{s/.*/data_source \"UVVCluster\" 10 localhost $1.10/; :a;n;ba}" /etc/ganglia/gmetad.conf

        sed -i "/^  name = \"unspecified\"/{s/.*/  name = \"UVVCluster\"/; :a;n;ba}" /etc/ganglia/gmond.conf
        sed -i "/^  owner = \"unspecified\"/{s/.*/  owner = \"MarcosViniciusCoutinho\"/; :a;n;ba}" /etc/ganglia/gmond.conf

        sed -i "/^  mcast_join = 239.2.11.71/{s/.*/  \/\*mcast_join = 239.2.11.71\*\//; :a;n;ba}" /etc/ganglia/gmond.conf

        sed -i "/^  mcast_join = 239.2.11.71/{s/.*/  \/\*mcast_join = 239.2.11.71\*\//; :a;n;ba}" /etc/ganglia/gmond.conf
        sed -i "/^  bind = 239.2.11.71/{s/.*/  \/\*bind = 239.2.11.71\*\//; :a;n;ba}" /etc/ganglia/gmond.conf

        cp /etc/ganglia-webfrontend/apache.conf /etc/apache2/sites-enabled/ganglia.conf

        systemctl restart ganglia-monitor
        systemctl restart gmetad
        systemctl restart apache2
else
        apt-get install ganglia-monitor -y
        sed -i "/^  mcast_join = 239.2.11.71/{s/.*/  host = $1.10/; :a;n;ba}" /etc/ganglia/gmond.conf
        systemctl restart ganglia-monitor
fi
