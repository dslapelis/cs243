/// file: warning.c
/// description: faulty code example that needs fixing.
/// @author RIT CS Dept.

#include <stdio.h>

/// compute a value.
int compute(int a) {
    int b = a * 5;
}

/// main function
int main(int argc) {
    int x = 10;
    int y = compute(20);

    printf("%d\n", y);

    return 0;
}

