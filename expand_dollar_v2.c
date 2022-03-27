/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar_v2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 15:22:39 by anasr             #+#    #+#             */
/*   Updated: 2022/03/27 13:10:57 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_meta(char	**meta)
{
	meta[0] = "<<";
	meta[1] = "<";
	meta[2] = ">>";
	meta[3] = ">";
	meta[4] = "|";
	meta[5] = "=";
	meta[6] = "+";
	meta[7] = ".";
	meta[8] = ",";
	meta[9] = ":";
	meta[10] = "^";
	meta[11] = "~";
	meta[12] = "-";
	meta[13] = 0;
}

static char	*ft_getenv(int i, char *str, t_parser_info *p)
{
	int		j;
	char	*meta[14];//"=" is added bec echo "$USER=" is anasr= or echo "$=" is $=
	char	temp[1024];

	init_meta(meta);
	j = 0;
	ft_bzero(temp, 1024);
	while (str[++i] != '$' && ft_ismeta(&str[i], meta) == 0 && ft_isquote(str[i]) == 0 && ft_isspace(str[i]) == 0 && str[i])
	{
		temp[j++] = str[i];
	}
	return (local_getenv(temp, p->env)); //getenv() MUST BE CHANGED TO local_getenv()
}

void	skip_dollar_content(int *i, char *str)
{
	char	*meta[14];//check things with equal

	(*i)++;
	init_meta(meta);
	while (str[*i] != '$' && ft_ismeta(&str[*i], meta) == 0 && ft_isquote(str[*i]) == 0 && ft_isspace(str[*i]) == 0 && str[*i])
		(*i)++;
	// if (str[*i] != '$')
	// 	(*i)++;
}

int		len_with_expansion(char	*str, t_parser_info *p) //keeping the quotes because i deal with them later
{
	int	i;
	int	len;
	char	*meta[14];
	char	*temp;

	init_meta(meta);
	i = 0;
	len = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
		{
			temp = ft_itoa(p->exit_code);
			len += ft_strlen(temp);
			i += 2;
			free(temp);
		}
		else if (str[i] == '$' && ft_isspace(str[i + 1]) == 0 && ft_ismeta(&str[i + 1], meta) == 0 && str[i + 1])
		{
			// printf("dollar expanded: %s\n", ft_getenv(i, str) ? ft_getenv(i, str) : "");
			len += ft_strlen(ft_getenv(i, str, p));
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
				if (str[i] == '$' && ft_isspace(str[i + 1]) == 0 && ft_ismeta(&str[i + 1], meta) == 0 && str[i + 1])
				{
					// printf("dollar expanded: %s\n", ft_getenv(i, str) ? ft_getenv(i, str) : "");
					len += ft_strlen(ft_getenv(i, str, p));
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
		{
			len++;
			i++;
		}
		// printf("currently: %s, previous len: %d\n", str + i, len);
	}
	return (len);
}

char	*expand_dollars_in_str(char *str, t_parser_info *p, bool append_flag)
{
	int		i;
	int		new_index;
	char	*expanded;
	char	*meta[14];
	char	*temp;

	// printf("LEN: %d\n", len_with_expansion(str));
	init_meta(meta);
	expanded = ft_calloc(len_with_expansion(str, p) + 1, sizeof(char));
	i = 0;
	new_index = 0;
	while (str[i])
	{
		// printf("currently: %s\n", str + i);
		
		if (str[i] == '$' && str[i + 1] == '?')
		{
			temp = ft_itoa(p->exit_code);
			ft_strcpy(&expanded[new_index], temp);
			free(temp);
			i += 2;
		}
		else if (str[i] == '$' && ft_isspace(str[i + 1]) == 0 && ft_ismeta(&str[i + 1], meta) == 0 && str[i + 1]) //last condition is for "echo $= "
		{
			// printf("dollar expanded: %s\n", ft_getenv(i, str) ? ft_getenv(i, str) : "");
			ft_strcpy(&expanded[new_index], ft_getenv(i, str, p));
			new_index += ft_strlen(ft_getenv(i, str, p));
			skip_dollar_content(&i, str);
			// printf("length of $: %d\n", len);
		}
		else if (ft_isquote(str[i]) == 1 && append_flag != true)
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
		else if (ft_isquote(str[i]) == 2 || (ft_isquote(str[i]) == 1 && append_flag == true))
		{
			expanded[new_index++] = str[i];
			i++;
			while ((ft_isquote(str[i]) != 2 && str[i]) || (ft_isquote(str[i]) != 1 && str[i] && append_flag == true))
			{
				// printf("currently: %s\n", str + i);
				if (str[i] == '$' && ft_isspace(str[i + 1]) == 0 && ft_ismeta(&str[i + 1], meta) == 0 && str[i + 1] && ft_isquote(str[i + 1]) == 0)  //last condition is for "echo "heyey$" "
				{
					// printf("dollar expanded: %s\n", ft_getenv(i, str) ? ft_getenv(i, str) : "");
					ft_strcpy(&expanded[new_index], ft_getenv(i, str, p));
					new_index += ft_strlen(ft_getenv(i, str, p));
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
