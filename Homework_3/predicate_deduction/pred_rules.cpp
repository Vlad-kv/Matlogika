#include "abstract_check.h"
using namespace std;
#include <set>

bool check_is_var_free_in_expr(string &var, expr_sp c) {
	if (!c) {
		return 0;
	}
	if (c->val == var) {
		return 1;
	}
	if ((c->val == FOR_ALL) || (c->val == EXISTS)) {
		if (c->a[0]->val == var) {
			return 0;
		}
	}
	for (int w = 0; w < 2; w++) {
		if (check_is_var_free_in_expr(var, c->a[w])) {
			return 1;
		}
	}
	return 0;
}
namespace {
	set<string> res;
	multiset<string> locked_vars;
	
	void calc(expr_sp c) {
		if (!c) {
			return;
		}
		
		if (('a' <= c->val[0]) && (c->val[0] <= 'z')) {
			if (locked_vars.find(c->val) == locked_vars.end()) {
				res.insert(c->val);
			}
		}
		if ((c->val == FOR_ALL) || (c->val == EXISTS)) {
			locked_vars.insert(c->a[0]->val);
			calc(c->a[1]);
			
			locked_vars.erase(locked_vars.find(c->a[0]->val));
			return;
		}
		for (int w = 0; w < 2; w++) {
			calc(c->a[w]);
		}
	}
}
set<string> get_set_of_free_vars_in_expr(expr_sp c) {
	res.clear();
	locked_vars.clear();
	calc(c);
	
	return res;
}

pred_rules_res abstract_check::check_if_it_new_pred_rule(expr_sp c) {
	if (c->val != CONSEQUENCE) {
		return {0, -1};
	}
	
	pred_rules_res ret_res;
	
	if (c->a[1]->val == FOR_ALL) {
		expr_sp v = make_shared<expr>(c->a[0], CONSEQUENCE, c->a[1]->a[1]);
		
		auto it = all_consequences.find(to_string(v));
		if (it != all_consequences.end()) {
			int no = (*it).second;
			
			pred_rules_res lok_res(1, no, c->a[1]->a[0]->val, c->a[0]);
			if (check_is_var_free_in_expr(lok_res.var, lok_res.formula)) {
				lok_res.res *= -1;
			}
			
			if ((lok_res.res > 0) || (ret_res.res == 0)) {
				ret_res = lok_res;
			}
		}
	}
	
	if (c->a[0]->val == EXISTS) {
		expr_sp v = make_shared<expr>(c->a[0]->a[1], CONSEQUENCE, c->a[1]);
		
		auto it = all_consequences.find(to_string(v));
		if (it != all_consequences.end()) {
			int no = (*it).second;
			
			pred_rules_res lok_res(2, no, c->a[0]->a[0]->val, c->a[1]);
			if (check_is_var_free_in_expr(lok_res.var, lok_res.formula)) {
				lok_res.res *= -1;
			}
			
			if ((lok_res.res > 0) || (ret_res.res == 0)) {
				ret_res = lok_res;
			}
		}
	}
	return ret_res;
}

