#ifndef LIMITS_H
#define LIMITS_H

#define CHAR_MIN	(-128)
#define CHAR_MAX	127
#define UCHAR_MAX	255

#define SHRT_MAX	((short)(~0U>>1))
#define SHRT_MIN	(-SHRT_MAX - 1)
#define USHRT_MAX	((short)(~0U>>1))

#define INT_MAX         ((int)(~0U>>1))
#define INT_MIN         (-INT_MAX - 1)
#define UINT_MAX        (~0U)

#define LONG_MAX        ((long)(~0UL>>1))
#define LONG_MIN        (-LONG_MAX - 1)
#define ULONG_MAX       (~0UL)

#endif
