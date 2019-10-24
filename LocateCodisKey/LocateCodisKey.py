#!/bin/env python

#input the key name ,output which group this key locate

import binascii
import sys
  
try:
    print binascii.crc32(sys.argv[1])%1024
except Exception,e:
    print 'ERROR:%s' %(e,)
sys.exit(1)
