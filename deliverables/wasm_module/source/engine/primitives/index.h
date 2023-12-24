/** @file */

#ifndef INDEX_H

#define INDEX_H

#include "s32.h"

/**
 * Represents an index into an array.
 */
typedef s32 index;

/**
 * The minimum value of an array index.
 */
#define INDEX_MIN 0

/**
 * The maximum value of an array index.
 */
#define INDEX_MAX S32_MAX

/**
 * A value which can be used in place of an index to represent nothing.
 */
#define INDEX_NONE -1

/**
 * Finds an vacant slot within a field of indices.  Attempts to maintain as
 * small and contiguous a range of used indices as possible.
 * @param vacancies An array which can be used to check for occupancy.
 * @param vacant A constant which indicates a vacancy in the array of potential
 *               vacancies.
 * @param count The number of slots.
 * @param first_occupied The name of the previously defined variable containing
 *                       the index of the first occupied slot.  This must be
 *                       INDEX_NONE if no slots are occupied.
 * @param last_occupied The name of the previously defined variable containing
 *                      the index of the last occupied slot.
 * @param error The error which is raised if there are no vacancies.
 * @param output The name of the variable to declare and populate with the index
 *               of the vacant slot.
 */
#define FIND_EMPTY_INDEX(vacancies, vacant, count, first_occupied, last_occupied, error, output) \
  index output;                                                                                  \
                                                                                                 \
  if (first_occupied == INDEX_NONE)                                                              \
  {                                                                                              \
    output = 0;                                                                                  \
    first_occupied = 0;                                                                          \
    last_occupied = 0;                                                                           \
  }                                                                                              \
  else                                                                                           \
  {                                                                                              \
    for (output = first_occupied; output < last_occupied; output++)                              \
    {                                                                                            \
      if (vacancies[output] == vacant)                                                           \
      {                                                                                          \
        break;                                                                                   \
      }                                                                                          \
    }                                                                                            \
                                                                                                 \
    if (output == last_occupied)                                                                 \
    {                                                                                            \
      output++;                                                                                  \
                                                                                                 \
      if (output == count)                                                                       \
      {                                                                                          \
        if (first_occupied == 0)                                                                 \
        {                                                                                        \
          throw(error);                                                                          \
        }                                                                                        \
        else                                                                                     \
        {                                                                                        \
          output = first_occupied - 1;                                                           \
          first_occupied = output;                                                               \
        }                                                                                        \
      }                                                                                          \
      else                                                                                       \
      {                                                                                          \
        last_occupied = output;                                                                  \
      }                                                                                          \
    }                                                                                            \
  }

/**
 * Vacates a slot then maintains look-up state.
 * @param vacated The index of the newly vacated slot.
 * @param vacancies An array which can be used to check for occupancy.
 * @param vacant A constant which indicates a vacancy in the array of potential
 *               vacancies.
 * @param first_occupied The name of the previously defined variable containing
 *                       the index of the first occupied slot.  This must be
 *                       INDEX_NONE if no slots are occupied.
 * @param last_occupied The name of the previously defined variable containing
 *                      the index of the last occupied slot.
 */
#define INDEX_VACATE(vacated, vacancies, vacant, first_occupied, last_occupied) \
  vacancies[vacated] = vacant;                                                  \
                                                                                 \
  if (vacated == first_occupied)                                                \
  {                                                                             \
    if (vacated == last_occupied)                                               \
    {                                                                           \
      first_occupied = INDEX_NONE;                                              \
    }                                                                           \
    else                                                                        \
      do                                                                        \
      {                                                                         \
        first_occupied++;                                                       \
      } while (vacancies[first_occupied] == vacant);                            \
  }                                                                             \
  else if (vacated == last_occupied)                                            \
  {                                                                             \
    do                                                                          \
    {                                                                           \
      last_occupied--;                                                          \
    } while (vacancies[last_occupied] == vacant);                               \
  }

#endif
