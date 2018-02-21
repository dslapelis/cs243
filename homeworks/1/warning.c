/// file: warning.c
/// description: faulty code example that needs fixing.
/// @author RIT CS Dept.

#include <stdio.h>

int compute(int);
/// compute a value.
int compute(int a) {
    return  a * 5;
}

/// main function
int main() {
    int y = compute(20);

    printf("%d\n", y);

    return 0;
}

