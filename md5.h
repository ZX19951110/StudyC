#ifndef MD5_H
#define MD5_H

#include <stdint.h>
#include <stddef.h>

typedef struct md5_t
{
	uint8_t n[16];
}md5_t;

md5_t md5(const void* data, size_t len);

#endif