from binary_string import BinaryString
import sauer

def parse_packet(p, cn=None):
    token = p.read_int()

    if token == 0x01: # SV_SERVINFO
        sauer.client_num = p.read_int()
        assert p.read_int() == 257
        sauer.send_packet("\0[mVa]bot\0\0\0")
    elif token == 0x02: # SV_WELCOME
        sauer.send_packet("\x37%c\1" % sauer.client_num)
    elif token == 0x51: # SV_CLIENT
        cl, len = p.read_uint(2)
        parse_packet(p.sub_buffer(len), cl)
    else:
        print '%02x' % token

    return 0
