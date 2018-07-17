//--------/* �����R�[�h�o�g���[ */-------//
//-------/* �w�b�_�[�t�@�C�� */-------//
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

//-------/* �}�N���錾 */-------//
/* �萔�錾 */
#define ELEM 8               //�z��̗v�f���i����̐��j
#define CH_SIZE 32           //�z��̗v�f���i���햼�j
#define NAME_LEN 41          //�z��̗v�f���i���O�j
#define MIN_LEN 10           //���O�̍Œᕶ����
#define RAND_NUM 100         //�Z�o�萔

/* �ȈՏo�͊֐��錾 */
#define P_WAIT printf("�����{�^�����N���b�N..."); getchar()
#define P_FINISH puts("�Q�[�����I�����܂��B"); getchar()


//-------/* �\���̐錾 */-------//

/* �v���C���[�X�e�[�^�X */
typedef struct {
    char NAME[NAME_LEN];     //���O
    int HP;                 //�̗�
    int ATTACK;             //�U����
    int DEFENCE;            //�h���
    int LUCK;               //�^�̗ǂ�
    char WPN_NAME[CH_SIZE]; //����̖��O
    int WPN_POW;           //����̈З�
} STATUS;

/* ����̃X�e�[�^�X */
typedef struct {
    char Wpn_Name[ELEM][CH_SIZE];
    int Wpn_Pow[ELEM];
} WEAPON_SPEC;


//-------/* �v���g�^�C�v�錾 */-------//
int Name_Entry(STATUS *PLAY, int Turn);                            //���O����
void Output(STATUS *PLAY, int Turn);                               //�X�e�[�^�X�̏o��
void Battle(STATUS *PLAY_A, STATUS *PLAY_B, int Turn_A, int Turn_B); //�o�g���Ǘ�
void Retry(void);                                                  //���g���C�̊m�F


//-------/* �O���[�o���錾 */-------//

/* ���탊�X�g */
char Weapon_List[ELEM][CH_SIZE] = {
        {"AWM"},
        {"���{��"},
        {"�`�F�[���\�["},
        {"�Γ�e"},
        {"�o�[��"},
        {"RPG_7"},
        {"�_�C�i�}�C�g"},
        {"���C�g�Z�C�o�["}
};

/* ���L�J�E���g�ϐ� */
int Cnt;


//-------/* �J��Ԃ��g�p����v�Z�֐��ꗗ */-------//

/* ����X�e�[�^�X����U�� */
void Weapon_Data(WEAPON_SPEC *data) {
    for (size_t i = 0; i < ELEM; i++) {
        strcpy(data->Wpn_Name[i], Weapon_List[i]);  //���O���
        data->Wpn_Pow[i] = (i * 5 + 1);                  //�З͎Z�o
    }
    return;
}

/* �_���[�W�Z�o */
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

/* �X�e�[�^�X�̐U�蕪�� */
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

    //�U����
    calc_1 = (int) (PLAY->NAME[1] & 0x0F);
    calc_2 = (int) (PLAY->NAME[1] & 0xF0) >> 4;
    calc_3 = (int) (PLAY->NAME[length - 2] & 0xF0) >> 4;
    PLAY->ATTACK = abs(calc_1 + 1) * abs(calc_2 + 1) + calc_3;

    //�h���
    calc_1 = (int) (PLAY->NAME[length - 1] & 0x0F);
    calc_2 = (int) (PLAY->NAME[length - 1] & 0xF0) >> 4;
    PLAY->DEFENCE = abs(calc_1 + 1) * abs(calc_2 + 1);

    //�^�̗ǂ�
    calc_1 = (int) (PLAY->NAME[length - 2] & 0x0F);
    PLAY->LUCK = abs(calc_1 + 1) + abs(calc_3 + 1);

    //����
    calc_1 = (int) (PLAY->NAME[0] & 0x07);
    strcpy(PLAY->WPN_NAME, data.Wpn_Name[calc_1]);
    PLAY->WPN_POW = data.Wpn_Pow[calc_1];

    return;
}


/* -------------------------------------------------------------------------------------------------------- */

//-------/* ���C���v���O���� */-------//

int main(void) {
    STATUS PLAY_1, PLAY_2;
    int turn = 0;

    while (1) {
        /* ���O���� */
        turn = 1;
        turn = Name_Entry(&PLAY_1, turn);
        turn = Name_Entry(&PLAY_2, turn);

        /* �X�e�[�^�X�U�蕪�� */
        Dispatching(&PLAY_1);
        Dispatching(&PLAY_2);

        while (1) {
            /* PLAYER_1�̃^�[�� */
            Battle(&PLAY_1, &PLAY_2, 1, 2);
            if (Cnt == 0) {
                P_FINISH;
                return 0;
            }
            if (Cnt == 1) { break; }

            /* PLAYER_2�̃^�[�� */
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
        if (Cnt > 0) { puts("���������s�����Ă��܂��B�ē��͂��Ă��������B"); }
        printf("�v���C���[%d�̖��O���h�S�p�T�����ȏ�Q�O�����ȉ��h �h���p�P�O�����ȏ�S�O�����ȉ��h�œ��͂��Ă������� �B\n", Turn, Cnt++);
        fgets(PLAY->NAME, sizeof(PLAY->NAME), stdin);
        rewind(stdin);
    } while (strlen(PLAY->NAME) < MIN_LEN);

    return ++Turn;
}

void Output(STATUS *PLAY, int Turn) {
    printf("�v���C���[%d�̃X�^�[�^�X\n", Turn);
    printf("���O�F%s", PLAY->NAME);
    printf("HP�F%d\n", PLAY->HP);
    printf("�U���́F%d\n", PLAY->ATTACK);
    printf("�h��́F%d\n", PLAY->DEFENCE);
    printf("�^�̗ǂ��F%d\n", PLAY->LUCK);
    printf("����[�З�]�F%s[%d]\n\n", PLAY->WPN_NAME, PLAY->WPN_POW);

    return;
}

void Battle(STATUS *PLAY_A, STATUS *PLAY_B, int Turn_A, int Turn_B) {
    int damage = 0;

    system("cls");
    Output(PLAY_A, Turn_A);
    Output(PLAY_B, Turn_B);
    printf("\n�@%s��%s�֍U���I�����Ă���%s�ōU���͂��A�b�v�I\n", PLAY_A->NAME, PLAY_B->NAME, PLAY_A->WPN_NAME);
    P_WAIT;
    system("cls");
    damage = Pow_Calc(PLAY_A, PLAY_B);
    Output(PLAY_A, Turn_A);
    Output(PLAY_B, Turn_B);
    if (Cnt > 0) { printf("��S�̈ꌂ�I"); }
    printf("%d�̃_���[�W�I\n", damage);
    P_WAIT;

    Cnt = RAND_NUM;
    if (PLAY_B->HP <= 0) {
        printf("\n�퓬�I���I%s��HP�������Ȃ����̂�%s�̏����ł��I\n", PLAY_B->NAME, PLAY_A->NAME);
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
        puts("������x�V�т܂����HY/N");
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
                puts("���̓~�X�ł��BY��N�œ��͂��Ă��������B");
        }
    }
}
