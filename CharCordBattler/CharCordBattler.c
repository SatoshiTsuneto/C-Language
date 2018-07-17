//--------/* 文字コードバトラー */-------//
//-------/* ヘッダーファイル */-------//
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

//-------/* マクロ宣言 */-------//
/* 定数宣言 */
#define ELEM 8               //配列の要素数（武器の数）
#define CH_SIZE 32           //配列の要素数（武器名）
#define NAME_LEN 41          //配列の要素数（名前）
#define MIN_LEN 10           //名前の最低文字数
#define RAND_NUM 100         //算出定数

/* 簡易出力関数宣言 */
#define P_WAIT printf("何かボタンをクリック..."); getchar()
#define P_FINISH puts("ゲームを終了します。"); getchar()


//-------/* 構造体宣言 */-------//

/* プレイヤーステータス */
typedef struct {
    char NAME[NAME_LEN];     //名前
    int HP;                 //体力
    int ATTACK;             //攻撃力
    int DEFENCE;            //防御力
    int LUCK;               //運の良さ
    char WPN_NAME[CH_SIZE]; //武器の名前
    int WPN_POW;           //武器の威力
} STATUS;

/* 武器のステータス */
typedef struct {
    char Wpn_Name[ELEM][CH_SIZE];
    int Wpn_Pow[ELEM];
} WEAPON_SPEC;


//-------/* プロトタイプ宣言 */-------//
int Name_Entry(STATUS *PLAY, int Turn);                            //名前入力
void Output(STATUS *PLAY, int Turn);                               //ステータスの出力
void Battle(STATUS *PLAY_A, STATUS *PLAY_B, int Turn_A, int Turn_B); //バトル管理
void Retry(void);                                                  //リトライの確認


//-------/* グローバル宣言 */-------//

/* 武器リスト */
char Weapon_List[ELEM][CH_SIZE] = {
        {"AWM"},
        {"日本刀"},
        {"チェーンソー"},
        {"火縄銃"},
        {"バール"},
        {"RPG_7"},
        {"ダイナマイト"},
        {"ライトセイバー"}
};

/* 共有カウント変数 */
int Cnt;


//-------/* 繰り返し使用する計算関数一覧 */-------//

/* 武器ステータス割り振り */
void Weapon_Data(WEAPON_SPEC *data) {
    for (size_t i = 0; i < ELEM; i++) {
        strcpy(data->Wpn_Name[i], Weapon_List[i]);  //名前情報
        data->Wpn_Pow[i] = (i * 5 + 1);                  //威力算出
    }
    return;
}

/* ダメージ算出 */
int Pow_Calc(STATUS *PLAY_A, STATUS *PLAY_B) {
    int power = 0,
            damage = 0,
            critical = 0;
    Cnt = 0;
    srand((unsigned) time(NULL));

    power = PLAY_A->ATTACK + PLAY_A->WPN_POW;
    damage = power / ((rand() % PLAY_B->DEFENCE) + 1);
    if (damage < 0) { damage = rand() % 6; }
    critical = PLAY_A->LUCK - (rand() % RAND_NUM);
    if (critical >= 0) {
        damage = damage * 2;
        Cnt++;
    }
    PLAY_B->HP = PLAY_B->HP - damage;

    return damage;
}

/* ステータスの振り分け */
void Dispatching(STATUS *PLAY) {
    WEAPON_SPEC data;
    int calc_1 = 0,
            calc_2 = 0,
            calc_3 = 0,
            length = 0;

    Weapon_Data(&data);
    length = strlen(PLAY->NAME);
    //HP
    calc_1 = (int) (PLAY->NAME[0] & 0x0F);
    calc_2 = (int) (PLAY->NAME[0] & 0xF0) >> 4;
    PLAY->HP = abs(calc_1 + 1) * (calc_2 + 1);

    //攻撃力
    calc_1 = (int) (PLAY->NAME[1] & 0x0F);
    calc_2 = (int) (PLAY->NAME[1] & 0xF0) >> 4;
    calc_3 = (int) (PLAY->NAME[length - 2] & 0xF0) >> 4;
    PLAY->ATTACK = abs(calc_1 + 1) * abs(calc_2 + 1) + calc_3;

    //防御力
    calc_1 = (int) (PLAY->NAME[length - 1] & 0x0F);
    calc_2 = (int) (PLAY->NAME[length - 1] & 0xF0) >> 4;
    PLAY->DEFENCE = abs(calc_1 + 1) * abs(calc_2 + 1);

    //運の良さ
    calc_1 = (int) (PLAY->NAME[length - 2] & 0x0F);
    PLAY->LUCK = abs(calc_1 + 1) + abs(calc_3 + 1);

    //武器
    calc_1 = (int) (PLAY->NAME[0] & 0x07);
    strcpy(PLAY->WPN_NAME, data.Wpn_Name[calc_1]);
    PLAY->WPN_POW = data.Wpn_Pow[calc_1];

    return;
}


/* -------------------------------------------------------------------------------------------------------- */

//-------/* メインプログラム */-------//

int main(void) {
    STATUS PLAY_1, PLAY_2;
    int turn = 0;

    while (1) {
        /* 名前入力 */
        turn = 1;
        turn = Name_Entry(&PLAY_1, turn);
        turn = Name_Entry(&PLAY_2, turn);

        /* ステータス振り分け */
        Dispatching(&PLAY_1);
        Dispatching(&PLAY_2);

        while (1) {
            /* PLAYER_1のターン */
            Battle(&PLAY_1, &PLAY_2, 1, 2);
            if (Cnt == 0) {
                P_FINISH;
                return 0;
            }
            if (Cnt == 1) { break; }

            /* PLAYER_2のターン */
            Battle(&PLAY_2, &PLAY_1, 2, 1);
            if (Cnt == 0) {
                P_FINISH;
                return 0;
            }
            if (Cnt == 1) { break; }
        }
        system("cls");
    }
}


int Name_Entry(STATUS *PLAY, int Turn) {
    Cnt = 0;
    do {
        if (Cnt > 0) { puts("文字数が不足しています。再入力してください。"); }
        printf("プレイヤー%dの名前を”全角５文字以上２０文字以下” ”半角１０文字以上４０文字以下”で入力してください 。\n", Turn, Cnt++);
        fgets(PLAY->NAME, sizeof(PLAY->NAME), stdin);
        rewind(stdin);
    } while (strlen(PLAY->NAME) < MIN_LEN);

    return ++Turn;
}

void Output(STATUS *PLAY, int Turn) {
    printf("プレイヤー%dのスタータス\n", Turn);
    printf("名前：%s", PLAY->NAME);
    printf("HP：%d\n", PLAY->HP);
    printf("攻撃力：%d\n", PLAY->ATTACK);
    printf("防御力：%d\n", PLAY->DEFENCE);
    printf("運の良さ：%d\n", PLAY->LUCK);
    printf("武器[威力]：%s[%d]\n\n", PLAY->WPN_NAME, PLAY->WPN_POW);

    return;
}

void Battle(STATUS *PLAY_A, STATUS *PLAY_B, int Turn_A, int Turn_B) {
    int damage = 0;

    system("cls");
    Output(PLAY_A, Turn_A);
    Output(PLAY_B, Turn_B);
    printf("\n　%sが%sへ攻撃！持っている%sで攻撃力がアップ！\n", PLAY_A->NAME, PLAY_B->NAME, PLAY_A->WPN_NAME);
    P_WAIT;
    system("cls");
    damage = Pow_Calc(PLAY_A, PLAY_B);
    Output(PLAY_A, Turn_A);
    Output(PLAY_B, Turn_B);
    if (Cnt > 0) { printf("会心の一撃！"); }
    printf("%dのダメージ！\n", damage);
    P_WAIT;

    Cnt = RAND_NUM;
    if (PLAY_B->HP <= 0) {
        printf("\n戦闘終了！%sのHPが無くなったので%sの勝利です！\n", PLAY_B->NAME, PLAY_A->NAME);
        P_WAIT;
        system("cls");
        Retry();
    }
    return;
}

void Retry(void) {
    char Response[2];
    memset(Response, 0, sizeof(Response));

    while (1) {
        puts("もう一度遊びますか？Y/N");
        fgets(Response, sizeof(Response), stdin);
        rewind(stdin);
        switch (Response[0]) {
            case 'Y':
                Cnt = 1;
                return;

            case 'N':
                Cnt = 0;
                return;

            default:
                puts("入力ミスです。YかNで入力してください。");
        }
    }
}
