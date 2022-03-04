/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 12:30:34 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/03 12:30:35 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main()
{
	char	cwd[PATH_MAX];
	if (!getcwd(cwd, sizeof(cwd)))
		perror("minishell: pwd");
	else
		printf("%s\n", cwd);
	return (0);
}