#include<stdio.h>
#include<stdlib.h>
#include"rand_num.h"

int IO_A(void);
void IO_QA(int);
int num_Q[4], num_A[4];

void main(void) {
    int cnt;
    rand_num(num_Q);  //�����ɂ�鐔�l�����Ƃ��̐��l��z��֊i�[
    cnt = IO_A();
    IO_QA(cnt);

    return;
}


int IO_A(void) {
    int cnt = 0, check = 0, blow, hit, same;

    for (;;) {
        cnt++, blow = 0, hit = 0, same = 0;

        printf("�񓚎҂͐��l���S���͂��Ă�������\n");
        for (int input = 0; input < 4; input++) {
            scanf_s("%d", &num_A[input]);
            rewind(stdin);
        }
        printf("\n");
        for (int output = 0; output < 4; output++) {
            printf("%d�@", num_A[output]);
        }
        for (int select = 0; select < 4; select++) {
            check = select + 1;
            while (check < 4) {
                if (num_A[check] == num_A[select]) {
                    same++;
                }
                check++;
            }
        }
        if (same >= 1) {
            printf("�F�G���[�@���������l���Q�ȏ゠��܂�\n\n");
            continue;
        }
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (num_A[i] == num_Q[i]) {
                    hit++;
                    break;
                }
                if (num_A[i] == num_Q[j]) {
                    blow++;
                }
            }
        }
        printf("�F%d�u���[�@%d�q�b�g\n", blow, hit);
        if (hit == 4)
            break;
    }
    printf("\nENTER�������Ă��������E�E�E");
    getchar();
    system("cls");

    return cnt;
}


void IO_QA(int cnt) {
    printf("�o��҂́@�@�@�@�I�񂾐��l�F");
    for (int output = 0; output < 4; output++) {
        printf("%d�@", num_Q[output]);
    }
    printf("\n");
    printf("�񓚎҂�%2d��ڂɑI�񂾐��l�F", cnt);
    for (int output = 0; output < 4; output++) {
        printf("%d�@", num_A[output]);
    }
    printf("\n\n");
    printf("�N���A�[�I\n�N���A�܂łɁu%d��v������܂����B\n", cnt);
    getchar();

    return;
}