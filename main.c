#include "kernel.h"
#include "console.h"

int main(void)
{
	int i;

	while (1)
		for (i = 0; i < __COLOR_END__; i++)
			console_clear(WHITE, i);
	return 0;
}
