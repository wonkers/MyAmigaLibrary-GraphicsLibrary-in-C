#include "extras.h"

VOID IntToString(int n, char *s, int length)
{
}

VOID itoas(int n, char *s)
{
	int i = 0, j;
	int sign = FALSE;
	
	if(n < 0)
	{
		sign = TRUE;
		n = -n;
	}
	
	do
	{
		s[i++] = (n % 10) + '0';
	}while(n /=10);
	
	if(sign)
	{
		s[i++] = '-';
	}
	
	for(j = 0; j < i; j++)
	{
		if(s[j] < '0' || s[j] > '9')
			s[j] = '0';
	}
	s[i] = EOS;
	reverse(s);
		
}

VOID reverse(char *s)
{
	int c, i, j;
	
	for(i = 0, j = strlen(s) - 1; i < j; i++, j--)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}