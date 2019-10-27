#!/bin/bash
#该脚本用于清楚redis缓存中的数据结果维护
#note: 313161量较大，如果只执行一次会存在不能清楚干净的问题，该问题怀疑是代理节点和各节点在大数据量更改时会有问题，脚本需要多执行几次
#时间没有实现自从更新，需手动改

/program/codis/redis-cli -a GScjyw@8954 -h 10.212.246.70 -p 45001 del ret:313161:20190902000000
/program/codis/redis-cli -a GScjyw@8954 -h 10.212.246.70 -p 45001 del ret:313001:20190902000000
/program/codis/redis-cli -a GScjyw@8954 -h 10.212.246.70 -p 45001 del ret:313002:20190902000000
/program/codis/redis-cli -a GScjyw@8954 -h 10.212.246.70 -p 45001 del ret:313003:20190902000000
/program/codis/redis-cli -a GScjyw@8954 -h 10.212.246.70 -p 45001 del ret:313004:20190902000000

echo $(/program/codis/redis-cli -a GScjyw@8954 -h 10.212.246.70 -p 45001 hlen ret:313161:20190902000000) 
echo $(/program/codis/redis-cli -a GScjyw@8954 -h 10.212.246.70 -p 45001 hlen ret:313001:20190830000000) 
echo $(/program/codis/redis-cli -a GScjyw@8954 -h 10.212.246.70 -p 45001 hlen ret:313002:20190830000000) 
echo $(/program/codis/redis-cli -a GScjyw@8954 -h 10.212.246.70 -p 45001 hlen ret:313003:20190830000000) 
echo $(/program/codis/redis-cli -a GScjyw@8954 -h 10.212.246.70 -p 45001 hlen ret:313004:20190830000000) 
 
