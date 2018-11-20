#include <u.h>
#include <libc.h>
void main(void) {
	uvlong time_s = 0;
	uvlong time_e = 0;
	uvlong res = 0;
	uvlong total = 0;
	char * mything = malloc(1600 * (1 << 20));

	for (int stride = 1; stride < ((1 << 20) * 16); stride = (int) (1.1 * stride + 1)) {
		total = 0;
		for (int i = 0; i < 100; i++) {
			cycles(&time_s);
			if (mything[i * stride] == 420) {}
			cycles(&time_e);
			res = time_e - time_s;
			total += res;
		}
		print("%d, %f\n", stride, res / 100.0);
	}
	free(mything);
	exits(nil);
}
