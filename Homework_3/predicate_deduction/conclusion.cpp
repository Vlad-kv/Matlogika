#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>

#include "conclusion.h"
#include "predicate_deduction.h"

using namespace std;
void conclusion::read() {
	string assumption, str;
	getline(std::cin, str);
	size_t w;
	
	for (w = 0; w < str.length(); w++) {
		if ((str[w] == '|') && (str[w + 1] == '-')) {
			assumptions = to_assumptions(assumption);
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

void conclusion::print() {
	if (!err_mess.empty()) {
		cout << err_mess << "\n";
		return;
	}
	
	bool b = 0;
	for (auto w : assumptions) {
		if (b) {
			cout << ",";
		}
		b = 1;
		
		if (w == 0) {
			cout << " !!!!\n";
		}
		
		cout << to_string(w);
	}
	cout << "|-" << to_string(need_to_prove) << "\n";
	
	for (auto w : proofs) {
		cout << to_string(w) << "\n";
	}
}

void conclusion::add(const string& c) {
	proofs.push_back(to_expr(c));
}
void conclusion::add(const vector<string>& c) {
	for (auto &w : c) {
		proofs.push_back(to_expr(w));
	}
}
void conclusion::add(const vector<expr_sp>& c) {
	for (auto &w : c) {
		proofs.push_back(w);
	}
}
void conclusion::add(expr_sp c) {
	proofs.push_back(c);
}
void conclusion::add(const conclusion& concl) {
	for (auto w : concl.proofs) {
		proofs.push_back(w);
	}
}

conclusion build_concl(vector<const char*> assumptions, const char* need_to_prove, vector<const char*> proofs) {
	conclusion res;
	for (auto w : assumptions) {
		res.assumptions.push_back(to_expr(w));
	}
	res.need_to_prove = to_expr(need_to_prove);
	for (auto w : proofs) {
		res.proofs.push_back(to_expr(w));
	}
	return res;
}

void conclusion::remove_recurring() {
	vector<expr_sp> buff;
	swap(buff, proofs);
	
	set<string> strs;
	
	for (auto w : buff) {
		string str = to_string(w);
		if (strs.find(str) == strs.end()) {
			strs.insert(str);
			add(w);
		}
	}
}

void remove_one_ass(conclusion &concl) {
	predicate_deduction ded(concl);
	concl = ded.start_deduction();
}

void remove_ass(conclusion &concl) {
	while (concl.assumptions.size() > 0) {
		remove_one_ass(concl);
	}
}

