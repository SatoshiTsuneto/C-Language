//----------/* はさみ将棋 */----------//
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//----------// 盤面管理用マクロ //----------//
#define ROW (9)      //行
#define COL (9)      //列
#define PLAY_1 (1)   //プレイヤー１の指標
#define PLAY_2 -(1)  //プレイヤー２の指標
#define NONE (0)     //コマが無い時の指標

//----------// 出力用マクロ //----------//
#define ZERO(X) if(X == NONE) printf("□")                         //コマが無い場所
#define ONE(X) if(X == PLAY_1) printf("●")                        //プレイヤー１
#define ONE_M(X) if(X == PLAY_2) printf("○")                      //プレイヤー２
#define COORDINATE(X) printf("%c%c", (-126),(80 + X))             //プレイヤー入力用座標

//----------// プロトタイプ宣言 //----------//
void Setting(void);                     //盤面の初期化処理
int Playing(int);                       //盤面への入力処理
void Display(void);                     //盤面の出力処理
void Delete(int);                       //挟んだコマの削除処理
int Count(int);                         //コマのカウント処理
void Retry(int);                        //リトライ確認処理

//----------// グローバル変数宣言 //----------//
int BOARD[ROW][COL];                    //はさみ将棋の盤面


//----------// メイン関数 //----------//
int main(void) {
    int Turn = 0;

    Setting();
    Display();
    while (1) { Turn = Playing(Turn); }

    return 0;
}


//----------// 使用する関数群 //----------//
void Setting(void) {
    memset(BOARD, 0, sizeof(BOARD));
    for (int i = 0; i < ROW; i++) {
        BOARD[0][i] = PLAY_1;
        BOARD[ROW - 1][i] = PLAY_2;
    }
    return;
}


int Playing(int Turn) {
    int Play, Piece, search,
            b_row, b_col,
            a_row, a_col;

/* ターン数の管理の処理 */
    Turn++;
    if (Turn % 2 == 0) {
        Play = PLAY_2;
        puts("\n\nPLAYER２のターン→○");
    }
    if (Turn % 2 != 0) {
        Play = PLAY_1;
        puts("\n\nPLAYER１のターン→●");
    }
    while (1) {
        /* 動かすコマの選択処理 */
        puts("動かす自分のコマの座標を縦・横の順番でENTERで区切って入力してください。");
        scanf_s("%d%d", &b_col, &b_row);
        rewind(stdin);
        if (BOARD[b_row - 1][b_col - 1] != Play) {
            puts("動かすことのできないコマです。再入力してください。");
            continue;
        }
        /* コマの移動先の処理 */
        puts("移動先の座標を縦・横の順番でENTERで区切って入力してください。");
        scanf_s("%d%d", &a_col, &a_row);
        rewind(stdin);
        if (BOARD[a_row - 1][a_col - 1] != NONE) {
            puts("そのマスには移動させることができません。再入力してください。");
            continue;
        }
        /* コマが移動可能か確認する処理 */
        if (b_col != a_col && b_row != a_row) {
            puts("そのマスには移動させることができません。再入力してください。");
            continue;
        }
        search = 0;
        if (b_col == a_col) {
            if (b_row > a_row) {
                for (int i = 0; BOARD[a_row + i][b_col - 1] != BOARD[b_row - 1][b_col - 1]; i++) {
                    if (BOARD[a_row + i][a_col - 1] != NONE) {
                        search++;
                        break;
                    }
                }
            } else {
                for (int i = 0; BOARD[b_row + i][a_col - 1] != BOARD[a_row - 1][a_col - 1]; i++) {
                    if (BOARD[b_row + i][b_col - 1] != NONE) {
                        search++;
                        break;
                    }
                }
            }
        }
        if (b_row == a_row) {
            if (b_col > a_col) {
                for (int i = 0; BOARD[b_row - 1][a_col + i] != BOARD[b_row - 1][b_col - 1]; i++) {
                    if (BOARD[a_row - 1][a_col + i] != NONE) {
                        search++;
                        break;
                    }
                }
            } else {
                for (int i = 0; BOARD[a_row - 1][b_col + i] != BOARD[a_row - 1][a_col - 1]; i++) {
                    if (BOARD[b_row - 1][b_col + i] != NONE) {
                        search++;
                        break;
                    }
                }
            }
        }
        if (search != 0) {
            puts("そのマスには移動させることができません。再入力してください。");
            continue;
        }
        break;     //正常終了時ループを終了させる
    }

    /* 盤面更新処理 */
    BOARD[b_row - 1][b_col - 1] = NONE;
    BOARD[a_row - 1][a_col - 1] = Play;
    Delete(Play);
    system("cls");
    Display();

    /* コマ数チェック＆リトライの確認 */
    Piece = Count(Play);
    if (Piece < 3) {
        Retry(Play);
        Turn = 0;
    }
    return Turn;
}


void Delete(int Play) {
    int now_r, now_c,
            next_r, next_c,
            i, j, check, cnt, prev, side, own;

    /* 横の判定処理 */
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            if (BOARD[i][j] == Play) {
                prev = i;
                side = j;
                cnt = 0;
                for (side += 1; side < COL; side++) {
                    cnt = 1;
                    if (BOARD[prev][side] == Play || BOARD[prev][side] == NONE) { break; }
                    cnt = 0;
                    if (BOARD[prev][side + 1] == Play) { break; }
                }
                if (side == COL - 1) { break; }
                if (cnt == 0) {
                    now_r = i;
                    now_c = j;
                    own = 0;
                    for (check = j + 1; check < COL; check++) {
                        if (BOARD[i][check] == Play) {
                            next_r = i;
                            next_c = check;
                            own++;
                            break;
                        }
                        own = 0;
                    }
                    if (own != 0) {
                        while (1) {
                            now_c++;
                            if (BOARD[now_r][now_c] == BOARD[next_r][next_c]) { break; }
                            BOARD[now_r][now_c] = NONE;
                        }
                    }
                }
            }
        }
    }
    /* 縦の判定処理  */
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            if (BOARD[i][j] == Play) {
                prev = i;
                side = j;
                cnt = 0;
                for (prev += 1; prev < ROW; prev++) {
                    cnt = 1;
                    if (BOARD[prev][side] == Play || BOARD[prev][side] == NONE) { break; }
                    cnt = 0;
                    if (BOARD[prev + 1][side] == Play) { break; }
                }
                if (prev == ROW - 1) { break; }
                if (cnt == 0) {
                    now_r = i;
                    now_c = j;
                    own = 0;
                    for (check = i + 1; check < ROW; check++) {
                        if (BOARD[check][j] == Play) {
                            next_r = check;
                            next_c = j;
                            own++;
                            break;
                        }
                        own = 0;
                    }
                    if (own != 0) {
                        while (1) {
                            now_r++;
                            if (BOARD[now_r][now_c] == BOARD[next_r][next_c]) { break; }
                            BOARD[now_r][now_c] = NONE;
                        }
                    }
                }
            }
        }
    }
    return;
}


void Display(void) {
    printf("＼");
    for (int i = 0; i < ROW; i++) { COORDINATE(i); }
    for (int i = 0; i < ROW; i++) {
        printf("\n");
        COORDINATE(i);
        for (int j = 0; j < COL; j++) {
            ZERO(BOARD[i][j]);
            ONE(BOARD[i][j]);
            ONE_M(BOARD[i][j]);
        }
    }
    return;
}


int Count(int Play) {
    int Piece = 0;

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (BOARD[i][j] != NONE && BOARD[i][j] != Play) {
                Piece++;
            }
        }
    }
    return Piece;
}


void Retry(int Play) {
    char ans;

    puts("\n\nゲーム終了！");
    if (Play == 1) {
        puts("Player１の勝利です。\n");
    } else {
        puts("Player２の勝利です。\n");
    }
    while (1) {
        puts("もう一度遊びますか？Y/N");
        scanf_s("%c", &ans);
        rewind(stdin);
        if (ans == 'Y') { break; }
        if (ans == 'N') { exit(0); }
        else {
            puts("入力ミスです。YかNを入力してください。");
            continue;
        }
    }
    /* 盤面の初期化と出力 */
    Setting();
    system("cls");
    Display();

    return;
}
