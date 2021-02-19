#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFERSIZE 4

int main(int argc, char *argv[]){
    unsigned short a;
    FILE *fpt;
    // Open the file
    fpt=fopen(argv[1], "rb");
    if (fpt == NULL){
        printf("Unable to open %s for reading\n",argv[2]);
        exit(0);
    }

    while (1){

        if (feof(fpt)) {
            break;
        }

        fread(&a, sizeof(unsigned short), 1, fpt);

        printf("%ld is %hu \n",ftell(fpt), a);


        if (ftell(fpt) > 100) exit(0);
    }

    fclose(fpt);
    return 0;
}
