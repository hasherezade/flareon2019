#include <Windows.h>
#include <iostream>

unsigned char enc_bytes[32] = {
    0x79, 0x5A, 0xB8, 0xBC, 0xEC, 0xD3, 0xDF, 0xDD, 0x99, 0xA5, 0xB6, 0xAC,
    0x15, 0x36, 0x85, 0x8D, 0x09, 0x08, 0x77, 0x52, 0x4D, 0x71, 0x54, 0x7D,
    0xA7, 0xA7, 0x08, 0x16, 0xFD, 0xD7, 0x41, 0x20
};

char flare_flag[33] = { 0 };

bool decode_flag_chunk(unsigned char host_addr[4])
{
    if (host_addr[0] != 127 || host_addr[3] & 1) {
        return false;
    }
    int indx = (host_addr[2] & 15);
    if ((2 * indx + 1) >= sizeof(enc_bytes)) {
        std::cout << "Buffer exceeded\n";
        return false;
    }
    flare_flag[2 * indx] = host_addr[1] ^ enc_bytes[2 * indx];
    flare_flag[2 * indx + 1] = host_addr[1] ^ enc_bytes[2 * indx + 1];
    return true;
}

bool fill_moves()
{
    unsigned char addresses[40][4] = {
        { 127,150,96,223 },
        { 127,252,212,90 },
        { 127,215,177,38 },
        { 127,118,118,207 },
        { 127,89,38,84 },
        { 127,109,155,97 },
        { 127,217,37,102 },
        { 127,49,59,14 },
        { 127,182,147,24 },
        { 127,0,143,11 },
        { 127,227,42,139 },
        { 127,101,64,243 },
        { 127,201,85,103 },
        { 127,200,76,108 },
        { 127,50,67,23 },
        { 127,157,96,119 },
        { 127,99,253,122 },
        { 127,25,74,92 },
        { 127,168,171,31 },
        { 127,148,37,223 },
        { 127,108,24,10 },
        { 127,37,251,13 },
        { 127,34,217,88 },
        { 127,57,238,51 },
        { 127,196,103,147 },
        { 127,141,14,174 },
        { 127,238,7,163 },
        { 127,230,231,104 },
        { 127,55,220,79 },
        { 127,184,171,45 },
        { 127,196,146,199 },
        { 127,191,78,251 },
        { 127,159,162,42 },
        { 127,184,48,79 },
        { 127,127,29,123 },
        { 127,191,34,35 },
        { 127,5,22,189 },
        { 127,233,141,55 },
        { 127,55,250,81 },
        { 127,53,176,56 }
    };
    size_t addresse_len = 40;
    for (size_t i = 0; i < addresse_len; i++) {
        decode_flag_chunk(addresses[i]);
    }
    std::cout << flare_flag << "\n";
    return true;
}

int main(int argc, char *argv[])
{
    fill_moves();
    system("pause");
    return 0;
}
