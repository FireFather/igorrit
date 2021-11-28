#ifndef MERSENNE_H
#define MERSENNE_H

/*
 * $Id: mersenne.h,v 1.17 2007/10/26 07:21:06 geoff Exp $
 *
 * Header file for C/C++ use of the Mersenne-Twist pseudo-RNG.  See
 * http://www.math.keio.ac.jp/~matumoto/emt.html for full information.
 *
 * Author of this header file: Geoffrey H. Kuenning, March 18, 2001.
 *
 * IMPORTANT NOTE: this software requires access to a 32-bit type.  Be
 * sure that "mt_u32bit_t" is set to an unsigned 32-bit integer type.
 * The Mersenne Twist algorithms are not guaranteed to produce correct
 * results with a 64-bit type.
 *
 * The executable part of this software is based on LGPL-ed code by
 * Takuji Nishimura.  The header file is therefore also distributed
 * under the LGPL:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.  You should have
 * received a copy of the GNU Library General Public License along
 * with this library; if not, write to the Free Foundation, Inc., 59
 * Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * $Log: mersenne.h,v $
 * Revision 1.17  2007/10/26 07:21:06  geoff
 * Introduce, document, and use the new mt_u32bit_t type so that the code
 * will compile and run on 64-bit platforms (although it does not
 * currently use the 64-bit Mersenne Twist algorithm).
 *
 * Revision 1.16  2005/11/11 08:21:39  geoff
 * If possible, try to infer MT_MACHINE_BITS from limits.h.
 *
 * Revision 1.15  2003/09/11 23:56:20  geoff
 * Allow stdio references in C++ files; it turns out that ANSI has
 * blessed it.  Declare the various functions as external even if they're
 * inlined or being compiled directly (in mersenne.c).  Get rid of a #ifdef
 * that can't ever be true.
 *
 * Revision 1.14  2003/09/11 05:50:53  geoff
 * Don't allow stdio references from C++, since they're not guaranteed to
 * work on all compilers.  Disable inlining using the MT_INLINE keyword
 * rather than #defining inline, since doing the latter can affect other
 * files and functions than our own.
 *
 * Revision 1.13  2003/07/01 23:29:29  geoff
 * Refer to streams from the standard library using the correct namespace.
 *
 * Revision 1.12  2002/10/30 07:39:54  geoff
 * Declare the new seeding functions.
 *
 * Revision 1.11  2001/06/19 00:41:16  geoff
 * For consistency with other C++ types, don't put out a newline after
 * the saved data.
 *
 * Revision 1.10  2001/06/18 10:09:24  geoff
 * Fix some places where I forgot to set one of the result values.  Make
 * the C++ state vector protected so the random-distributions package can
 * pass it to the C functions.
 *
 * Revision 1.9  2001/06/18 05:40:12  geoff
 * Prefix the compile options with MT_.
 *
 * Revision 1.8  2001/06/14 10:26:59  geoff
 * Invert the sense of the #define flags so that the default is the
 * normal case (if gcc is normal!).  Also default MT_MACHINE_BITS to 32.
 *
 * Revision 1.7  2001/06/14 10:10:38  geoff
 * Move the critical-path PRNG code into the header file so that it can
 * be inlined.  Add saving/loading of state.  Add functions to seed based
 * on /dev/random or the time.  Add the function-call operator in the C++
 * code.
 *
 * Revision 1.6  2001/06/11 10:00:04  geoff
 * Add declarations of the refresh and /dev/random seeding functions.
 * Change getstate to return a complete state pointer, since knowing the
 * position in the state vector is critical to restoring the state.
 *
 * Revision 1.5  2001/04/23 08:36:03  geoff
 * Remember to zero the state pointer when constructing, since otherwise
 * proper initialization won't happen.
 *
 * Revision 1.4  2001/04/14 01:33:32  geoff
 * Clarify the license
 *
 * Revision 1.3  2001/04/14 01:04:54  geoff
 * Add a C++ class, mt_prng, that makes usage more convenient for C++
 * programmers.
 *
 * Revision 1.2  2001/04/09 08:45:00  geoff
 * Fix the name in the #ifndef wrapper, and clean up some outdated comments.
 *
 * Revision 1.1  2001/04/07 09:43:41  geoff
 * Initial revision
 *
 */

#include <stdio.h>

#ifndef MT_MACHINE_BITS
#include <limits.h>
#if INT_MAX == 2147483647
#define MT_MACHINE_BITS	32
#else /* INT_MAX */
#define MT_MACHINE_BITS	64
#endif /* INT_MAX */
#endif /* MT_MACHINE_BITS */

/*
 * Define an unsigned type that is guaranteed to be 32 bits wide.
 */
#if MT_MACHINE_BITS == 32
typedef unsigned long	mt_u32bit_t;
#else /* MT_MACHINE_BITS */
typedef unsigned int	mt_u32bit_t;
#endif /* MT_MACHINE_BITS */


/*
 * The following value is a fundamental parameter of the algorithm.
 * It was found experimentally using methods described in Matsumoto
 * and Nishimura's paper.  It is exceedingly magic; don't change it.
 */
#define MT_STATE_SIZE	624		/* Size of the MT state vector */

/*
 * Internal state for an MT RNG.  The user can keep multiple mt_state
 * structures around as a way of generating multiple streams of random
 * numbers.
 *
 * In Matsumoto and Nishimura's original paper, the state vector was
 * processed in a forward direction.  I have reversed the state vector
 * in this implementation.  The reason for the reversal is that it
 * allows the critical path to use a test against zero instead of a
 * test against 624 to detect the need to refresh the state.  on most
 * machines, testing against zero is slightly faster.  It also means
 * that a state that has been set to all zeros will be correctly
 * detected as needing initialization; this means that setting a state
 * vector to zero (either with memset or by statically allocating it)
 * will cause the RNG to operate properly.
 */
typedef struct
    {
    mt_u32bit_t		statevec[MT_STATE_SIZE];
					/* Vector holding current state */
    int			stateptr;	/* Next state entry to be used */
    int			initialized;	/* NZ if state was initialized */
    }
			mt_state;

/*
 * Functions for manipulating any generator (given a state pointer).
 */
extern void		mts_mark_initialized(mt_state* state);
					/* Mark a PRNG state as initialized */
extern void		mts_seed32(mt_state* state, unsigned long seed);
					/* Set random seed for any generator */
extern void		mts_refresh(mt_state* state);
					/* Generate 624 more random values */

/*
 * Functions for manipulating the default generator.
 */
extern void		mt_seed32(unsigned long seed);
					/* Set random seed for default gen. */


/*
 * Functions for generating random numbers.  The actual code of the
 * functions is given in this file so that it can be declared inline.
 * For compilers that don't have the inline feature, mersenne.c will
 * incorporate this file with some clever #defining so that the code
 * actually gets compiled.  In that case, however, "extern"
 * definitions will be needed here, so we give them.
 */

extern unsigned long		mt_lrand(void);
								/* Generate 32-bit random value */
extern unsigned long long	mt_llrand(void);	
								/* Generate 64-bit random value */

/*
 * Tempering parameters.  These are perhaps the most magic of all the magic
 * values in the algorithm.  The values are again experimentally determined.
 * The values generated by the recurrence relation (constants above) are not
 * equidistributed in 623-space.  For some reason, the tempering process
 * produces that effect.  Don't ask me why.  Read the paper if you can
 * understand the math.  Or just trust these magic numbers.
 */
#define MT_TEMPERING_MASK_B 0x9d2c5680
#define MT_TEMPERING_MASK_C 0xefc60000
#define MT_TEMPERING_SHIFT_U(y) \
			(y >> 11)
#define MT_TEMPERING_SHIFT_S(y) \
			(y << 7)
#define MT_TEMPERING_SHIFT_T(y) \
			(y << 15)
#define MT_TEMPERING_SHIFT_L(y) \
			(y >> 18)

/*
 * Macros to do the tempering.  MT_PRE_TEMPER does all but the last step;
 * it's useful for situations where the final step can be incorporated
 * into a return statement.  MT_FINAL_TEMPER does that final step (not as
 * an assignment).  MT_TEMPER does the entire process.  Note that
 * MT_PRE_TEMPER and MT_TEMPER both modify their arguments.
 */
#define MT_PRE_TEMPER(value)						\
    do									\
	{								\
	value ^= MT_TEMPERING_SHIFT_U(value);				\
	value ^= MT_TEMPERING_SHIFT_S(value) & MT_TEMPERING_MASK_B;	\
	value ^= MT_TEMPERING_SHIFT_T(value) & MT_TEMPERING_MASK_C;	\
	}								\
	while (0)
#define MT_FINAL_TEMPER(value) \
			((value) ^ MT_TEMPERING_SHIFT_L(value))
#define MT_TEMPER(value)						\
    do									\
	{								\
	value ^= MT_TEMPERING_SHIFT_U(value);				\
	value ^= MT_TEMPERING_SHIFT_S(value) & MT_TEMPERING_MASK_B;	\
	value ^= MT_TEMPERING_SHIFT_T(value) & MT_TEMPERING_MASK_C;	\
	value ^= MT_TEMPERING_SHIFT_L(value);				\
	}								\
	while (0)

extern mt_state		mt_default_state;
					/* State of the default generator */
extern double		mt_32_to_double;
					/* Multiplier to convert long to dbl */
extern double		mt_64_to_double;
					/* Mult'r to cvt long long to dbl */


/*
 * Generate a random number in the range 0 to 2^32-1, inclusive, working
 * from the default state vector.
 *
 * See mts_lrand for full commentary.
 */
__inline unsigned long mt_lrand()
    {
    register unsigned long
			random_value;	/* Pseudorandom value generated */

    if (mt_default_state.stateptr <= 0)
	mts_refresh(&mt_default_state);

    random_value = mt_default_state.statevec[--mt_default_state.stateptr];
    MT_PRE_TEMPER(random_value);

    return MT_FINAL_TEMPER(random_value);
    }

/*
 * Generate a random number in the range 0 to 2^64-1, inclusive, working
 * from the default state vector.
 *
 * See mts_llrand for full commentary.
 */
__inline unsigned long long mt_llrand()
    {
    register unsigned long
			random_value_1;	/* 1st pseudorandom value generated */
    register unsigned long
			random_value_2;	/* 2nd pseudorandom value generated */

    /*
     * For maximum speed, we'll handle the two overflow cases
     * together.  That will save us one test in the common case, at
     * the expense of an extra one in the overflow case.
     */
    if (--mt_default_state.stateptr <= 0)
	{
	if (mt_default_state.stateptr < 0)
	    {
	    mts_refresh(&mt_default_state);
	    random_value_1 =
	      mt_default_state.statevec[--mt_default_state.stateptr];
	    }
	else
	    {
	    random_value_1 =
	      mt_default_state.statevec[mt_default_state.stateptr];
	    mts_refresh(&mt_default_state);
	    }
	}
    else
	random_value_1 =
	  mt_default_state.statevec[--mt_default_state.stateptr];

    MT_TEMPER(random_value_1);

    random_value_2 = mt_default_state.statevec[--mt_default_state.stateptr];
    MT_PRE_TEMPER(random_value_2);

    return ((unsigned long long) random_value_1 << 32)
      | (unsigned long long) MT_FINAL_TEMPER(random_value_2);
    }

#endif /* MERSENNE_H */
