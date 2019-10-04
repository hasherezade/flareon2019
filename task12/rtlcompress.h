#pragma once

#include <Windows.h>

BOOL compress_buffer(const char *buffer, const ULONG bufferLen, UCHAR *compBuffer, ULONG compBufferLen, ULONG *compBufferSize);

BOOL decompress_buffer(const char *buffer, const int bufferLen, UCHAR *uncompBuffer, const int uncompBufferLen, ULONG *uncompBufferSize);
