#ifndef AXIOMS_UTIL_H
#define AXIOMS_UTIL_H

#include "predicate_deduction.h"
#include "parser.h"

struct ax_res {
	int finded_ax;
	expr_sp term, formula;
	string var;
	
	ax_res() 
	: finded_ax(-1) {
	}
	
	ax_res(int finded_ax)
	: finded_ax(finded_ax), term(0),
	  formula(0) {
	}
	
	ax_res(int finded_ax, expr_sp term, expr_sp formula, string var)
	: finded_ax(finded_ax), term(term), 
	  formula(formula), var(var) {
	}
};

bool check_is_it_can_be_substituted(string &var, expr_sp c1, expr_sp c2);
bool check_is_it_free_for_substitution(expr_sp c);
ax_res check_if_it_can_be_turned_out_from_correct_substitution(string &var, expr_sp c1, expr_sp c2);
ax_res check_if_it_11_ax(expr_sp c);
ax_res check_if_it_12_ax(expr_sp c);


bool compare(expr_sp ax, expr_sp c);
ax_res check_if_it_scheme_of_ax(expr_sp c);
ax_res check_if_it_scheme_of_ax(string &s);

#endif // AXIOMS_UTIL_H
