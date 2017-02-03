#ifndef PROOFS_UTIL_H
#define PROOFS_UTIL_H

#include "predicate_deduction/predicate_deduction.h"

conclusion prove_contraposition(expr_sp c);
conclusion prove_remove_double_not_from_cons(expr_sp c);
conclusion prove_for_all_not_is_not_exists(expr_sp c);

#endif // PROOFS_UTIL_H
