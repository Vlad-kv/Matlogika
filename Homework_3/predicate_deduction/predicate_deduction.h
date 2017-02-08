#ifndef PREDICATE_DEDUCTION_H
#define PREDICATE_DEDUCTION_H

#include "abstract_check.h"

struct predicate_deduction : public abstract_check {
	
	predicate_deduction(conclusion m_conclusion) : abstract_check(m_conclusion) {
	}
	
	conclusion start_deduction() {
		m_conclusion.correction();
		calc(1);
		return res_conclusion;
	}
	
	virtual void is_scheme_of_ax(int no, expr_sp ex) {
		add_to_ans_if_it_scheme_of_ax_or_ass(ex);
	}
	virtual void is_assumption(int no, expr_sp ex) {
		add_to_ans_if_it_scheme_of_ax_or_ass(ex);
	}
	virtual void is_highlighted_assumption() {
		add_to_ans_highlighted_assumption();
	}
	virtual void is_MP(int first, int second) {
		add_to_ans_if_it_MP(first, second);
	}
	
	virtual void is_2_rule(int no, expr_sp c) {
		add_to_ans_if_it_2_rule(c);
	}
	virtual void is_3_rule(int no, expr_sp c) {
		add_to_ans_if_it_3_rule(c);
	}
	
	virtual bool is_not_proved(string poss_error) {
		res_conclusion.clear();
		res_conclusion.err_mess = string("Вывод некорректен начиная с формулы номер ");
		res_conclusion.err_mess += m_to_string(pos + 1);
		
		if (!poss_error.empty()) {
			res_conclusion.err_mess += string(": ") + poss_error;
		}
		return 1;
	}
	
	void add_to_ans_if_it_scheme_of_ax_or_ass(expr_sp ex);
	void add_to_ans_highlighted_assumption();
	void add_to_ans_if_it_MP(int first, int second);
	
	void add_to_ans_if_it_2_rule(expr_sp c);
	void add_to_ans_if_it_3_rule(expr_sp c);
};

#endif // PREDICATE_DEDUCTION_H
