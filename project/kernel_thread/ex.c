#include <stdio.h>
// Slow fibonacci function
unsigned int fibonacci (unsigned int n) {
	if ( n == 0)
		return 0;
	else if ( n == 1)
		return 1;
	else
		return fibonacci(n-1) + fibonacci(n-2);
}

int main (void) {
	fibonacci(42);
	printf("done!\n");
}
