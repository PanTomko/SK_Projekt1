#include "boolguard.h"

BoolGuard::BoolGuard(bool* wsk)
{
    _wsk = wsk;
    *_wsk = false;
}

BoolGuard::~BoolGuard()
{
    *_wsk = true;
}
