#ifndef PLATFORM_H
#define PLATFORM_H

// ปิด warning บางตัวของ MSVC/MinGW ได้ (ไม่บังคับ)
#if defined(_WIN32) || defined(_WIN64)
  #ifndef _CRT_SECURE_NO_WARNINGS
  #define _CRT_SECURE_NO_WARNINGS
  #endif
  #include <string.h>
  #define STRCASECMP _stricmp
#else
  #include <strings.h>   // สำหรับ strcasecmp บน POSIX
  #define STRCASECMP strcasecmp
#endif

#ifdef _WIN32
  #define CLEAR_SCREEN "cls"
#else
  #define CLEAR_SCREEN "clear"
#endif

#endif /* PLATFORM_H */