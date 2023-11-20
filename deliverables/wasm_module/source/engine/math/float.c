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
