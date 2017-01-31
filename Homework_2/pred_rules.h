#ifndef PRED_RULES_H
#define PRED_RULES_H

struct pred_rules_res {
	int res, finded_no;
	string var;
	expr_sp formula;
	
	pred_rules_res()
	: res(0), finded_no(-1) {
	}
	
	pred_rules_res(int res, int finded_no)
	: res(res), finded_no(finded_no) {
	}
	
	pred_rules_res(int res, int finded_no, string& var, expr_sp formula)
	: res(res), finded_no(finded_no),
	  var(var), formula(formula) {
	}
};

#endif // PRED_RULES_H
