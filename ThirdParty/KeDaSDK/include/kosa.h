#ifndef __KOSA_H__
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define __KOSA_H__

/* =============================================================================
 * Defines
 * =============================================================================
 */
#define KOSA_SOK                            0
#define KOSA_SFAIL                          -1
#define MAX_STRING_LENGTH_128               128
#define MAX_STRING_LENGTH_64                64
#define MAX_STRING_LENGTH_32                32
#define MAX_STRING_LENGTH_16                16
#define MAX_STRING_LENGTH_8                 8

/** \brief Floor a integer value. */
#define KOSA_floor(val, align)  (((val) / (align)) * (align))

/** \brief Align a integer value. */
#define KOSA_align(val, align)  KOSA_floor(((val) + (align)-1), (align))

#define uABS(a, b)   (a>b ? (a-b) : (b-a))

#ifndef TRUE
#define TRUE                    1
#endif

#ifndef FALSE
#define FALSE                   0
#endif

#ifdef _LINUX_
#define ERRNO_GET                    errno
#elif _MSC_VER
#define ERRNO_GET                    WSAGetLastError()
#endif
/* =============================================================================
 * Enums
 * =============================================================================
 */


/* =============================================================================
 * Structures
 * =============================================================================
 */
#ifndef Uint64 
typedef unsigned long long      Uint64;
#endif
#ifndef Int64 
typedef long long               Int64;
#endif
#ifndef Uint32
typedef unsigned int            Uint32;
#endif
#ifndef Int32
typedef int                     Int32;
#endif
#ifndef Uint16
typedef unsigned short          Uint16;
#endif
#ifndef Int16
typedef short                   Int16;
#endif
#ifndef Uint8
typedef unsigned char           Uint8;
#endif
#ifndef Int8
typedef signed char              Int8;
#endif
#ifndef Void
typedef void                     Void;
#endif
#ifndef Char
typedef char                     Char;
#endif
#ifndef Bool
typedef unsigned char            Bool;
#endif
#ifndef Float
typedef float		            Float;
#endif
#ifndef KOSA_HANDLE
typedef unsigned long           KOSA_HANDLE;
#endif
/*点*/
typedef struct{
    Int32 X;
    Int32 Y;
}POS_S;

/*线*/
typedef struct{
    Int32 X1;
    Int32 Y1;
    Int32 X2;
    Int32 Y2;
}LINE_S;

/*矩形*/
typedef struct{
    Int32 X;
    Int32 Y;
    Int32 W;
    Int32 H;
}RECTANGLE_S;

/*梯形*/
typedef struct{
    Int32 X1;
    Int32 Y1;
    Uint32 W1;
    Int32 X2;
    Int32 Y2;
    Uint32 W2;
}ECHELON_S;

/* =============================================================================
 * APIs
 * =============================================================================
 */

/*不受系统时间的修改影响*/
Uint32 KOSA_getCurTimeInMsec();

/*系统时间*/
Int64 KOSA_getTimeStampInMsec();

Void KOSA_waitMsecs(Uint32 msecs);

Void KOSA_sysReboot();

Uint32 KOSA_ClkRateGet();

#define KOSA_assert(x) \
        { \
            if((x) == 0) \
            { \
                do \
                { \
                    KOSA_log("ASSERT (%s|%s|%d)\r\n", __FILE__, __FUNCTION__, __LINE__); \
                    KOSA_waitMsecs(20*1000); \
                    KOSA_sysReboot(); \
                }while (1); \
            } \
        } \

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __KOSA_H__ */

