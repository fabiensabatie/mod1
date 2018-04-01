#include "mod1.h"
#include <stdio.h>
#include <stdlib.h>

void	fduplagy(t_lagrange *lag, float ptmp[lag->xd_s])
{
	size_t	i = 0;

	lag->yd = (float *)malloc(sizeof(float) * (lag->yd_s));
	while (i < lag->yd_s)
	{
		lag->yd[i] = ptmp[i];
		i++;
	}
}

size_t	flookfy(t_lagrange *lag, float ptmp[lag->xd_s + 1], float ref)
{
	size_t	i = 0;

	while (i < lag->yd_s)
	{
		if (ptmp[i] == ref)
			return (1);
		i++;
	}
	return (0);
}

void	diffy(short n, float points[n][3], t_lagrange *lag)
{
	short	i = 0;
	float	ptmp[n];
	lag->yd_s = 0;

	while (i < n)
	{
		if (!lag->yd_s || !flookfy(lag, ptmp, points[i][1]))
			ptmp[lag->yd_s++] = points[i][1];
		i++;
	}
	fduplagy(lag, ptmp);
}

float	Ly(t_lagrange *lag, float yi, float Y)
{
	size_t	s = 0;
	float	polynome = 1;

	while (s < lag->yd_s)
	{
		if (lag->yd[s] != yi)
		 	polynome *= (Y - lag->yd[s]) / (yi - lag->yd[s]);
		s++;
	}
	return (polynome);
}
