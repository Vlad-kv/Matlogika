#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <string>
#include <memory>

using namespace std;

bool is_poss_id_char(char c);

extern const char NEGATION[];
extern const char CONJUNCTION[];
extern const char DISJUNCTION[];
extern const char CONSEQUENCE[];
extern const char ARGUMENTS[];
extern const char ZERO[];
extern const char STROKE[];
extern const char MULTIPLICATION[];
extern const char SUM[];
extern const char FOR_ALL[];
extern const char EXISTS[];
extern const char EQUALITY[];

extern map<string, int> rang;

struct expr;

typedef std::shared_ptr<expr> expr_sp;

struct expr{
	expr_sp a[2];
	int rang;
	string val;
	
	expr(expr_sp c, string &&val, expr_sp v, int rang) :
		rang(rang), val(val) {
		a[0] = c;
		a[1] = v;
	}
	
	expr(expr_sp c, string &val, expr_sp v, int rang) :
		rang(rang), val(val) {
		a[0] = c;
		a[1] = v;
	}
	
	expr(expr_sp c, const char *val, expr_sp v, int rang) :
		rang(rang), val(val) {
		a[0] = c;
		a[1] = v;
	}
	
	expr(expr_sp c, const char *val, expr_sp v) :
		rang (::rang[val]), val(val) {
		
		a[0] = c;
		a[1] = v;
	}
	
	expr(expr_sp c, string val, expr_sp v) :
		rang (::rang[val]), val(val) {
		
		a[0] = c;
		a[1] = v;
	}
	
	friend expr_sp expr_copy(expr_sp ex) {
		expr_sp res(new expr(0, ex->val, 0, ex->rang));
		
		for (int w = 0; w < 2; w++) {
			if (ex->a[w]) {
				res->a[w] = expr_copy(ex->a[w]);
			}
		}
		
		return res;
	}
};

expr_sp to_expr(string &s);
expr_sp to_expr(const char* s);

void to_string(expr_sp c, string &res, int last_rang, int pos);
string to_string(expr_sp c);

bool check_is_it_var(expr_sp c);

#endif // PARSER_H
