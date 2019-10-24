#This file can copy test you inputs and output in one file.

#!/usr/bin/python
import sys



info = "input the Json that you want to copy as the argv[1],\n \
	the outcome int the file which named out_5000       \n \
	the default copy number is 5000                     \n   "
Json =sys.argv[1]
if(Json == "--help"):
	print info
copy_number = 5000
if(3 == len(sys.argv)):
	copy_number = sys.argv[2]
	out_file = "out_"+str(copy_number)
else:
	out_file = "out_5000"	
out = open (out_file,'w')

for  i in range(0,int(copy_number)):
	out.write(Json)
