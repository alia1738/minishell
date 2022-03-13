/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 11:54:56 by anasr             #+#    #+#             */
/*   Updated: 2022/03/13 13:20:33 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getenv(int i, char *str)
{
	int		j;
	char	temp[1024];

	j = 0;
	ft_bzero(temp, 1024);
	while (str[++i] != '$' && ft_isspace(str[i]) == 0 && str[i])
	{
		temp[j++] = str[i];
	}
	return (getenv(temp));
}

int		count_dollar(char *str)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '$' && ft_isspace(str[i + 1]) == 0 && str[i + 1])
		{
			count += ft_strlen(ft_getenv(i, str));
			while (str[++i] != '$' && ft_isspace(str[i]) == 0 && str[i])
				;
		}
		else
		{
			count++;
			i++;
		}
		
	}
	return (count);
}

char	*expand_dollar(char *str)
{
	int		i;
	int		j;
	char	*expanded_str;

	i = 0;
	j = 0;
	expanded_str = ft_calloc(count_dollar(str) + 1, sizeof(char));
	while (str[i])
	{
		if (str[i] == '$' && ft_isspace(str[i + 1]) == 0 && str[i + 1])
		{
			if (ft_getenv(i, str))
			{
				ft_strcpy(&expanded_str[j], ft_getenv(i, str));
				j += ft_strlen(ft_getenv(i, str));
			}
			while (str[++i] != '$' && ft_isspace(str[i]) == 0 && str[i])
				;
		}
		else
		{
			expanded_str[j++] = str[i];
			i++;
		}
	}
	free(str);
	str = expanded_str;
	return (expanded_str);
}

// int main(int argc, char **argv)
// {
// 	char	*ptr;

// 	ptr = ft_strdup(argv[1]);
// 	printf("previous: %s\n", ptr);
// 	printf("after: %s\n", expand_dollar(ptr));
// }