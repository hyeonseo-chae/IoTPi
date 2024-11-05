from flask import Flask, render_template_string
import os

app = Flask(__name__)

# HTML 코드 정의
html_code = '''
<!doctype html>
<html lang="ko">
  <head>
    <meta charset="utf-8">
    <title>Raspberry Pi Camera Test</title>
  </head>
  <body>
    <h1>카메라 촬영하기</h1>
    <form action="/capture" method="post">
      <button type="submit">촬영 시작</button>
    </form>
    <img src="/static/capture.jpg" alt="촬영된 이미지" style="margin-top: 20px;"/>
  </body>
</html>
'''

@app.route('/')
def index():
    return render_template_string(html_code)

@app.route('/capture', methods=['POST'])
def capture():
    # v4l2-ctl 명령어로 촬영
    os.system("sudo v4l2-ctl --device=/dev/video0 --set-fmt-video=width=1920,height=1080,pixelformat=MJPG --stream-mmap --stream-count=1 --stream-to=static/capture.jpg")
    return render_template_string(html_code)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
