#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFERSIZE 4

int main(int argc, char *argv[]){
    unsigned char a;
    FILE *fpt;
    // Open the file
    fpt=fopen(argv[1], "rb");
    if (fpt == NULL){
        printf("Unable to open %s for reading\n",argv[2]);
        exit(0);
    }

    while (1){

        fread(&a, sizeof(unsigned char), 1, fpt);

        printf("%ld is %x \n",ftell(fpt), a);
        // If EOF reached
        if (feof(fpt)) {
            break;
        }

        if (ftell(fpt) > 6460) exit(0);

    }

    fclose(fpt);
    return 0;
}
