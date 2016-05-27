#ifndef MEMORY_LEAK_DETECTION_H_INCLUDE
#define MEMORY_LEAK_DETECTION_H_INCLUDE

#include <cstddef>

void * operator new(std::size_t sz, const char *file, long line);
void * operator new[](std::size_t sz, const char *file, long line);
void operator delete(void * ptr) noexcept;
void operator delete[](void * ptr) noexcept;

void memory_leak_detect_clear();
void memory_leak_detect_report();
void memory_leak_detect_on();
void memory_leak_detect_off();

#define MEMORY_LEAK_DETECT_ON()     (::memory_leak_detect_on())
#define MEMORY_LEAK_DETECT_OFF()    (::memory_leak_detect_off())
#define MEMORY_LEAK_DETECT_CLEAR()     (::memory_leak_detect_clear())
#define MEMORY_LEAK_DETECT_REPORT()    (::memory_leak_detect_report())
#define MEMORY_LEAK_DETECT_TEST(expr)       MEMORY_LEAK_DETECT_ON(); expr; MEMORY_LEAK_DETECT_OFF(); MEMORY_LEAK_DETECT_REPORT(); MEMORY_LEAK_DETECT_CLEAR();

#define new new (__FILE__, __LINE__)

#endif
