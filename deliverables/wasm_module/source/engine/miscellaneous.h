/** @file */

#ifndef MISCELLANEOUS_H

#define MISCELLANEOUS_H

/**
 * A falsy pointer value, representing "nothing".
 */
#define NULL ((void *)0)

/**
 * Performs a compile-time assertion.
 * @param description A description of the assertion; only lower case letters,
 *                    underscores and digits permitted.
 * @param condition The condition which must be truthy (non-zero) for
 *                  compilation to succeed.
 */
#define ASSERT(description, condition) typedef int assert_##description[2 * !!(condition)-1]

#endif
