#!/usr/bin/env python
import socket
import getopt
import sys
import struct

#18行的注释去掉之后，tcp链接通过reset的方式关闭
#不去掉tcp链接正常关闭

def telnet(ip,port):
        server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        try:
                print ip,int(port)
                server.connect((ip,int(port)))
                l_onoff = 1 
                l_linger = 0                                                                                                         
                #turn the SO_LINGER socket option on and set the linger time to 0 seconds.
                # This will cause TCP to abort the connection when it is closed
                server.setsockopt(socket.SOL_SOCKET, socket.SO_LINGER,struct.pack('ii', l_onoff, l_linger))
                server.close()
                print("Success!")
        except Exception as err:
                print ("Faili!")
        finally:
                server.close()

if __name__ == '__main__':
        if len(sys.argv) !=3 :
                print("Please input the ip and port !!")
        else:
                telnet(sys.argv[1],sys.argv[2])
