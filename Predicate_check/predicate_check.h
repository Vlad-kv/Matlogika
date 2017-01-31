#ifndef PREDICATE_CHECK_H
#define PREDICATE_CHECK_H

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>

#include "parser.h"
#include "axioms_util.h"
#include "pred_rules.h"

struct conclusion {
	vector<expr_sp> assumptions;
	expr_sp need_to_prove;
	vector<expr_sp> proofs;
	
	void read();
};

struct predicate_check {
	vector<string> str_axioms = {
		"A->B->A",
		"(A->B)->(A->B->C)->(A->C)",
		"A->B->A&B",
		"A&B->A",
		"A&B->B",
		"A->A|B",
		"B->A|B",
		"(A->C)->(B->C)->(A|B->C)",
		"(A->B)->(A->!B)->!A",
		"!!A->A"
	};

	static vector<expr_sp> expr_axioms;

	map<string, int> all_consequences;
	
	predicate_check(conclusion m_conclusion)
	: m_conclusion(m_conclusion) {
		if (expr_axioms.size() != str_axioms.size()) {
			expr_axioms.clear();
			for (auto &s : str_axioms) {
				expr_axioms.push_back(to_expr(s));
			}
		}
	}

	vector<expr_sp> assumptions;
	map<string, int> map_of_assumptions;

	expr_sp need_to_prove;
	string original_need_to_prove;

	vector<expr_sp> proofs;

	map<string, int> existing_proofs;

	multimap<string, pair<string, int> > poss_poss_m_p;
	map<string, pair<int, int> > poss_m_p;

	string first_error_message;
	conclusion m_conclusion;
	
	pred_rules_res check_if_it_new_pred_rule(expr_sp c);
	
	void global_check();
};

#endif // PREDICATE_CHECK_H
