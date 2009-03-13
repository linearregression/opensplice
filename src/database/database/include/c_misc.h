#ifndef C_MISC_H
#define C_MISC_H

#include "c_typebase.h"
#include "c_metabase.h"

#if defined (__cplusplus)
extern "C" {
#endif
#include "os_if.h"

#ifdef OSPL_BUILD_DB
#define OS_API OS_API_EXPORT
#else
#define OS_API OS_API_IMPORT
#endif
/* !!!!!!!!NOTE From here no more includes are allowed!!!!!!! */

#ifdef _TYPECHECK_
#define C_CAST(o,t) ((t)c_checkType(o,#t))
#else
#define C_CAST(o,t) ((t)(o))
#endif

#define C_TYPECHECK(o,t) (o == c_checkType(o,#t))

/**
 This function verifies the object type.
 The type of the object must be the same as or be derived from the specified type.
 If the type is correct the function will return the given object otherwise it will
 return NULL, so it can simply be added into existing code.
 If a type mismatch occures an error report is generated.
 If the standard assert functionality is enabled a type mismatch will result in
 an abort signal.
**/
OS_API c_object c_checkType (c_object o, const c_char *typeName);
OS_API void     c_copyIn    (c_type type, c_voidp data, c_object *o);
OS_API void     c_copyOut   (c_type type, c_object object, c_voidp *data);

#undef OS_API

#if defined (__cplusplus)
}
#endif

#endif