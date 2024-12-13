#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    char *input;
    (void)argc;
    (void)argv;

    if (init_env(envp) < 0)
        return 1; // 環境初期化失敗時終了

    while (1)
    {
        input = readline("minishell> ");
        if (input == NULL) // Ctrl+D(EOF)対応
        {
            printf("exit\n");
            break;
        }

        if (strlen(input) == 0) // 空行なら何もしない
        {
            free(input);
            continue;
        }

        if (strcmp(input, "exit") == 0)
        {
            free(input);
            break;
        }

        add_history(input);
        execute_command(input, envp);
        free(input);
    }
    return 0;
}
