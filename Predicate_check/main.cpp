#include <iostream>
#include <cstdlib>
#include <cstdio>
using namespace std;

#include "predicate_check.h"

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	
	conclusion m_concl;
	m_concl.read();
	
	predicate_check check(m_concl);
	check.global_check();
    return 0;
}
