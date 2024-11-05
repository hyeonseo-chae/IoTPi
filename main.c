#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define BTN_PIN 3

/* 동시에 사용을 1회 */
static int being_captured = 0;

// 버튼을 눌렀을 때 발생시 호출되는 함수
void buttonPressed(void) {
    if (being_captured != 0) { return; }

    being_captured = 1;

    printf("촬영을 시작합니다...\n");
    system("sudo v4l2-ctl --device=/dev/video0 --set-fmt-video=width=1920,height=1080,pixelformat=MJPG --stream-mmap --stream-count=1 --stream-to=capture.jpg");

    being_captured = 0;
}

int main(void) {
    wiringPiSetup();
    pinMode(BTN_PIN, INPUT);
    wiringPiISR(BTN_PIN, INT_EDGE_RISING, buttonPressed);

    while(1) {
        delay(100);
    }

    return 0;
}