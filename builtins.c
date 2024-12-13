#include "minishell.h"

// 既存のis_builtinを拡張
int is_builtin(char *cmd)
{
    if (strcmp(cmd, "echo") == 0) return 1;
    if (strcmp(cmd, "cd") == 0) return 1;
    if (strcmp(cmd, "pwd") == 0) return 1;
    if (strcmp(cmd, "exit") == 0) return 1;
    if (strcmp(cmd, "env") == 0) return 1;
    if (strcmp(cmd, "export") == 0) return 1;
    if (strcmp(cmd, "unset") == 0) return 1;
    return 0;
}

// echo, pwd, cd, exitは前回と同様
static int builtin_echo(char **args)
{
    int i = 1;
    int newline = 1;
    if (args[i] && strcmp(args[i], "-n") == 0)
    {
        newline = 0;
        i++;
    }
    while (args[i])
    {
        fputs(args[i], stdout);
        if (args[i+1])
            fputc(' ', stdout);
        i++;
    }
    if (newline)
        fputc('\n', stdout);
    return 0;
}

static int builtin_pwd(void)
{
    char cwd[4096];
    if (!getcwd(cwd, sizeof(cwd)))
    {
        perror("getcwd");
        return 1;
    }
    printf("%s\n", cwd);
    return 0;
}

static int builtin_cd(char **args)
{
    if (!args[1])
    {
        fprintf(stderr, "cd: missing argument\n");
        return 1;
    }
    if (chdir(args[1]) != 0)
    {
        perror("cd");
        return 1;
    }
    return 0;
}

static int builtin_exit(char **args)
{
    (void)args;
    exit(0);
}

// envコマンド
static int builtin_env(void)
{
    for (size_t i = 0; g_env && g_env[i]; i++)
        printf("%s\n", g_env[i]);
    return 0;
}

// exportコマンド
static int builtin_export(char **args)
{
    if (!args[1])
    {
        for (size_t i = 0; g_env && g_env[i]; i++)
            printf("export %s\n", g_env[i]);
        return 0;
    }

    for (int i = 1; args[i]; i++)
    {
        char *eq = ft_strchr(args[i], '=');
        if (eq)
        {
            size_t keylen = (size_t)(eq - args[i]);
            char *key = malloc(keylen + 1);
            if (!key)
                return 1;
            strncpy(key, args[i], keylen);
            key[keylen] = '\0';
            char *value = eq + 1;
            if (set_env_value(key, value) < 0)
            {
                free(key);
                fprintf(stderr, "export: failed to set %s\n", args[i]);
                return 1;
            }
            free(key);
        }
        else
        {
            if (set_env_value(args[i], "") < 0)
            {
                fprintf(stderr, "export: failed to set %s\n", args[i]);
                return 1;
            }
        }
    }
    return 0;
}

// unsetコマンド
static int builtin_unset(char **args)
{
    if (!args[1])
        return 0;
    for (int i = 1; args[i]; i++)
    {
        if (unset_env(args[i]) < 0)
        {
            fprintf(stderr, "unset: failed to unset %s\n", args[i]);
            return 1;
        }
    }
    return 0;
}

int exec_builtin(char **args, char **envp)
{
    (void)envp;
    if (strcmp(args[0], "echo") == 0) return builtin_echo(args);
    else if (strcmp(args[0], "pwd") == 0) return builtin_pwd();
    else if (strcmp(args[0], "cd") == 0) return builtin_cd(args);
    else if (strcmp(args[0], "exit") == 0) return builtin_exit(args);
    else if (strcmp(args[0], "env") == 0) return builtin_env();
    else if (strcmp(args[0], "export") == 0) return builtin_export(args);
    else if (strcmp(args[0], "unset") == 0) return builtin_unset(args);

    return 0;
}
