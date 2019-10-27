#!/bin/bash
#codis集群有时会出现从节点同步主节点失败的情况，该脚本可以实现5分钟进行一次检测，如果有不同的问题会强制同步所有主从节点
#使用需要修改codis-admin的地址 以及 dashboard的IP、port

while true
do
        echo $(date)
        #if [ $(date +%H) -eq 14 ]
        #then
        #       if [ $(date +%M) -eq 0 ]
        #       then
                        out_of_sync_num=$(/opt/go/src/github.com/CodisLabs/codis/bin/codis-admin --dashboard=20.76.189.174:18080 --list-group|grep out_of_sync | grep true| wc -l)
                        #echo ${out_of_sync_num}
                        if [ ${out_of_sync_num} -ne 0 ]
                        then
                                echo ${out_of_sync_num} "groups need resync,resync all group "
                                /opt/go/src/github.com/CodisLabs/codis/bin/codis-admin --dashboard=20.76.189.174:18080 --resync-group --all
                        else
                                echo "none groups out_of_sync"
                        fi
        #       fi
        #fi
sleep 300
done
