#include <assert.h>
#include <pwd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    struct stat s;
    fstat(STDIN_FILENO, &s);
    struct passwd* pw = getpwuid(s.st_uid);
    assert(pw);
    printf("USER NAME: %s\n", pw->pw_name);
    return 0;
}