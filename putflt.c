

/* Print out floating point numbers demo, file putflt.c */
/* Public Domain, by C.B. Falconer. Attribution appreciated */

#include "putflt.h"

/* ------------ */

/* Returns the count of columns used, or -ve for error
Calling with a dummy dest allows evaluating fields */
int putflt(double val, int sigdigs, putchr dest)
{
int dp;
int ch;
int used;

used = 0;
if (sigdigs <= 0) sigdigs = 1;

if (val < 0) {
dest('-'); used++;
val = - val;
}
/* Normalize */
dp = 0;
if (val != 0.0) {
while (val >= 10.0) {
val = val / 10.0; dp++;
}
while (val < 1.0) {
val = 10.0 * val; dp--;
}
}
/* Handle leading zeroes */
if (dp < 0) {
dest('0'); dest('.'); used += 2;
while (++dp) {
dest('0'); used++;
}
dp--;
}
/* The real work */
do {
ch = val; val = val - ch;
dest(ch + '0'); used++;
val = 10.0 * val;
if (0 == dp--) {
dest('.'); used++;
}
} while (--sigdigs > 0);

/* handle trailing zeroes */
while (dp >= 0) {
dest('0'); used++;
if (0 == dp--) {
dest('.'); used++;
}
}
/* Handle isolated '.' at end */
if (-1 == dp) {
dest('0'); used++;
}
return used; /* no errors handled yet */
} /* putflt */

/* ------------ */

/* do nothing, to allow evaluating field needed */
static int dummy(int ch)
{
return ch;
} /* dummy */

/* ------------ */

/* write the value right justified in field, do not truncate
* Negative value for field left justifies.
* return value signifies field actually used */
int putfltfld(double val, int sigdigs, int field, putchr dest)
{
int needed;

needed = putflt(val, sigdigs, dummy);
if (field > 0)
while (needed < field) {
dest(' '); needed++;
}
putflt(val, sigdigs, dest);
if (field < 0)
while ((needed + field) < 0) {
dest(' '); needed++;
}
return needed;
} /* putfltfld */

/* ------------ */

#include <stdio.h>

#define show(fld) printf(" Field = %d\n", fld)

int main(void)
{
double val;
int i, fld;

val = 10.0 / 7.0; /* A normalized value, 6 dig rep */

fld = putflt(-val / 100, 10, (putchar)); show(fld);
fld = putflt(-val, 10, (putchar)); show(fld);
fld = putflt(-val * 100, 10, (putchar)); show(fld);
fld = putflt(val / 100, 10, (putchar)); show(fld);
fld = putflt(val, 10, (putchar)); show(fld);
for (i = 12; i > 0; i--) {
val = 10 * val;
fld = putflt(val, 10, (putchar)); show(fld);
}
fld = putflt(0.0, 10, (putchar)); show(fld);
fld = putflt(1e-30, 10, (putchar)); show(fld);
fld = putflt(1e30, 10, (putchar)); show(fld);
fld = putflt(0.1e-30, 10, (putchar)); show(fld);
fld = putflt(0.2e-30, 10, (putchar)); show(fld);
fld = putflt(0.3e-30, 10, (putchar)); show(fld);
fld = putflt(0.01e-30, 10, (putchar)); show(fld);
fld = putflt(0.001e-30, 10, (putchar)); show(fld);

val = 30.0 / 13.0; /* Another normalized value */
fld = putfltfld(-val, 10, 20, (putchar)); show(fld);
fld = putfltfld(-val * 100, 10, -20, (putchar)); show(fld);
fld = putfltfld(val / 100, 10, -20, (putchar)); show(fld);
fld = putfltfld(val, 10, 20, (putchar)); show(fld);
fld = putfltfld(val, 25, 35, (putchar)); show(fld);

val = 5419351.0 / 1725033.0; /* Another normalized value */
fld = putfltfld(val, 25, 35, (putchar)); show(fld);

return 0;
} /* main putflt demo */

/* ---------- EOF FILE putflt.c ---------- */

