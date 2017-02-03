#include "predicate_deduction.h"

#include <cassert>

using namespace std;

void predicate_deduction::add_to_ans_if_it_scheme_of_ax_or_ass(expr_sp ex) {
	if (highlighted_assumption == 0) {
		return;
	}
	auto& res = res_conclusion.proofs;
	
	res.push_back(ex);
	
	expr_sp new_expr1 = make_shared<expr>(highlighted_assumption, CONSEQUENCE, ex);
	expr_sp new_expr2 = make_shared<expr>(ex, CONSEQUENCE, new_expr1);
	
	res.push_back(new_expr2);
	res.push_back(new_expr1);
}

void predicate_deduction::add_to_ans_highlighted_assumption() {
	if (highlighted_assumption == 0) {
		return;
	}
	
	expr_sp A = highlighted_assumption;
	expr_sp B(new expr(A, CONSEQUENCE, A)); // A->A
	
	expr_sp C(new expr(A, CONSEQUENCE, B)); // (A->A->A)
	
	expr_sp C1(new expr(A, CONSEQUENCE,
						make_shared<expr>(B, CONSEQUENCE, A))); // A->(A->A)->A
	expr_sp C2(new expr(C1, CONSEQUENCE, B)); // (A->(A->A)->A) -> (A->A)
	
	expr_sp C3(new expr(C, CONSEQUENCE, C2));
	
	auto& res = res_conclusion.proofs;
	res.push_back(C3);
	res.push_back(C);
	res.push_back(C2);
	res.push_back(C1);
	res.push_back(B);
}

void predicate_deduction::add_to_ans_if_it_MP(int first, int second) {
	if (highlighted_assumption == 0) {
		return;
	}
	
	expr_sp A = highlighted_assumption;
	expr_sp B = proofs[first];
	
	expr_sp BC = proofs[second]; // B->C
	
	expr_sp C = BC->a[1];
	
	expr_sp AC(new expr(A, CONSEQUENCE, C));
	
	expr_sp C1(new expr(make_shared<expr>(A, CONSEQUENCE, BC),
						CONSEQUENCE, AC)); // (A->B->C)->(A->C)
	expr_sp C2(new expr(make_shared<expr>(A, CONSEQUENCE, B)
						, CONSEQUENCE, C1));
	
	auto& res = res_conclusion.proofs;
	res.push_back(C2);
	res.push_back(C1);
	res.push_back(AC);
}

void predicate_deduction::add_to_ans_if_it_2_rule(expr_sp c) {
	auto& res = res_conclusion.proofs;
	
	map<string, expr_sp> disp = {   {"A", highlighted_assumption},
									{"C", c->a[0]},
									{"D", c->a[1]->a[1]}
								};
	conclusion concl = build_concl( {"A->C->D", "A&C"}, "D",
									  { "A->C->D", "A&C",
										"A&C->A", "A&C->C",
										"A", "C", "C->D", "D"}
									);
	remove_ass(concl);
	for (auto w : concl.proofs) {
		res.push_back(substitute(w, disp));
	}
//	concl_1.print();
	
	res.push_back(substitute(concl.need_to_prove->a[1], disp));
	
	disp = { {"A", highlighted_assumption},
			 {"C", c->a[0]},
			 {"D", c->a[1]}
		   };
	res.push_back(substitute(to_expr("A&C->D"), disp));
	
	concl = build_concl( {"A&C->D", "A", "C"}, "D",
						 {"A&C->D", "A", "C",
						  "A->C->A&C", "C->A&C",
						  "A&C", "D"}
						);
	remove_ass(concl);
	for (auto w : concl.proofs) {
		res.push_back(substitute(w, disp));
	}
	
	res.push_back(substitute(concl.need_to_prove->a[1], disp));
}

void predicate_deduction::add_to_ans_if_it_3_rule(expr_sp c) {
	auto& res = res_conclusion.proofs;
	
	map<string, expr_sp> disp = {   {"A", highlighted_assumption},
									{"C", c->a[0]->a[1]},
									{"D", c->a[1]}
								};
	conclusion concl = build_concl( {"A->C->D", "C", "A"}, "D",
									  {"A->C->D", "C", "A",
									   "C->D", "D"}
									);
	remove_ass(concl);
	for (auto w : concl.proofs) {
		res.push_back(substitute(w, disp));
	}
//	concl_1.print();
	
	res.push_back(substitute(concl.need_to_prove->a[1], disp));
	
	disp = { {"A", c->a[0]},
			 {"C", highlighted_assumption},
			 {"D", c->a[1]}
		   };
	res.push_back(substitute(to_expr("A->C->D"), disp));
	
	for (auto w : concl.proofs) {
		res.push_back(substitute(w, disp));
	}
	
	res.push_back(substitute(concl.need_to_prove->a[1], disp));
}


