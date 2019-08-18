#ifndef _rx_types_
#define _rx_types_

#include "rx/rx_config.h"
#include "rx/rx_os.h"
#include "rx/rx_libc.h"


#if     defined(__cplusplus)
#define RX_STATIC_ASSERT(expr,message)     static_assert(expr,message)
#else
#if   defined(_MSC_VER)
#define RX_STATIC_ASSERT(expr,message)     typedef char __RX_STATIC_ASSERT[(expr)?1:-1]
#else
#define RX_STATIC_ASSERT(expr,message)     _Static_assert(expr,message)
#endif
#endif




#define RX_ASSERT(expr,msg) assert(expr)


#ifdef __cplugsplus
#define RX_EXTERN    extern  "C"
#else
#define RX_EXTERN    extern
#endif





#if defined(_MSC_VER)
#define RX_CALL __stdcall
#else
#define RX_CALL 
#endif




#ifdef RX_STATIC
#define RX_API(ret,body)        RX_EXTERN   ret     RX_CALL body
#else
#if defined(RX_EXPORTS)
#	if defined(_MSC_VER)
#		define RX_API(ret,body) RX_EXTERN   __declspec(dllexport)   ret     RX_CALL body
#	else
#error  "Unsupported compiler"
#	endif
#else
#	if defined(_MSC_VER)
#		define RX_API(ret,body) RX_EXTERN __declspec(dllimport)     ret     RX_CALL body
#	else
#error  "Unsupported compiler"
#	endif
#endif
#endif




#ifdef _MSC_VER
typedef unsigned __int64    uint64_t;
typedef __int64             int64_t;
#else
typedef unsigned long long  uint64_t;
typedef long long           int64_t;
#endif
typedef unsigned int        uint32_t;
typedef int                 int32_t;
typedef unsigned short      uint16_t;
typedef short               int16_t;
typedef unsigned char       uint8_t;
typedef char                int8_t;
typedef char                char_t;
#ifndef _WCHAR_T_DEFINED
#define _WCHAR_T_DEFINED
typedef unsigned short      wchar_t;
#endif


#ifdef WIN64
typedef    uint64_t         uintptr_t;
#else
typedef    uint32_t         uintptr_t;
#endif


typedef     int32_t         boot_t;
#define     rx_true         (!(0))
#define     rx_false        (0)


RX_STATIC_ASSERT((sizeof(uintptr_t) == sizeof(void*)), "无论CPU、OS、编译器为什么类型，数据类型长度必须保持一致");
RX_STATIC_ASSERT((sizeof(int64_t) == 8), "无论CPU、OS、编译器为什么类型，数据类型长度必须保持一致");
RX_STATIC_ASSERT((sizeof(uint64_t) == 8), "无论CPU、OS、编译器为什么类型，数据类型长度必须保持一致");
RX_STATIC_ASSERT((sizeof(int32_t) == 4), "无论CPU、OS、编译器为什么类型，数据类型长度必须保持一致");
RX_STATIC_ASSERT((sizeof(uint32_t) == 4), "无论CPU、OS、编译器为什么类型，数据类型长度必须保持一致");
RX_STATIC_ASSERT((sizeof(int16_t) == 2), "无论CPU、OS、编译器为什么类型，数据类型长度必须保持一致");
RX_STATIC_ASSERT((sizeof(uint16_t) == 2), "无论CPU、OS、编译器为什么类型，数据类型长度必须保持一致");
RX_STATIC_ASSERT((sizeof(int8_t) == 1), "无论CPU、OS、编译器为什么类型，数据类型长度必须保持一致");
RX_STATIC_ASSERT((sizeof(uint8_t) == 1), "无论CPU、OS、编译器为什么类型，数据类型长度必须保持一致");
RX_STATIC_ASSERT((sizeof(char_t) == 1), "无论CPU、OS、编译器为什么类型，数据类型长度必须保持一致");
RX_STATIC_ASSERT((sizeof(wchar_t) == 2), "无论CPU、OS、编译器为什么类型，数据类型长度必须保持一致");


#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif
#endif


#define RX_OK (0)

//  rx 库的版本，格式为：MNPPPBBB
//  M   主版本号，不兼容变更
//  N   次版本号，不兼容变更
//  PP  补丁版本号，兼容变更
//  BBBB 内部构建版本
#define RX_VERSION (0x01000000)

#endif//_rx_types_

