#include <stdio.h>
#include <windows.h>

#include <peconv.h>

int main(int argc, char *argv[])
{
    if (argc < 3) {
        return -1;
    }
    char *in_path = argv[1];
    size_t payloadImageSize = 0; //path to "wopr.exe"
    BYTE* loaded_pe = peconv::load_pe_module(in_path, payloadImageSize, false, false);
    if (!loaded_pe) {
        printf("Loading failed!\n");
        return -2;
    }
    char *out_path = argv[2];
    
    //relocate the module to base=0
    peconv::relocate_module(loaded_pe, payloadImageSize, 0);
    
    //get the section with the Entry Point (that is the .text section)
    IMAGE_SECTION_HEADER *hdr1 = peconv::get_section_hdr(loaded_pe, payloadImageSize, 0);
    if (!hdr1) return -3;
    
    FILE *fp = fopen(out_path, "wb");
    if (!fp) return -4;
    
    // save the relocated .text section
    size_t written = fwrite(loaded_pe + hdr1->VirtualAddress,1, hdr1->Misc.VirtualSize, fp);
    fclose(fp);
    int res = 1;
    if (written == hdr1->Misc.VirtualSize) {
        printf("OK! dumped to: %s\n", out_path);
        res = 0;
    }
    peconv::free_pe_buffer(loaded_pe);
    system("pause");
    return res;
}
