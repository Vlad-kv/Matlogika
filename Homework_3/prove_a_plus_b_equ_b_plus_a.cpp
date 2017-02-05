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
	
	return prove_equalityes( {"a'+0=a'", "(a+0)'=a'",
									  "(a+0)'=(0+a)'", "0+a'=(0+a)'"},
									 "a'+0", "0+a'");
	
	return transition;
	
}

