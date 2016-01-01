import socket
import struct

## Quaternion class
class quat:
    def __init__(self, x, y, z, w):
        self.x = x
        self.y = y
        self.z = z
        self.w = w

## TCP sender class
class TCPsender:
    def __init__(self, port):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect(('localhost', port))

    def __del__(self):
        self.sock.close()

    def sendQuat(self, q):
        values = (9999.0, q.x, q.y, q.z, q.w) # 9999.0 is used as starting marker
        packer = struct.Struct('f f f f f')
        packed_data = packer.pack(*values)
        self.sock.sendall(packed_data)


tcpSender = TCPsender(1234) # Port 1234

# Example: quaternion (1.0, 2.0, 3.0, 4.0)
q = quat(1.0, 0.0, 0.0, 0.0)
tcpSender.sendQuat(q)
