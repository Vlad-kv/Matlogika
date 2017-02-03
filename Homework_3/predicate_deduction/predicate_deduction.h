#ifndef PREDICATE_DEDUCTION_H
#define PREDICATE_DEDUCTION_H

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <map>

#include "parser.h"
#include "axioms_util.h"
#include "pred_rules.h"
#include "conclusion.h"
#include "substitution.h"

struct predicate_deduction {
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
	
	predicate_deduction(conclusion m_conclusion)
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
	conclusion m_conclusion, res_conclusion;
	
	expr_sp highlighted_assumption;
	set<string> free_vars_in_h_a;
	
	pred_rules_res check_if_it_new_pred_rule(expr_sp c);
	
	conclusion start_deduction();
	
	void add_to_ans_if_it_scheme_of_ax_or_ass(expr_sp ex);
	void add_to_ans_highlighted_assumption();
	void add_to_ans_if_it_MP(int first, int second);
	
	void add_to_ans_if_it_2_rule(expr_sp c);
	void add_to_ans_if_it_3_rule(expr_sp c);
};

#endif // PREDICATE_DEDUCTION_H
