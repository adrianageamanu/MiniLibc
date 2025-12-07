#include <time.h>
#include <internal/syscall.h>

int nanosleep(const struct timespec *duration, struct timespec *rem)
{
    return syscall(35, duration, rem);
}
