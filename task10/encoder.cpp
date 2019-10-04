//Test the algorithm: encrypting/decrypting
#include <Windows.h>
#include <iostream>

void decipher(DWORD* v, BYTE *k)
{
    unsigned int num_rounds = 32;
    unsigned int i;
    DWORD v0 = v[0], v1 = v[1];
    DWORD delta = -0x61C88647;
    DWORD sum = delta * num_rounds;

    for (i = 0; i < num_rounds; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum >> 11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
    }
    v[0] = v0; v[1] = v1;
}

void encipher(DWORD* v, BYTE *k)
{
    unsigned int num_rounds = 32;
    unsigned int i;
    DWORD v0 = v[0], v1 = v[1];
    DWORD sum = 0, delta = -0x61C88647;

    for (i = 0; i < num_rounds; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum >> 11) & 3]);
    }
    v[0] = v0; v[1] = v1;
}

bool dump_to_file(char *out_filename, BYTE *buf, size_t fsize)
{
    FILE *f2 = fopen(out_filename, "wb");
    if (!f2) {
        std::cout << "Couldn't open the file: " << out_filename << "\n";
        return false;
    }
    fwrite(buf, 1, fsize, f2);
    fclose(f2);
    std::cout << "Saved output to: " << out_filename << "\n";
    return true;
}

int main(int argc, char *argv[])
{
    char key[] = "TEST;
    
    if (argc < 4) {
        printf("Args: <input file> <output file> <decrypt?>\n");
        system("pause");
        return -1;
    }
    char *filename = argv[1];
    FILE *f = fopen(filename, "rb");
    if (!f) {
        std::cout << "Couldn't read the file: " << filename << "\n";
        system("pause");
        return -2;
    }

    char *out_filename = argv[2];
    bool is_decrypt = false;
    if (argv[3][0] != '0') {
        is_decrypt = true;
        std::cout << "Decrypt mode ON\n";
    }
    fseek(f, 0, SEEK_END);
    size_t fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    BYTE* buf = (BYTE*)calloc(fsize, 1);
    if (!buf) {
        system("pause");
        return -3;
    }
    fread(buf, 1, fsize, f);
    fclose(f); f = NULL;

    if (!is_decrypt) {
        BYTE *chunk = buf;
        for (size_t i = 0; i < (fsize / 8); i++) {
            encipher((DWORD*)((ULONG_PTR)chunk), (BYTE*)key);
            chunk += 8;
        }
    }
    else {
        BYTE *chunk = buf;
        for (size_t i = 0; i < (fsize / 8); i++) {
            decipher((DWORD*)((ULONG_PTR)chunk), (BYTE*)key);
            chunk += 8;
        }
    }

    dump_to_file(out_filename, buf, fsize);
    return 0;
}
