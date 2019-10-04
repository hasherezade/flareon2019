import hashlib, io, pkgutil, random, struct, sys, time, lzma
from ctypes import *

def calc_pe_md5(module):
    pe_header = string_at(module, 1024)
    pe_offset, = struct.unpack_from('=I', pe_header, 0x3C)

    _, _, sections_count, _, _, _, size_of_opt_hdr, _ =  struct.unpack_from('=IHHIIIHH', pe_header, pe_offset)
    assert size_of_opt_hdr >= 0x90

    entry_point, = struct.unpack_from('=I', pe_header, pe_offset + 0x28) #entry point
    for section_indx in range(sections_count):
        name, virtual_size, virtual_address, raw_size, raw_address, _, _, _, _, _ = struct.unpack_from('=8sIIIIIIHHI', pe_header, 0x28 * section_indx + pe_offset + size_of_opt_hdr + 0x18)
        if virtual_address <= entry_point < virtual_address + virtual_size:
            break

    code_section = bytearray(string_at(module + virtual_address, virtual_size))
    
    reloc_rva, reloc_size = struct.unpack_from('=II', pe_header, pe_offset + 0xa0) #base relocation table
    relocs = string_at(module + reloc_rva, reloc_size)

    processed_relocs = 0
    while processed_relocs <= len(relocs) - 8:
        reloc_page, reloc_block_size = struct.unpack_from('=II', relocs, processed_relocs)

        if reloc_page == 0 and reloc_block_size == 0:
            break

        next_reloc = relocs[processed_relocs + 8:processed_relocs + reloc_block_size]

        for i in range(len(next_reloc) >> 1):
            reloc_val, = struct.unpack_from('=H', next_reloc, 2 * i)
            reloc_type = reloc_val >> 12
            if reloc_type != 3: continue
            reloc_val = reloc_val & 0xFFF
            ready = reloc_page + reloc_val - virtual_address
            if 0 <= ready < len(code_section): 
                struct.pack_into('=I', code_section, ready, struct.unpack_from('=I', code_section, ready)[0] - module)

        processed_relocs += reloc_block_size

    return hashlib.md5(code_section).digest()