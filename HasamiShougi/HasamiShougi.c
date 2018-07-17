//----------/* �͂��ݏ��� */----------//
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//----------// �ՖʊǗ��p�}�N�� //----------//
#define ROW (9)      //�s
#define COL (9)      //��
#define PLAY_1 (1)   //�v���C���[�P�̎w�W
#define PLAY_2 -(1)  //�v���C���[�Q�̎w�W
#define NONE (0)     //�R�}���������̎w�W

//----------// �o�͗p�}�N�� //----------//
#define ZERO(X) if(X == NONE) printf("��")                         //�R�}�������ꏊ
#define ONE(X) if(X == PLAY_1) printf("��")                        //�v���C���[�P
#define ONE_M(X) if(X == PLAY_2) printf("��")                      //�v���C���[�Q
#define COORDINATE(X) printf("%c%c", (-126),(80 + X))             //�v���C���[���͗p���W

//----------// �v���g�^�C�v�錾 //----------//
void Setting(void);                     //�Ֆʂ̏���������
int Playing(int);                       //�Ֆʂւ̓��͏���
void Display(void);                     //�Ֆʂ̏o�͏���
void Delete(int);                       //���񂾃R�}�̍폜����
int Count(int);                         //�R�}�̃J�E���g����
void Retry(int);                        //���g���C�m�F����

//----------// �O���[�o���ϐ��錾 //----------//
int BOARD[ROW][COL];                    //�͂��ݏ����̔Ֆ�


//----------// ���C���֐� //----------//
int main(void) {
    int Turn = 0;

    Setting();
    Display();
    while (1) { Turn = Playing(Turn); }

    return 0;
}


//----------// �g�p����֐��Q //----------//
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

/* �^�[�����̊Ǘ��̏��� */
    Turn++;
    if (Turn % 2 == 0) {
        Play = PLAY_2;
        puts("\n\nPLAYER�Q�̃^�[������");
    }
    if (Turn % 2 != 0) {
        Play = PLAY_1;
        puts("\n\nPLAYER�P�̃^�[������");
    }
    while (1) {
        /* �������R�}�̑I������ */
        puts("�����������̃R�}�̍��W���c�E���̏��Ԃ�ENTER�ŋ�؂��ē��͂��Ă��������B");
        scanf_s("%d%d", &b_col, &b_row);
        rewind(stdin);
        if (BOARD[b_row - 1][b_col - 1] != Play) {
            puts("���������Ƃ̂ł��Ȃ��R�}�ł��B�ē��͂��Ă��������B");
            continue;
        }
        /* �R�}�̈ړ���̏��� */
        puts("�ړ���̍��W���c�E���̏��Ԃ�ENTER�ŋ�؂��ē��͂��Ă��������B");
        scanf_s("%d%d", &a_col, &a_row);
        rewind(stdin);
        if (BOARD[a_row - 1][a_col - 1] != NONE) {
            puts("���̃}�X�ɂ͈ړ������邱�Ƃ��ł��܂���B�ē��͂��Ă��������B");
            continue;
        }
        /* �R�}���ړ��\���m�F���鏈�� */
        if (b_col != a_col && b_row != a_row) {
            puts("���̃}�X�ɂ͈ړ������邱�Ƃ��ł��܂���B�ē��͂��Ă��������B");
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
            puts("���̃}�X�ɂ͈ړ������邱�Ƃ��ł��܂���B�ē��͂��Ă��������B");
            continue;
        }
        break;     //����I�������[�v���I��������
    }

    /* �ՖʍX�V���� */
    BOARD[b_row - 1][b_col - 1] = NONE;
    BOARD[a_row - 1][a_col - 1] = Play;
    Delete(Play);
    system("cls");
    Display();

    /* �R�}���`�F�b�N�����g���C�̊m�F */
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

    /* ���̔��菈�� */
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
    /* �c�̔��菈��  */
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
    printf("�_");
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

    puts("\n\n�Q�[���I���I");
    if (Play == 1) {
        puts("Player�P�̏����ł��B\n");
    } else {
        puts("Player�Q�̏����ł��B\n");
    }
    while (1) {
        puts("������x�V�т܂����HY/N");
        scanf_s("%c", &ans);
        rewind(stdin);
        if (ans == 'Y') { break; }
        if (ans == 'N') { exit(0); }
        else {
            puts("���̓~�X�ł��BY��N����͂��Ă��������B");
            continue;
        }
    }
    /* �Ֆʂ̏������Əo�� */
    Setting();
    system("cls");
    Display();

    return;
}
