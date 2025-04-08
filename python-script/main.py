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
# IRQDesign d.o.o Tuzla
# 2021.10.22
#
# Notes:
#-----------------------------------------------

print("\nwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww")
print("w OBD2 Testing script                               w")
print("wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww")

# ✅ Change this to True to enable flashing
if True:  
    fw_fid = open(r"C:\Users\irtaz\OneDrive\Desktop\output_packet.bin", 'rb')
    fw_data = bytearray(fw_fid.read())  # ✅ Correct indentation

    obd = OBD2("COM5", 4800)  # ✅ Corrected COM port
    res = obd.enterBootloader()
    res = obd.firmwareUpgrade(fw_data)
    res = obd.reset()
    exit()

obd = OBD2("COM5", 4800)