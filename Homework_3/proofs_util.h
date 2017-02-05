#ifndef PROOFS_UTIL_H
#define PROOFS_UTIL_H

#include "predicate_deduction/predicate_deduction.h"

conclusion prove_contraposition(expr_sp c);
conclusion prove_remove_double_not_from_cons(expr_sp c);
conclusion prove_for_all_not_is_not_exists(expr_sp c);

conclusion prove_a_equ_a(expr_sp c);
conclusion prove_a_equ_b_is_b_equ_a(expr_sp c);
conclusion prove_equality(int c1, int c2, int c3);

conclusion prove_no_bigger(int c1, int c2);

conclusion prove_induction(expr_sp c, string var);

conclusion prove_equalityes(vector<expr_sp> assumpt, expr_sp start, expr_sp finish);
conclusion prove_equalityes(vector<string>  assumpt, string  start, string  finish);

conclusion prove_main_base();

#endif // PROOFS_UTIL_H
