#!/bin/bash
# 用于计算redis中三天超时时间是何时写入的
# 输入剩余生存时间
allTime=`expr 3\* 24 \* 60 \* 60`
#echo $allTime
subTime=`expr ${allTime} - $1`
timel=`expr $(date +%s) - $subTime`
Time=$(date -d \@$timel)
echo 
echo "写入时间为：" $Time
echo 
