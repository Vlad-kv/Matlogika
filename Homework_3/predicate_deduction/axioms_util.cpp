#include <set>
#include "axioms_util.h"
#include <vector>

#include "abstract_check.h"

using namespace std;

namespace {
	set<string> locked_vars;
	expr_sp new_finded_expr;

	multiset<string> can_be_locked;
}
bool check_is_it_can_be_substituted(string &var, expr_sp c1, expr_sp c2) {
	if (c1->val == var) {
		if (new_finded_expr == 0) {
			new_finded_expr = c2;
		} else {
			if (to_string(new_finded_expr) != to_string(c2)) {
				return 0;
			}
		}
		for (auto v : can_be_locked) {
			locked_vars.insert(v);
		}
		can_be_locked.clear();
		return 1;
	}
	if (c1->val != c2->val) {
		return 0;
	}
	if ((c1->val == FOR_ALL) || (c1->val == EXISTS)) {
		if (c1->a[0]->val != c2->a[0]->val) {
			return 0;
		}
		string &quant_var = c1->a[0]->val;
		
		if (quant_var == var) {
			return (to_string(c1->a[1]) == to_string(c2->a[1]));
		}
		if (locked_vars.find(quant_var) == locked_vars.end()) {
			can_be_locked.insert(quant_var);
		}
		bool res = check_is_it_can_be_substituted(var, c1->a[1], c2->a[1]);
		can_be_locked.erase(quant_var);
		return res;
	}
	if ( ((c1->a[0] == 0) ^ (c2->a[0] == 0)) ||
		 ((c1->a[1] == 0) ^ (c2->a[1] == 0)) ) {
			return 0;
		 }
	for (int w = 0; w < 2; w++) {
		if (c1->a[w]) {
			if (!check_is_it_can_be_substituted(var, c1->a[w], c2->a[w])) {
				return 0;
			}
		}
	}
	return 1;
}

bool check_is_it_free_for_substitution(expr_sp c) {
	if (!c) {
		return 1;
	}
	if (check_is_it_var(c)) {
		return (locked_vars.find(c->val) == locked_vars.end());
	}
	for (int w = 0; w < 2; w++) {
		if (!check_is_it_free_for_substitution(c->a[w])) {
			return 0;
		}
	}
	return 1;
}

ax_res check_if_it_can_be_turned_out_from_correct_substitution(string &var, expr_sp c1, expr_sp c2) {
	locked_vars.clear();
	new_finded_expr = 0;
	can_be_locked.clear();
	
	bool res = check_is_it_can_be_substituted(var, c1, c2);
	if (res == 0) {
		return ax_res(0);
	}
	
	if (check_is_it_free_for_substitution(new_finded_expr)) {
		return ax_res(1);
	}
	return ax_res(-1, new_finded_expr, c1, var);
}

ax_res check_if_it_11_ax(expr_sp c) {
	if ((!c) || (c->val != CONSEQUENCE) || (c->a[0]->val != FOR_ALL)) {
		return 0;
	}
	string &var = c->a[0]->a[0]->val;
	expr_sp c1 = c->a[0]->a[1];
	expr_sp c2 = c->a[1];
	
	ax_res res = check_if_it_can_be_turned_out_from_correct_substitution(var, c1, c2);
	res.finded_ax *= 10;
	return res;
}

ax_res check_if_it_12_ax(expr_sp c) {
	if ((!c) || (c->val != CONSEQUENCE) || (c->a[1]->val != EXISTS)) {
		return 0;
	}
	string &var = c->a[1]->a[0]->val;
	expr_sp c1 = c->a[1]->a[1];
	expr_sp c2 = c->a[0];
	
	ax_res res = check_if_it_can_be_turned_out_from_correct_substitution(var, c1, c2);
	res.finded_ax *= 11;
	return res;
}

namespace {
	map<char, string> disp;
}

bool compare(expr_sp ax, expr_sp c) {
	if (!c) {
		return 1;
	}
	if ((ax->val.length() == 1) && (is_poss_id_char(ax->val[0]))) {
		char v = ax->val[0];
		if (disp.find(v) == disp.end()) {
			disp.insert({v, to_string(c)});
		} else {
			if (disp[v] != to_string(c)) {
				return 0;
			}
		}
		return 1;
	}
	if (ax->val != c->val) {
		return 0;
	}
	if ( ( (ax->a[0] == 0) ^ (c->a[0] == 0) ) || 
		 ( (ax->a[1] == 0) ^ (c->a[1] == 0) ) ) {
		 	return 1;
	}
	
	for (int w = 0; w < 2; w++) {
		if (ax->a[w]) {
			if (!compare(ax->a[w], c->a[w])) {
				return 0;
			}
		}
	}
	return 1;
}

ax_res check_if_it_scheme_of_ax(expr_sp c) {
	for (size_t w = 0; w < abstract_check::expr_axioms.size(); w++) {
		disp.clear();
		if (compare(abstract_check::expr_axioms[w], c) == 1) {
			return ax_res(w);
		}
	}
	for (size_t w = 0; w < abstract_check::expr_ar_axioms.size(); w++) {
		disp.clear();
		if (compare(abstract_check::expr_ar_axioms[w], c) == 1) {
			return ax_res(12 + w);
		}
	}
	ax_res ret_res, lok_res;
	
	///
	lok_res = check_if_it_11_ax(c);
	if (lok_res.finded_ax > 0) {
		return lok_res;
	} else {
		if (lok_res.finded_ax < 0) {
			ret_res = lok_res;
		}
	}
	///
	lok_res = check_if_it_12_ax(c);
	if (lok_res.finded_ax > 0) {
		return lok_res;
	} else {
		if (lok_res.finded_ax < 0) {
			ret_res = lok_res;
		}
	}
	return ret_res;
}
ax_res check_if_it_scheme_of_ax(string &s) {
	expr_sp c = to_expr(s);
	return check_if_it_scheme_of_ax(c);
}
