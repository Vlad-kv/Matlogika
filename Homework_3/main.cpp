#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <fstream>
using namespace std;

#include "predicate_deduction/predicate_deduction.h"
#include "predicate_deduction/predicate_check.h"
#include "proofs_util.h"

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	
	int a, b;
	cin >> a >> b;
	
	conclusion concl = compare(a, b);
	concl.print();
	
//	{
//		try {
//			conclusion concl = compare(a, b);
//			predicate_check check(concl);
//			check.global_check();
//		} catch (string ex) {
//			cout << "  " << ex << "  !!!!\n";
//		}
//	}
    return 0;
}
