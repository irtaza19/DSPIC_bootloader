from __future__ import print_function 
import serial
import delay

class ecOBD2(object):
    pass

class bootloaderCmdOBD2(object):
    pass

c_ecOBD2 = ecOBD2();
c_ecOBD2.crcError = 0x01;
c_ecOBD2.packageTooLong = 0x02;
c_ecOBD2.unknownCommand = 0x10;
c_ecOBD2.invalidCommandFormat = 0x20;
c_ecOBD2.sequenceError = 0x30;
c_ecOBD2.authenticationError = 0x50;
c_ecOBD2.programmingError = 0x80;
c_ecOBD2.verifitacitonError = 0x90;

c_bootloadercmdOBD2 = bootloaderCmdOBD2();
c_bootloadercmdOBD2.reset = 0x02;
c_bootloadercmdOBD2.getVersion = 0x06;
c_bootloadercmdOBD2.getDevID = 0x07;
c_bootloadercmdOBD2.getHWRev = 0x08;
c_bootloadercmdOBD2.getDateTime = 0x09;
c_bootloadercmdOBD2.getSerialNumber = 0x0A;
c_bootloadercmdOBD2.getDeviceName = 0x0B;
c_bootloadercmdOBD2.getICID = 0x0C;
c_bootloadercmdOBD2.getExtendedName = 0x0D;
c_bootloadercmdOBD2.getDeviceStatus = 0x0E;
c_bootloadercmdOBD2.getFwStatus = 0x0F;
c_bootloadercmdOBD2.startUpload = 0x30;
c_bootloadercmdOBD2.sendChunk = 0x31;
c_bootloadercmdOBD2.getOscInfo = 0x1B;
c_bootloadercmdOBD2.getMinimumFwVersion = 0x2E;
c_bootloadercmdOBD2.getFwInfo = 0x2F;



class OBD2:
    def __init__(self,port,baudrate):
        self.baudrate = baudrate;
        self.port = port;

        self.serial = serial.Serial(self.port, self.baudrate);
        self.serial.bytesize = serial.EIGHTBITS #number of bits per bytes
        self.serial.parity = serial.PARITY_NONE #set parity check: no parity
        self.serial.stopbits = serial.STOPBITS_ONE #number of stop bits
        self.serial.timeout = None          #block read
        self.serial.xonxoff = False     #disable software flow control
        self.serial.rtscts = False     #disable hardware (RTS/CTS) flow control
        self.serial.dsrdtr = False       #disable hardware (DSR/DTR) flow control
        self.serial.writeTimeout = 2     #timeout for write

        if self.serial.isOpen():
            self.serial.flush();

    def setBaudrate(self,baudrate):
        self.serial.baudrate = baudrate;
    # Bootloader
    def getBootloaderCmdCRC(self,data):
        crc = 0x0000;
        for k in range(0,len(data)):
            crc = crc ^ (data[k] << 8);
            crc = crc & 0xffff
            i = 8;
            for n in range(0,8):
                if (crc & 0x8000):
                    crc = (crc << 1) ^ 0x1021;
                else:
                    crc = crc << 1;
                crc &= 0xFFFF
        return crc
    def sendBootloaderCMD(self,cmd,data):
        size = 0x0000 | len(data);
        tmp = bytearray([((size >> 8) & 0xff),(size & 0xff)]);
        # print("Size[%d][%x][%x][%s][%x][%s]\n"%(size,((size >> 8) & 0xff),(size & 0xff),tmp.hex(),cmd));

        pkt_data = bytearray([cmd]) + bytearray([((size >> 8) & 0xff),(size & 0xff)]) + bytearray(data);
        crc16 = self.getBootloaderCmdCRC(pkt_data);
        pkt_data.append((crc16 >> 8) & 0xff);
        pkt_data.append(crc16 & 0xff);
        pkt = bytearray(0);
        pkt.append(0x55);
        pkt.append(0x55);
        for k in range(0,len(pkt_data)):
            if((pkt_data[k] == 0x04) or  (pkt_data[k] == 0x05) or (pkt_data[k] == 0x55)):
                pkt.append(0x05);
            pkt.append(pkt_data[k]);
        pkt.append(0x04);
        print("OBD Bootloader tx pkt[%s]\n"%pkt.hex());
        self.txData(pkt);
    def wait4ResponseBootloader(self,wait_period):
        data = bytearray(0);
        flag = 0;
        for k in range(0,wait_period):		# delay max 1 second
            size = self.serial.inWaiting();
            if size > 0 :
                tmp = self.serial.read(1);
                if(flag == 0):
                    if(tmp[0] == 0x55):
                        flag = 1;
                        data.append(tmp[0]);
                else:
                    data.append(tmp[0]);
                    if(tmp[0] == 0x04):
                        return data;
            delay.delay_ms(1);
    def enterBootloader(self):
        self.sendCMD("ATZ");
        delay.delay_ms(100);
        self.setBaudrate(4800);
        self.sendBootloaderCMD(0x03,[]);
        res = self.wait4ResponseBootloader(1000);
        print("OBD response[%s]\n"%res.hex());
        if(len(res) != 0):
            if(res[2] & 0x40):
                self.sendBootloaderCMD(c_bootloadercmdOBD2.getVersion,[]);
                res = self.wait4ResponseBootloader(1000);
                version = res[4];
                version = (version << 8) | res[5];

                self.sendBootloaderCMD(c_bootloadercmdOBD2.getDevID,[]);
                res = self.wait4ResponseBootloader(1000);
                deviceID = res[4];
                deviceID = (deviceID << 8) | res[5];

                self.sendBootloaderCMD(c_bootloadercmdOBD2.getHWRev,[]);
                res = self.wait4ResponseBootloader(1000);
                hwRev = res[4];
                hwRev = (hwRev << 8) | res[5];

                self.sendBootloaderCMD(c_bootloadercmdOBD2.getSerialNumber,[]);
                res = self.wait4ResponseBootloader(1000);
                serialNumber = res[4:12].decode("utf-8") ;

                self.sendBootloaderCMD(c_bootloadercmdOBD2.getDeviceName,[]);
                res = self.wait4ResponseBootloader(1000);
                deviceName = res[4:36].decode("utf-8") ;

                self.sendBootloaderCMD(c_bootloadercmdOBD2.getFwStatus,[]);
                res = self.wait4ResponseBootloader(1000);
                fwstatus = res[4];

                self.sendBootloaderCMD(c_bootloadercmdOBD2.getDeviceStatus,[]);
                res = self.wait4ResponseBootloader(1000);
                deviceStatus = res[4];

                self.sendBootloaderCMD(c_bootloadercmdOBD2.getICID,[]);
                res = self.wait4ResponseBootloader(1000);
                icID = res[4];
                icID = (icID << 8) | res[5];

                self.sendBootloaderCMD(c_bootloadercmdOBD2.getExtendedName,[]);
                res = self.wait4ResponseBootloader(1000);
                extendedName = res[4:11].decode("utf-8") ;

                self.sendBootloaderCMD(c_bootloadercmdOBD2.getFwInfo,[]);
                res = self.wait4ResponseBootloader(1000);
                res = self.parseCmd(res);
                fwInfo = res[0:len(res)-2];
                # extendedName = res[4:11].decode("utf-8") ;
                self.sendBootloaderCMD(c_bootloadercmdOBD2.getOscInfo,[]);
                res = self.wait4ResponseBootloader(1000);
                oscInfo = self.parseCmd(res);

                self.sendBootloaderCMD(c_bootloadercmdOBD2.getMinimumFwVersion,[]);
                res = self.wait4ResponseBootloader(1000);
                minFwVersion = self.parseCmd(res);

                self.sendBootloaderCMD(c_bootloadercmdOBD2.getDateTime,[]);
                res = self.wait4ResponseBootloader(1000);
                dateTime = self.parseCmd(res);

                print("--------------------------------------------------------------\n")
                print("| Bootloader info                                            |\n")
                print("--------------------------------------------------------------\n")
                print("Version: %x"%version);
                print("Device ID: %x"%deviceID);
                print("Hardware revision: %x"%hwRev);
                print("Serial number: %s"%serialNumber);
                print("Device name: %s"%deviceName);
                print("Firmware status: %d"%fwstatus);
                print("Device Status[%d]"%deviceStatus);
                print("ICID: %x"%icID);
                print("Extended name: %s"%extendedName);
                print("Firmware info: %s"%fwInfo.hex());
                print("Osc info: %s"%oscInfo.hex());
                print("Minimum fw version: %s"%minFwVersion.hex());
                print("Date & time: %s"%dateTime.hex());
                return 0;
                #ack
            elif(res[2] & 0x80):
                return 1;
                #nack
            else:
                return 2;
                #unkwnonw
    def parseCmd(self,pkt):
        data = bytearray();
        size = len(pkt);
        flag = 0;
        for k in range(2,size-1):
            if (pkt[k] == 0x05) and (flag == 0):
                flag = 1;
            else:
                data.append(pkt[k]);
                flag = 0;
        return data;
    def firmwareUpgrade(self,data):
        size = 0x000000 | len(data);
        uploadPkt = bytearray(0);
        uploadPkt.append((size >> 16) & 0xff);
        uploadPkt.append((size >> 8) & 0xff);
        uploadPkt.append((size) & 0xff);
        uploadPkt.append(1);
        self.sendBootloaderCMD(c_bootloadercmdOBD2.startUpload,uploadPkt);
        res = self.wait4ResponseBootloader(1000);
        res = self.parseCmd(res);
        chunk_size = res[2];
        chunk_size = (chunk_size << 8) | res[3];
        # print("Max chunk size[%d]\n"%chunk_size);
        baddr = 0x00000000;
        eaddr = baddr + size;
        #chunk_size = 16;
        chunk_size = 256;
        chunk_cnt = size // chunk_size;
        if size % chunk_size > 0:
            chunk_cnt += 1
        chunk_id = 0;
        print("\nFirmware flashing: \n",end='');
        while True:
            addr = baddr + (chunk_id * chunk_size);
            if(addr >= eaddr):
                # print("Firmware sent\n");
                break;
            pkt =  bytearray([(chunk_id >> 8) & 0xff, chunk_id & 0xff]) + data[addr:addr+(chunk_size)];
            res = self.sendBootloaderCMD(c_bootloadercmdOBD2.sendChunk,pkt);
            res = self.wait4ResponseBootloader(1000);
            if res is None:
                continue;
            res = self.parseCmd(res);
            # print("[%d/%d][%s]\n"%(chunk_id,chunk_cnt,pkt.hex()));
            # print(f"[{chunk_id+1}/{int(chunk_cnt)}] {pkt.hex()}");
            chunk_id += 1;

        print("\nFirmware flashing completed\n",end='');
        return res;
    def reset(self):
        self.sendBootloaderCMD(c_bootloadercmdOBD2.reset,[]);


    # Main app
    def txData(self,data):
        self.serial.write(data);
    def sendCMD(self,cmd):
        data = cmd.encode();
        pkt = bytearray(0);
        for k in range(0,len(data)):
            pkt.append(data[k])
        pkt.append(0x0D);
        print("OBD tx pkt[%s]\n"%pkt.hex());
        self.txData(pkt);
    def wait4Response(self,wait_period):
        data = bytearray(0);
        flag = 0;
        for k in range(0,wait_period):		# delay max 1 second
            size = self.serial.inWaiting();
            if size > 0 :
                tmp = self.serial.read(1);
                print("Size[%d][%x]\n"%(size,tmp));
                if(flag == 0):
                    if(tmp[0] == 0x55):
                        flag = 1;
                        data.append(tmp[0]);
                else:
                    data.append(tmp);
                    if(tmp == 0x04):
                        return data;
            delay.delay_ms(1);
            
