#ifndef MACROS_H
#define MACROS_H

#define Concat(a, b) a##b
#define OffsetOf(type, member) ((u64) & (((type*)0)->member))
#define ContainerOf(type, member, ptr) ptr - OffsetOf(type, member)

#define Kilobytes(x) (u64) x * 1024
#define Mebabytes(x) (u64) x * 1024 * 1024
#define Gigabytes(x) (u64) x * 1024 * 1024 * 1024

#endif
