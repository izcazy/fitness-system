#ifndef PLATFORM_H
#define PLATFORM_H

#if defined(_MSC_VER) || defined(_WIN32)
  #define STRCASECMP _stricmp
#else
  #include <strings.h>
  #define STRCASECMP strcasecmp
#endif

#ifdef _WIN32
  #define CLEAR_SCREEN "cls"
#else
  #define CLEAR_SCREEN "clear"
#endif

#endif /* PLATFORM_H */
