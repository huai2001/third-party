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
#ifndef _CC_IMAGE_H_INCLUDED_
#define _CC_IMAGE_H_INCLUDED_

#include "color.h"

#define _CC_3RD_LIBPNG_
#define _CC_3RD_LIBJPG_

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __BIG_ENDIAN__
#define __BYTE_SWAP_16(a,b) do {\
    a = _cc_swap16(*(uint16_t*)b);\
    b += sizeof(uint16_t);\
} while (0)

#define __BYTE_SWAP_32(a,b) do {\
    a = _cc_swap32(*(uint32_t*)b);\
    b += sizeof(uint32_t);\
} while (0)
#else
#define __BYTE_SWAP_16(a,b) do {\
    a = (*(uint16_t*)b);\
    b += sizeof(uint16_t);\
} while (0)

#define __BYTE_SWAP_32(a,b) do {\
    a = (*(uint32_t*)b);\
    b += sizeof(uint32_t);\
} while (0)
#endif

typedef struct __cc_image {
    byte_t *data;
    byte_t format;
    uint32_t width;
    uint32_t height;    
    uint32_t pitch;
    uint32_t channel;
    uint32_t size;
    
    struct {
        uint32_t *data;
        uint32_t size;
    } palette;
    
}_cc_image_t;


_CC_IMAGE_DECLSPEC(uint16_t) _cc_pixel_blend16 ( const uint16_t c2, const uint16_t c1);
_CC_IMAGE_DECLSPEC(uint32_t) _cc_pixel_blend16_simd ( const uint32_t c2, const uint32_t c1 );
_CC_IMAGE_DECLSPEC(uint32_t) _cc_pixel_blend32( const uint32_t c2, const uint32_t c1);

_CC_IMAGE_DECLSPEC(uint16_t) _cc_pixel_blend16A ( const uint16_t c2, const uint32_t c1, const uint16_t alpha );
_CC_IMAGE_DECLSPEC(uint32_t) _cc_pixel_blend32A( const uint32_t c2, const uint32_t c1, uint32_t alpha );

/* load a BMP image */
_CC_IMAGE_DECLSPEC(_cc_image_t*) _cc_load_BMP(const byte_t *data, uint32_t size);
/* write a BMP image */
_CC_IMAGE_DECLSPEC(bool_t) _cc_write_BMP(const tchar_t *file_name, _cc_image_t *image);
/* load a TGA image */
_CC_IMAGE_DECLSPEC(_cc_image_t*) _cc_load_TGA(const byte_t *data, uint32_t size);
/* write a BMP image */
_CC_IMAGE_DECLSPEC(bool_t) _cc_write_TGA(const tchar_t *file_name, _cc_image_t *image);
/* load a PCX image */
_CC_IMAGE_DECLSPEC(_cc_image_t*) _cc_load_PCX(const byte_t *data, uint32_t size);
/* write a BMP image */
_CC_IMAGE_DECLSPEC(bool_t) _cc_write_PCX(const tchar_t *file_name, _cc_image_t *image);
#ifdef _CC_3RD_LIBPNG_
/* load a PNG image */
_CC_IMAGE_DECLSPEC(_cc_image_t*) _cc_load_PNG(const byte_t *data, uint32_t size);
/* write a PNG image */
_CC_IMAGE_DECLSPEC(bool_t) _cc_write_PNG(const tchar_t *file_name, _cc_image_t *image);
#endif
    
#ifdef _CC_3RD_LIBJPG_
/* load a JPG image */
_CC_IMAGE_DECLSPEC(_cc_image_t*) _cc_load_JPG(const byte_t *data, uint32_t size);
/* write a PNG image */
_CC_IMAGE_DECLSPEC(bool_t) _cc_write_JPG(const tchar_t *file_name, _cc_image_t *image, uint32_t quality);
#endif


/**/
_CC_IMAGE_DECLSPEC(_cc_image_t*) _cc_load_image(const tchar_t *file_name);

/**/
_CC_IMAGE_DECLSPEC(_cc_image_t*) _cc_init_image(uint32_t format, uint32_t width, uint32_t height);

/**/
_CC_IMAGE_DECLSPEC(_cc_image_t*) _cc_init_image_data(uint32_t format, uint32_t width, uint32_t height, byte_t *data);

/**/
_CC_IMAGE_DECLSPEC(bool_t) _cc_destroy_image(_cc_image_t** image);

/**/
_CC_IMAGE_DECLSPEC(int32_t) _cc_get_bits_per_pixel_from_format(const byte_t format);

//! returns a pixel
_CC_IMAGE_DECLSPEC(uint32_t) _cc_get_pixel(_cc_image_t *image, uint32_t x, uint32_t y);
//! sets a pixel
_CC_IMAGE_DECLSPEC(void) _cc_set_pixel(_cc_image_t *image, uint32_t x, uint32_t y, const _cc_color_t clr, bool_t blend);

/**/
_CC_IMAGE_DECLSPEC(void) _cc_image_scaling(_cc_image_t *dst, _cc_image_t *src);
/**/
_CC_IMAGE_DECLSPEC(void) _cc_image_resampled (_cc_image_t* dst, _cc_image_t* src,
                                      int32_t dstX, int32_t dstY, int32_t srcX, int32_t srcY,
                                      int32_t dstW, int32_t dstH, int32_t srcW, int32_t srcH);
/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif    
#endif /*_CC_IMAGE_H_INCLUDED_*/
