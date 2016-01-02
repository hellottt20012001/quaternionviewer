import socket
import struct

## Quaternion class
class quat:
    def __init__(self, w, x, y, z):
        self.w = w
        self.x = x
        self.y = y
        self.z = z

## TCP sender class
class TCPsender:
    def __init__(self, port):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect(('localhost', port))

    def __del__(self):
        self.sock.close()

    def sendQuat(self, q):
        values = (9999.0, q.w, q.x, q.y, q.z) # 9999.0 is used as starting marker
        packer = struct.Struct('f f f f f')
        packed_data = packer.pack(*values)
        self.sock.sendall(packed_data)


tcpSender = TCPsender(1234) # Port 1234

# Example: quaternion (1.0, 2.0, 3.0, 4.0)
q = quat(1.0, 0.0, 0.0, 0.0)
tcpSender.sendQuat(q)
