#include "mod1.h"
#include <stdio.h>
#include <stdlib.h>

void	fduplagx(t_lagrange *lag, float ptmp[lag->xd_s])
{
	size_t	i = 0;

	lag->xd = (float *)malloc(sizeof(float) * (lag->xd_s));
	while (i < lag->xd_s)
	{
		lag->xd[i] = ptmp[i];
		i++;
	}
}

size_t	flookfx(t_lagrange *lag, float ptmp[lag->xd_s + 1], float ref)
{
	size_t	i = 0;

	while (i < lag->xd_s)
	{
		if (ptmp[i] == ref)
			return (1);
		i++;
	}
	return (0);
}

void	diffx(short n, float points[n][3], t_lagrange *lag)
{
	short	i = 0;
	float	ptmp[n];
	lag->xd_s = 0;

	while (i < n)
	{
		if (!lag->xd_s || !flookfx(lag, ptmp, points[i][0]))
			ptmp[lag->xd_s++] = points[i][0];
		i++;
	}
	fduplagx(lag, ptmp);
}

float	Lx(t_lagrange *lag, float xi, float X)
{
	size_t	s = 0;
	float	polynome = 1;

	while (s < lag->xd_s)
	{
		if (lag->xd[s] != xi)
		 	polynome *= (X - lag->xd[s]) / (xi - lag->xd[s]);
		s++;
	}
	return (polynome);
}
