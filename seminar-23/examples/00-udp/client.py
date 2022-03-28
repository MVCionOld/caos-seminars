import socket
import sys

BUFFER_SIZE = 2 * 12  # 4Kb

host = "localhost"
port = int(sys.argv[1])
addr = (host, port)

sock = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

while True:
    response = None
    try:
        payload = input()
        sock.sendto(payload.encode(), addr)
        response, _ = sock.recvfrom(BUFFER_SIZE)
        print(f"{payload}! = {response.decode()}")
    except Exception as e:
        print(f"Send payload={payload} to address={addr}, got response={response}, error={e}", file=sys.stderr)
