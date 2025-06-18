#ifndef TESTS_H
#define TESTS_H

#include <check.h>
#include <stdlib.h>

void runTests();
void runTestcase(Suite *testcase);
Suite *suite_tetris_base();

#endif