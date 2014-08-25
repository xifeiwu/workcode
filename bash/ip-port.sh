#!/bin/sh
LOGFILE="ip-port.log"
if [ -f ${LOGFILE} ]; then
    rm ${LOGFILE}
fi

function port_scan()
{
    echo -e "\033[2;32m-$@ is UP\033[0m" | tee -a ${LOGFILE}
    nmap $@ | tee -a ${LOGFILE}
}

net160=192.168.160
net162=192.168.162
for((cnt=1;cnt<255;cnt++))
do
    ping -c1 -W1 ${net160}.${cnt} &> /dev/null
    if [ "$?" == "0" ]; then
        port_scan ${net160}.${cnt}
    fi
done
for((cnt=1;cnt<255;cnt++))
do
    ping -c1 -W1 ${net162}.${cnt} &> /dev/null
    if [ "$?" == "0" ]; then
        port_scan ${net162}.${cnt}
    fi
done
