/* 
 * Copyright (c) 2006 - 2014 QIU ZHONG HUAI <bodhi8@163.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:

 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    "This product includes GHO software, freely available from
 *    <https://github.com/huai2001/tinyGL>".
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _CC_IMAGE_HEADER_H_INCLUDED_
#define _CC_IMAGE_HEADER_H_INCLUDED_

#include <libcc.h>
#ifdef __CC_ANDROID__
#include <android/asset_manager_jni.h>
#elif defined(__CC_WINDOWS__)
#ifdef IMAGE_EXPORTS
#define _CC_IMAGE_DECLSPEC(t) __declspec(dllexport) t _CC_CALL_
#else
#define _CC_IMAGE_DECLSPEC(t) __declspec(dllimport) t _CC_CALL_
#endif

#endif
#pragma pack(1)

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _PCXHeader
{
    byte_t Manufacturer;
    byte_t Version;
    byte_t Encoding;
    byte_t BitsPerPixel;
    int16_t XMin;
    int16_t YMin;
    int16_t XMax;
    int16_t YMax;
    int16_t HorizDPI;
    int16_t VertDPI;
    byte_t Palette[48];
    byte_t Reserved;
    byte_t Planes;
    int16_t BytesPerLine;
    int16_t PaletteType;
    int16_t HScrsize;
    int16_t VScrsize;
    byte_t Filler[54];
} PCXHeader_t;

typedef struct _BMPHeader
{
    uint16_t Id;                    //    BM - Windows 3.1x, 95, NT, 98, 2000, ME, XP
    //    BA - OS/2 Bitmap Array
    //    CI - OS/2 Color Icon
    //    CP - OS/2 Color Pointer
    //    IC - OS/2 Icon
    //    PT - OS/2 Pointer
    uint32_t FileSize;
    uint32_t Reserved;
    uint32_t BitmapDataOffset;
    uint32_t BitmapHeaderSize;        // should be 28h for windows bitmaps or
    // 0Ch for OS/2 1.x or F0h for OS/2 2.x
    uint32_t Width;
    uint32_t Height;
    uint16_t Planes;
    uint16_t BPP;                    // 1: Monochrome bitmap
    // 4: 16 color bitmap
    // 8: 256 color bitmap
    // 16: 16bit (high color) bitmap
    // 24: 24bit (true color) bitmap
    // 32: 32bit (true color) bitmap

    uint32_t Compression;            // 0: none (Also identified by BI_RGB)
    // 1: RLE 8-bit / pixel (Also identified by BI_RLE4)
    // 2: RLE 4-bit / pixel (Also identified by BI_RLE8)
    // 3: Bitfields  (Also identified by BI_BITFIELDS)

    uint32_t BitmapDataSize;        // size of the bitmap data in bytes. This number must be rounded to the next 4 byte boundary.
    uint32_t PixelPerMeterX;
    uint32_t PixelPerMeterY;
    uint32_t Colors;
    uint32_t ImportantColors;
} BMPHeader_t;

typedef struct _TGAHeader
{
    byte_t IdLength;
    byte_t ColorMapType;
    byte_t ImageType;
    byte_t FirstEntryIndex[2];
    int16_t ColorMapLength;
    byte_t ColorMapEntrySize;
    byte_t XOrigin[2];
    byte_t YOrigin[2];
    int16_t Width;
    int16_t Height;
    byte_t PixelDepth;
    byte_t ImageDescriptor;
}TGAHeader_t;

typedef struct _TGAFooter
{
    uint32_t ExtensionOffset;
    uint32_t DeveloperOffset;
    char  Signature[18];
} TGAFooter_t;

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#pragma pack()

#endif /*_TINYGL_IMAGE_HEADER_H_INCLUDED_*/