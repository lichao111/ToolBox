#!/bin/bash
#甘肃新集群通信服务器进程运行情况统计

echo "========服务器时间:============="
date
echo " "
echo "=====================================3761=================================="
echo "通信程序运行情况:"
ps -eo pid,user,comm,start,etime|grep -e txapp -e txstart.sh

echo -e "\nport 29001:"
netstat -apn|grep txapp|grep 29001

echo -e "\nport 8101:"
port8101=$(netstat -apn|grep txapp|grep LISTEN|grep 8101)
echo ${port8101}
echo "terminal number:"  $(netstat -apn|grep txapp|grep 8101|grep -v LISTEN|wc -l)
echo -e "\n=====================================698=================================="
echo "通信程序运行情况:"
ps -eo pid,user,comm,start,etime|grep -e txserver698 -e  txstart698

echo -e "\nport 39001:"
netstat -apn|grep txserver698|grep 39001

echo -e "\nport 9003:"
port8101=$(netstat -apn|grep txserver698|grep LISTEN|grep 9003)
echo ${port8101}
echo "terminal number:"  $(netstat -apn|grep txserver698|grep 9003|grep -v LISTEN|wc -l)
