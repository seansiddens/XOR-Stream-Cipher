#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

long fileSize(FILE* fp);

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        printf("Usage: encrypt -i [FILE] -o [FILE]\n");
        return 1;
    }
    FILE* in;
    FILE* out;
    long size;
    char* msgBuffer;
    size_t result;
    char inputFile[256];
    char outputFile[256];
    srand(time(NULL));
    int input, output = 0;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if (argv[i][1] == 'i')
            {
                input = 1;
                strcpy(inputFile, argv[i+1]);
            }
            if (argv[i][1] == 'o')
            {
                output = 1;
                strcpy(outputFile, argv[i+1]);
            }
        }
    
    }

    if ((input == 0) || (output == 0))
    {
        printf("Please include all required arguments\n");
        return 1;
    }

    /* open msg file for read */
    in = fopen(inputFile, "r");
    if (in == NULL) {fputs("File error", stderr); exit(1);}
    
    /* obtain file size */
    size = fileSize(in);

    /* create cipher stream key and write to key file */
    unsigned char* key = (char*)malloc(sizeof(char) * size);
    for(unsigned char* ptr = key; (ptr - key) < size; ptr++)
    {
        *ptr = (unsigned char)rand() % 256; 
    }
    FILE* keyFile = fopen("key.dat", "wb");
    fwrite(key, sizeof(char), size, keyFile);

    /* allocate memory to contain the whole file */
    msgBuffer = (char*) malloc(sizeof(char) * size);
    if (msgBuffer == NULL) {fputs("Memory error", stderr); exit(2);}

    /* copy the file into the buffer */
    result = fread(msgBuffer, sizeof(char), size, in);
    if (result != size) {fputs("Reading error", stderr); exit(3);}

    /* print original message buffer */
    printf("\nMessage:\n");
    fwrite(msgBuffer, sizeof(char), size, stdout);

    /* encrypt message buffer */
    char* ptr = msgBuffer; 
    char* keyPtr = key;

    printf("\nEncrypted message:\n");
    for(int i = 0; i < size; i++)
    {
        *ptr = *ptr ^ *keyPtr;
        printf("%x ", (unsigned char)*ptr);

        keyPtr++;
        ptr++;
    }
    printf("\n\n");

    /* write encrypted buffer to file */
    out = fopen(outputFile, "wb");
    fwrite(msgBuffer, sizeof(char), size, out);

    /* print cipher stream key */
    printf("Key:\n");
    for(unsigned char* ptr = key; (ptr - key) < size; ptr++)
    {
        printf("%x ", (unsigned char)*ptr);
    }
    printf("\n");

    /* terminate */
    fclose(in);
    fclose(out);
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


