/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar_v2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 15:22:39 by anasr             #+#    #+#             */
/*   Updated: 2022/03/11 11:49:50 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_getenv(int i, char *str)
{
	int		j;
	char	*meta[6] = {"<<", "<", ">>", ">", "|", 0};
	char	temp[1024];

	j = 0;
	ft_bzero(temp, 1024);
	while (str[++i] != '$' && ft_ismeta(&str[i], meta) == 0 && ft_isquote(str[i]) == 0 && ft_isspace(str[i]) == 0 && str[i])
	{
		temp[j++] = str[i];
	}
	return (getenv(temp)); //this getenv might need to be changed
}

void	skip_dollar_content(int *i, char *str)
{
	char	*meta[6] = {"<<", "<", ">>", ">", "|", 0};

	(*i)++;
	while (str[*i] != '$' && ft_ismeta(&str[*i], meta) == 0 && ft_isquote(str[*i]) == 0 && ft_isspace(str[*i]) == 0 && str[*i])
		(*i)++;
	// if (str[*i] != '$')
	// 	(*i)++;
}

int		len_with_expansion(char	*str) //keeping the quotes because i deal with them later
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			// printf("dollar expanded: %s\n", ft_getenv(i, str) ? ft_getenv(i, str) : "");
			len += ft_strlen(ft_getenv(i, str));
			skip_dollar_content(&i, str);
			// printf("length of $: %d\n", len);
		}
		else if (ft_isquote(str[i]) == 1)
			len += skip_quote_content(&i, str);
		else if (ft_isquote(str[i]) == 2)
		{
			len++;
			i++;
			while (ft_isquote(str[i]) != 2 && str[i])
			{
				// printf("currently: %s\n", str + i);
				if (str[i] == '$')
				{
					// printf("dollar expanded: %s\n", ft_getenv(i, str) ? ft_getenv(i, str) : "");
					len += ft_strlen(ft_getenv(i, str));
					skip_dollar_content(&i, str);
					// printf("length of $: %d\n", len);
				}
				else
				{
					len++;
					i++;
				}
			}
		}
		else
			i++;
		// printf("currently: %s, previous len: %d\n", str + i, len);
	}
	return (len);
}

char	*expand_dollars_in_str(char *str)
{
	int		i;
	int		new_index;
	char	*expanded;
	
	// printf("LEN: %d\n", len_with_expansion(str));
	expanded = ft_calloc(len_with_expansion(str) + 1, sizeof(char));
	i = 0;
	new_index = 0;
	while (str[i])
	{
		// printf("currently: %s\n", str + i);
		if (str[i] == '$')
		{
			// printf("dollar expanded: %s\n", ft_getenv(i, str) ? ft_getenv(i, str) : "");
			ft_strcpy(&expanded[new_index], ft_getenv(i, str));
			new_index += ft_strlen(ft_getenv(i, str));
			skip_dollar_content(&i, str);
			// printf("length of $: %d\n", len);
		}
		else if (ft_isquote(str[i]) == 1)
		{
			expanded[new_index++] = str[i++];
			// printf("expanded: %s\n", expanded);
			while (ft_isquote(str[i]) != 1 && str[i])
			{
				expanded[new_index++] = str[i];
				i++;
			}
			if (str[i])
				expanded[new_index++] = str[i++];
		}
		else if (ft_isquote(str[i]) == 2)
		{
			expanded[new_index++] = str[i];
			i++;
			while (ft_isquote(str[i]) != 2 && str[i])
			{
				// printf("currently: %s\n", str + i);
				if (str[i] == '$')
				{
					// printf("dollar expanded: %s\n", ft_getenv(i, str) ? ft_getenv(i, str) : "");
					ft_strcpy(&expanded[new_index], ft_getenv(i, str));
					new_index += ft_strlen(ft_getenv(i, str));
					skip_dollar_content(&i, str);
					// printf("length of $: %d\n", len);
				}
				else
				{
					expanded[new_index++] = str[i];
					i++;
				}
			}
			if (str[i])
				expanded[new_index++] = str[i++];
			
		}
		else
		{
			expanded[new_index++] = str[i];
			i++;
		}
	}
	free(str);
	return (expanded);
}

// int	main(int argc, char **argv)
// {
// 	// char	*str = "\"hello'$LANG'$yeye$USER$PWD\"";
// 	// printf("%sBEFOR EXPANSION: %s\n%s", RED, argv[1], RESET);
// 	// printf("%sAFTER EXPANSION: %s\n%s", GREEN, expand_dollars_in_str(argv[1]), RESET);
// 	printf("%s\n", expand_dollars_in_str("\"heye$USER\"'$USER'"));
// 	// printf("len: %d\n", len_with_expansion("'$USER'$USER"));
// }

/*********************************/
/*Examples to keep in mind:      */
/*echo "hello$LANG$yeye$USER"$PWD*/
/*echo '$USER'$PWD				 */
/*echo '$USER'$PWD				 */
/*echo '$USER'$PWD				 */
/*********************************/
