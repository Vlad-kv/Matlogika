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
	
	void clear() {
		assumptions.clear();
		need_to_prove = 0;
		proofs.clear();
		err_mess.clear();
	}
};

#endif // CONCLUSION_H
