import base64
import random


def xor_bytes(lhs, rhs):
    return bytes(ilhs ^ irhs for (ilhs, irhs) in zip(lhs, rhs))


plain_text = b"Hello, spies, you won't hack my exclusive encoding:)"

common_secret = bytes(random.randint(0, 255) for i in range(len(plain_text)))
print(f"common_secret = {base64.b64encode(common_secret)}")

cipher_text = xor_bytes(plain_text, common_secret)
print(f"cipher_text = {base64.b64encode(cipher_text)}")

recovered_plain_text = xor_bytes(cipher_text, common_secret)
print(recovered_plain_text)

assert plain_text == xor_bytes(xor_bytes(plain_text, common_secret), common_secret)
