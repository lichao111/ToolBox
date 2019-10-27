#!/bin/bash
#甘肃新集群，解析前置服务器各进程运行情况统计

echo "========服务器时间:============="
date
echo " "
echo "=====================================3761=================================="
echo "任务程序运行情况:"
ps -eo pid,user,comm,start,etime|grep -e rwapp -e rwstart.sh
if [ $(ps -eo pid,user,comm,start,etime|grep -e rwapp -e rwstart.sh|wc -l) -ne 2 ];then
	echo "异常！！！"
fi

echo -e "\n解析程序运行情况:"
ps -eo pid,user,comm,start,etime|grep -e jxapp -e jxstart.sh
if [ $(ps -eo pid,user,comm,start,etime|grep -e jxapp -e jxstart.sh|wc -l) -ne 2 ];then
	echo "异常！！！"
fi
echo -e "\n入库程序运行情况:"
ps -eo pid,user,comm,start,etime|grep -e rkeventapp -e eventstart.sh -e rkdataapp -e datastart.sh
if [ $(ps -eo pid,user,comm,start,etime|grep -e rkeventapp -e eventstart.sh -e rkdataapp -e datastart.sh|wc -l) -ne 4 ];then
	echo "异常！！！"
fi

echo -e "\nport 29003(rw listen port):"
netstat -apn|grep rwapp|grep 29003
if [ $(netstat -apn|grep rwapp|grep 29003|grep LISTEN|wc -l) -ne 1 ];then
	echo "异常！！！"
fi

echo -e "\nport 29002(jx listen port):"
netstat -apn|grep jxapp|grep 29002
JxClientNum=$(netstat -apn|grep jxapp|grep 29002|grep -v LISTEN|wc -l)
if [ ${JxClientNum} -ne 10 ];then
	echo -e "任务程序与解析程序链接异常，有${JxClientNum}个链接！！！"
fi

echo -e "\nport 29001(tx listen port):"
netstat -apn|grep jxapp|grep 29001
JxClientNum=$(netstat -apn|grep jxapp|grep 29001|grep -v LISTEN|wc -l)
if [ ${JxClientNum} -ne 1 ];then
	echo -e "解析程序与通信程序链接异常，有${JxClientNum}个链接！！！"
fi

echo -e "\n=====================================698=================================="
echo "任务程序运行情况:"
ps -eo pid,user,comm,start,etime|grep -e rwserver698 -e rwstart698.sh
if [ $(ps -eo pid,user,comm,start,etime|grep -e rwserver698 -e rwstart698.sh|wc -l) -ne 2 ];then
	echo "异常！！！"
fi

echo -e "\n解析程序运行情况:"
ps -eo pid,user,comm,start,etime|grep -e jxserver698 -e jxstart698.sh
if [ $(ps -eo pid,user,comm,start,etime|grep -e jxserver698 -e jxstart698.sh|wc -l) -ne 2 ];then
	echo "异常！！！"
fi

echo -e "\n入库程序运行情况:"
ps -eo pid,user,comm,start,etime|grep -e rkserver698 -e rkstart698.sh
if [ $(ps -eo pid,user,comm,start,etime|grep -e rkserver698 -e rkstart698.sh|wc -l) -ne 2 ];then
	echo "异常！！！"
fi

echo -e "\nport 39003(rw listen port):"
netstat -apn|grep rwserver698|grep 39003
if [ $(netstat -apn|grep rwserver698|grep 39003|grep LISTEN|wc -l) -ne 1 ];then
	echo "异常！！！"
fi

echo -e "\nport 39002(jx listen port):"
netstat -apn|grep jxserver698|grep 39002
if [ $(netstat -apn|grep jxserver698|grep 39002|grep -v LISTEN|wc -l) -ne 4 ];then
	echo "异常！！！"
fi

echo -e "\nport 39001(tx listen port):"
netstat -apn|grep jxserver698|grep 39001
if [ $(netstat -apn|grep jxserver698|grep 39001|wc -l) -ne 4 ];then
	echo "异常！！！"
fi
