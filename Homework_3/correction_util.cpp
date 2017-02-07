#include "predicate_deduction/predicate_check.h"

vector<expr_sp> correct(expr_sp c) {
	map<char, string> disp = can_be_corrected(c);
	if (disp.empty()) {
		return {c};
	}
	string corr("("+disp['@']+")");
	string ax = "(A->A->A)";
	
	vector<string> res = {ax, corr, corr+"->"+ax+"->"+corr, ax+"->"+corr};
	
	for (int w = 0; w < disp.size() - 1; w++) {
		string str = ax+"->";
		for (int e = w; e >= 0; e--) {
			str.push_back('@');
			str.push_back('a' + e);
		}
		res.push_back(str+corr);
	}
	expr_sp prev = to_expr(res[res.size() - 1])->a[1];
	res.push_back(to_string(prev));
	
	for (int w = disp.size() - 2; w >= 0; w--) {
		string buf; buf.push_back('a' + w);
		map<string, expr_sp> disp_0 = { {buf, to_therm(buf + "0")} };
		expr_sp next = substitute(prev->a[1], disp_0);
		
		res.push_back(to_string(prev)+"->"+to_string(next));
		res.push_back(to_string(next));
		prev = next;
	}
	///------------------------------------------
	corr = "("+res[res.size() - 1]+")";
	
	res.push_back(corr+"->"+ax+"->"+corr);
	res.push_back(ax+"->"+corr);
	
	for (int w = 0; w < disp.size() - 1; w++) {
		string str = ax+"->";
		for (int e = w; e >= 0; e--) {
			str.push_back('@');
			str.push_back('a' + e);
			str.push_back('0');
		}
		res.push_back(str+corr);
	}
	prev = to_expr(res[res.size() - 1])->a[1];
	res.push_back(to_string(prev));
	
	for (int w = disp.size() - 2; w >= 0; w--) {
		string buf; buf.push_back('a' + w); buf += "0";
		map<string, expr_sp> disp_0 = { {buf, to_therm(disp['a' + w])} };
		expr_sp next = substitute(prev->a[1], disp_0);
		
		res.push_back(to_string(prev)+"->"+to_string(next));
		res.push_back(to_string(next));
		prev = next;
	}
	
	vector<expr_sp> result;
	for (auto &w : res) {
		result.push_back(to_expr(w));
	}
	return result;
}

void conclusion::correction() {
	vector<expr_sp> buff;
	swap(buff, proofs);
	
	for (auto w : buff) {
		add(correct(w));
	}
}

