#ifndef Types_H
#define Types_H

#include <stdbool.h>
#include <stddef.h>

typedef signed char int8;
typedef signed short int int16;
typedef signed long int int32;
typedef signed long long int int64;

typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned long int uint32;
typedef unsigned long long int uint64;

typedef unsigned int uint;
typedef uint8 byte;
typedef wchar_t rune;

typedef float float32;
typedef double float64;

#define nil ((void*)0)

#define PACK_ENUM __attribute__ ((__packed__))
#endif // Types_H