#include "predicate_check.h"
#include <cassert>
#include <windows.h>
using namespace std;

char bufRus[1001];
char* rus(const char* text) {
   OemToChar(text, bufRus);
   return bufRus;
}
