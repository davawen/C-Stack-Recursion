#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef unsigned long long u64;

int fibo(int n) {
	if(n <= 1) return n;

	return fibo(n - 1) + fibo(n - 2);
}

u64 fibo_iter(u64 n) {
	static_assert(sizeof(u64) == sizeof(void *), "Pointer type isn't 64 bit wide");

	const u64 STACK_SIZE = 1024;

	u64 stack[STACK_SIZE];
	u64 *sp = stack - 1;
	u64 *bp = stack;

#define panic(message) do { \
	printf("Panicked! Message: %s\n\n", (message)); \
	exit(-1); \
} while(0)

#define push(value) do { \
	if(sp >= stack + STACK_SIZE) panic("Stack overflow!"); \
	u64 intermediate = (value); /* Evaluate value before changing sp */ \
	sp++; \
	*sp = intermediate; \
} while(0)

#define pops() do { \
	if(sp < stack) panic("Stack underflow!"); \
	sp--; \
} while(0)

#define pop(output) do { \
	if(sp < stack) panic("Stack underflow!"); \
	(output) = *sp; \
	sp--; \
} while(0)


	// Push argument to stack
	push(n);

	// Push end address to stack
	push((u64)&&end);

	u64 reg = 0;

	/*
Pseudocode:
Create new frame
Load argument in current frame as argument

if less or equal than 1:
	move return value (1) into reg
else:
	push argument for function: n-1
	push return address
	"call" function (goto start of loop)
	push retun value (reg)
	push argument for function: n-2
	push return address
	"call" function (goto start of loop)
	push return value (reg)
	add them together (pop them both)
	move result into reg

leave frame
	   */

	while(1) {
		// push rbp
		// mov rbp, rsp
		push((u64)bp);
		bp = sp;

		push(*(bp - 2)); // Load argument

		if(*sp <= 1) reg = *sp;
		else {
			push((*sp)-1);
			
			push((u64)&&func_call_return1);
			continue;

		func_call_return1:
			*sp = reg; // Save return value

			push(*(sp-1) - 2);

			push((u64)&&func_call_return2);
			continue;

		func_call_return2:

			pops();

			// Result of n - 2 is in reg
			// Result of n - 1 is in *sp

			reg += *sp;
		}
		
		// mov rsp, rbp
		// pop rbp
		sp = bp;
		pop(bp);

		void *return_addr;
		pop(return_addr);
		
		goto *return_addr;
	}

end:

	return reg;
}

int main() {
	for(size_t i = 0; i < 170; i++) {  
		printf("%llu\n", fibo_iter(i));
	}

	return 0;
}


