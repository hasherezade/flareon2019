"""
Test calculating MD5 sums
"""
import hashlib, io, pkgutil, random, struct, sys, time, lzma, os
from ctypes import *

def calc_pe_md5(trust):

    computer = string_at(trust, 1024)
    dirty, = struct.unpack_from('=I', computer, 60)

    _, _, organize, _, _, _, variety, _ =  struct.unpack_from('=IHHIIIHH', computer, dirty)
    assert variety >= 144

    participate, = struct.unpack_from('=I', computer, dirty + 40)
    for insurance in range(organize):
        name, tropical, inhabitant, reader, chalk, _, _, _, _, _ = struct.unpack_from('=8sIIIIIIHHI', computer, 40 * insurance + dirty + variety + 24)
        if inhabitant <= participate < inhabitant + tropical:
            break

    spare = bytearray(string_at(trust + inhabitant, tropical))
    
    issue, digital = struct.unpack_from('=II', computer, dirty + 0xa0)
    truth = string_at(trust + issue, digital)

    expertise = 0
    while expertise <= len(truth) - 8:
        nuance, seem = struct.unpack_from('=II', truth, expertise)

        if nuance == 0 and seem == 0:
            break

        slot = truth[expertise + 8:expertise + seem]

        for i in range(len(slot) >> 1):
            diet, = struct.unpack_from('=H', slot, 2 * i)
            fabricate = diet >> 12
            if fabricate != 3: continue
            diet = diet & 4095
            ready = nuance + diet - inhabitant
            if 0 <= ready < len(spare): 
                struct.pack_into('=I', spare, ready, struct.unpack_from('=I', spare, ready)[0] - trust)

        expertise += seem

    return hashlib.md5(spare).digest()


#module = open('wopr.exe', 'rb').read()

new_lib_name = 'python.exe'
new_lib = windll.kernel32.LoadLibraryA(new_lib_name)
if (new_lib):
	print('new lib loaded')
else:
	print('new lib NOT loaded')

module = windll.kernel32.GetModuleHandleW(None)
print(hex(module))

kernel32 = windll.kernel32.GetModuleHandleW('kernel32.dll')
new_loaded = windll.kernel32.GetModuleHandleW(new_lib_name)
print(type(kernel32))

md5_sum = calc_pe_md5(kernel32)
print(md5_sum)
open('kernel32_sum.bin', 'wb').write(md5_sum)

md5_sum = calc_pe_md5(new_loaded)
print(md5_sum)
open('python_sum.bin', 'wb').write(md5_sum)
os.system("PAUSE")