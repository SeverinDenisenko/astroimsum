/* wcsconfig.h.  Generated from wcsconfig.h.in by configure.  */
/*============================================================================
* wcsconfig.h is generated from wcsconfig.h.in by 'configure'.  It contains
* C preprocessor macro definitions for compiling WCSLIB 8.4
*
* Author: Mark Calabretta, Australia Telescope National Facility, CSIRO.
* http://www.atnf.csiro.au/people/Mark.Calabretta
* $Id: wcsconfig.h.in,v 8.4 2024/10/28 13:56:17 mcalabre Exp $
*===========================================================================*/

// wcslib_version() is available (as of 5.0).
#define HAVE_WCSLIB_VERSION

// WCSLIB library version number.
#define WCSLIB_VERSION 8.4

// Define to 1 if sincos() is available.
/* #undef HAVE_SINCOS */

// 64-bit integer data type.
#define WCSLIB_INT64 long long int
