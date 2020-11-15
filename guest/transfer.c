#include <stdio.h>

int main(void) {
    FILE* file;
    char buf[8];

    for (int i = 0; i < sizeof(buf); i++) {
        buf[i] = 'A';
    }
    buf[sizeof(buf) - 1] = '\n';

    while (1) {
        file = fopen("/dev/ttyGS1", "w+");
        fwrite(buf, 1, sizeof(buf), file);
        fclose(file);
    }
    return 0;
}
