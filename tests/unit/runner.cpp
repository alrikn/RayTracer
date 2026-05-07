/*
** This file is the entry point for the unit test binary.
** It defines main() via doctest's macro — exactly ONE translation unit must do this.
** Every other test file just includes helpers.hpp (which includes doctest.h) and
** defines TEST_CASEs; they do NOT define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN.
*/
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
