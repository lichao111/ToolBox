#!/bin/bash
#该脚本实现对采集前置redis缓存中的二类数据进行统计，该脚本日期可以自动滚动
#note: 由于redis操作速度比较快，且量比较大，该脚本的数据统计并不完全精确，但可以反应缓存中的数据量

total_json=0
json=0
while :
do
for i in 11 12 13 14 15 16 17 18 21 23 24 26 27 28   #所有机器的IP尾号，IP前三字节已经在第10行写死
do
	for j in {0..19}   #解析程序开启的读取缓存线程数
	do
		json=$(/program/codis/redis-cli -a GScjyw@8954 -p 45001 -h 10.212.246.109 llen dcq:10.212.246.${i}:13:${j})
		#echo "llen dcq:10.212.246.${i}:13:${j}   " $json
		total_json=`expr $json + $total_json`
	done
done
echo $(date)  "数据缓存总数据总量:     ${total_json}"
sleep 5
total_json=0
_yyyymm=$(date +%Y%m)
_day=`expr $(date +%d) - 1`
if [ ${#_day} -eq 1 ]
then
	_day='0'${_day}
fi
_date=${_yyyymm}${_day}'000000'
echo "ret:mr" $(/program/codis/redis-cli -a GScjyw@8954 -h 10.212.246.70 -p 45001 hlen ret:313161:${_date})
done
