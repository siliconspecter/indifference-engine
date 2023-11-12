#include "../../engine/exports/buffers/error.h"
#include "../../engine/exports/buffers/persist.h"
#include "../../engine/primitives/s32.h"
#include "scripts.h"
#include "../../engine/scripting_api.h"

void script_body(const script script)
{
  switch (script)
  {
  default:
    throw(ERROR_UNKNOWN_SCRIPT);
  }
}