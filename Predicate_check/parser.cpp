#include <iostream>
#include <map>
#include "parser.h"
using namespace std;

const char NEGATION[] = "!";
const char CONJUNCTION[] = "&";
const char DISJUNCTION[] = "|";
const char CONSEQUENCE[] = "->";
const char ARGUMENTS[] = "ARGS";
const char ZERO[] = "0";
const char STROKE[] = "\'";
const char MULTIPLICATION[] = "*";
const char SUM[] = "+";
const char FOR_ALL[] = "@";
const char EXISTS[] = "?";
const char EQUALITY[] = "=";

map<string, int> rang = {
			{ZERO           , 3},
			{STROKE         , 4},
			{MULTIPLICATION , 5},
			{SUM            , 6},
			
			{FOR_ALL        , 1},
			{EXISTS         , 1},
			
			{EQUALITY       , 7},
			
			{NEGATION       , 8},
			{CONJUNCTION    , 9},
			{DISJUNCTION    , 10},
			{CONSEQUENCE    , 11},
			{ARGUMENTS      , 12}
		};

typedef std::shared_ptr<expr> expr_sp;

size_t pos;

void to_string(expr_sp c, string &res, int last_rang, int pos);

bool is_poss_id_char(char c) {
	return ( (('A' <= c) && (c <= 'Z')) ||
			 (('0' <= c) && (c <= '9')) );
}

void to_string(expr_sp c, string &res, int last_rang, int pos) {
	bool brackets = 0;
	
	if ((c->rang > last_rang) || ((c->rang == last_rang) && (pos == 0))) {
		brackets = 1;
	}
	if ((c->rang == last_rang) && (pos == 0) && (c->val == STROKE)) {
		brackets = 0;
	}
	
	if (brackets) {
		res.push_back('(');
	}
	
	if ((c->val == FOR_ALL) || (c->val == EXISTS)) {
		res += c->val;
	}
	
	if (c->a[0]) {
		to_string(c->a[0], res, c->rang, 0);
	}
	
	if (!((c->val == FOR_ALL) || (c->val == EXISTS))) {
		if (c->val == ARGUMENTS) {
			if ((c->a[0]) && (c->a[1])) {
				res.append(",");
			}
		} else {
			res.append(c->val);
		}
	}
	
	if (c->a[1]) {
		to_string(c->a[1], res, c->rang, 1);
	}
	
	if (brackets) {
		res.push_back(')');
	}
}

string to_string(expr_sp c) {
	string res;
	to_string(c, res, 1<<30, 1);
	return res;
}

string to_string(char c) {
	string res;
	res.push_back(c);
	return res;
}

bool check_is_it_var(expr_sp c) {
	return ((('a' <= c->val[0]) && (c->val[0] <= 'z')) && ((c->a[0] == 0) && (c->a[1] == 0)));
}

namespace {
	void next(string &s);
	
	expr_sp get_expression(string &s);
	expr_sp get_disjunction(string &s);
	expr_sp get_conjunction(string &s);
	expr_sp get_unary(string &s);
	
	expr_sp get_sum(string &s);
	expr_sp get_multiplication(string &s);
	expr_sp get_multiplier(string &s);
	
	expr_sp get_arguments_body(string &s);
	expr_sp get_arguments(string &s);
	
	expr_sp get_function(string &s);
	expr_sp get_predicate(string &s);
	
	string get_predicate_name(string &s) {
		string res;
		if (! (('A' <= s[pos]) && (s[pos] <= 'Z'))) {
			throw string("Error!");
		}
		do {
			res.push_back(s[pos]);
			pos++;
		} while (('0' <= s[pos]) && (s[pos] <= '9'));
		pos--;
		next(s);
		return res;
	}
	
	string get_function_name(string &s) {
		string res;
		if (! (('a' <= s[pos]) && (s[pos] <= 'z'))) {
			throw string("Error!");
		}
		do {
			res.push_back(s[pos]);
			pos++;
		} while (('0' <= s[pos]) && (s[pos] <= '9'));
		pos--;
		next(s);
		return res;
	}
	
	void next(string &s) {
		::pos++;
		while (pos < s.length()) {
			if ((s[pos] == ' ') || (s[pos] == 9) || (s[pos] == 13) ) {
				pos++;
			} else {
				break;
			}
		}
	}
	
	
	expr_sp get_expression(string &s) {
//		cout << pos << " get_expression\n";
		
		expr_sp res(get_disjunction(s));
		
		if (s[pos] == '-') {
			next(s);
			if (s[pos] != '>') {
				throw string("Error");
			}
			next(s);
			res = make_shared<expr>(res, CONSEQUENCE, get_expression(s));
		}
		return res;
	}
	
	expr_sp get_disjunction(string &s) {
//		cout << pos << " get_disjunction\n";
		
		expr_sp res(get_conjunction(s));
		
		if (s[pos] == '|') {
			next(s);
			res = make_shared<expr>(expr(res, DISJUNCTION, get_disjunction(s)));
		}
		return res;
	}
	
	expr_sp get_conjunction(string &s) {
//		cout << pos << " get_conjunction\n";
		expr_sp res(get_unary(s));
		
		if (s[pos] == '&') {
			next(s);
			res = make_shared<expr>(expr(res, CONJUNCTION, get_conjunction(s)));
		}
		return res;
	}
	
	expr_sp get_unary(string &s) {
//		cout << pos << " get_unary\n";
		if (s[pos] == '!') {
			next(s);
			return make_shared<expr>(expr(0, NEGATION, get_unary(s)));
		}
		if ((s[pos] == '@') || (s[pos] == '?')) {
			expr_sp res= make_shared<expr>(expr(0, to_string(s[pos]), 0));
			next(s);
			res->a[0] = make_shared<expr>(expr(0, get_function_name(s), 0, 0));
			res->a[1] = get_unary(s);
			return res;
		}
		int saved_pos = pos;
		try {
			if (s[pos] == '(') {
				next(s);
				expr_sp res(get_expression(s));
				
				if (s[pos] != ')') {
					throw string("Error");
				}
				next(s);
				return res;
			}
		} catch (string ex) {
//			cout << "catched\n";
			pos = saved_pos;
		}
		return get_predicate(s);
	}
	
	expr_sp get_sum(string &s) {
//		cout << pos << " get_sum\n";
		expr_sp res(get_multiplication(s));
		if (s[pos] == '+') {
			next(s);
			res = make_shared<expr>(expr(res, SUM, get_sum(s)));
		}
		return res;
	}
	
	expr_sp get_multiplication(string &s) {
		expr_sp res(get_multiplier(s));
		if (s[pos] == '*') {
			next(s);
			res = make_shared<expr>(expr(res, MULTIPLICATION, get_multiplication(s)));
		}
		return res;
	}
	
	expr_sp get_multiplier(string &s) {
		expr_sp res = 0;
		if (('a' <= s[pos]) && (s[pos] <= 'z')) {
			res = get_function(s);
			goto cnt;
		}
		if (s[pos] == '(') {
			next(s);
			res = get_sum(s);
			if (s[pos] != ')') {
				throw string("Error!");
			}
			next(s);
			goto cnt;
		}
		if (s[pos] == '0') {
			res = make_shared<expr>(expr(0, ZERO, 0));
			next(s);
			goto cnt;
		}
		throw string("Error!");
		
		cnt:;
		while (s[pos] == '\'') {
			res = make_shared<expr>(expr(res, STROKE, 0));
			next(s);
		}
		return res;
	}
	
	expr_sp get_arguments_body(string &s) {
//		cout << pos << " get_arguments_body\n";
		expr_sp res = make_shared<expr>(expr(get_sum(s), ARGUMENTS, 0));
		
		if (s[pos] == ',') {
			next(s);
			
			res->a[1] = get_arguments_body(s);
		}
		return res;
	}

	expr_sp get_arguments(string &s) {
//		cout << pos << " get_arguments\n";
		if (s[pos] != '(') {
			throw string("Error");
		}
		next(s);
		expr_sp res = make_shared<expr>(expr(0, ARGUMENTS, get_arguments_body(s)));
		
		if (s[pos] != ')') {
			throw string("Error");
		}
		next(s);
		
		return res;
	}

	expr_sp get_function(string &s) {
		expr_sp res = make_shared<expr>(expr(0, get_function_name(s), 0, 0));
		
		if (s[pos] == '(') {
			res->a[1] = get_arguments(s);
		}
		return res;
	}

	expr_sp get_predicate(string &s) {
//		cout << pos << " get_predicate\n";
		
		if (! (('A' <= s[pos]) && (s[pos] <= 'Z'))) {
			expr_sp res = make_shared<expr>(expr(0, EQUALITY, 0, 2));
			res->a[0] = get_sum(s);
			
			if (s[pos] != '=') {
				throw string("Error");
			}
			next(s);
			res->a[1] = get_sum(s);
			
			return res;
		} else {
			expr_sp res = make_shared<expr>(expr(0, get_predicate_name(s), 0, 2));
			if (s[pos] == '(') {
				res->a[1] = get_arguments(s);
			}
			return res;
		}
	}
}

expr_sp to_expr(string &s) {
	pos = -1;
	next(s);
	if (pos == s.length()) {
		return 0;
	}
	expr_sp res(get_expression(s));
	
	if (pos != s.length()) {
		throw string("Error!");
	}
	return res;
}

expr_sp to_expr(const char* s) {
	string str(s);
	return to_expr(str);
}
