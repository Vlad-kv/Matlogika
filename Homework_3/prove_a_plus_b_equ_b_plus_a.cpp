#include "proofs_util.h"

conclusion prove_main_base() { // z+0 = 0+z
	conclusion transition = build_concl(
				{"z+0=0+z"}, "z'+0=0+z'",
				 {"z+0=z", "z+0=z->(z+0)'=z'",
				  "z+0=0+z", "z+0=0+z->(z+0)'=(0+z)'"}
			);
	transition.add(prove_equalityes( {"z'+0=z'", "(z+0)'=z'",
									  "(z+0)'=(0+z)'", "0+z'=(0+z)'"},
									 "z'+0", "0+z'"));
	remove_ass(transition);
	
	transition.remove_recurring();
	
	conclusion res = build_concl( {}, "z+0=0+z", {});
	res.add(prove_a_equ_a(to_therm("0+0")));
	res.add(transition);
	
	res.add(prove_induction(res.need_to_prove, "z"));
	return res;
}

conclusion prove_b_ai_equ_bi_a() { // b+a'=b'+a
	conclusion base = build_concl(
					{}, "x+0'=x'+0",
					{"x+0=x", "x+0=x->(x+0)'=x'"});
	base.add(prove_equalityes( {"x+0'=(x+0)'", "(x+0)'=x'",
								"x'+0=x'"},
							   "x+0'", "x'+0" ));
	conclusion transition = build_concl( {"x+z'=x'+z"}, "x+z''=x'+z'",
										 {"x+z'=x'+z",
										  "x+z'=x'+z->(x+z')'=(x'+z)'"}
								       );
	transition.add(prove_equalityes( {"x'+z'=(x'+z)'", "(x+z')'=(x'+z)'",
									  "x+z''=(x+z')'"},
 									 "x+z''", "x'+z'" ));
	remove_ass(transition);
	
	conclusion res = build_concl({}, "x+z'=x'+z", {});
	res.add(base);
	res.add(transition);
	
	res.add(prove_induction(res.need_to_prove, "z"));
	
	string a2 = "z425";
	
	map<string, expr_sp> disp = { {"z", to_therm(a2)} };
	res = substitute(res, disp);
	
	res.add("x+"+a2+"'=x'+"+a2+"->0+0=0->x+"+a2+"'=x'+"+a2+"");
	res.add("0+0=0->x+"+a2+"'=x'+"+a2+"");
	res.add("0+0=0->@"+a2+"(x+"+a2+"'=x'+"+a2+")");
	res.add("@"+a2+"(x+"+a2+"'=x'+"+a2+")");
	res.add("@"+a2+"(x+"+a2+"'=x'+"+a2+")->x+z'=x'+z");
	res.add("x+z'=x'+z");
	
	res.remove_recurring();
	return res;
}

conclusion prove_a_plus_b_equ_b_plus_a(string a, string b) {
	a = "("+a+")";
	b = "("+b+")";
	conclusion transition = build_concl({"z+x=x+z"}, "z+x'=x'+z",
										{"z+x=x+z", "z+x=x+z->(z+x)'=(x+z)'"}
										);
	transition.add(prove_b_ai_equ_bi_a());
	transition.add(prove_equalityes( {"z+x'=(z+x)'", "(z+x)'=(x+z)'", "x+z'=(x+z)'", "x+z'=x'+z"},
									 "z+x'", "x'+z"));
	remove_ass(transition);
	
	conclusion res;
	res.need_to_prove = to_expr(a+"+"+b+"="+b+"+"+a);
	
	res.add(prove_main_base());
	res.add(transition);
	res.add(prove_induction(to_expr("z+x=x+z"), "x"));
	
	res.add( {"0+0=0", "z+x=x+z->0+0=0->z+x=x+z",
			  "0+0=0->z+x=x+z", "0+0=0->@x(z+x=x+z)", "@x(z+x=x+z)",
			  "@x(z+x=x+z)->0+0=0->@x(z+x=x+z)", "0+0=0->@x(z+x=x+z)",
			  "0+0=0->@z@x(z+x=x+z)", "@z@x(z+x=x+z)"} );
	res.add( {"@z@x(z+x=x+z)->@x("+a+"+x=x+"+a+")",
			  "@x("+a+"+x=x+"+a+")",
			  "@x("+a+"+x=x+"+a+")->"+a+"+"+b+"="+b+"+"+a,
			  a+"+"+b+"="+b+"+"+a} );
	res.remove_recurring();
	return res;
}

