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
	return res;
}

