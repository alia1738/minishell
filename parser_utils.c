/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 13:22:51 by anasr             #+#    #+#             */
/*   Updated: 2022/03/16 15:50:29 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		check_repeated_meta(char *input)
{
	int		i;
	bool	meta_place_taken;
	char	*meta[6] = {"<<", "<", ">>", ">", "|", 0};
	
	i = 0;
	meta_place_taken = false;
	skip_isspaces(&i, input);
	if (!ft_strncmp("|", &input[i], 1))
		return (-1);
	while (input[i])
	{
		if (ft_ismeta(&input[i], meta) > 0 && meta_place_taken)
			return (-1);
		else if (ft_ismeta(&input[i], meta) > 0)
		{
			if (ft_strncmp(&input[i], "|", 1) != 0)
				meta_place_taken = true;
			i += ft_ismeta(&input[i], meta);
		}
		else if (ft_isquote(input[i]) > 0)
			skip_quote_content(&i, input);
		else if (ft_isspace(input[i]) == 1)
			skip_isspaces(&i, input);
		else
		{
			i++;
			meta_place_taken = false;
		}
	}
	if (meta_place_taken == true) //for the case where the input ends with a meta character
		return (-1);
	return (0);
	// exit code is 258
}

// int main ()
// {
// 	errno = 258;
	
// 	if (check_repeated_meta("         <|echo '>>>>>>>'\" |'|||'|||hello>>>>\">>   d>p") == -1)
// 		printf("minishell: syntax error regarding the usage of metacharacters\n");
// 	else
// 		printf("YOURE GOOD FOR EXEC!\n");
// }