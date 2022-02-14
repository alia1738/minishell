#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>       
# include <sys/wait.h>
# include <sys/types.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

/*------- > >> REDIRECTION << < -----------*/

# define ONE_IN_ONE_OUT 1 
# define TWO_IN_ONE_OUT 2 
# define ONE_IN_TWO_OUT 3 
# define TWO_IN_TWO_OUT 4 
#endif