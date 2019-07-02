# arduino
## 준비물
Arduino, Arduino WIFI shield, SD Card, Arduino IDE, LED, 리드선, WiFi Router(SSID, Password 기억을 하고 있어야함)

## 사용법
1. Arduino와 Arduino WIFI shield 결합
<https://www.arduino.cc/en/Guide/ArduinoWiFiShield/>
윗 링크로 자세히 나와있습니다.

2. Arduino WIFI shield 펌웨어 업데이트
<https://www.arduino.cc/en/Hacking/WiFiShieldFirmwareUpgrading/></br>
제조 날짜에 따라 칩셋이 다를 수 있으니 위에 내용을 숙지 한 다음 업데이트 해 주세요.

3. 웹 서버 테스트
<https://www.arduino.cc/en/Guide/ArduinoWiFiShield#toc8/>

4. 코드 작성
git에 나와 있는 코드를 복사 해 주십시오.

5. SD Card 및 H/W 설계합니다.

## 결과
비밀번호가 맞으면 변수 GreenPiN이 활성화되면서 문이 열립니다.</br>
비밀번호가 틀리면 변수 redPin이 활성화되면서 문이 안 열립니다.</br>
비밀번호가 교체하면 변수 yellowPin 활성화되면서 비밀번호 교체가 됩니다.
