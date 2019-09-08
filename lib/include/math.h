#ifndef MATH_H
#define MATH_H

#include <composite.h>

#define MATH_FILE "/system/math"

#define MATH_FIND_EXP(sum) call_other(MATH_FILE,"find_exp",sum)

#define MATH_INCHES_TO_STRING(i) \
COMPOSITE_WORDS(call_other(MATH_FILE,"inches_to_strings",i))

#define MATH_STRINGS_TO_INCHES(s) \
call_other(MATH_FILE,"strings_to_inches",s)

#define SQRT(x) call_other(MATH_FILE,"sqrt",x)

#define LINE(x, p0x, p0y, p1x, p1y) (((x-p0x) * (p1y-p0y)) / (p1x-p0x) + p0y)

#endif
