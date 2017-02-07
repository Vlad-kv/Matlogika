#ifndef CONCLUSION_H
#define CONCLUSION_H

#include "parser.h"
#include <vector>
using namespace std;

struct conclusion {
	vector<expr_sp> assumptions;
	expr_sp need_to_prove;
	vector<expr_sp> proofs;
	string err_mess;
	
	void read();
	void print();
	
	void add(const string& c);
	void add(const vector<string>& c);
	void add(expr_sp c);
	void add(const conclusion& concl);
	
	void clear() {
		assumptions.clear();
		need_to_prove = 0;
		proofs.clear();
		err_mess.clear();
	}
};

conclusion build_concl(vector<const char*> assumptions, const char* need_to_prove, vector<const char*> proofs);
void remove_ass(conclusion &concl);
void remove_one_ass(conclusion &concl);

#endif // CONCLUSION_H
