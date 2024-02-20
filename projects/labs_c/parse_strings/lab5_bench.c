#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "work.h"
#include "generate.h"

#define REPLAYS 20
#define MAX 100000
#define START 500
#define STEP 500

int main() {
	for (int n = START; n <= MAX; n+=STEP) {
		srand(time(NULL));
		double time_total = 0, avg_time = 0;
		for (int j = 0; j < REPLAYS; ++j) {
			Queue *q = generate(n);
			clock_t t = clock();
			work(q);
			t = clock() - t;
			double time = (double) t / CLOCKS_PER_SEC;
			time_total += time;
			queue_delete(q);
		}
		avg_time = time_total / REPLAYS;
		printf("%.8lf, ", avg_time);
		if (n % 10000 == 0)
			printf("\n");
	}
	printf("\n%d - %d, step - %d\n", START, MAX, STEP);
}
