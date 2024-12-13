#include "minishell.h"

char **g_env = NULL;

char **dup_env(char **envp)
{
    size_t count = 0;
    while (envp[count])
        count++;

    char **new_env = malloc(sizeof(char*) * (count + 1));
    if (!new_env)
        return NULL;

    for (size_t i = 0; i < count; i++)
    {
        new_env[i] = strdup(envp[i]);
        if (!new_env[i])
        {
            for (size_t j = 0; j < i; j++)
                free(new_env[j]);
            free(new_env);
            return NULL;
        }
    }
    new_env[count] = NULL;
    return new_env;
}

int init_env(char **envp)
{
    g_env = dup_env(envp);
    if (!g_env)
    {
        fprintf(stderr, "Failed to initialize environment\n");
        return -1;
    }
    return 0;
}

char *get_env_value(const char *key)
{
    for (size_t i = 0; g_env && g_env[i]; i++)
    {
        char *eq = ft_strchr(g_env[i], '=');
        if (!eq)
            continue;
        size_t keylen = (size_t)(eq - g_env[i]);
        if (strlen(key) == keylen && strncmp(g_env[i], key, keylen) == 0)
        {
            return eq + 1; 
        }
    }
    return NULL;
}

int set_env_value(const char *key, const char *value)
{
    if (!key || !*key)
        return -1;
    size_t keylen = strlen(key);

    // 既存キー探し
    size_t i = 0;
    for (; g_env && g_env[i]; i++)
    {
        char *eq = ft_strchr(g_env[i], '=');
        if (!eq) continue;
        size_t klen = (size_t)(eq - g_env[i]);
        if (klen == keylen && strncmp(g_env[i], key, klen) == 0)
        {
            char *new_entry = malloc(keylen + 1 + (value ? strlen(value) : 0) + 1);
            if (!new_entry)
                return -1;
            sprintf(new_entry, "%s=%s", key, value ? value : "");
            free(g_env[i]);
            g_env[i] = new_entry;
            return 0;
        }
    }

    // 新規追加
    size_t env_count = 0;
    while (g_env && g_env[env_count])
        env_count++;

    char **new_env = realloc(g_env, sizeof(char*) * (env_count + 2));
    if (!new_env)
        return -1;
    g_env = new_env;

    char *new_entry = malloc(keylen + 1 + (value ? strlen(value):0) + 1);
    if (!new_entry)
        return -1;
    sprintf(new_entry, "%s=%s", key, value ? value : "");
    g_env[env_count] = new_entry;
    g_env[env_count+1] = NULL;

    return 0;
}

int unset_env(const char *key)
{
    if (!g_env || !key || !*key)
        return -1;

    size_t keylen = strlen(key);
    size_t i = 0;
    for (; g_env[i]; i++)
    {
        char *eq = ft_strchr(g_env[i], '=');
        if (!eq) continue;
        size_t klen = (size_t)(eq - g_env[i]);
        if (klen == keylen && strncmp(g_env[i], key, klen) == 0)
        {
            free(g_env[i]);
            for (size_t j = i; g_env[j]; j++)
            {
                g_env[j] = g_env[j+1];
            }
            return 0;
        }
    }
    return 0;
}
