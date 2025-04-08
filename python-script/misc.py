def getBytesFromInt(data):
    data_byte = bytearray(4);
    data_byte[0] = (data >> 24) & 0xFF;
    data_byte[1] = (data >> 16) & 0xFF;
    data_byte[2] = (data >> 8) & 0xFF;
    data_byte[3] = (data) & 0xFF;
    # print("SYS-> Data bytes: [%s]\n ", %(binascii.hexlify(data_byte));
    return data_byte

def getIntFromBytes(data_byte):
    data = (data_byte[0] << 24) + (data_byte[1]) << 16 + (data_byte[2] << 8) + (data_byte[3]);
    # print("SYS-> Data bytes: [%s]\n ", %(binascii.hexlify(data_byte));
    return data