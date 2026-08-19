#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAPE_SIZE 100
#define BLANK 'B'

// ヘッドの移動方向
typedef enum {
    LEFT = -1,
    RIGHT = 1,
    STAY = 0
} Direction;

// 状態遷移ルールの構造体
typedef struct {
    int current_state;   // 現在の状態
    char read_symbol;    // 読み取る文字
    int next_state;      // 次の状態
    char write_symbol;   // 書き込む文字
    Direction dir;       // 移動方向
} Transition;

// チューニングマシン本体
typedef struct {
    char tape[TAPE_SIZE];
    int head;
    int current_state;
    int halt_state;
} TuringMachine;

// 初期化関数
void init_tm(TuringMachine* tm, const char* input, int start_state, int halt_state) {
    // テープ全体を空白文字で埋める
    memset(tm->tape, BLANK, TAPE_SIZE);

    // 入力文字列をテープの中央付近（位置20〜）にセット
    int start_pos = 20;
    for (int i = 0; input[i] != '\0'; i++) {
        tm->tape[start_pos + i] = input[i];
    }

    tm->head = start_pos;
    tm->current_state = start_state;
    tm->halt_state = halt_state;
}

// テープの現在状態を表示する関数
void print_tape(TuringMachine* tm) {
    // 表示範囲（10〜40）
    for (int i = 10; i < 40; i++) {
        printf("%c ", tm->tape[i]);
    }
    printf("\n");

    // ヘッドの位置にポインタを表示
    for (int i = 10; i < 40; i++) {
        if (i == tm->head) {
            printf("^ ");
        }
        else {
            printf("  ");
        }
    }
    printf(" (State: %d)\n\n", tm->current_state);
}

// マシンの実行関数
void run_tm(TuringMachine* tm, Transition rules[], int rule_count) {
    printf("=== 初期状態 ===\n");
    print_tape(tm);

    int step = 0;
    while (tm->current_state != tm->halt_state) {
        char current_char = tm->tape[tm->head];
        int rule_found = 0;

        // 適用できる状態遷移ルールを検索
        for (int i = 0; i < rule_count; i++) {
            if (rules[i].current_state == tm->current_state && rules[i].read_symbol == current_char) {
                // ルールに従って実行
                tm->tape[tm->head] = rules[i].write_symbol;
                tm->current_state = rules[i].next_state;
                tm->head += rules[i].dir;
                rule_found = 1;
                break;
            }
        }

        // 一致するルールが見つからなければ異常終了
        if (!rule_found) {
            printf("エラー: 状態 %d、文字 '%c' に対する遷移ルールが存在しません。\n", tm->current_state, current_char);
            return;
        }

        step++;
        printf("--- Step %d ---\n", step);
        print_tape(tm);

        // 安全のための無限ループ防止
        if (step > 100) {
            printf("ステップ数が上限（100）に達したため停止しました。\n");
            break;
        }
    }

    printf("=== 停止（ACCEPT） ===\n");
}

int main() {
    TuringMachine tm;

    // 遷移ルールの定義: 「111」の末尾に「1」を追加して「1111」にするマシン
    // 状態 0: '1'の右端を探す
    // 状態 1: 空白('B')を発見したら '1' を書いて停止状態(99)へ
    Transition rules[] = {
        {0, '1', 0, '1', RIGHT},  // 状態0で'1'なら右へ
        {0, BLANK, 1, '1', STAY}, // 状態0で'B'なら'1'を書いて状態1へ
        {1, '1', 99, '1', STAY}   // 状態1ならそのまま停止状態(99)へ
    };

    int rule_count = sizeof(rules) / sizeof(rules[0]);

    // 入力「111」、開始状態 0、停止状態 99 で初期化
    init_tm(&tm, "111", 0, 99);

    // 実行
    run_tm(&tm, rules, rule_count);

    return 0;
}
