/*
 * Copyright (c) 2025 ZhaoCake
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <os/types.h>
#include <os/string.h>

/* 内存设置 */
void *memset(void *ptr, int value, size_t num)
{
    unsigned char *p = (unsigned char*)ptr;
    unsigned char val = (unsigned char)value;
    
    while (num--) {
        *p++ = val;
    }
    
    return ptr;
}

/* 内存复制 */
void *memcpy(void *dest, const void *src, size_t num)
{
    unsigned char *d = (unsigned char*)dest;
    const unsigned char *s = (const unsigned char*)src;
    
    while (num--) {
        *d++ = *s++;
    }
    
    return dest;
}

/* 内存移动（处理重叠区域） */
void *memmove(void *dest, const void *src, size_t num)
{
    unsigned char *d = (unsigned char*)dest;
    const unsigned char *s = (const unsigned char*)src;
    
    if (d < s) {
        /* 正向复制 */
        while (num--) {
            *d++ = *s++;
        }
    } else if (d > s) {
        /* 反向复制 */
        d += num;
        s += num;
        while (num--) {
            *--d = *--s;
        }
    }
    
    return dest;
}

/* 内存比较 */
int memcmp(const void *ptr1, const void *ptr2, size_t num)
{
    const unsigned char *p1 = (const unsigned char*)ptr1;
    const unsigned char *p2 = (const unsigned char*)ptr2;
    
    while (num--) {
        if (*p1 != *p2) {
            return *p1 - *p2;
        }
        p1++;
        p2++;
    }
    
    return 0;
}

/* 在内存中查找字节 */
void *memchr(const void *ptr, int value, size_t num)
{
    const unsigned char *p = (const unsigned char*)ptr;
    unsigned char val = (unsigned char)value;
    
    while (num--) {
        if (*p == val) {
            return (void*)p;
        }
        p++;
    }
    
    return NULL;
}
