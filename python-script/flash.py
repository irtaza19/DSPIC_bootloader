from __future__ import print_function
import sys
import time
import delay
import uart
import misc
import os
import uart
import delay
from obd2 import OBD2

#-----------------------------------------------
#IRQDesign d.o.o Tuzla
#2021.10.22
#
#Notes:
#----------------------------------------------

print("\nwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww")
print("w OBD2 Testing script                               w")
print("wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww")


fw_fid = open("..C:\Users\irtaz\OneDrive\Desktop\output_packet.bin", 'rb');
fw_data = bytearray(fw_fid.read());
obd = OBD2("COM5",4800);
res = obd.enterBootloader();
res = obd.firmwareUpgrade(fw_data);
res = obd.reset();
exit();