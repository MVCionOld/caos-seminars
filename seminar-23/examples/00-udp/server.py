import math
import socket
import sys

BUFFER_SIZE = 2 * 12  # 4Kb

host = sys.argv[1]
port = int(sys.argv[2])

sock = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
sock.bind((host, port))

while True:
    try:
        payload, addr = sock.recvfrom(BUFFER_SIZE)
        response = str(math.factorial(int(payload.decode())))
        sock.sendto(response.encode(), addr)
    except Exception as e:
        print(f"Got payload={payload} from address={addr}, error={e}", file=sys.stderr)
