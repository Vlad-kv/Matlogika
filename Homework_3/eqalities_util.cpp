#include "proofs_util.h"

const int MAX_NUM_EQU = 50;
int next_no;
map<string, int> disp;
expr_sp disp_inv[MAX_NUM_EQU];

bool g[MAX_NUM_EQU][MAX_NUM_EQU];

void add_to_disp(expr_sp c) {
	if (disp.find(m_to_string(c)) == disp.end()) {
		disp[m_to_string(c)] = next_no;
		disp_inv[next_no] = c;
		next_no++;
	}
}
vector<int> path;
bool used[MAX_NUM_EQU];

bool find_path(int w, int finish) {
	path.push_back(w);
	
	used[w] = 1;
	
	if (w == finish) {
		return 1;
	}
	
	for (int e = 0; e < next_no; e++) {
		if ((! used[e]) && ((g[e][w]) || (g[w][e]))) {
			if (find_path(e, finish)) {
				return 1;
			}
		}
	}
	path.pop_back();
	return 0;
}

conclusion prove_equalityes(vector<expr_sp> assumpt, expr_sp start, expr_sp finish) {
	disp.clear();
	next_no = 0;
	path.clear();
	
	for (auto w : assumpt) {
		add_to_disp(w->a[0]);
		add_to_disp(w->a[1]);
	}
	add_to_disp(start);
	add_to_disp(finish);
	
	for (int w = 0; w < next_no; w++) {
		used[w] = 0;
	}
	for (int e = 0; e < next_no; e++) {
		for (int r = 0; r < next_no; r++) {
			g[e][r] = 0;
		}
	}
	
	for (auto w : assumpt) {
		g[disp[m_to_string(w->a[0])]][disp[m_to_string(w->a[1])]] = 1;
	}
	
	if (! find_path(disp[m_to_string(start)], disp[m_to_string(finish)])) {
		throw string("No path between ") + m_to_string(start) + " and " + 
		      m_to_string(finish) + ".";
	}
	
	conclusion res;
	res.assumptions = assumpt;
	res.need_to_prove = make_shared<expr>(start, EQUALITY, finish);
	
	for (int w = 1; w < path.size(); w++) {
		expr_sp c = disp_inv[path[w - 1]];
		expr_sp v = disp_inv[path[w]];
		
		if (! g[path[w - 1]][path[w]]) {
			swap(c, v);
		}
		res.add(prove_a_equ_b_is_b_equ_a(make_shared<expr>(c, EQUALITY, v)));
	}
	
	for (int w = 2; w < path.size(); w++) {
		expr_sp c1 = start;
		expr_sp c2 = disp_inv[path[w - 1]];
		expr_sp c3 = disp_inv[path[w - 0]];
		
		expr_sp v1 = make_shared<expr>(c2, EQUALITY, c1);
		expr_sp v2 = make_shared<expr>(c2, EQUALITY, c3);
		expr_sp v3 = make_shared<expr>(c3, EQUALITY, c1);
		
		expr_sp cons = make_shared<expr>(v1, CONSEQUENCE, v3);
		
		res.add(make_shared<expr>(v2, CONSEQUENCE, cons));
		res.add(cons);
		res.add(v3);
	}
	
	if (! g[disp[m_to_string(start)]][disp[m_to_string(finish)]]) {
		res.add(prove_a_equ_b_is_b_equ_a(make_shared<expr>(finish, EQUALITY, start)));
	}
	
	for (int w = 0; w < next_no; w++) {
		disp_inv[w] = 0;
	}
	return res;
}

conclusion prove_equalityes(vector<string>  assumpt, string  start, string  finish) {
	vector<expr_sp> ass;
	for (auto &w : assumpt) {
		ass.push_back(to_expr(w));
	}
	return prove_equalityes(ass, to_therm(start), to_therm(finish));
}

