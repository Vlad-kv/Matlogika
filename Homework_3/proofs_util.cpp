#include "predicate_deduction/predicate_deduction.h"
#include <cassert>

conclusion prove_contraposition(expr_sp c) { // A->B |- !B->!A
	assert(c->val == CONSEQUENCE);
	
	conclusion concl = build_concl( {"A->B", "!B"}, "!A",
									{"A->B", "!B",
									 "(A->B)->(A->!B)->!A",
									 "(A->!B)->!A",
									 "!B->A->!B", "A->!B",
									 "!A"}
									);
	remove_one_ass(concl);
	map<string, expr_sp> disp = { {"A", c->a[0]},
								  {"B", c->a[1]} };
	return substitute(concl, disp);
}

conclusion prove_remove_double_not_from_cons(expr_sp c) { // !!A->B |- A->B
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
	remove_one_ass(concl);
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
	
	res.add(prove_remove_double_not_from_cons(to_expr("!!A->!@x!A")));
	res.add("?xA->!@x!A");
	
	res.add(prove_contraposition(to_expr("?xA->!@x!A")));
	
	res.add(prove_remove_double_not_from_cons(to_expr("!!@x!A->!?xA")));
	
	map<string, expr_sp> disp = { {"x", c->a[0]},
								  {"A", c->a[1]->a[1]} };
	return substitute(res, disp);
}

conclusion prove_a_equ_a(expr_sp c) {
	conclusion res = build_concl( {}, "a=a",
  								  {"a+0=a->a+0=a->a=a",
  								   "a+0=a", "a+0=a->a=a",
  								   "a=a"} );
	map<string, expr_sp> disp = {{"a", c}};
	return substitute(res, disp);
}

conclusion prove_a_equ_b_is_b_equ_a(expr_sp c) {
	assert(c->val == EQUALITY);
	
	conclusion res = build_concl( {"a=b"}, "b=a",
								  {"a=b", "a=b->a=a->b=a",
								   "a=a->b=a"} );
	res.add(prove_a_equ_a(to_therm("a")));
	res.add("b=a");
	map<string, expr_sp> disp = { {"a", c->a[0]},
								  {"b", c->a[1]} };
	return substitute(res, disp);
}

string get_number(int c) {
	string res = "0";
	while (c) {
		res += "'";
		c--;
	}
	return res;
}

conclusion prove_equality(int c1, int c2, int c3) {
	assert(c1 + c2 == c3);
	
	conclusion res;
	res.need_to_prove = to_expr(get_number(c1) + "+" + get_number(c2) + "=" + get_number(c3));
	
	if (c2 == 0) {
		res.add(res.need_to_prove);
		return res;
	}
	
	string v1 = string("(") + get_number(c1) + "+" +
				get_number(c2 - 1) + ")'";
	string v2 = get_number(c1) + "+" + get_number(c2);
	string v3 = get_number(c3);
	
	res.add(to_expr(v1 + "=" + v2 + "->" + v1 + "=" + v3 +
					"->" + v2 + "=" + v3));
	res.add(prove_a_equ_b_is_b_equ_a(to_expr(v2 + "=" + v1)));
	res.add(to_expr(v1 + "=" + v3 + "->" + v2 + "=" + v3));
	res.add(prove_equality(c1, c2 - 1, c3 - 1));
	res.add(to_expr(get_number(c1) + "+" + get_number(c2 - 1) + "=" + get_number(c3 - 1) + "->" +
					v1 + "=" + v3));
	res.add(to_expr(v1 + "=" + v3));
	res.add(res.need_to_prove);
	return res;
}

conclusion prove_no_bigger(int c1, int c2) {
	assert(c1 <= c2);
	
	conclusion res;
	res.need_to_prove = to_expr(string("?p(") + get_number(c1) +
								"+p=" + get_number(c2) + ")");
	conclusion concl = prove_equality(c1, c2 - c1, c2);
	res.add(concl);
	
	res.add(make_shared<expr>(concl.need_to_prove, CONSEQUENCE, res.need_to_prove));
	res.add(res.need_to_prove);
	return res;
}

