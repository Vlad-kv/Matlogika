#include <iostream>
#include <cstdlib>
#include <cstdio>
using namespace std;

#include "predicate_deduction.h"

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	
	conclusion m_concl, res_concl;
	m_concl.read();
	
	predicate_deduction deduct(m_concl);
	res_concl = deduct.start_deduction();
	
	res_concl.print();
    return 0;
}
