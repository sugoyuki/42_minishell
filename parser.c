#include "minishell.h"

// ヘルパー関数：整数を文字列に変換(itoa簡易版)
static char *int_to_str(int n)
{
    char buf[12]; // intを格納するには十分なサイズ(最大10桁+符号+null)
    sprintf(buf, "%d", n);
    return strdup(buf);
}

// 環境変数展開: $VAR → VARの値, $?: 最後の終了ステータス
char *expand_variables(const char *input)
{
    // シンプルな実装: 
    // 1. inputを一文字ずつ走査
    // 2. '$'が出たら次の文字を見て'?'ならg_last_exit_status展開、
//    変数名なら環境変数展開
    // クォート無視、$VAR名は英数字とアンダースコアのみ対応と仮定

    size_t len = strlen(input);
    char *result = malloc(len * 2 + 1); 
    // 適当に大きめ確保: 環境変数展開で長くなる可能性があるため、本来はreallocが必要だが簡易実装
    // 本格的には動的拡張が必要

    if (!result)
        return NULL;

    size_t ri = 0; // resultインデックス
    for (size_t i = 0; i < len; i++)
    {
        if (input[i] == '$')
        {
            // '$'発見
            i++;
            if (i >= len) {
                // 行末に$があった場合は何もしないで終了
                break;
            }
            if (input[i] == '?') {
                // $?展開
                char *status_str = int_to_str(g_last_exit_status);
                size_t slen = strlen(status_str);
                memcpy(&result[ri], status_str, slen);
                ri += slen;
                free(status_str);
            } else {
                // 環境変数名取得
                // 環境変数名は英数字と'_'とする
                size_t start = i;
                while (i < len && (isalnum((unsigned char)input[i]) || input[i] == '_'))
                    i++;
                size_t var_len = i - start;
                char var_name[256];
                if (var_len >= sizeof(var_name))
                    var_len = sizeof(var_name)-1; 
                strncpy(var_name, &input[start], var_len);
                var_name[var_len] = '\0';

                // 1文字も取得できなかった場合($後に変数名がない場合)は'$'と同じように扱わないで空文字
                if (var_len == 0) {
                    // $だけ出たが変数名なし → '$'文字は無視または'$'として出力しないようにする
                    // ここでは$だけの場合は無視
                } else {
                    // 環境変数展開
                    char *val = get_env_value(var_name);
                    if (!val)
                        val = ""; // 未定義変数は空文字
                    size_t vlen = strlen(val);
                    memcpy(&result[ri], val, vlen);
                    ri += vlen;
                }
                // iは変数名を読んだ後の位置になっているのでi--しない
                i--; 
            }
        }
        else
        {
            // 通常文字はそのままコピー
            result[ri++] = input[i];
        }
    }

    result[ri] = '\0';
    // ここでは余ったメモリは気にしない(簡易実装)
    return result;
}
