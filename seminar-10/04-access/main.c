#include <assert.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
    assert(argc >= 2);
    int status = access(argv[1], X_OK);
    fprintf(stderr, "Can execute %s: %s\n", argv[1], (status == 0) ? "yes" : "no");
    return 0;
}