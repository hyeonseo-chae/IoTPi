#include <stdio.h>
#include <stdlib.h>

int main(void) {
    printf("Content-type: text/html\n\n");  // HTTP 헤더 출력
    printf("<html><body>\n");
    printf("<h1>촬영을 시작합니다...</h1>\n");

    // 사진 촬영 명령어 실행
    system("sudo v4l2-ctl --device=/dev/video0 --set-fmt-video=width=1920,height=1080,pixelformat=MJPG --stream-mmap --stream-count=1 --stream-to=/var/www/html/capture.jpg");

    printf("<img src=\"/capture.jpg\" alt=\"촬영된 이미지\" style=\"margin-top: 20px;\"/>\n");
    printf("</body></html>\n");

    return 0;
}
