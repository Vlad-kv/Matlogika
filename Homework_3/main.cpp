#include <iostream>
#include <cstdlib>
#include <cstdio>
using namespace std;

#include "predicate_deduction/predicate_deduction.h"
#include "predicate_deduction/predicate_check.h"
#include "proofs_util.h"

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	
//	conclusion m_concl, res_concl;
//	m_concl.read();
//	
//	predicate_deduction deduct(m_concl);
//	res_concl = deduct.start_deduction();
//	
//	res_concl.print();
	
	{
		conclusion concl = prove_no_bigger(2, 4);
		predicate_check check(concl);
		check.global_check();
	}
	
	
//	{
//		
//		conclusion m_concl, res_concl;
//		m_concl.read();
//		
//		predicate_check check(m_concl);
//		check.global_check();
//	}
	
    return 0;
}
