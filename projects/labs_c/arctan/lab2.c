#include <stdio.h>
#include <math.h>


double arctg_min (double x, double e)
{
	double a, current, fix_x;
	a = 0;
	current = e + 1;
	fix_x = x;
	int n = 0;
	int sign = 1;

	while (e < current)
	{
		current = (sign * x) / (1 + 2*n);
		sign *= -1;
		x = x * fix_x * fix_x;
		++n;
		a = a + current;
		printf("          a = %lf\n", a);
		current = fabs(current);
	}
	return a;
}


double arctg_max (double x, double e)
{
	double a, current, fix_x;
	a = M_PI * fabs(x)/(2*x);
	current = e + 1;
	fix_x = x;
	int n = 0;
    int sign  = -1;

	while (e < current)
	{
		current = sign / (x * (1 + 2*n));
		sign *= -1;
		x = x *fix_x * fix_x;
		++n;
		a = a + current;
		printf("          a = %lf\n", a);
		current = fabs(current);
	}
	return a;
}


int main()
{
	double x = 0.0, e = 0.0;
	int check_in1 = 0 , check_in2 = 0;

	printf ("Enter a number: ");
	check_in1 = scanf("%lf", &x);
	if (check_in1 == 0)
	printf ("You have entered not a number\n");

	
	else
	{
		printf ("Enter epsilon: ");
		check_in2 = scanf("%lf", &e);
	
	if (check_in2 == 0)
			printf ("You have entered not a number\n");
	
	else if (e <= 0)
			printf ("That is impossible\n");
	
	else
		{
			double e_temp = e;
			int count = 0;
			
			while (e_temp < 1)
			{
				e_temp *= 10;
				++count;
			}

			double a = 0, a1 = 0;

			a1 = atan(x);

			if (fabs(x) <= 1)
				{
					a = arctg_min(x,e);
					printf("atan = %.*lf\n", count+1, a1);
					printf("arctg = %.*lf\n", count+1, a);
				}
			else
				{
					a = arctg_max(x,e);
					printf("atan = %.*lf\n", count+1, a1);
					printf("arctg = %.*f\n", count+1, a);
				}
	
	
		}
	}
	return 0;
}

