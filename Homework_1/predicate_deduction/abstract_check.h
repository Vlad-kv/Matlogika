#ifndef ABSTRACT_CHECK_H
#define ABSTRACT_CHECK_H

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

struct abstract_check {
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
	vector<string> str_ar_axioms = {
		"a=b->a'=b'",
		"a=b->a=c->b=c",
		"a'=b'->a=b",
		"!a'=0",
		"a+b'=(a+b)'",
		"a+0=a",
		"a*0=0",
		"a*b'=a*b+a"
	};
	
	static vector<expr_sp> expr_axioms;
	static vector<expr_sp> expr_ar_axioms;
	
	map<string, int> all_consequences;
	
	abstract_check(conclusion m_conclusion)
	: m_conclusion(m_conclusion) {
		if (expr_axioms.size() != str_axioms.size()) {
			expr_axioms.clear();
			expr_ar_axioms.clear();
			for (auto &s : str_axioms) {
				expr_axioms.push_back(to_expr(s));
			}
			for (auto &s : str_ar_axioms) {
				expr_ar_axioms.push_back(to_expr(s));
			}
		}
	}

	vector<expr_sp> assumptions;
	map<string, int> map_of_assumptions;

	expr_sp need_to_prove;

	vector<expr_sp> proofs;

	map<string, int> existing_proofs;

	multimap<string, pair<string, int> > poss_poss_m_p;
	map<string, pair<int, int> > poss_m_p;

	string first_error_message;
	conclusion m_conclusion, res_conclusion;
	
	expr_sp highlighted_assumption;
	set<string> free_vars_in_h_a;
	
	size_t pos;
	
	pred_rules_res check_if_it_new_pred_rule(expr_sp c);
	
	void calc(bool is_deduction);
	
	virtual void is_scheme_of_ax(int no, expr_sp ex) = 0;
	virtual void is_assumption(int no, expr_sp ex) = 0;
	virtual void is_highlighted_assumption() = 0;
	virtual void is_MP(int first, int second) = 0;
	
	virtual void is_2_rule(int no, expr_sp c) = 0;
	virtual void is_3_rule(int no, expr_sp c) = 0;
	
	virtual bool is_not_proved(string poss_error) = 0;
	
	virtual void new_proove() {
	}
};


#endif // ABSTRACT_CHECK_H
