import ctypes

INT32_MAX = (1 << 31) - 1
UNSIGNED_INT32_MAX = (1 << 32) - 1

lib = ctypes.CDLL("./lib_isgreater.so")
lib.signed_isgreater.argtypes = [ctypes.c_int]
lib.unsigned_isgreater.argtypes = [ctypes.c_uint]

print(f"42 + 1 > 42 is {lib.signed_isgreater(42)}")
print(f"INT32_MAX + 1 > INT32_MAX is {lib.signed_isgreater(INT32_MAX)}")
print(f"42u + 1 > 42u is {lib.unsigned_isgreater(42)}")
print(f"UNSIGNED_INT32_MAX + 1 > UNSIGNED_INT32_MAX is {lib.unsigned_isgreater(UNSIGNED_INT32_MAX)}")