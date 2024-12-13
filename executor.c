#include "minishell.h"
#include <sys/wait.h>

int g_last_exit_status = 0; // グローバル終了ステータス変数定義

// ディレクトリとコマンド名を結合して実行可能かチェック
static char *try_path_dir(const char *dir, const char *cmd)
{
    size_t len = strlen(dir) + strlen(cmd) + 2;
    char *full_path = malloc(len);
    if (!full_path)
        return NULL;
    strcpy(full_path, dir);
    strcat(full_path, "/");
    strcat(full_path, cmd);

    if (access(full_path, X_OK) == 0)
        return full_path;

    free(full_path);
    return NULL;
}

static char *find_command_path(const char *cmd, char **envp)
{
    (void)envp; 

    // '/'が含まれていればPATH検索しない
    if (contain_char(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return strdup(cmd);
        return NULL;
    }

    char *path_env = getenv("PATH");
    if (!path_env || !(*path_env))
        return NULL;

    char **dirs = ft_split(path_env, ':');
    if (!dirs)
        return NULL;

    char *found_path = NULL;
    for (size_t i = 0; dirs[i]; i++)
    {
        found_path = try_path_dir(dirs[i], cmd);
        if (found_path)
            break;
    }
    free_str_array(dirs);
    return found_path;
}

void execute_command(char *input, char **envp)
{
    extern int g_last_exit_status;
    char **args = parse_line(input);
    if (!args)
        return;
    if (!args[0]) {
        free_str_array(args);
        return;
    }

    if (is_builtin(args[0])) {
        int status = exec_builtin(args, envp);
        g_last_exit_status = status;
        free_str_array(args);
        return;
    }

    char *cmd_path = find_command_path(args[0], envp);
    if (!cmd_path) {
        fprintf(stderr, "%s: command not found\n", args[0]);
        free_str_array(args);
        g_last_exit_status = 127;
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        free_str_array(args);
        free(cmd_path);
        g_last_exit_status = 1;
        return;
    } else if (pid == 0) {
        if (execve(cmd_path, args, g_env) == -1) {
            perror("execve");
            free_str_array(args);
            free(cmd_path);
            exit(1);
        }
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            g_last_exit_status = WEXITSTATUS(status);
        else
            g_last_exit_status = 1;
    }

    free_str_array(args);
    free(cmd_path);
}