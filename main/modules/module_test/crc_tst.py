import binascii
import zlib

max_crc = 0xffffffff

def inverse_crc(data):
    crc = binascii.crc32(data) & max_crc
    invcrc = max_crc - crc
    return invcrc.to_bytes(4, 'little')

# data = b"01yhhaljfanbajhdhjaeuy1y4qitruhfhajald"
# crc_calc = b"\x60\x1b\x9e\xce"
# data += crc_calc
data = b"hello world"
crc = binascii.crc32(data) & max_crc
print("CRC:\t\t%X" %crc)

crc = binascii.crc32(b"hello ") & max_crc
crc = binascii.crc32(b"world", crc) & max_crc
print("CRC:\t\t%X" %crc)

# crc = zlib.crc32(data) & max_crc
# print("CRC:\t\t%X" %crc)

crc = 0x0
for i in range(len(data)):
    print("%X" %crc)
    crc = binascii.crc32(data[i:i+1], crc) & max_crc
# for i in range(len(data)-1, -1, -1):
#     crc = binascii.crc32(data[i:i+1], crc) & max_crc
print("CRC:\t\t%X" %crc)

# inv_crc = inverse_crc(data)
# print("Inv CRC:\t%s" %binascii.b2a_hex(inv_crc))
