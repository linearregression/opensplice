
/** \file os/common/code/os_mutex_attr.c
 *  \brief Common mutual exclusion semaphore attributes
 *
 * Implements os_mutexAttrInit and sets attributes
 * to platform independent values:
 * - scope is OS_SCOPE_SHARED
 */

#include <assert.h>

/** \brief Initialize mutex attribute
 *
 * Set \b mutexAttr->scopeAttr to \b OS_SCOPE_SHARED
 */
os_result
os_mutexAttrInit (
    os_mutexAttr *mutexAttr)
{
    assert (mutexAttr != NULL);
    mutexAttr->scopeAttr = OS_SCOPE_SHARED;
    return os_resultSuccess;
}