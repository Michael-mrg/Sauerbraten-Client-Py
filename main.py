import sauer

def parse_packet(p):
    token = p.read_int()

    if token == 0x01: # SV_SERVINFO
        sauer.client_num = p.read_int()
        assert p.read_int() == 257
        sauer.send_packet("\0[mVa]bot\0\0\0")
    else:
        print token

    return 0
