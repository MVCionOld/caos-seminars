gcc main.c -o aarch64.out && ./aarch64.out
gcc -m32 main.c -o aarch32.out && ./aarch32.out
arm-linux-gnueabi-gcc -marm main.c -o arm.out && qemu-arm -L $ARM_GNUEABI_PATH ./arm.out