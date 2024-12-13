#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>

char    **ft_split(const char *s, char c);
void    free_str_array(char **arr);
// int     contain_char(const char *s, char c);
char    *ft_strchr(const char *s, int c);

// 環境変数管理
extern char **g_env; 
int     init_env(char **envp);
char    *get_env_value(const char *key);
int     set_env_value(const char *key, const char *value);
int     unset_env(const char *key);
char    **dup_env(char **envp);

// builtins
int     is_builtin(char *cmd);
int     exec_builtin(char **args, char **envp);

// executor
void    execute_command(char *input, char **envp);

//parser
extern int g_last_exit_status;

char *expand_variables(const char *input);

// トークンの種類(今回はシンプルにWORDのみ使う想定)
typedef enum e_tokentype {
    TT_WORD,
    // 将来はTT_PIPE, TT_REDIRECT等を追加
} t_tokentype;

typedef struct s_token {
    t_tokentype type;
    char        *value;
    int         in_squotes; // シングルクォート内で作られた部分か
    int         in_dquotes; // ダブルクォート内で作られた部分か
    struct s_token *next;
} t_token;

// lexer関数
t_token *lexer_tokenize(const char *input);

// parser関数
char **parse_line(const char *input);

#endif
