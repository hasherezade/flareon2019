#include <Windows.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>

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

bool test_key(BYTE* buf,  BYTE *key)
{
    const size_t chunk_size = 8;
    BYTE chunk[chunk_size] = { 0 };
    memcpy(chunk, buf, chunk_size);

    decipher((DWORD*)((ULONG_PTR)chunk), key);

    unsigned char check_gif[4] = {
        0x47, 0x49, 0x46, 0x38
    };

    if (memcmp(chunk, check_gif,sizeof(check_gif)) == 0) {
        return true;
    }
    return false;
}

bool brute_key(BYTE *buf, BYTE *key, size_t key_len, size_t key_start)
{
    srand(time(NULL));

    while (true) {
        for (size_t i = key_start; i < key_len; i++) {
            key[i] = rand() % 0xFF;
        }
        if (test_key(buf, key)) {
            std::cout << "Found the key: " << key << "\n";
            break;
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Args: <input file>\n");
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

    const size_t key_len = 4;
    BYTE key[key_len + 1] = { 0 };
    /*
    key[0] = 0x31;
    brute_key(buf, key, key_len, 1);
    */
    brute_key(buf, key, key_len, 0);
    BYTE *chunk = buf;
    for (size_t i = 0; i < (fsize / 8); i++) {
        decipher((DWORD*)((ULONG_PTR)chunk), (BYTE*)key);
        chunk += 8;
    }
    char *out_filename = "out.gif";
    dump_to_file(out_filename, buf, fsize);
    return 0;
}
