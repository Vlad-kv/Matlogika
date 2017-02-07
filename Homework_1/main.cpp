#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <fstream>
using namespace std;

#include "predicate_deduction/predicate_deduction.h"
#include "predicate_deduction/predicate_check.h"

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	
	conclusion concl;
	concl.read();
	predicate_check check(concl);
	
	check.global_check();
    return 0;
}
