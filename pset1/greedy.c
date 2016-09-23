#include <stdio.h>
#include <cs50.h>

int main(void)
{
	double money;
	do
	{
		printf("O hai! How much change is owed?\n");
		money = GetDouble();
	} while (money < 0);

	int money_int = (int)money;
	money -= money_int; money *= 100;

	{
		int fix = (int)money;
		money = (double)fix;
	}

	int counter = 0;
	for (; money != 0; counter++)
	{
		money -= 25;
		if (money < 0)
		{
			money += 15;
			if (money < 0)
			{
				money += 5;
				if (money < 0)
				{
					money += 4;
				}
			}

		}
	}

	printf("%i\n", counter + money_int * 4);
	
}