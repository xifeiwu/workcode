#!/bin/sh
count=0
for siteip in $(seq 1 254)
do
	site="192.168.160.$siteip"
	#echo $site
	ping -c3 -W3 $site &> /dev/null
	if [ "$?" == "0" ]; then
		echo "$site is UP"
		$((count++))
	else
		echo "$site is DOWN"
	fi
done

#for siteip in $(seq 1 254)
#do
#        site="192.168.161.$siteip"
#        #echo $site
#        ping -c1 -W1 $site &> /dev/null
#        if [ "$?" == "0" ]; then
#                echo "$site is UP"
#                $((count++))
#        else
#                echo "$site is DOWN"
#        fi
#done
#for siteip in $(seq 1 254)
#do
#        site="192.168.162.$siteip"
#        #echo $site
#        ping -c1 -W1 $site &> /dev/null
#        if [ "$?" == "0" ]; then
#                echo "$site is UP"
#                $((count++))
#        else
#                echo "$site is DOWN"
#        fi
#done

echo ${count} IP is Online.
