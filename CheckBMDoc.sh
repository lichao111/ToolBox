#!/bin/bash
# 用于核对BM档案是否存在
# 输入 需要被校验的TerminalID
cat TerminalIDList | while read LINE
do
  ttl=$(redis-cli -p 16381 -a Longshine01# -h 20.81.11.135 -c TTL BM\$10002\$$LINE 2>&1 | grep -v Warning)
  echo $LINE $ttl
done
