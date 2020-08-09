#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

long fileSize(FILE* fp);

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        printf("Usage: encrypt -i [FILE] -k [KEY_FILE]\n");
        return 1;
    }
    FILE* in;
    FILE* key;
    long size;
    char* msgBuffer;
    char* keyBuffer;
    size_t result;
    char inputFile[256];
    char keyFile[256];
    srand(time(NULL));
    int input, arg2 = 0;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if (argv[i][1] == 'i')
            {
                input = 1;
                strcpy(inputFile, argv[i+1]);
            }
            if (argv[i][1] == 'k')
            {
                arg2 = 1;
                strcpy(keyFile, argv[i+1]);
            }
        }
    
    }

    if ((input == 0) || (arg2 == 0))
    {
        printf("Please include all required arguments\n");
        return 1;
    }

    /* open msg file for read */
    in = fopen(inputFile, "rb");
    if (in == NULL) {fputs("File error", stderr); exit(1);}
    
    /* obtain file size */
    size = fileSize(in);

    /* load key file into buffer */
    key = fopen(keyFile, "rb");
    keyBuffer = (char*)malloc(sizeof(char) * size);
    fread(keyBuffer, sizeof(char), size, key);

    /* allocate memory to contain the whole file */
    msgBuffer = (char*) malloc(sizeof(char) * size);
    if (msgBuffer == NULL) {fputs("Memory error", stderr); exit(2);}

    /* copy the file into the buffer */
    result = fread(msgBuffer, sizeof(char), size, in);
    if (result != size) {fputs("Reading error", stderr); exit(3);}

    /* print encrypted message */
    printf("\nEncrypted message:\n");
    fwrite(msgBuffer, sizeof(char), size, stdout);
    printf("\n");

    /* decrypt message buffer */
    char* ptr = msgBuffer; 
    char* keyPtr = keyBuffer;

    for(int i = 0; i < size; i++)
    {
        *ptr = *ptr ^ *keyPtr;

        keyPtr++;
        ptr++;
    }
    printf("\nDecrypted message:\n");
    fwrite(msgBuffer, sizeof(char), size, stdout);
    printf("\n\n");


    /* terminate */
    fclose(in);
    fclose(key);
    free(msgBuffer);

    return 0;
}

/* Returns size in bytes of file */
long fileSize(FILE* fp)
{
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    return size;
}


