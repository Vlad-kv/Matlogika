#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>

#include "predicate_deduction.h"
using namespace std;
void conclusion::read() {
	string assumption, str;
	getline(std::cin, str);
	size_t w;
	
	for (w = 0; w < str.length(); w++) {
		if (str[w] == ',') {
			assumptions.push_back(to_expr(assumption));
			assumption.clear();
			continue;
		}
		if ((str[w] == '|') && (str[w + 1] == '-')) {
			expr_sp ass = to_expr(assumption);
			if (ass == 0) {
				w += 2;
				break;
			}
			assumptions.push_back(ass);
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
