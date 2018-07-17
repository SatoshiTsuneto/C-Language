#include<time.h>
#include<stdlib.h>

#define MAXRAND 10  //生成する乱数の最大値
#define ELEM 4             //配列の要素数

int rand_num(int num_Q[ELEM]) {
    int array[MAXRAND];  //乱数候補の数値を入れる配列
    int remain;                     //残り乱数候補数
    int randget;                   //乱数候補取得用変数
    srand(time(NULL));  //シード値初期化
    remain = MAXRAND;

    for (int i = 0; i < MAXRAND; i++) {                //仮の配列に０～MAXRAND－１の数値を代入
        array[i] = i;
    }
    for (int i = 0; i < ELEM; i++) {
        randget = rand();                                            //乱数の取得
        randget %= remain;                                  //取得した乱数を残り配列で割った余りを取得(この場合０～MAXRAND-1の中の数値となる)
        num_Q[i] = array[randget];                   //配列array[]のrandget番目に入っている数字を配列num_Q[]に代入
        array[randget] = array[remain - 1];   //つかった乱数を未使用の乱数で置き換える(remain - 1番目の配列は次からは候補外)
        remain--;                                                      //乱数候補を一つ減らす
    }
    return 0;
}