#ifndef __glfixed_h_
#define __glfixed_h_

/*
** Copyright 1991, Silicon Graphics, Inc.
** All Rights Reserved.
**
** This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics, Inc.;
** the contents of this file may not be disclosed to third parties, copied or
** duplicated in any form, in whole or in part, without the prior written
** permission of Silicon Graphics, Inc.
**
** RESTRICTED RIGHTS LEGEND:
** Use, duplication or disclosure by the Government is subject to restrictions
** as set forth in subdivision (c)(1)(ii) of the Rights in Technical Data
** and Computer Software clause at DFARS 252.227-7013, and/or in similar or
** successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished -
** rights reserved under the Copyright Laws of the United States.
*/
#include "types.h"
#include "imports.h"

/* XXX this whole file is pretty bogus */
/* XXX it has the word rex all over it */
/* XXX it is wrong for 1280x1024 screens */

/*
** These constants in this file must be valid for all adapters using
** these macros and code which uses these macros.
*/
#define __GL_MAX_WINDOW_WIDTH	2048
#define __GL_MAX_WINDOW_HEIGHT	2048

/*
** Bias numbers for moving window coordinates into a positive space.
** These values are used during viewport computations.
*/
#define __GL_VERTEX_X_BIAS	2048
#define __GL_VERTEX_Y_BIAS	2048

/* 
** Fixing numbers.  These are used to move the biased window coordinates
** into a range where the number of fraction bits are constant from the
** minimal value in the range to the largest value in the range.
*/
#define __GL_VERTEX_X_FIX	4096
#define __GL_VERTEX_Y_FIX	4096

/************************************************************************/

/*
** Macros for implementing fixed point coordinate arithmetic for the REX.
*/

/*
** For the fixed point arithmetic to be accurate, the following breakdown
** is needed.  The drawing area defined by the GL extends a screens
** amount to the left and right and above and below the normal screen
** area.  This gives an area 9 times the screen area.  To make things
** simpler, the larger of the screen width/height is used.
**
** For REX, the maximum window size is 1024x768.  The largest dimension
** is 1024, requiring a GL rendering range of -1024 to +2047 (inclusive).
** After biasing, the range is +5120 to +8191 (inclusive).  This can
** be represented in 13 bits.  We give ourselves one bit extra for
** comfort, and to deal with a value that is exactly on the furthest
** positive edge of the viewport.  This value, 8192 needs 14 bits to
** represent.  Please note that all of the numbers being used will be
** positive, so the sign bit can be used to hold the extra bit.
** implementation does not use the sign bit.
**
** For iteration to work properly, it needs to handle a maximal change in
** one coordinate with a minimal unit change in the other.  For instance,
** a line drawn from (-1024,0) to (2047,1) should render properly.  For
** this to work right, the change in y across this line must be representable
** in the fixed point number.  This requires a fraciton of 1/(3*1024) thus
** necessitating 12 bits.
**
** So, 15 bits are needed for the integer portion, 12 bits are required
** for the iteration to work, thus consuming 27 bits out of 32 bits in a
** long word.  Thus, 5 bits of subpixel precision remain.
**
** Here is what it looks like in a word:
** 	 +-+-----------+--------+------------+
** 	 +S+ integer   + subpix + fraction   +
** 	 +-+-----------+--------+------------+
*/
#define __GL_COORD_INT_BITS		16
#define __GL_COORD_SUBPIXEL_BITS	3
#define __GL_COORD_FRACTION_BITS	(__GL_COORD_SUBPIXEL_BITS + 13)
#define __GL_COORD_ALMOST_HALF		((1 << (__GL_COORD_FRACTION_BITS-1))-1)
#define __GL_COORD_EXACTLY_HALF		(1 << (__GL_COORD_FRACTION_BITS-1))
#define __GL_COORD_ALMOST_ONE		((1 << __GL_COORD_FRACTION_BITS) - 1)
#define __GL_COORD_EXACTLY_ONE		(1 << __GL_COORD_FRACTION_BITS)
#define __GL_COORD_MAX_VALUE		(2 * __GL_REX_VERTEX_X_FIX - 1)

#define __GL_COORD_SIGNED_FLOAT_TO_FIXED(flt) \
    (__GL_FLOORF((flt) * (1 << (__GL_COORD_FRACTION_BITS)) + __glHalf))

#define __GL_COORD_FLOAT_TO_FIXED(flt) \
    ((__GLfixedCoord) ((flt) * (1 << (__GL_COORD_FRACTION_BITS)) + __glHalf))

#define __GL_COORD_FIXED_TO_FLOAT(fixed) \
    ((__GLfloat) (fixed) * ((__GLfloat)1 / (1 << __GL_COORD_FRACTION_BITS)))

#define __GL_COORD_FIXED_TO_INT(fixed) \
    ((fixed) >> __GL_COORD_FRACTION_BITS)

#define __GL_COORD_INT_TO_FIXED(i) \
    ((i) << __GL_COORD_FRACTION_BITS)

#define __GL_COORD_FRACTION(fixed) \
    ((fixed) & ((1 << __GL_COORD_FRACTION_BITS) - 1))

#define __GL_COORD_ADD(f1,f2,f3) \
    (f1) = (f2) + (f3)

#define __GL_COORD_SUB(f1,f2,f3) \
    (f1) = (f2) - (f3)

#endif /* __glfixed_h_ */
