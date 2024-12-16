#ifndef ASPRINTF_H
#define ASPRINTF_H

#if defined(__GNUC__) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE /* needed for (v)asprintf, affects '#include <stdio.h>' */
#endif
#include <stdio.h>  /* needed for vsnprintf    */
#include <stdlib.h> /* needed for malloc, free */
#include <stdarg.h> /* needed for va_*         */

#ifdef _MSC_VER
/*
 * vscprintf:
 * MSVC implements this as _vscprintf, thus we just 'symlink' it here
 */
#define vscprintf _vscprintf

#else
/*
 * vscprintf:
 * GNU-C-compatible compilers do not implement this, thus we implement it here
 */
int vscprintf(const char* format, va_list ap) {
    va_list ap_copy;
    va_copy(ap_copy, ap);
    int retval = vsnprintf(NULL, 0, format, ap_copy);
    va_end(ap_copy);
    return retval;
}
#endif

/*
 * asprintf, vasprintf:
 * MSVC does not implement these, so we implement them here.
 * For GCC/Clang, they are implemented natively, so no further action is needed.
 */
#ifdef _MSC_VER
int vasprintf(char** strp, const char* format, va_list ap) {
    if (!strp || !format) {
        return -1; // Check for null pointers
    }

    // Calculate required buffer size
    int len = vscprintf(format, ap);
    if (len == -1) {
        return -1;
    }

    // Allocate memory for the formatted string
    char* str = (char*)malloc((size_t)len + 1);
    if (!str) {
        return -1;
    }

    // Print the formatted string into the allocated memory
    int retval = vsnprintf(str, len + 1, format, ap);
    if (retval == -1) {
        free(str);
        return -1;
    }

    *strp = str;
    return retval;
}

int asprintf(char** strp, const char* format, ...) {
    if (!strp || !format) {
        return -1; // Check for null pointers
    }

    va_list ap;
    va_start(ap, format);
    int retval = vasprintf(strp, format, ap);
    va_end(ap);
    return retval;
}
#endif

#endif // ASPRINTF_H