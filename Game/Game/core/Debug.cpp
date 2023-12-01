#include "Debug.h"

static bool isDebugBoxesShown = false;

bool debug_util::isDebugBoxes()
{
    return isDebugBoxesShown;
}

void debug_util::switchDebugBoxes()
{
    isDebugBoxesShown = !isDebugBoxesShown;
}
