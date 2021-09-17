a = 0b0110


def my_bin(x, digits=4):
    m = (1 << digits)
    x = ((x % m) + m) % m
    return bin(x + m).replace('0b1', '0b')


print(my_bin(a))  # 4-битное число
print(my_bin(~a))  # Его побитовое отрицание
print(my_bin(a >> 1))  # Его сдвиг вправо на 1
print(my_bin(a << 1))  # Его сдвиг влево на 1


print()

x = 0b0011
y = 0b1001

print(my_bin(x    ))  # X
print(my_bin(y    ))  # Y

print(my_bin(x | y))  # Побитовый OR
print(my_bin(x ^ y))  # Побитовый XOR
print(my_bin(x & y))  # Побитовый AND