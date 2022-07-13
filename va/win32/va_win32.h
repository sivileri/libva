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

#ifndef _VA_WIN32_H_
#define _VA_WIN32_H_

#include <va/va.h>
#include <va/va_backend.h>
#include <windows.h>
#include <dxgiformat.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _VADisplayContextWin32 {
    LUID adapter_luid;
    char default_driver_name[MAX_PATH];
    char registry_driver_name[MAX_PATH];
    uint8_t registry_driver_available_flag;
} VADisplayContextWin32;

/** \brief NTHANDLE memory type
 *
 * Handle is an NTHANDLE. Can be used with D3D12 sharing resource functions such as
 * https://docs.microsoft.com/en-us/windows/win32/api/d3d12/nf-d3d12-id3d12device-opensharedhandle
 * https://docs.microsoft.com/en-us/windows/win32/api/d3d12/nf-d3d12-id3d12device-createsharedhandle
 */
#define VA_SURFACE_ATTRIB_MEM_TYPE_NTHANDLE       0x00010000
/** \brief ID3D12Resource memory type
 * Handle is a pointer to an ID3D12Resource object
 */
#define VA_SURFACE_ATTRIB_MEM_TYPE_D3D12_RESOURCE 0x00020000

typedef struct _VAWin32SurfaceDescriptor {
   /* Surface handle in the memory type specified */
   void* handle;
   /* Surface Width */
   uint32_t width;
   /* Surface Height */
   uint32_t height;
   /* Surface DXGI_FORMAT */
   DXGI_FORMAT format;
} VAWin32SurfaceDescriptor;

/*
 * Returns a suitable VADisplay for VA API
 */
VADisplay vaGetDisplayWin32(
    /* Can be null for adapter autoselection in the VA driver */
    const LUID* adapter_luid
);

VAStatus vaPutSurface(
    VADriverContextP ctx,
    VASurfaceID surface,
    HWND draw,
    short srcx,
    short srcy,
    unsigned short srcw,
    unsigned short srch,
    short destx,
    short desty,
    unsigned short destw,
    unsigned short desth,
    VARectangle *cliprects, /* client supplied clip list */
    unsigned int number_cliprects, /* number of clip rects in the clip list */
    unsigned int flags /* de-interlacing flags */
);

#ifdef __cplusplus
}
#endif

#endif /* _VA_WIN32_H_ */
