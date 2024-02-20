#include <stdio.h>

int amount(long int x)
{
int count = 0;
while (x>0)
	{
	x =x/10;
	++count;
	}
return count;
}

long int my_pow (long int x, int n)
{
int q = 1, check = 1;
while (check)
	{
		if (n%2)
			q *= x;
		n /= 2;
		if (n>0)
			x *= x;
		else
			check = 0;
	}
return q;
}

int sign (long int x)
{
	if (x>=0)
		return 1;
	else 
		return -1;
}

int main()
{
long int x, p = 0, number = 0, temp = 0;
printf ("Please enter x\n");

int check_input = 0; 
check_input = scanf ("%ld", &x);

if (check_input == 0)
	printf("You've entered not a number\n");

else
{
int s = sign(x);
x *= s;

int a = 0, last = 0, c = 0;
a = amount(x);

if (a%2 == 1)
{
	last = x%10;
	x = x/10;
	a = a - 1;
	c = 1;
}

else
{
	last = 0;
	c = 0;
}

int a1 = a;
int temp_x = x;

while (temp_x > 99)
	temp_x = temp_x / 10;
temp_x = temp_x % 10;

if (temp_x == 0)
	--a1;

while (x>0)
	{
		p = my_pow(10, a-2);
		temp = x / p;
		temp = (temp%10)*10 + temp/10;
		number = number + temp;
		number = number * 100;
		x = x%p;
		a = a - 2;
	}

number = number/100;
int a2 = amount(number);

if (a2 != a1)
	number = number * my_pow(10, (a1 - a2));

if (c == 1)
	number = number*10 + last;

printf ("Result: %ld\n", number*s);
}
return 0;
}

