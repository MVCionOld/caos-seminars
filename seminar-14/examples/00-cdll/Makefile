py_utils.o:
	gcc -c py_utils.c -o py_utils.o
c_objdump: py_utils.o
	objdump -t py_utils.o
	rm -f py_utils.o
cpp_objdump: py_utils.o
	objdump -t py_utils.o
	rm -f py_utils.o
lib_py_utils.so: py_utils.o
	gcc -shared -fPIC py_utils.o -o lib_py_utils.so
run: lib_py_utils.so
	pytest test.py
clean:
	rm -rf py_utils.o lib_py_utils.so .pytest_cache __pycache__
