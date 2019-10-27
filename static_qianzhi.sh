#!/bin/bash
#该脚本实现对旧前置系统的所有程序运行情况进行检测

ps -eo user,comm,start,etime |grep -e eventinput -e dataInput -e jxapp -e comm_
echo 'terminal num' $(netstat -apn|grep 8101 |wc -l)
echo '======================='
jxpd=$(ps -ef|grep jxapp_ | grep -v "grep"|awk '{print $2}')
totalfd=$(lsof -p ${jxpd} | wc -l)
errorfd=$(lsof -p ${jxpd} | grep protocol |wc -l)
echo 'jx total fd:' ${totalfd}
echo 'jx error fd:' ${errorfd}
port_connects=$(netstat -apn|grep jxapp_|grep 9999|wc -l)
echo '9999 port clients:' ${port_connects}
echo '======================='
datafd=$(ps -ef|grep dataInput | grep -v "grep"|awk '{print $2}')
echo 'dataInput total fd:' $(lsof -p ${datafd} |wc -l)
echo 'dataInput error fd:' $(lsof -p ${datafd} |grep protocol |wc -l)
echo '======================='
datafd=$(ps -ef|grep eventinput | grep -v "grep"|awk '{print $2}')
echo 'eventinput total fd:' $(lsof -p ${datafd} |wc -l)
echo 'eventinput error fd:' $(lsof -p ${datafd} |grep protocol |wc -l)
