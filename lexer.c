#include "minishell.h"
#include <ctype.h>

// ヘルパー: 新規トークン作成
static t_token *new_token(void)
{
    t_token *t = malloc(sizeof(t_token));
    if (!t) return NULL;
    t->type = TT_WORD;
    t->value = NULL;
    t->in_squotes = 0;
    t->in_dquotes = 0;
    t->next = NULL;
    return t;
}

// ヘルパー: トークンリストにトークンを追加
static void token_add(t_token **head, t_token *new)
{
    if (!*head) {
        *head = new;
        return;
    }
    t_token *cur = *head;
    while (cur->next)
        cur = cur->next;
    cur->next = new;
}

// ヘルパー: トークンに文字を追加
static int token_append_char(t_token *token, char c)
{
    size_t len = token->value ? strlen(token->value) : 0;
    char *newval = malloc(len + 2);
    if (!newval) return -1;
    if (token->value)
        strcpy(newval, token->value);
    newval[len] = c;
    newval[len+1] = '\0';
    free(token->value);
    token->value = newval;
    return 0;
}

t_token *lexer_tokenize(const char *input)
{
    t_token *head = NULL;
    t_token *current = NULL;
    int in_squotes = 0;
    int in_dquotes = 0;
    size_t i = 0;

    while (input[i])
    {
        char c = input[i];

        // クォート切り替え処理
        if (c == '\'' && !in_dquotes) {
            // シングルクォートトグル
            in_squotes = !in_squotes;
            i++;
            continue;
        } else if (c == '"' && !in_squotes) {
            // ダブルクォートトグル
            in_dquotes = !in_dquotes;
            i++;
            continue;
        }

        // 空白判定: クォート外のみ空白でトークン区切り
        if (!in_squotes && !in_dquotes && isspace((unsigned char)c)) {
            // トークン終了
            if (current) {
                token_add(&head, current);
                current = NULL;
            }
            i++;
            continue;
        }

        // トークンがなければ新規作成
        if (!current) {
            current = new_token();
            if (!current) {
                // メモリエラー時はリソース解放が必要(省略)
                return head; 
            }
        }

        // 現在クォート状態をトークン記録
        current->in_squotes = in_squotes;
        current->in_dquotes = in_dquotes;

        // 文字追加
        if (token_append_char(current, c) < 0) {
            // メモリエラー (簡略処理)
            return head;
        }

        i++;
    }

    // 最後のトークンがあれば追加
    if (current) {
        token_add(&head, current);
    }

    return head;
}
