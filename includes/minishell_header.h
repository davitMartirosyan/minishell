/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   minishell_header.h								 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: dmartiro <dmartiro@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/09/21 20:20:22 by dmartiro		  #+#	#+#			 */
/*   Updated: 2023/01/11 21:50:56 by tumolabs         ###   ########.fr       */
/*																			*/
/* ************************************************************************** */

#ifndef MINISHELL_HEADER_H
# define MINISHELL_HEADER_H
# define RESERVED			"echo pwd cd unset export exit env"
# define TOKENS				"| < > ; &"
# define CMD_REGEX		 	"[:[A-Za-z]:]* [:[A-Za-z]:[[-]*[A-Za-z]]*]*"
# define HEREDOC_REGEX		"[[<<][A-Za-z]]*"
# define SHELL 				"Minishell-$ "
# define SHELLERR		   "-sadm"

/*Error Handlers*/
# define TOKEN_SYNTAX_ERR "Syntax error near unexpected token"
# define COMMANDERR "Command Not Found\n"
# define FILEERR "No Such file or directory\n"
# define HEREDOC_SYNTAX_WARNING "Warning: HEREDOC (wanted ${"
# define HEREDOC_LIMIT_ERR  "maximum here-document count exceeded"

# include <stdio.h>
# include <string.h>
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <stdarg.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <termios.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include "./structs.h"
# include "./builtins.h"

char		*ft_readline(char *line);
void		inthandle(int sig);
void		handleterm(int sig);
void		ft_signal(int handle);
int			ft_fprintf(int fd, const char *placeholder, ...);

/**********************************************/
/*******************Listing********************/
/**********************************************/
t_tok		*new_token(int len, char *token, int type);
void		add(t_tok **lst, t_tok *new);

/**********************************************/
/****************Initializing******************/
/**********************************************/
t_env		*env_tokenizing(char **envp);
char		*join_env(char *key, char eq, char *value);
char		**create_envp(t_env **env);
void		bash_setup(t_table **table, char **envp);
void		reserved(t_table **table);
int			get_pid(void);

/**********************************************/
/**************Lexical Analyzing***************/
/**********************************************/
char		*find_replace(char *cmdline, t_table *table);
char		*keyof(char *cmdline, int pos);
char		*valueof(char *key, t_env *env);
char		*replace(char *cmdline, char *key, char *val, int *pos);
char		*exit_status_code(char *cmd, t_table *table, int *pos);
void		quote_error(int *t_f, t_table *table);
void		token_replacment(char *cmdline, char schr, char rchr);
void		openquotes(char *cmdline);
void		inside_quote(char *cmdline, int *pos, int find);
int			contains(char *tok, char *cmdline, int *pos);
int			lexical_analyzer(char *cmdline, t_table *table);
int			check_quotes(char *cmdline, int *q_counts);
int			quote_syntax_analyzer(char *cmdline, int *q_c);

/**********************************************/
/****************Tokenizing********************/
/**********************************************/
t_tok		*tokenization(char *cmdline);
char		*word(char *cmdline, int len, int s_pos);
void		add_word(char *cmdline, int *pos, t_tok **token);
void		redirection(char *cmdline, int *pos, int io, t_tok **token);
void		expansion(char *cmdline, int *pos, int quote, t_tok **token);
void		add_pipe(char *cmdline, int *pos, int _p_ch, t_tok **token);
void		space(char *cmdline, int *pos, t_tok **token);
int			wordlen(char *wordstart, int s_pos);
int			typeface(int c, int len);

/**********************************************/
/************Parsing (Parse Tree)**************/
/**********************************************/
t_cmdline	*parse_tree(t_table *table);
t_cmds		*parse(t_tok *token, t_table *table);
char		*new_file(t_table *table);
char		*heredoc_delimiter(t_tok **token, t_vars **v);
char		*open_heredoc_prompt(char *delim, int flag, t_table *table);
char		*join_arguments(char *s1, int delimiter, char *s2);
char		*word_expansions(t_tok **token);
void		std(t_cmds **cmds);
void		separate(t_cmds **commands);
void		parse_to(t_tok *token, t_table *table, t_cmds **cmds);
void		select_filename(t_tok **token, t_cmds *cmds);
void		open__file__check__type(int type, char *filename, t_cmds *cmds);
void		check_type(int fd, int type, t_cmds *cmds);
void		heredoc(t_tok **token, t_cmds *cmds, t_table *table);
int			pipes(t_tok **token);
int			type_is_p_h(t_tok **tok, t_cmds ***cmds, t_table *table, int *rtr);
int			typeis_redirection(int type);
int			typeis_arg(int type);
int			typeis_heredoc(int type);

/**********************************************/
/****************Err Handling******************/
/**********************************************/
void		syntax_error(t_table *table);
void		_errno_(t_table *table, char *err);
void		print_tokens(t_tok *tok);
int			redirection_error(t_tok *tmp, t_table *table);
int			pipe_error(t_tok *tmp, t_table *table);

/**********************************************/
/*****************Execution********************/
/**********************************************/
char		**add_paths(t_env **env);
char		*join_paths(char *s1, int delimiter, char *s2);
void		file_mode(t_table *table, t_cmds *cmds);
void		execution(t_cmdline **cmdline, t_table **table);
void		execute(t_cmdline *cmd, t_table *table);
void		_execute(t_vars *v, t_cmdline *cmd, t_table *table);
void		_ffork(t_cmdline *cmd, t_table *table);
void		piping_execute(int pip, t_cmdline *cmd, t_table *table);
void		execute_pipe_command(t_cmds *cmds, t_vars *v, t_table *table);
void		piping(t_cmds *cmd, int pip_ptr[][2], int i, int pip);
void		handle_status__and_wait(int pid, int *status);
void		close_all_pipes(int pips[][2], int pip);
int			cmd_check(t_cmds *cmd, t_table *table);
int			check_executables(t_cmds *cmd, t_table *table, char **paths);
int			check_in_paths(t_cmds *cmd, char **paths, char *path);
int			find_in(char *builtin, char **reserved);
int			istream(t_cmds *cmd, int (*pipe)[2], int i);
int			ostream(t_cmds *cmd, int (*pipe)[2], int i);
int			check_command(t_cmds *cmds, t_vars *v, t_table *table);
int			_execute_pipes(t_cmds *cmds, t_vars *v, t_table *t, int (*pptr)[2]);

/**********************************************/
/****************Free Resources****************/
/**********************************************/
void		free_tokens(t_tok *token);
void		free_parse_tree(t_cmdline *tree);
void		free_char_pp(char ***pp);
void		update_table(t_cmdline *tree, t_table *table);
#endif
