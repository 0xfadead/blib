#ifndef __BLIB_DEFINES_STDATTR_H__
#define __BLIB_DEFINES_STDATTR_H__

#if 0
/* `__attribute__' is compiler specific, so we redefine it,
 * if it's not supported by the compiler.
 * We still use it, because there's no alternative in C99. */
#ifndef __GNUC__
#define __attribute__(x)
#endif

/* `nonnull' is also a compiler-specific attribute, 
 * which we redefine if it's not supported
 */
#ifndef nonnull
#define nonnull
#endif

/* Communicate, that this element can't be NULL */
#define nonnullable __attribute__((nonnull))
/* Communicate, that the element can be NULL */
#define nullable
#endif

#ifndef _Nonnull
#define _Nonnull
#endif

#define nonnullable _Nonnull
#define nullable

#endif // !__BLIB_DEFINES_STDATTR_H__
