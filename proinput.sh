#!/bin/bash
#该脚本实现把前置入库程序的入库线程用pstack进行检测，默认两个入库线程 

service_name='dataInput'
while :
do
	date
	process_num=$(ps -ef|grep $service_name|grep -v "grep"|wc -l)
	if [ $process_num -eq 0 ];then
		echo "dataInput died" 
		sleep 1
		continue
	fi
	PID=$(ps -ef|grep $service_name|grep -v "grep"|awk '{print $2}')
	#获取第一个入库线程的PID
	InputPID1=$(pstack ${PID} | head -n 1|awk -F 'LWP ' '{print $2}'|awk -F ')' '{print $1}')
	#一般情况下第二个入库线程与第一个相差1
	InputPID2=`expr $InputPID1 + 1`
	pstack ${InputPID1}
	pstack ${InputPID2}
	echo -e '\n'
done
