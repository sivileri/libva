/*
 * Copyright © Microsoft Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef _VA_WIN32_ADAPTER_H_
#define _VA_WIN32_ADAPTER_H_

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NTSTATUS
typedef LONG NTSTATUS;
#define STATUS_SUCCESS  ((NTSTATUS)0x00000000L)
#define STATUS_BUFFER_TOO_SMALL ((NTSTATUS)0xC0000023)
#define NT_SUCCESS(status) (((NTSTATUS)(status)) >= 0)
#endif

typedef struct LoaderEnumAdapters2 {
    ULONG adapter_count;
    struct {
        UINT handle;
        LUID luid;
        ULONG source_count;
        BOOL present_move_regions_preferred;
    } * adapters;
} LoaderEnumAdapters2;

typedef _Check_return_ NTSTATUS(APIENTRY *PFN_LoaderEnumAdapters2)(const LoaderEnumAdapters2 *);

typedef enum AdapterInfoType {
    LOADER_QUERY_TYPE_REGISTRY = 48,
} AdapterInfoType;

typedef struct LoaderQueryAdapterInfo {
    UINT handle;
    AdapterInfoType type;
    VOID *private_data;
    UINT private_data_size;
} LoaderQueryAdapterInfo;

typedef _Check_return_ NTSTATUS(APIENTRY *PFN_LoaderQueryAdapterInfo)(const LoaderQueryAdapterInfo *);

typedef enum LoaderQueryRegistryType {
    LOADER_QUERY_REGISTRY_ADAPTER_KEY = 1,
} LoaderQueryRegistryType;

typedef enum LoaderQueryRegistryStatus {
    LOADER_QUERY_REGISTRY_STATUS_SUCCESS = 0,
    LOADER_QUERY_REGISTRY_STATUS_BUFFER_OVERFLOW = 1,
} LoaderQueryRegistryStatus;

typedef struct LoaderQueryRegistryFlags {
    union {
        struct {
            UINT translate_path : 1;
            UINT mutable_value : 1;
            UINT reserved : 30;
        };
        UINT value;
    };
} LoaderQueryRegistryFlags;

typedef struct LoaderQueryRegistryInfo {
    LoaderQueryRegistryType query_type;
    LoaderQueryRegistryFlags query_flags;
    WCHAR value_name[MAX_PATH];
    ULONG value_type;
    ULONG physical_adapter_index;
    ULONG output_value_size;
    LoaderQueryRegistryStatus status;
    union {
        DWORD output_dword;
        UINT64 output_qword;
        WCHAR output_string[1];
        BYTE output_binary[1];
    };
} LoaderQueryRegistryInfo;

static const char VAAPI_REG_SUB_KEY[] = "VAAPIDriverName";

#ifndef _WIN64
static const char VAAPI_REG_SUB_KEY_WOW[] = "VAAPIDriverNameWow";
#endif

// Do not free the memory returned by this function.
inline const char* getVAAPIRegKeyName(void)
{
#ifdef _WIN64
    return VAAPI_REG_SUB_KEY;
#else
    // The suffix/substring "WoW" is meaningful only when a 32-bit
    // application is running on a 64-bit Windows OS. A 32-bit application
    // running on a 32-bit OS uses non-WoW names.
    BOOL is_wow64;
    if (IsWow64Process(GetCurrentProcess(), &is_wow64) && is_wow64)
    {
        return VAAPI_REG_SUB_KEY_WOW;
    }

    return VAAPI_REG_SUB_KEY;
#endif
}

#ifdef __cplusplus
}
#endif

#endif /* _VA_WIN32_ADAPTER_H_ */