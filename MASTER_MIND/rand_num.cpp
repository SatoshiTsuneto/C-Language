#include<time.h>
#include<stdlib.h>

#define MAXRAND 10  //�������闐���̍ő�l
#define ELEM 4             //�z��̗v�f��

int rand_num(int num_Q[ELEM]) {
    int array[MAXRAND];  //�������̐��l������z��
    int remain;                     //�c�藐����␔
    int randget;                   //�������擾�p�ϐ�
    srand(time(NULL));  //�V�[�h�l������
    remain = MAXRAND;

    for (int i = 0; i < MAXRAND; i++) {                //���̔z��ɂO�`MAXRAND�|�P�̐��l����
        array[i] = i;
    }
    for (int i = 0; i < ELEM; i++) {
        randget = rand();                                            //�����̎擾
        randget %= remain;                                  //�擾�����������c��z��Ŋ������]����擾(���̏ꍇ�O�`MAXRAND-1�̒��̐��l�ƂȂ�)
        num_Q[i] = array[randget];                   //�z��array[]��randget�Ԗڂɓ����Ă��鐔����z��num_Q[]�ɑ��
        array[randget] = array[remain - 1];   //�����������𖢎g�p�̗����Œu��������(remain - 1�Ԗڂ̔z��͎�����͌��O)
        remain--;                                                      //������������炷
    }
    return 0;
}