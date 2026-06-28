
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* filename;

void Write(char* text) {
    FILE* fp = fopen(filename, "wb");
    
    int wrote = fwrite(text, sizeof(char), strlen(text), fp);
    
    printf("bytes wrote: %i\n", wrote);

    fclose(fp);
}

void Append(char* text) {
    FILE* fp = fopen(filename, "rb");
    FILE* tmp = fopen("temp.bin", "wb");  

    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        fputc(ch, tmp);
        if (ch == '\n')
            break;
    }

    fwrite(text, 1, strlen(text), tmp);

    char buffer[256];
    unsigned int read;
    while ((read = fread(buffer, 1, sizeof(buffer), fp)) > 0)
        fwrite(buffer, 1, read, tmp);

    
    fclose(fp);
    fclose(tmp);

    remove(filename);
    rename("temp.bin", filename);

}

void Read() {
    FILE* fp = fopen(filename, "rb");
    
    char buffer[64];
    while (fgets(buffer, sizeof(buffer), fp))
        printf(buffer);

    fclose(fp);
}

int GetSize() {
    FILE* fp = fopen(filename, "rb");

    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    rewind(fp);

    fclose(fp);
    return size;
}

int main() {
    filename = "code.bin";
    
    Append("adrien\n");

    Read();

    printf("size: %i\n", GetSize());
}
