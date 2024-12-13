#include "minishell.h"

int contain_char(const char *s, char c)
{
    int i = 0;
    while (s[i])
    {
        if (s[i] == c)
            return 1;
        i++;
    }
    return 0;
}

// ft_strchr実装: s中の文字cを探し、見つかればそのポインタを返す。
// 見つからなければNULL。
char *ft_strchr(const char *s, int c)
{
    size_t i = 0;
    while (s[i])
    {
        if (s[i] == (char)c)
            return (char *)&s[i];
        i++;
    }
    if (c == '\0')
        return (char *)&s[i];
    return NULL;
}

// 単語数をカウント
static size_t count_words(const char *s, char c)
{
    size_t count = 0;
    int in_word = 0;

    while (*s)
    {
        if (*s != c && in_word == 0)
        {
            in_word = 1;
            count++;
        }
        else if (*s == c)
            in_word = 0;
        s++;
    }
    return count;
}

static char *word_dup(const char *start, size_t len)
{
    char *word = malloc(len + 1);
    if (!word)
        return NULL;
    for (size_t i = 0; i < len; i++)
        word[i] = start[i];
    word[len] = '\0';
    return word;
}

char **ft_split(const char *s, char c)
{
    if (!s)
        return NULL;

    size_t words = count_words(s, c);
    char **arr = malloc((words + 1) * sizeof(char *));
    if (!arr)
        return NULL;

    size_t w_i = 0;
    int in_word = 0;
    const char *word_start = NULL;
    for (size_t i = 0; s[i]; i++)
    {
        if (s[i] != c && in_word == 0)
        {
            in_word = 1;
            word_start = &s[i];
        }
        else if ((s[i] == c || s[i+1] == '\0') && in_word == 1)
        {
            size_t length = &s[i] - word_start + ((s[i] == c) ? 0 : 1);
            arr[w_i] = word_dup(word_start, length);
            if (!arr[w_i])
            {
                free_str_array(arr);
                return NULL;
            }
            w_i++;
            in_word = 0;
        }
    }
    arr[w_i] = NULL;
    return arr;
}

void free_str_array(char **arr)
{
    if (!arr)
        return;
    size_t i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}
