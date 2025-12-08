#ifndef VALT_VALT_H
#define VALT_VALT_H

// Boolean types
#undef bool
#define bool _Bool
#undef true
#define true 1
#undef false
#define false 0

// Int types
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long long int64;

// Terminal formating
#define STYLE_BOLD "\x1b[1m"
#define STYLE_DIM "\x1b[2m"
#define STYLE_ITALIC "\x1b[3m"
#define STYLE_UNDERLINE "\x1b[4m"
#define STYLE_BLINK "\x1b[5m"
#define STYLE_REVERSE "\x1b[7m"
#define STYLE_HIDDEN "\x1b[8m"
#define STYLE_STRIKE "\x1b[9m"
// Terminal colors
#define COLOR_RED "\x1b[31m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_BLUE "\x1b[34m"
// Reset Style
#define STYLE_RESET "\x1b[0m"
#endif // VALT_VALT_H
