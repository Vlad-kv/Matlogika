#include "predicate_deduction/predicate_deduction.h"
#include <cassert>

conclusion prove_contraposition(expr_sp c) {
	assert(c->val == CONSEQUENCE);
	
	conclusion concl = build_concl( {"A->B", "!B"}, "!A",
									{"A->B", "!B",
									 "(A->B)->(A->!B)->!A",
									 "(A->!B)->!A",
									 "!B->A->!B", "A->!B",
									 "!A"}
									);
	remove_ass(concl);
	map<string, expr_sp> disp = { {"A", c->a[0]},
								  {"B", c->a[1]} };
	return substitute(concl, disp);
}

conclusion prove_remove_double_not_from_cons(expr_sp c) {
	assert(c->val == CONSEQUENCE);
	assert(c->a[0]->val == NEGATION);
	assert(c->a[0]->a[1]->val == NEGATION);
	
	conclusion concl = build_concl( {"!!A->B", "A"}, "B",
									{"!!A->B", "A", "A->!A->A", "!A->A",
									 "(!A->A)->(!A->A->!A)->(!A->!A)",
									 "(!A->A->!A)->(!A->!A)", "!A->A->!A",
									 "!A->!A", "(!A->A)->(!A->!A)->!!A",
									 "(!A->!A)->!!A", "!!A", "B"}
									);
	remove_ass(concl);
	map<string, expr_sp> disp = { {"A", c->a[0]->a[1]->a[1]},
								  {"B", c->a[1]} };
	return substitute(concl, disp);
}

conclusion prove_for_all_not_is_not_exists(expr_sp c) {
	assert(c->val == FOR_ALL);
	assert(c->a[1]->val == NEGATION);
	
	conclusion res;
	res.need_to_prove = to_expr("@x!A->!?xA");
	
	res.add("@x!A->!A");
	res.add(prove_contraposition(to_expr("@x!A->!A")));
	res.add("!!A->!@x!A");
	
	res.add(prove_remove_double_not_from_cons(to_expr("!!A->!@x!A")));
	res.add("A->!@x!A");
	res.add("?xA->!@x!A");
	
	res.add(prove_contraposition(to_expr("?xA->!@x!A")));
	res.add("!!@x!A->!?xA");
	
	res.add(prove_remove_double_not_from_cons(to_expr("!!@x!A->!?xA")));
	res.add("@x!A->!?xA");
	
	map<string, expr_sp> disp = { {"x", c->a[0]},
								  {"A", c->a[1]->a[1]} };
	return substitute(res, disp);
}

