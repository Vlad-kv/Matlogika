#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <fstream>
using namespace std;

#include "predicate_deduction/predicate_deduction.h"
#include "predicate_deduction/predicate_check.h"
#include "proofs_util.h"

int main() {
//	setlocale(LC_ALL,"Rus");
	
	
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
		try {
			conclusion concl = prove_main_base();
			predicate_check check(concl);
			check.global_check();
		} catch (string ex) {
			cout << "  " << ex << "  !!!!\n";
		}
	}
	
	string str;
	
	
	
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
