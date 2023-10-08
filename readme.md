# Indifference Engine

## Conventions

American English in `snake_case` is used wherever possible for declaration
names.

Preprocessor macro names are in `UPPER_CASE` while everything else is in
`lower_case`.

### Namespacing

All engine-related declarations are prefixed `ie_{file name}` or
`IE_{file name}` as appropriate to reduce the chance of collisions with
declarations made by the game, compiler or libraries.

Additionally, header files are to contain a header guard named
`IE_{file name}_H`.

For example, a file named `car.h` might resemble the following:

```c
#ifndef IE_CAR_H

#define IE_CAR_H

/**
 * The value of the car, in dollars.
 */
#define IE_CAR_VALUE_DOLLARS 32000

/**
 * The altitude of the car, in meters.
 */
extern float ie_car_altitude_meters;

/**
 * Drives the car.
 */
void ie_car_drive();

#endif
```

### Callbacks

A typedef is created for each callback type.  It is named
`ie_{file name}_{verb}_callback`.  Any parameters using the callback are
named `on_{verb}`.  Verbs are in non-continuous present tense, e.g. `click`
rather than `clicked` or `clicking`.
