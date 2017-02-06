#include "proofs_util.h"

conclusion prove_main_base() { // a+0 = 0+a
	conclusion transition = build_concl(
				{"a+0=0+a"}, "a'+0=0+a'",
				 {"a+0=a", "a+0=a->(a+0)'=a'",
				  "a+0=0+a", "a+0=0+a->(a+0)'=(0+a)'"}
			);
	transition.add(prove_equalityes( {"a'+0=a'", "(a+0)'=a'",
									  "(a+0)'=(0+a)'", "0+a'=(0+a)'"},
									 "a'+0", "0+a'"));
	remove_ass(transition);
	
	conclusion res = build_concl( {}, "a+0=0+a", {});
	res.add(prove_a_equ_a(to_therm("0+0")));
	res.add(transition);
	
	res.add(prove_induction(res.need_to_prove, "a"));
	return res;
}

conclusion prove_b_ai_equ_bi_a() { // b+a'=b'+a
	conclusion base = build_concl(
					{}, "b+0'=b'+0",
					{"b+0=b", "b+0=b->(b+0)'=b'"});
	base.add(prove_equalityes( {"b+0'=(b+0)'", "(b+0)'=b'",
								"b'+0=b'"},
							   "b+0'", "b'+0" ));
	conclusion transition = build_concl( {"b+a'=b'+a"}, "b+a''=b'+a'",
										 {"b+a'=b'+a",
										  "b+a'=b'+a->(b+a')'=(b'+a)'"}
								       );
	transition.add(prove_equalityes( {"b'+a'=(b'+a)'", "(b+a')'=(b'+a)'",
									  "b+a''=(b+a')'"},
 									 "b+a''", "b'+a'" ));
	remove_ass(transition);
	
	conclusion res = build_concl({}, "b+a'=b'+a", {});
	res.add(base);
	res.add(transition);
	
	res.add(prove_induction(res.need_to_prove, "a"));
	
	string a2 = "a425";
	
	map<string, expr_sp> disp = { {"a", to_therm(a2)} };
	res = substitute(res, disp);
	
	res.add("b+"+a2+"'=b'+"+a2+"->0+0=0->b+"+a2+"'=b'+"+a2+"");
	res.add("0+0=0->b+"+a2+"'=b'+"+a2+"");
	res.add("0+0=0->@"+a2+"(b+"+a2+"'=b'+"+a2+")");
	res.add("@"+a2+"(b+"+a2+"'=b'+"+a2+")");
	res.add("@"+a2+"(b+"+a2+"'=b'+"+a2+")->b+a'=b'+a");
	res.add("b+a'=b'+a");
	
	return res;
}

conclusion prove_a_plus_b_equ_b_plus_a(string a, string b) {
	a = "("+a+")";
	b = "("+b+")";
	conclusion transition = build_concl({"a+b=b+a"}, "a+b'=b'+a",
										{"a+b=b+a", "a+b=b+a->(a+b)'=(b+a)'"
										
										}
										);
	transition.add(prove_b_ai_equ_bi_a());
	transition.add(prove_equalityes( {"a+b'=(a+b)'", "(a+b)'=(b+a)'", "b+a'=(b+a)'", "b+a'=b'+a"},
									 "a+b'", "b'+a"));
	remove_ass(transition);
	
	conclusion res;
	res.need_to_prove = to_expr(a+"+"+b+"="+b+"+"+a);
	
	res.add(prove_main_base());
	res.add(transition);
	res.add(prove_induction(to_expr("a+b=b+a"), "b"));
	
	res.add( {"0+0=0", "a+b=b+a->0+0=0->a+b=b+a",
			  "0+0=0->a+b=b+a", "0+0=0->@b(a+b=b+a)", "@b(a+b=b+a)",
			  "@b(a+b=b+a)->0+0=0->@b(a+b=b+a)", "0+0=0->@b(a+b=b+a)",
			  "0+0=0->@a@b(a+b=b+a)", "@a@b(a+b=b+a)"} );
	res.add( {"@a@b(a+b=b+a)->@b("+a+"+b=b+"+a+")",
			  "@b("+a+"+b=b+"+a+")",
			  "@b("+a+"+b=b+"+a+")->"+a+"+"+b+"="+b+"+"+a,
			  a+"+"+b+"="+b+"+"+a} );
	return res;
}

