import struct
import sys

class BinaryString(str):
    def __init__(self, s=''):
        self.position = 0
        str.__init__(s)

    def read_int(self, num=None):
        if not num: return self.read_int(1)[0]
        L = []
        for i in range(num):
            c = self.read_char()
            if c == -128: # 16-bit
                c = struct.unpack('h', self[self.position:self.position+2])[0]
                self.position += 2
            elif c == -127: # 32-bit
                c = struct.unpack('i', self[self.position:self.position+4])[0]
                self.position += 4
            L.append(c)
        return L

    def read_char(self, num=None):
        if not num: return self.read_char(1)[0]
        self.position += num
        if self.position > len(self):
            sys.exit(-1)
        return struct.unpack('b' * num, self[self.position - num:self.position])

    def read_uchar(self, num=None):
        if not num: return self.read_char(1)[0]
        self.position += num
        return struct.unpack('B' * num, self[self.position - num:self.position])

    def read_string(self, num=None):
        if not num: return self.read_string(1)[0]
        L = []
        for i in range(num):
            start, self.position = self.position, self.index('\x00', self.position)+1
            L.append(self[start:self.position-1])
        return L

    def __repr__(self):
        return ''.join(self.putint(ord(i)) for i in self)

    def __str__(self):
        return str([[ord(j) for j in self.putint(ord(i))] for i in self])

    def putint(self, i):
        y = { (-127, 128): ('', 'b'), (-0x8000, 0x8000): ('\x80', 'h') }
        try:
            v = next(y[k] for k in y if k[0] < i < k[1])
        except:
            v = ('\x81', 'i')
        return v[0] + struct.pack(v[1], i)
        

