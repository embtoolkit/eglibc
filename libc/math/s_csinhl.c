/* Complex sine hyperbole function for long double.
   Copyright (C) 1997, 2011 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1997.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <complex.h>
#include <fenv.h>
#include <math.h>

#include <math_private.h>


__complex__ long double
__csinhl (__complex__ long double x)
{
  __complex__ long double retval;
  int negate = signbit (__real__ x);
  int rcls = fpclassify (__real__ x);
  int icls = fpclassify (__imag__ x);

  __real__ x = fabsl (__real__ x);

  if (__builtin_expect (rcls >= FP_ZERO, 1))
    {
      /* Real part is finite.  */
      if (__builtin_expect (icls >= FP_ZERO, 1))
	{
	  /* Imaginary part is finite.  */
	  long double sinh_val = __ieee754_sinhl (__real__ x);
	  long double cosh_val = __ieee754_coshl (__real__ x);
	  long double sinix, cosix;

	  __sincosl (__imag__ x, &sinix, &cosix);

	  __real__ retval = sinh_val * cosix;
	  __imag__ retval = cosh_val * sinix;

	  if (negate)
	    __real__ retval = -__real__ retval;
	}
      else
	{
	  if (rcls == FP_ZERO)
	    {
	      /* Real part is 0.0.  */
	      __real__ retval = __copysignl (0.0, negate ? -1.0 : 1.0);
	      __imag__ retval = __nanl ("") + __nanl ("");

	      if (icls == FP_INFINITE)
		feraiseexcept (FE_INVALID);
	    }
	  else
	    {
	      __real__ retval = __nanl ("");
	      __imag__ retval = __nanl ("");

	      feraiseexcept (FE_INVALID);
	    }
	}
    }
  else if (__builtin_expect (rcls == FP_INFINITE, 1))
    {
      /* Real part is infinite.  */
      if (__builtin_expect (icls > FP_ZERO, 1))
	{
	  /* Imaginary part is finite.  */
	  long double sinix, cosix;

	  __sincosl (__imag__ x, &sinix, &cosix);

	  __real__ retval = __copysignl (HUGE_VALL, cosix);
	  __imag__ retval = __copysignl (HUGE_VALL, sinix);

	  if (negate)
	    __real__ retval = -__real__ retval;
	}
      else if (icls == FP_ZERO)
	{
	  /* Imaginary part is 0.0.  */
	  __real__ retval = negate ? -HUGE_VALL : HUGE_VALL;
	  __imag__ retval = __imag__ x;
	}
      else
	{
	  /* The addition raises the invalid exception.  */
	  __real__ retval = HUGE_VALL;
	  __imag__ retval = __nanl ("") + __nanl ("");

#ifdef FE_INVALID
	  if (icls == FP_INFINITE)
	    feraiseexcept (FE_INVALID);
#endif
	}
    }
  else
    {
      __real__ retval = __nanl ("");
      __imag__ retval = __imag__ x == 0.0 ? __imag__ x : __nanl ("");
    }

  return retval;
}
weak_alias (__csinhl, csinhl)
