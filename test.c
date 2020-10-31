#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    printf("Encryption/Decryption kernel module (vignere cipher)\n");
    printf("Usage:\n");
    printf("E:<key>:<string to encrypt>\n");
    printf("D:<key>:<string to decrypt>\n");
    printf("eg: E:pascal:somethingtoencrypt\n\n");
    char data[1024];
    scanf("%[^\n]s", data);

    FILE *fp = fopen("/dev/encdec", "w");
    if (fp == NULL) {
        printf("Failed to write to device...");
        return -1;
    }

    fprintf(fp, "%s\n", data);
    fclose(fp);

    char output[1024];
    fp = fopen("/dev/encdec", "r");
    fscanf(fp, "%[^\n]", output);
    printf("\nOutput:\n%s\n", output);
    return 0;
}
