from __future__ import print_function
import serial
import delay

global ser;

pkt_size = 38

def init(port, baudrate):
    global ser;
    ser = serial.Serial(port, baudrate)
    ser.bytesize = serial.EIGHTBITS #number of bits per bytes
    ser.parity = serial.PARITY_NONE #set parity check: no parity
    ser.stopbits = serial.STOPBITS_ONE #number of stop bits
    ser.timeout = None          #block read
    #ser.timeout = 1            #non-block read
    #ser.timeout = 2              #timeout block read
    ser.xonxoff = False     #disable software flow control
    ser.rtscts = False     #disable hardware (RTS/CTS) flow control
    ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
    ser.writeTimeout = 2     #timeout for write

     # setDTRHigh();
     # delay.delay_ms(1000);
     # setDTRLow();
     # delay.delay_ms(3000);
     # ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control

    if ser.isOpen():
        print("-> SYS: stick ready, sending commands...")
        delay.delay_ms(1000);
        ser.flush();
    else:
        exit()


def deinit():
    global ser;
    ser.close()

def getCRC(data):
    crc = 0x0000;
    for k in range(0,len(data)):
        crc = crc ^ (data[k] << 8);
        crc = crc & 0xffff
        i = 8;
        for n in range(0,8):
            if (crc & 0x8000):
                crc = crc << 1 ^ 0x1021;
            else:
                crc = crc << 1;
    return crc

def txPkt(data):
    size = len(data)
    pkt = bytearray(size + 5)
    k = 0;
    pkt[k] = 0x55;
    k += 1
    pkt[k] = 0x55;
    k += 1
    for n in range(0,size):
        pkt[k] = data[n];
        k +=1
    crc16 = getCRC(data)
    pkt[k] = (crc16 >> 8) & 0xff;
    k += 1
    pkt[k] = (crc16 & 0xff);
    k += 1
    pkt[k] = 0x04;
    txData(pkt);

def txData(data):
    global ser;
    # Log the packet data to a text file
    with open("sent_packets.txt", "a") as log_file:
        log_file.write(f"Sent Packet: {data.hex()}\n")  # Write the hex representation of the data to the file
    
    # Send the packet via serial communication
    ser.write(data);
    print("Tx PKT[%s]\n" % data.hex());
    # ser.flushInput();
    # ser.flushOutput();
    # ser.reset_input_buffer();

def dataAvaialbe():
    global ser;
    return ser.inWaiting();

def read(size):
    global ser;
    return ser.read(size);

def setDTRLow():
    ser.setDTR(False);

def setDTRHigh():
    ser.setDTR(True);

def test():
    while 1:
        size = ser.inWaiting()
        if(size > 0):
            print("Data availalbe %d\n" % size);
            data = ser.read(size);
            print(data.hex());

def wait4Pkt(wait_period, cmd):
    # ser.flushInput();
    id = 0
    sync = bytearray([0x5A, 0xA5]);
    data = bytearray();
    error = bytearray([0x00]);
    crc_data = bytearray();
    for k in range(0, wait_period):		# delay max 1 second
        size = ser.inWaiting();
        if size > 1 :
            tmp = ser.read(1);
            # print("TMP[%x]\n"%tmp[0]);
            if tmp[0] == sync[id]:
                id += 1
            elif tmp[0] == sync[0]:
                id = 1
            if id > 1:
                break;
        delay.delay_ms(1)
    # print("SYNC ID[%d]"%id);
    if(id < 2 ):
       return error;

    # found sync get the command and payload size
    for k in range(0, wait_period):		# delay max 1 second
        size = ser.inWaiting();
        if size > 2:
            tmp = ser.read(2);
            # print("TMP[%s]\n"%tmp.hex());
            rcmd = tmp[0];
            rsize = tmp[1];
            break;
        delay.delay_ms(1)

    for k in range(0, wait_period):		# delay max 1 second
        size = ser.inWaiting();
        if size >= (rsize + 2) :
            data = ser.read(rsize);
            crc_data = ser.read(2);
            break;
        delay.delay_ms(1)
    crc16 = sync[0] + sync[1] + rcmd + rsize;
    for k in range(0, len(data)):
        crc16 += data[k];

    crc = crc_data[0];
    crc = (crc << 8) | (crc_data[1]);
    if crc16 != crc :
        print("Wrong crc\n");
        return error;
    for k in range(0, len(cmd)):
        if cmd[k] == rcmd:
            cmd_byte = bytearray([cmd[k]]);
            newdata = cmd_byte + data;
            # print(newdata.hex());
            return newdata
    else:
        return error;

    # size = ser.inWaiting();
    # if size > 0 :
    #     print("USB size available %d" %size);
    #     data = ser.read(size);
    #     print(data);

def wait4data(wait_period):
    data = bytearray(32)
    for k in range(0, wait_period):		# delay max 1 second
        size = ser.inWaiting();
        if size > 0 :
            data = ser.read(1);
            print("[%x][%c]" % (data[0], data[0]));
        delay.delay_ms(1)
