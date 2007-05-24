/* Copyright (C) 1991, 1995, 1996, 1997, 2001, 2004, 2005, 2006
   Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <stdarg.h>
#include <wchar.h>
#include "../libio/libioP.h"


/* Write formatted output to FP from the format string FORMAT.  */
int
__vfwprintf_chk (FILE *fp, int flag, const wchar_t *format, va_list ap)
{
  int done;

  _IO_acquire_lock (fp);
  if (flag > 0)
    fp->_flags2 |= _IO_FLAGS2_FORTIFY;

  done = _IO_vfwprintf (fp, format, ap);

  if (flag > 0)
    fp->_flags2 &= ~_IO_FLAGS2_FORTIFY;
  _IO_release_lock (fp);

  return done;
}
libc_hidden_def (__vfwprintf_chk)
