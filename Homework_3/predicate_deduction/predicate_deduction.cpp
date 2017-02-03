#include "predicate_deduction.h"

#include <cassert>

using namespace std;
vector<expr_sp> predicate_deduction::expr_axioms;

conclusion predicate_deduction::start_deduction() {
	if (m_conclusion.assumptions.size() == 0) {
		return m_conclusion;
	}
	
	assumptions = m_conclusion.assumptions;
	need_to_prove = m_conclusion.need_to_prove;
	proofs = m_conclusion.proofs;
	
	res_conclusion.clear();
	for (int w = 0; w < assumptions.size() - 1; w++) {
		res_conclusion.assumptions.push_back(assumptions[w]);
	}
	
	highlighted_assumption = assumptions[assumptions.size() - 1];
	free_vars_in_h_a = get_set_of_free_vars_in_expr(highlighted_assumption);
	
	res_conclusion.need_to_prove = make_shared<expr>(
													highlighted_assumption,
													CONSEQUENCE,
													need_to_prove);
	int c = 0;
	for (auto w : assumptions) {
		c++;
		map_of_assumptions.insert({to_string(w), c});
	}
	
	int no = 1;
	
	for (size_t w = 0; w < proofs.size(); w++) {
		no++;
		
		string str = to_string(proofs[w]);
		
		map<string, int>::iterator it;
		multimap<string, pair<string, int> >::iterator it2;
		
		{
			string poss_error;
			
			ax_res c = check_if_it_scheme_of_ax(proofs[w]);
			if (c.finded_ax >= 0) {
				add_to_ans_if_it_scheme_of_ax_or_ass(proofs[w]);
				goto cont;
			}
			if (c.finded_ax < -1) {
				poss_error = string("терм ") + to_string(c.term) + " не свободен дл€ подстановки в формулу ";
				poss_error += to_string(c.formula) + " вместо переменной ";
				poss_error += c.var + ".";
			}
			
			///---------------------------------------------
			it = map_of_assumptions.find(str);
			if (it != map_of_assumptions.end()) {
				if ((*it).second == assumptions.size()) {
					add_to_ans_highlighted_assumption();
				} else {
					add_to_ans_if_it_scheme_of_ax_or_ass(proofs[w]);
				}
				goto cont;
			}
			///---------------------------------------------
			map<string, pair<int, int> >::iterator it3;
			
			it3 = poss_m_p.find(str);
			if (it3 != poss_m_p.end()) {
				add_to_ans_if_it_MP((*it3).second.first, (*it3).second.second);
				goto cont;
			}
			///---------------------------------------------
			
			pred_rules_res res = check_if_it_new_pred_rule(proofs[w]);
			if (res.res > 0) {
				if (free_vars_in_h_a.find(res.var) == free_vars_in_h_a.end()) {
					
					if (res.res == 1) {
						add_to_ans_if_it_2_rule(proofs[w]);
					} else {
						add_to_ans_if_it_3_rule(proofs[w]);
					}
					
//					assert(0);/// ƒоделать.
					goto cont;
				} else {
					if (poss_error.empty()) {
						poss_error = "используетс€ правило с квантором по переменной ";
						poss_error += res.var + ", вход€щей свободно в допущение ";
						poss_error += to_string(highlighted_assumption) + ".";
					}
				}
			} else {
				if ((res.res < 0) && (poss_error.empty())) {
					poss_error = string("переменна€ ") + res.var;
					poss_error += string(" входит свободно в формулу ");
					poss_error += to_string(res.formula) + ".";
				}
			}
			///---------------------------------------------
			
			if (first_error_message.empty()) {
				first_error_message = poss_error;
			}
			
			res_conclusion.clear();
			res_conclusion.err_mess = string("¬ывод некорректен начина€ с формулы номер ");
			res_conclusion.err_mess += to_string(w + 1);
			
			if (!first_error_message.empty()) {
				res_conclusion.err_mess += string(": ") + first_error_message;
			}
			return res_conclusion;
		}
		cont:
		
		if (proofs[w]->val == CONSEQUENCE) {
			all_consequences[to_string(proofs[w])] = w;
			
			string left_child = to_string(proofs[w]->a[0]);
			
			it = existing_proofs.find(left_child);
			if (it != existing_proofs.end()) {
				poss_m_p[ to_string(proofs[w]->a[1]) ] = {(*it).second, w};
			} else {
				poss_poss_m_p.insert({left_child, 
										{to_string(proofs[w]->a[1]), w}
									  } );
			}
		}
		
		it2 = poss_poss_m_p.find(str);
		while (it2 != poss_poss_m_p.end()) {
			poss_m_p.insert({(*it2).second.first, {w, (*it2).second.second} });
			
			poss_poss_m_p.erase(it2);
			it2 = poss_poss_m_p.find(str);
		}
		existing_proofs.insert({str, w});
	}
	return res_conclusion;
}

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

