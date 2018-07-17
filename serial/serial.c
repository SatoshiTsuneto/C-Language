//-----/* シリアル通信でデータの送受信をするプログラム */-----//
//----------/* ヘッダーファイル */----------//
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>

//----------/* グローバル変数など */----------//
#define DEV_NAME ("/dev/ttyUSB0")
#define BAUD_RATE (B9600)
#define BUFF_SIZE (16)

enum {  //各モードの状態定義
    PM_INIT, PM_OPEN, PM_MAIN
};

char Data[BUFF_SIZE];
int Fd = 0;  //シリアルOpenの結果を入れる
int Mode = 0;

//----------/* 関数群 */----------//
int serialOpen(void) {
    printf("start serial port read...\n");
    // デバイスファイル（シリアルポート）オープン
    Fd = open(DEV_NAME, O_RDWR | O_NONBLOCK);
    if (Fd < 0) { // デバイスオープンに失敗
        printf("ERROR on device open.\n\n");
        return -1;
    }
    //デバイスオープンに成功
    printf("success serial port open!\n\n");
    return 0;
}

void serialInit(void) {
    struct termios tio;
    memset(&tio, 0, sizeof(tio));
    tio.c_cflag = CS8 | CLOCAL | CREAD;
    tio.c_cc[VTIME] = 100;
    // ボーレートの設定
    cfsetispeed(&tio, BAUD_RATE);
    cfsetospeed(&tio, BAUD_RATE);
    // デバイスに設定を行う
    tcsetattr(Fd, TCSANOW, &tio);
}

void serialSend(void) {
    memset(Data, 0, BUFF_SIZE);
    printf("文字列を入力→ ");
    scanf("%s", Data);
    write(Fd, Data, BUFF_SIZE);
    printf("文字列”%s”を送信しました\n\n", Data);
}

int serialRecv(void) {
    memset(Data, 0, BUFF_SIZE);
    int rc = read(Fd, Data, BUFF_SIZE);
    if (rc == -1) { return rc; } // 受信失敗時の処理
    printf("%s", Data);
    return rc;
}

void progInit(void) {
    close(Fd);
    Mode = PM_OPEN;
}

void progOpen(void) {
    int res = serialOpen();  //シリアルポート読み込み
    sleep(1);
    // コネクションが途切れたら再コネクションを試みる
    if (res == 0) {
        printf("init serial port...\n");
        serialInit(); // シリアルポートの初期化
        sleep(1);
        Mode = PM_MAIN;
        printf("start main action.\n\n");
        return;
    }
}

void progMain(void) {
    int rc = serialRecv();
    if (rc == 0) {
        printf("connection not found.\n");
        sleep(1);
        Mode = PM_INIT;
        return;
    }
//    serialSend();
}

int main(void) {
    // メインループ
    while (Mode > -1) {
        switch (Mode) {
            case PM_INIT:
                progInit();
                break;

            case PM_OPEN:
                progOpen();
                break;

            case PM_MAIN:
                progMain();
                break;

            default:
                break;
        }
    }
}
