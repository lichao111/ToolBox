#!/bin/bash
TerminalList=$1
TerminalArry=(${TerminalList//,/ }) #单斜杠替换一次 双斜杠替换所有
for var in ${TerminalArry[@]}
do
  a1=$(echo $var|cut -c1-4)
  a2=$(echo $var|cur -c5-8)
  A2=$((0x$a2))
  Addr="S\$10002\$${a1}\$${A2}\$\$"
  echo $Addr $(redis-cli -p 1234 -a xxxx -h 127.0.0.1 -c hget $Addr TADDR 2>&1 |grep -v Warnin|grep -v nil)
done
