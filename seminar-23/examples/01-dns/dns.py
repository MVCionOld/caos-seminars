import socket
from functools import reduce

# About dig
#
## https://clck.ru/eaRVx

# About DNS req & res
#
## https://routley.io/posts/hand-writing-dns-messages/
## https://www.ibm.com/docs/en/qsip/7.3.3?topic=levels-parsing-dns-query-response-fields
## https://cabulous.medium.com/dns-message-how-to-read-query-and-response-message-cfebcb4fe817

BUFFER_SIZE = 1024

dns_host = "8.8.8.8"
port = 53
addr = (dns_host, port)

sock = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)


def encode_hostname(hostname):
    return reduce(
        lambda acc, part: (
            acc +
            len(part).to_bytes(length=1, byteorder="little") +
            bytes(part, encoding="ascii")
        ),
        hostname.split('.'),
        bytes("", encoding="ascii")
    )


cnt = 0
while True:
    hostname = input().strip()

    query = (
            cnt.to_bytes(length=2, byteorder="little") +
            b"\x01\x00\x00\x01\x00\x00\x00\x00\x00\x00" +
            encode_hostname(hostname) +
            b"\x00" + b"\x00\x01\x00\x01"
    )
    sock.sendto(query, addr)

    data, addr = sock.recvfrom(BUFFER_SIZE)
    ip_from_udp_request = ".".join(str(b) for b in data[-4:])  # compare with `dig +short <hostname>`

    print(f"hostname={hostname} resolved as {ip_from_udp_request}")

    cnt += 1
