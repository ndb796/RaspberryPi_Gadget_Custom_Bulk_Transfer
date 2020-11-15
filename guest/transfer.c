#include <stdio.h>

int main(void) {
    FILE* file;
    char buf[4096];

    for (int i = 0; i < sizeof(buf); i++) {
        buf[i] = 'A';
    }

    while (1) {
        file = fopen("/dev/ttyGS1", "r+");
        fwrite(buf, 1, sizeof(buf), file);
        fclose(file);
    }
    return 0;
}
