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
