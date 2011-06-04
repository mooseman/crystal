

/* ---------- FILE putflt.h ---------- */
/* Print out floating point numbers
Public Domain, by C.B. Falconer.
Attribution appreciated */

#ifndef H_putflt_h
# define H_putflt_h 1
# ifdef __cplusplus
extern "C" {
# endif

/* (putchar) is a suitable value */
typedef int (*putchr)(int ch);

/* ------------ */

/* Returns the count of columns used, or -ve for error
Calling with a dummy dest allows evaluating fields */
int putflt(double val, int sigdigs, putchr dest);

/* ------------ */

/* write the value right justified in field, do not truncate
* Negative value for field left justifies.
* return value signifies field actually used */
int putfltfld(double val, int sigdigs, int field, putchr dest);

/* ------------ */

# ifdef __cplusplus
}
# endif
#endif
/* ---------- EOF FILE putflt.h ---------- */

