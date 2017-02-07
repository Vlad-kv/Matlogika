#ifndef PREDICATE_CHECK_H
#define PREDICATE_CHECK_H

#include "abstract_check.h"

struct predicate_check : public abstract_check {
	
	predicate_check(conclusion m_conclusion) : abstract_check(m_conclusion) {
	}
	
	void global_check() {
		int c = 0;
		for (auto w : m_conclusion.assumptions) {
			if (c) {
				cout << ",";
			}
			cout << to_string(w);
			
			c++;
		}
		cout << "|-" << to_string(m_conclusion.need_to_prove) << "\n";
		calc(0);
	}
	
	virtual void is_scheme_of_ax(int no, expr_sp ex) {
		cout << "(Сх. акс. " << no + 1 << ")\n";
	}
	virtual void is_assumption(int no, expr_sp ex) {
		cout << "(Предп. " << no << ")\n";
	}
	virtual void is_highlighted_assumption() {
		is_assumption(assumptions.size(), highlighted_assumption);
	}
	virtual void is_MP(int first, int second) {
		cout << "(M.P. " << first + 1 << ", ";
		cout << second + 1 << ")\n";
	}
	
	virtual void is_2_rule(int no, expr_sp c) {
		cout << "(Правило (2) " << no + 1 << ")\n";
	}
	virtual void is_3_rule(int no, expr_sp c) {
		cout << "(Правило (3) " << no + 1 << ")\n";
	}
	
	virtual bool is_not_proved(string poss_error) {
		cout << "(Не доказано)";
		if (!poss_error.empty()) {
			cout << "(" << poss_error << ")";
		}
		cout << "\n";
		
//		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
		return 0;
	}
	
	virtual void new_proove() {
		cout << "(" << pos + 1 << ") " << to_string(proofs[pos]) << " ";
	}
};

#endif // PREDICATE_CHECK_H
