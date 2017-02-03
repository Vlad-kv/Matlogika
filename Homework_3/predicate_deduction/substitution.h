#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H

expr_sp substitute(expr_sp c, map<string, expr_sp>& disp);
conclusion substitute(conclusion& c, map<string, expr_sp>& disp);

#endif // SUBSTITUTION_H

