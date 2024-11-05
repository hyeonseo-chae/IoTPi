import serial
import time
from flask import Flask, render_template_string, request
import os

app = Flask(__name__)

# Arduino와의 직렬 통신 설정
ser = serial.Serial('/dev/ttyACM1', 9600, timeout=1)

time.sleep(2)  # Arduino 초기화 시간을 위해 지연

# HTML 코드 정의
html_code = '''
<!doctype html>
<html lang="ko">
  <head>
    <meta charset="utf-8">
    <title>Raspberry Pi Camera and Arduino Control</title>
  </head>
  <body>
    <h1>카메라 촬영하기</h1>
    <form action="/capture" method="post">
      <button type="submit">촬영 시작</button>
    </form>
    {% if image %}
    <img src="{{ image }}" alt="촬영된 이미지" style="margin-top: 20px;"/>
    {% endif %}
    
    <h2>7세그먼트 숫자 입력</h2>
    <form action="/display_number" method="post">
      <input type="number" name="number" min="0" max="9">
      <button type="submit">숫자 표시</button>
    </form>
    
    <h2>LCD 텍스트 입력</h2>
    <form action="/display_text" method="post">
      <input type="text" name="text">
      <button type="submit">텍스트 표시</button>
    </form>
  </body>
</html>
'''

@app.route('/')
def index():
    return render_template_string(html_code, image=None)

@app.route('/capture', methods=['POST'])
def capture():
    # v4l2-ctl 명령어로 촬영
    os.system("sudo v4l2-ctl --device=/dev/video0 --set-fmt-video=width=1920,height=1080,pixelformat=MJPG --stream-mmap --stream-count=1 --stream-to=static/capture.jpg")
    
    # 촬영 후 Arduino에 버저 울리기 명령 전송
    ser.write(b'BUZZER\n')
    
    return render_template_string(html_code, image='/static/capture.jpg')

@app.route('/display_number', methods=['POST'])
def display_number():
    number = request.form['number']
    ser.write(f'NUMBER:{number}\n'.encode())
    return render_template_string(html_code, image='/static/capture.jpg')

@app.route('/display_text', methods=['POST'])
def display_text():
    text = request.form['text']
    ser.write(f'TEXT:{text}\n'.encode())
    return render_template_string(html_code, image='/static/capture.jpg')

if __name__ == '__main__':
    try:
        app.run(host='0.0.0.0', port=5000)
    finally:
        ser.close()
