#include <stdio.h>      // printfなど標準入出力関数
#include <stdlib.h>     // malloc, freeなどメモリ処理やexit関数
#include <string.h>     // strcmpなど文字列比較関数
#include <readline/readline.h>  // readline関数
#include <readline/history.h>   // add_history関数

int main(void)
{
    char *input;

    while (1)
    {
        // readline関数を使って、"minishell> " というプロンプトを表示し、ユーザ入力を受け取る
        input = readline("minishell> ");

        // readlineは、Ctrl+Dが押された場合やEOFに達した場合にNULLを返します。
        // その場合はシェルを終了することにします。
        if (input == NULL)
        {
            printf("exit\n"); // bashなどでもCtrl+Dでシェル終了時に改行が入るので追加
            break;
        }

        // もしユーザが空の行を入力した場合は、何もせず次のループへ
        // 空文字ではなく、NULL終端の"" (長さ0の文字列) が返る。
        if (strlen(input) == 0)
        {
            free(input);
            continue;
        }

        // "exit"と入力されたらシェルを終了
        if (strcmp(input, "exit") == 0)
        {
            free(input);
            break;
        }

        // 入力があったらヒストリに追加
        add_history(input);

        // 入力された文字列を表示してみる(ここではまだ実行などはしない)
        printf("You entered: %s\n", input);

        // readlineで確保されたメモリを開放
        free(input);
    }

    return 0;
}
