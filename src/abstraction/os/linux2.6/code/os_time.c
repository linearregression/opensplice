
/** \file os/linux/code/os_time.c
 *  \brief Linux time management
 *
 * Implements time management for Linux
 * by including the common services and
 * the SVR4 os_timeGet implementation and
 * the POSIX os_nanoSleep implementation
 */

#include <../common/code/os_time.c>
#include <../posix/code/os_time.c>

/** \brief Translate calendar time into readable string representation
 *
 * ctime_r provides a re-entrant translation function.
 * ctime_r function adds '\n' to the string which must be removed.
 */
char *
os_ctime_r (
    os_time *t,
    char *buf)
{
    if (buf) {
        ctime_r((const time_t *)(&t->tv_sec), buf);
    }
    return buf;
}

/** \brief Get high resolution relative time
 *
 */
os_time
os_hrtimeGet (
    void
    )
{
    os_time t;
    struct timespec tv;

    clock_gettime (CLOCK_REALTIME, &tv);
    t.tv_sec = tv.tv_sec;
    t.tv_nsec = tv.tv_nsec;

    return t;
}