#include "../primitives/s32.h"
#include "error.h"
#include "persist.h"

s32 error = ERROR_NONE;

__attribute__((noreturn)) void throw(const s32 _error)
{
  error = _error;
  persist = PERSIST_NOTHING;

  __asm__("unreachable");

  // Note: The compiler is unaware that the inline assembly above halts the
  //       execution of the WASM module, so to satisfy "noreturn", we need to
  //       indeed block the return.
  while (1)
  {
  }
}
