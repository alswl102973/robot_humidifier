#define left_forward 3     // 왼쪽모터 전진
#define left_backward 5    // 왼쪽모터 후진
#define right_forward 6    // 오른쪽모터 전진
#define right_backward 11  // 오른쪽모터 후진

const int motorSpeed = 200;  // 전진,후진 속도
const int turnSpeed = 150;   // 회전 속도

const unsigned long TIME_FOR_5CM = 500;    // 5cm 이동하는데 필요한 시간 (조정 필요)
const unsigned long TIME_FOR_90DEG = 800;  // 90도 회전하는데 필요한 시간 (조정 필요)

char start;

void setup(){
    Serial.begin(9600);  //시리얼 통신 초기화
    pinMode(left_forward, OUTPUT);   //모터를 모두 출력으로 지정
    pinMode(left_backward, OUTPUT);
    pinMode(right_forward, OUTPUT);
    pinMode(right_backward, OUTPUT);
    
    stop();
    Serial.println("작동 준비완료");
    Serial.println("w(전진), s(후진), a(좌회전), d(우회전), x(정지) 중 고르시오.: ");
}

void loop(){
    if(Serial.available()>0){ // 시리얼 통신에서 읽을 데이터가 있다면 시리얼 통신을 읽겠다.
    start = Serial.read();
    
    switch(start){   // 각 이동의 명령어 동작실행
        case 'w':  // 전진
        forward();
        Serial.println("전진");
        break;
        
        case 's': // 후진
        backward();
        Serial.println("후진");
        break;
        
        case 'a': // 좌회전 
        left();
        Serial.println("좌회전");
        break;
        
        case 'd': // 우회전
        right();
        Serial.println("우회전");
        break;
        
        case 'x': // 정지
        stop();
        Serial.println("정지");
        break;
      }
    }
}

// 기본 동작 함수들
void forward(){
  // 왼쪽 모터 전진
  analogWrite(left_forward, motorSpeed);
  analogWrite(left_backward, 0);
  // 오른쪽 모터 전진
  analogWrite(right_forward, motorSpeed);
  analogWrite(right_backward, 0);

  delay(TIME_FOR_5CM);          // 5cm 이동에 필요한 시간만큼 대기
  stop();                       // 모터 정지
}

void backward() {
  // 왼쪽 모터 후진
  analogWrite(left_forward, 0);
  analogWrite(left_backward, motorSpeed);
  
  // 오른쪽 모터 후진
  analogWrite(right_forward, 0);
  analogWrite(right_backward, motorSpeed);

  delay(TIME_FOR_5CM);          // 5cm 이동에 필요한 시간만큼 대기
  stop();                       // 모터 정지
}

void left() {
  // 왼쪽 모터 정지
  analogWrite(left_forward, 0);
  analogWrite(left_backward, 0);

  // 오른쪽 모터 전진
  analogWrite(right_forward, turnSpeed);
  analogWrite(right_backward, 0);

  delay(TIME_FOR_90DEG);        // 90도 회전에 필요한 시간만큼 대기
  stop();                       // 모터 정지
}

void right() {
  // 왼쪽 모터 전진
  analogWrite(left_forward, turnSpeed);
  analogWrite(left_backward, 0);
  // 오른쪽 모터 정지
  analogWrite(right_forward, 0);
  analogWrite(right_backward, 0);

  delay(TIME_FOR_90DEG);        // 90도 회전에 필요한 시간만큼 대기
  stop();                       // 모터 정지
}

void stop() {
  // 모든 모터 정지
  analogWrite(left_forward, 0);
  analogWrite(left_backward, 0);
  analogWrite(right_forward, 0);
  analogWrite(right_backward, 0);
}



