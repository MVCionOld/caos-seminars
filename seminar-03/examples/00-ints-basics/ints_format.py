n_bits = 4
pow2_n_bits = 1 << n_bits


def normalize(val): return ((val % pow2_n_bits) + pow2_n_bits) % pow2_n_bits


def explain(val):
    return "unsigned %d, signed % 2d, bytes %s" % (
        val,
        val if val < (pow2_n_bits >> 1) else val - pow2_n_bits,
        bin(val + pow2_n_bits)[n_bits - 1:]
    )

if __name__ == '__main__':
    for val in range(pow2_n_bits):
        print("val = %d <-> %s" % (val, explain(normalize(val))))
