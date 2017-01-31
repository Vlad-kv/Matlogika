#include "predicate_check.h"
using namespace std;
vector<expr_sp> predicate_check::expr_axioms;

void conclusion::read() {
	string assumption, str;
	getline(std::cin, str);
	size_t w;
	
	for (w = 0; w < str.length(); w++) {
		if (str[w] == ',') {
			assumptions.push_back(to_expr(assumption));
			assumption.clear();
			continue;
		}
		if ((str[w] == '|') && (str[w + 1] == '-')) {
			expr_sp ass = to_expr(assumption);
			if (ass == 0) {
				w += 2;
				break;
			}
			assumptions.push_back(ass);
			assumption.clear();
			
			w += 2;
			break;
		}
		assumption.push_back(str[w]);
	}
	string to_prove = "";
	
	while (w < str.length()) {
		to_prove.push_back(str[w]);
		w++;
	}
	need_to_prove = to_expr(to_prove);
	
	while (!feof(stdin)) {
		getline(cin, str);
		
		if (str == "") {
			continue;
		}
		proofs.push_back(to_expr(str));
	}
}

void predicate_check::global_check() {
	assumptions = m_conclusion.assumptions;
	need_to_prove = m_conclusion.need_to_prove;
	proofs = m_conclusion.proofs;
	
	int c = 0;
	
	for (auto w : assumptions) {
		if (c) {
			cout << ",";
		}
		cout << to_string(w);
		
		c++;
		map_of_assumptions.insert({to_string(w), c});
	}
	cout << "|-" << to_string(need_to_prove) << "\n";
	
	int no = 1;
	
	for (size_t w = 0; w < proofs.size(); w++) {
		cout << "(" << no << ") " << to_string(proofs[w]) << " ";
		no++;
		
		string str = to_string(proofs[w]);
		
		map<string, int>::iterator it;
		multimap<string, pair<string, int> >::iterator it2;
		
		{
			string poss_error;
			
			ax_res c = check_if_it_scheme_of_ax(proofs[w]);
			if (c.finded_ax >= 0) {
				cout << "(Сх. акс. " << c.finded_ax + 1 << ")\n";
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
				cout << "(Предп. " << (*it).second << ")\n";
				goto cont;
			}
			///---------------------------------------------
			map<string, pair<int, int> >::iterator it3;
			
			it3 = poss_m_p.find(str);
			if (it3 != poss_m_p.end()) {
				cout << "(M.P. " << (*it3).second.first + 1 << ", ";
				cout << (*it3).second.second + 1 << ")\n";
				goto cont;
			}
			///---------------------------------------------
			
			pred_rules_res res = check_if_it_new_pred_rule(proofs[w]);
			if (res.res > 0) {
				cout << "(Правило (" << res.res << ") " << res.finded_no + 1 << ")\n";
				goto cont;
			} else {
				if ((res.res < 0) && (poss_error.empty())) {
					poss_error = string("переменная ") + res.var;
					poss_error += string(" входит свободно в формулу ");
					poss_error += to_string(res.formula) + ".";
				}
			}
			///---------------------------------------------
			
			if (first_error_message.empty()) {
				first_error_message = poss_error;
			}
			
			cout << "(Не доказано)";
			if (!poss_error.empty()) {
				cout << "(" << poss_error << ")";
			}
			
			cout << "\n";
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

