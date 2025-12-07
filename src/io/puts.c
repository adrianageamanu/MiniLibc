#include <unistd.h>
#include <string.h>

int puts(const char *str)
{
    if (!str)
        return -1;

    if (write(1, str, strlen(str)) < 0)
        return -1;

    if (write(1, "\n", 1) < 0)
        return -1;

    return 0;
}
