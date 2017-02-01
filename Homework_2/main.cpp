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
	
//	map<string, expr_sp> m_map = {
//		{string("A"), to_expr("E->V|C")},
//		{string("B"), to_expr("R->V|C")},
//		{string("C"), to_expr("E->V|C&C&V&B")}
//		};
//	expr_sp ex = substitute(predicate_deduction::expr_axioms[7], m_map);
//	cout << to_string(ex) << "\n";
    return 0;
}
