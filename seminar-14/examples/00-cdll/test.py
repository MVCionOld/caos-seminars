import ctypes
import sys

import pytest


def test_ints():
    lib = ctypes.cdll.LoadLibrary("./lib_py_utils.so")
    for i in range(-100, 101):
        for j in range(-100, 101):
            assert i + j == lib.sum_ints(i, j)


def test_floats_incorrect():
    lib = ctypes.cdll.LoadLibrary("./lib_py_utils.so")
    for i in range(1, 101):
        for j in range(1, 101):
            with pytest.raises(AssertionError):
                assert i + j == lib.sum_floats(i, j)


def test_floats_correct():
    lib = ctypes.cdll.LoadLibrary("./lib_py_utils.so")
    lib.sum_floats.argtypes = [ctypes.c_float, ctypes.c_float]
    lib.sum_floats.restype = ctypes.c_float
    for i in range(-100, 101):
        for j in range(-100, 101):
            assert i + j == lib.sum_floats(i, j)


if __name__ == "__main__":
    sys.exit(pytest.main())
