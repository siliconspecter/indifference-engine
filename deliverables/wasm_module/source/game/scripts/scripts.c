#include "../../engine/exports/buffers/error.h"
#include "../../engine/exports/buffers/persist.h"
#include "scripts.h"
#include "../../engine/scripting_api.h"

void script_body(const script script)
{
  switch (script)
  {
  case SCRIPT_START:
    // Add your start-up script here.
    break;

  default:
    throw(ERROR_UNKNOWN_SCRIPT);
  }
}
