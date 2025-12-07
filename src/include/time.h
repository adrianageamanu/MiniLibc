#ifndef __TIME_H__
#define __TIME_H__

#ifdef __cplusplus
extern "C"
{
#endif

    struct timespec
    {
        long sec;
        long nsec;
    };

    unsigned sleep(unsigned int seconds);
    int nanosleep(const struct timespec *duration, struct timespec *rem);

#ifdef __cplusplus
}
#endif
#endif
