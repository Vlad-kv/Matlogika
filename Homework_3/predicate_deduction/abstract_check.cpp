#include "abstract_check.h"

vector<expr_sp> abstract_check::expr_axioms;

void abstract_check::calc() {
		
	assumptions = m_conclusion.assumptions;
	need_to_prove = m_conclusion.need_to_prove;
	proofs = m_conclusion.proofs;
	
	res_conclusion.clear();
	for (int w = 0; w < (int)assumptions.size() - 1; w++) {
		res_conclusion.assumptions.push_back(assumptions[w]);
	}
	
	if (assumptions.size() > 0) {
		highlighted_assumption = assumptions[assumptions.size() - 1];
		free_vars_in_h_a = get_set_of_free_vars_in_expr(highlighted_assumption);
		
		res_conclusion.need_to_prove = make_shared<expr>(
														highlighted_assumption,
														CONSEQUENCE,
														need_to_prove);
	} else {
		highlighted_assumption = 0;
		free_vars_in_h_a.clear();
		res_conclusion.need_to_prove = need_to_prove;
	}
	int c = 0;
	for (auto w : assumptions) {
		c++;
		map_of_assumptions.insert({to_string(w), c});
	}
	
	int no = 1;
	
	for (size_t w = 0; w < proofs.size(); w++) {
		no++;
		
		pos = w;
		new_proove();
		
		string str = to_string(proofs[w]);
		
		map<string, int>::iterator it;
		multimap<string, pair<string, int> >::iterator it2;
		
		{
			string poss_error;
			
			ax_res c = check_if_it_scheme_of_ax(proofs[w]);
			if (c.finded_ax >= 0) {
				is_scheme_of_ax(no, proofs[w]);
				goto cont;
			}
			if (c.finded_ax < -1) {
				poss_error = string("терм ") + to_string(c.term) + " не свободен для подстановки в формулу ";
				poss_error += to_string(c.formula) + " вместо переменной ";
				poss_error += c.var + ".";
			}
			
			///---------------------------------------------
			it = map_of_assumptions.find(str);
			if (it != map_of_assumptions.end()) {
				if ((*it).second == assumptions.size()) {
					is_highlighted_assumption();
				} else {
					is_assumption((*it).second, proofs[w]);
				}
				goto cont;
			}
			///---------------------------------------------
			map<string, pair<int, int> >::iterator it3;
			
			it3 = poss_m_p.find(str);
			if (it3 != poss_m_p.end()) {
				is_MP((*it3).second.first, (*it3).second.second);
				goto cont;
			}
			///---------------------------------------------
			
			pred_rules_res res = check_if_it_new_pred_rule(proofs[w]);
			if (res.res > 0) {
				if (free_vars_in_h_a.find(res.var) == free_vars_in_h_a.end()) {
					
					if (res.res == 1) {
						is_2_rule(res.finded_no, proofs[w]);
					} else {
						is_3_rule(res.finded_no, proofs[w]);
					}
					goto cont;
				} else {
					if (poss_error.empty()) {
						poss_error = "используется правило с квантором по переменной ";
						poss_error += res.var + ", входящей свободно в допущение ";
						poss_error += to_string(highlighted_assumption) + ".";
					}
				}
			} else {
				if ((res.res < 0) && (poss_error.empty())) {
					poss_error = string("переменная ") + res.var;
					poss_error += string(" входит свободно в формулу ");
					poss_error += to_string(res.formula) + ".";
				}
			}
			///---------------------------------------------
			if (is_not_proved(poss_error)) {
				return;
			}
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
}
