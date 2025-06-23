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

/* 字符串长度 */
size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

/* 字符串比较 */
int strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

/* 字符串比较（最多n个字符） */
int strncmp(const char *s1, const char *s2, size_t n)
{
    if (n == 0) return 0;
    
    while (--n && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

/* 字符串复制 */
char *strcpy(char *dest, const char *src)
{
    size_t len = strlen(src) + 1;  /* 包括'\0' */
    return (char*)memcpy(dest, src, len);
}

/* 字符串复制（最多n个字符） */
char *strncpy(char *dest, const char *src, size_t n)
{
    size_t len = strlen(src);
    if (len < n) {
        /* 源字符串较短，复制全部并填充0 */
        memcpy(dest, src, len + 1);
        memset(dest + len + 1, 0, n - len - 1);
    } else {
        /* 源字符串较长，只复制n个字符 */
        memcpy(dest, src, n);
    }
    return dest;
}

/* 字符串连接 */
char *strcat(char *dest, const char *src)
{
    size_t dest_len = strlen(dest);
    size_t src_len = strlen(src);
    memcpy(dest + dest_len, src, src_len + 1);
    return dest;
}

/* 在字符串中查找字符 */
char *strchr(const char *str, int c)
{
    size_t len = strlen(str) + 1;  /* 包括'\0'以支持查找'\0' */
    return (char*)memchr(str, c, len);
}

/* 在字符串中查找子字符串 */
char *strstr(const char *haystack, const char *needle)
{
    size_t needle_len;
    
    if (!*needle) return (char*)haystack;
    
    needle_len = strlen(needle);
    
    for (; *haystack; haystack++) {
        if (memcmp(haystack, needle, needle_len) == 0) {
            return (char*)haystack;
        }
    }
    
    return NULL;
}

/* 字符串连接（最多n个字符） */
char *strncat(char *dest, const char *src, size_t n)
{
    size_t dest_len = strlen(dest);
    size_t src_len = strlen(src);
    size_t copy_len = (src_len < n) ? src_len : n;
    
    memcpy(dest + dest_len, src, copy_len);
    dest[dest_len + copy_len] = '\0';
    
    return dest;
}

/* 在字符串中查找字符（从右向左） */
char *strrchr(const char *str, int c)
{
    const char *last = NULL;
    
    do {
        if (*str == c) {
            last = str;
        }
    } while (*str++);
    
    return (char*)last;
}
