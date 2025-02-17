/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_config.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hberger <hberger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/15 18:38:34 by hberger           #+#    #+#             */
/*   Updated: 2019/11/03 19:22:21 by hberger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

/*
**  set_flags active la valeur du flag en structure
**  s'il est détecté et incrémente i (index de position dans la string format)
*/

static int	set_flags(t_printf *tab)
{
	if (tab->format[tab->i] == '-' && tab->i++)
		return ((tab->minus = 1));
	else if (tab->format[tab->i] == '+' && tab->i++)
		return ((tab->plus = 1));
	else if (tab->format[tab->i] == ' ' && tab->i++)
		return ((tab->space = 1));
	else if (tab->format[tab->i] == '0' && tab->i++)
		return ((tab->zero = 1));
	else if (tab->format[tab->i] == '#' && tab->i++)
		return ((tab->hash = 1));
	return (0);
}

/*
** set_width() intervient si on tombe sur un chiffre, via
** atoi et en incrémentant i de la taille du nombre
*/

static int	set_width(t_printf *tab)
{
	char *tmp;

	tmp = NULL;
	if (tab->format[tab->i] == '*')
	{
		tab->width = va_arg(tab->args, int);
		tab->width_config = 1;
		if (tab->width < 0)
		{
			tab->minus = 1;
			tab->width = -tab->width;
		}
		return ((tab->i += 1));
	}
	else if (ft_isdigit(tab->format[tab->i]))
	{
		while (tab->format[tab->i] == '0')
			tab->i++;
		tab->width = ft_atoi(&tab->format[tab->i]);
		tmp = ft_itoa(tab->width);
		tab->i += ft_strlen(tmp);
		free(tmp);
		return ((tab->width_config = 1));
	}
	return (0);
}

/*
**  set_precision() gère ça la précision si un . est détecté
**  On saute le . puis on atoi le nombre qui suit et incrémente i
**  par de la taille de ce nombre
**  Le while intervient pour gérer les éventuels 0 dans
** 	ft_printf("%.09s", "hi low");
**  afin que i soit correctement incrémenté, sans quoi, dans le cas ci dessus,
**  i serait incrémenté seulement de 1 car strlen atoi (09) = 1
*/

static int	set_precision(t_printf *tab, char *tmp)
{
	if (tab->format[tab->i] == '.')
	{
		tab->i++;
		if (tab->format[tab->i] == '*')
		{
			tab->precision = va_arg(tab->args, int);
			tab->precision_config = 1;
			if (tab->precision < 0 && !(tab->precision = 0))
				tab->precision_config = 0;
			return ((tab->i += 1));
		}
		while (tab->format[tab->i] == '0')
			tab->i++;
		if (!ft_isdigit(tab->format[tab->i]))
		{
			tab->precision = 0;
			return ((tab->precision_config = 1));
		}
		tab->precision = ft_atoi(&tab->format[tab->i]);
		tmp = ft_itoa(tab->precision);
		tab->i += ft_strlen(tmp);
		free(tmp);
		return ((tab->precision_config = 1));
	}
	return (0);
}

/*
**  setSize() gére les détection des flags (h, hh, l, ll, j, z)
**  et incrémente tab->i de la taille du flag
*/

static int	set_size(t_printf *tab)
{
	if (tab->format[tab->i] == 'h' && tab->format[tab->i + 1] == 'h'
									&& tab->i++ && tab->i++)
		return ((tab->hh = 1));
	else if (tab->format[tab->i] == 'l' && tab->format[tab->i + 1] == 'l'
										&& tab->i++ && tab->i++)
		return ((tab->ll = 1));
	else if (tab->format[tab->i] == 'h' && tab->i++)
		return ((tab->h = 1));
	else if (tab->format[tab->i] == 'l' && tab->i++)
		return ((tab->l = 1));
	else if (tab->format[tab->i] == 'j' && tab->i++)
		return ((tab->j = 1));
	else if (tab->format[tab->i] == 'z' && tab->i++)
		return ((tab->z = 1));
	return (0);
}

/*
** set_config() appelle les 4 fonctions ci-dessus et return dès que l'une
** d'elle modifie la config (si elle ne return pas, il pourrait y avoir des
** doublons lors de la détection des chiffres, etc ...)
*/

void		set_config(t_printf *tab)
{
	char	*tmp;

	tmp = NULL;
	if (set_flags(tab) == 1)
		return ;
	if (set_width(tab) == 1)
		return ;
	if (set_precision(tab, tmp) == 1)
		return ;
	if (set_size(tab) == 1)
		return ;
}
