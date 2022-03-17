#include "minishell.h"

void	create_pipes(t_parser_info *p, int **pip)
{
	int	i;
	
	i = 0;
	pip = ft_calloc((p->pipes_count), sizeof(int *));
	while (i < p->pipes_count)
	{
		pip[i] = ft_calloc(2, sizeof(int));
		pipe(pip[i]);
		i++;
	}
}

void	create_pipe_append(t_parser_info *p, int **pipe_append)
{
	int	i;
	
	i = 0;
	pipe_append = ft_calloc((p->pipes_count), sizeof(int *));
	while (i <= p->pipes_count)
	{
		pipe_append[i] = ft_calloc(2, sizeof(int));
		pipe(pipe_append[i]);
		i++;
	}
}

void	check_inpipe_use(int flag, int index, int **pip)
{
	if (!flag)
	{
		close(pip[index - 1][1]);
		dup2(pip[index - 1][0], STDIN_FILENO);
		close(pip[index - 1][0]);
	}
	else if (flag)
	{
		close(pip[index - 1][0]);
		close(pip[index - 1][1]);
	}
}

void	check_outpipe_use(int flag, int index, int **pip)
{
	if (!flag)
	{
		close(pip[index][0]);
		dup2(pip[index][1], STDOUT_FILENO);
		close(pip[index][1]);
	}
	else if (flag)
	{
		close(pip[index][0]);
		close(pip[index][1]);
	}
}
