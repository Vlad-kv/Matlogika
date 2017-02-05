#include "predicate_deduction.h"
#include <cassert>
using namespace std;

void predicate_deduction::add_to_ans_if_it_scheme_of_ax_or_ass(expr_sp ex) {
	if (highlighted_assumption == 0) {
		return;
	}
	conclusion concl = build_concl( {}, "A->B",
									{"B", "B->A->B", "A->B"});
	map<string, expr_sp> disp = { {"A", highlighted_assumption},
								  {"B", ex} };
	res_conclusion.add(substitute(concl, disp));
}

void predicate_deduction::add_to_ans_highlighted_assumption() {
	if (highlighted_assumption == 0) {
		return;
	}
	conclusion concl = build_concl( {}, "A->A",
									{"(A->(A->A))->(A->(A->A)->A)->A->A",
									 "A->A->A", "(A->(A->A)->A)->A->A",
									 "A->(A->A)->A", "A->A"} );
	map<string, expr_sp> disp = { {"A", highlighted_assumption} };
	res_conclusion.add(substitute(concl, disp));
}

void predicate_deduction::add_to_ans_if_it_MP(int first, int second) {
	if (highlighted_assumption == 0) {
		return;
	}
	conclusion concl = build_concl( {"A->B", "A->B->C"}, "A->C",
									{"(A->B)->(A->B->C)->(A->C)",
									 "(A->B->C)->(A->C)", "A->C"});
	map<string, expr_sp> disp = { {"A", highlighted_assumption},
								  {"B", proofs[first]},
								  {"C", proofs[second]->a[1]}};
	res_conclusion.add(substitute(concl, disp));
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
	remove_one_ass(concl);
	res_conclusion.add(substitute(concl, disp));
	
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
	remove_one_ass(concl);
	remove_one_ass(concl);
	
	res_conclusion.add(substitute(concl, disp));
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
	remove_one_ass(concl);
	remove_one_ass(concl);
	
	res_conclusion.add(substitute(concl, disp));
	
	disp = { {"A", c->a[0]},
			 {"C", highlighted_assumption},
			 {"D", c->a[1]}
		   };
	res.push_back(substitute(to_expr("A->C->D"), disp));
	
	res_conclusion.add(substitute(concl, disp));
}


