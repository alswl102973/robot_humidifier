// 3개의 초음파 센서 핀 정의
// 초음파 센서 1
#define TRIG1 2
#define ECHO1 3

// 초음파 센서 2
#define TRIG2 4
#define ECHO2 5

// 초음파 센서 3
#define TRIG3 6
#define ECHO3 7

// 물체 감지 거리 설정 (cm)
#define DETECTION_DISTANCE 30

// 센서 상태를 저장하는 변수
bool sensor1_detected = false;
bool sensor2_detected = false;
bool sensor3_detected = false;

// 디바운싱을 위한 변수
unsigned long lastDetectionTime1 = 0;
unsigned long lastDetectionTime2 = 0;
unsigned long lastDetectionTime3 = 0;
const unsigned long DEBOUNCE_DELAY = 500; // 0.5초 딜레이

void setup() {
  // 시리얼 통신 초기화
  Serial.begin(9600);
  
  // 초음파 센서 1 핀 설정
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  
  // 초음파 센서 2 핀 설정
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
  
  // 초음파 센서 3 핀 설정
  pinMode(TRIG3, OUTPUT);
  pinMode(ECHO3, INPUT);
  
  Serial.println("3개의 초음파 센서 시스템이 시작되었습니다.");
  Serial.println("감지 거리: " + String(DETECTION_DISTANCE) + "cm");
}

void loop() {
  // 현재 시간 가져오기
  unsigned long currentTime = millis();
  
  // 센서 1 - 거리 측정
  float distance1 = measureDistance(TRIG1, ECHO1);
  
  // 센서 1 - 물체 감지 확인
  if (distance1 > 0 && distance1 <= DETECTION_DISTANCE) {
    // 물체가 감지되고 마지막 감지로부터 일정 시간이 지났을 때
    if (!sensor1_detected || (currentTime - lastDetectionTime1 > DEBOUNCE_DELAY)) {
      Serial.println("초음파 1 작동 - 물체 감지 거리: " + String(distance1) + "cm");
      sensor1_detected = true;
      lastDetectionTime1 = currentTime;
    }
  } else {
    sensor1_detected = false;
  }
  
  // 센서 2 - 거리 측정
  float distance2 = measureDistance(TRIG2, ECHO2);
  
  // 센서 2 - 물체 감지 확인
  if (distance2 > 0 && distance2 <= DETECTION_DISTANCE) {
    // 물체가 감지되고 마지막 감지로부터 일정 시간이 지났을 때
    if (!sensor2_detected || (currentTime - lastDetectionTime2 > DEBOUNCE_DELAY)) {
      Serial.println("초음파 2 작동 - 물체 감지 거리: " + String(distance2) + "cm");
      sensor2_detected = true;
      lastDetectionTime2 = currentTime;
    }
  } else {
    sensor2_detected = false;
  }
  
  // 센서 3 - 거리 측정
  float distance3 = measureDistance(TRIG3, ECHO3);
  
  // 센서 3 - 물체 감지 확인
  if (distance3 > 0 && distance3 <= DETECTION_DISTANCE) {
    // 물체가 감지되고 마지막 감지로부터 일정 시간이 지났을 때
    if (!sensor3_detected || (currentTime - lastDetectionTime3 > DEBOUNCE_DELAY)) {
      Serial.println("초음파 3 작동 - 물체 감지 거리: " + String(distance3) + "cm");
      sensor3_detected = true;
      lastDetectionTime3 = currentTime;
    }
  } else {
    sensor3_detected = false;
  }
  
  // 측정 간격을 위한 짧은 대기
  delay(100);
}

// 초음파 센서로 거리를 측정하는 함수
float measureDistance(int trigPin, int echoPin) {
  // 트리거 핀 초기화
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // 트리거 핀에 10μs 동안 HIGH 신호 보내기
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // 에코 핀으로 신호가 돌아오는 시간 측정
  unsigned long duration = pulseIn(echoPin, HIGH, 30000); // 30ms 타임아웃
  
  // 시간을 거리(cm)로 변환
  // 음속은 약 340m/s 또는 0.034cm/μs
  // 왕복 거리이므로 2로 나눔
  float distance = (duration * 0.034) / 2;
  
  return distance;
}