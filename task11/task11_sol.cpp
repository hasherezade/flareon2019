#include <Windows.h>
#include <iostream>

#define IN_CHUNK_SIZE 4
#define OUT_CHUNK_SIZE 3

#define APP_PATH "vmmax_patched.exe"
size_t g_ChunkNum = 0;

bool g_Verbose = false;
char alnum_charset[] = "abcdefghijklmnopqrstuwxyzABCDEFGHIJKLMNOPRQRSTUVWXYZ0123456789";

unsigned char valid_hash[24] = {
    0x70, 0x70, 0xB2,
    0xAC, 0x01, 0xD2,
    0x5E, 0x61, 0x0A,
    0xA7, 0x2A, 0xA8,
    0x08, 0x1C, 0x86,
    0x1A, 0xE8, 0x45,
    0xC8, 0x29, 0xB2,
    0xF3, 0xA1, 0x1E
};

char g_Params[] = "FLARE2019 AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"; // valid: "FLARE2019 cHCyrAHSXmEKpyqoCByGGuhFyCmy86Ee"

bool patch_app(BYTE chunk_indx)
{
    HANDLE file = CreateFileA(APP_PATH, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (file == INVALID_HANDLE_VALUE) {
        std::cerr << "Could not open file!" << std::endl;
        return false;
    }
    HANDLE mapping = CreateFileMapping(file, 0, PAGE_READWRITE, 0, 0, 0);
    if (!mapping) {
        std::cerr << "Could not create mapping!" << std::endl;
        CloseHandle(file);
        return false;
    }
    BYTE *rawData = (BYTE*)MapViewOfFile(mapping, FILE_MAP_WRITE, 0, 0, 0);
    if (!rawData) {
        std::cerr << "Could not map view of file" << std::endl;
        CloseHandle(mapping);
        CloseHandle(file);
        return false;
    }
    size_t patch_offset = 0xA67;
    if (IsBadWritePtr(rawData, patch_offset + 1)) {
        std::cerr << "[-] Mapping of " << APP_PATH << " is invalid!" << std::endl;
        UnmapViewOfFile(rawData);
        CloseHandle(mapping);
        CloseHandle(file);
        return false;
    }

    BYTE *ptr = rawData + patch_offset;
    *ptr = chunk_indx;

    UnmapViewOfFile(rawData);
    CloseHandle(mapping);
    CloseHandle(file);
    std::cout << "Patched!\n";
    return true;
}

bool run_and_check(char val[4], size_t copy_start, size_t copy_count, size_t required_score)
{
    size_t c_offset = IN_CHUNK_SIZE * g_ChunkNum;
    size_t start = 10 + c_offset + copy_start;

    for (size_t i = 0; i < copy_count; i++) {
        g_Params[start + i] = val[copy_start + i];
    }
    
    SHELLEXECUTEINFO ShExecInfo = { 0 };
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = NULL;
    ShExecInfo.lpFile = APP_PATH;
    ShExecInfo.lpParameters = g_Params;
    ShExecInfo.lpDirectory = NULL;
    ShExecInfo.nShow = SW_HIDE;
    ShExecInfo.hInstApp = NULL;
    ShellExecuteEx(&ShExecInfo);
    WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

    DWORD code = 0;
    GetExitCodeProcess(ShExecInfo.hProcess, &code);
    uint8_t outs[sizeof(DWORD)] = { 0 };
    memcpy(outs, &code, sizeof(DWORD));

    if (g_Verbose) {
        
        std::cout << "params: " << g_Params << "\n";
        std::cout << "Triplet: " << std::hex << (int)outs[0] << " " << (int)outs[1] << " " << (int)outs[2] << "\n";
    }

    size_t score = 0;

    size_t h_offset = OUT_CHUNK_SIZE * g_ChunkNum;
    for (size_t i = 0; i < OUT_CHUNK_SIZE; i++) {
        if (outs[i] == valid_hash[i + h_offset]) score++;
    }
    if (score >= required_score) {
        std::cout << "score: " << score << " : params: " << g_Params << "\n"
            << "Triplet: " << std::hex << (int)outs[0] << " " << (int)outs[1] << " " << (int)outs[2] << std::endl;
        return true;
    }
    return false;
}

bool brute2(char *charset, size_t charset_len)
{
    char val[5] = { 0 };
    for (size_t i = 0; i < charset_len; i++) {
        for (size_t i1 = 0; i1 < charset_len; i1++) {
            val[1] = charset[i];
            val[2] = charset[i1];
            if (run_and_check(val, 1, 2, 3)) {
                std::cout << "Val: " << val + 1 << "\n";
                return true;
            }
        }
    }
    std::cout << "Brute finished, not found!\n";
    return false;
}


bool brute0(char *charset, size_t charset_len)
{
    char val[5] = { 0 };
    for (size_t i2 = 0; i2 < charset_len; i2++) {
        for (size_t i3 = 0; i3 < charset_len; i3++) {
            val[2] = charset[i2];
            val[3] = charset[i3];
            if (run_and_check(val, 2, 2, 1)) {
                std::cout << "Val: " << val + 2 << "\n";
                return true;
            }
        }
    }
    std::cout << "Brute finished, not found!\n";
    return false;
}

bool brute1(char *charset, size_t charset_len)
{
    char val[5] = { 0 };
    for (size_t i2 = 0; i2 < charset_len; i2++) {
        for (size_t i3 = 0; i3 < charset_len; i3++) {
            val[1] = charset[i2];
            val[0] = charset[i3];
            if (run_and_check(val, 0, 2, 2)) {
                std::cout << "Val: " << val << "\n";
                return true;
            }
        }
    }
    std::cout << "Brute finished, not found!\n";
    return false;
}

int main()
{
    bool solved = false;
    do {
        if (!patch_app(OUT_CHUNK_SIZE * g_ChunkNum)) {
            std::cerr << "Unable to patch the app!\n";
            break;
        }
        brute0(alnum_charset, strlen(alnum_charset));
        brute1(alnum_charset, strlen(alnum_charset));
        brute2(alnum_charset, strlen(alnum_charset));
        g_ChunkNum++;
        if (g_ChunkNum == 8) {
            solved = true;
        }
    } while (!solved);
    if (solved) {
        std::cout << "Success! Valid params:\n" << g_Params<< std::endl;
    }
    system("pause");
    return solved;
}
