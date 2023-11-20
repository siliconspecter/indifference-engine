#include "../primitives/f32.h"
#include "float.h"

f32 square_root(const f32 radicand)
{
  f32 root;

  __asm__("local.get %1; f32.sqrt; local.set %0"
          : "=r"(root)
          : "r"(radicand));

  return root;
}

f32 floor(const f32 x)
{
  f32 floor;

  __asm__("local.get %1; f32.floor; local.set %0"
          : "=r"(floor)
          : "r"(x));

  return floor;
}
