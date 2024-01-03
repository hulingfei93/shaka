#ifndef _CORE_H_INCLUDED_
#define _CORE_H_INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

#define false	0
#define true	1

typedef unsigned char bool;

#define algo_abs(value)				(((value) >= 0) ? (value) : -(value))	
#define algo_max(val1, val2)		(((val1) < (val2)) ? (val2) : (val1))	
#define algo_min(val1, val2)		(((val1) > (val2)) ? (val2) : (val1))	

#endif /* _CORE_H_INCLUDED_ */
