#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Servo.h>
  //  dc motor
int EN_A = 12;      //Enable pin for first motor
int IN1  = 14;       //control pin for first motor
int IN2  = 2; 
int EN_B = 13;      //Enable pin for first motor
int IN3  = 15;       //control pin for first motor
int IN4  = 3;

int motor_speed1;
int motor_speed2;

 /// servo  

  Servo servoA;
  Servo servoB;
int positionA = 90;
int positionB = 90;


String success;

typedef struct struct_message {
    int a;
    int b;
    int c;
    int d;
    int e;
} struct_message;

struct_message datatosend;

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&datatosend, incomingData, sizeof(datatosend));
int x_motor = datatosend.a;
int y_motor = datatosend.b;
int x_servo = datatosend.c;
int y_servo = datatosend.d;
int sw = datatosend.e;
/// dc forward and backward
if (x_motor < 700 ) {   // forward
  motor_speed1 = map(x_motor, 0, 699, 255, 0);  // Map the values to reverse motor speed
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(EN_A, motor_speed1);
  analogWrite(EN_B, motor_speed1);
} else if (x_motor >= 700 && x_motor <= 1300) {  // Motors will not move when the joystick is at the center
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
} else if (x_motor > 1301) {   // backward
  motor_speed1 = map(x_motor, 1301, 1993, 0, 255);  // Map the values to forward motor speed
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(EN_A, motor_speed1);
  analogWrite(EN_B, motor_speed1);
}

if (y_motor < 700  ) {   // Rotating the left motor in clockwise direction
  motor_speed2 = map(y_motor, 0, 699, 255, 0);  // Map the values to reverse motor speed
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
   digitalWrite(IN4, HIGH);
  analogWrite(EN_A, motor_speed2);
  analogWrite(EN_B, motor_speed2);

} else if (y_motor > 1301) {   // Rotating the left motor in anticlockwise direction
  motor_speed2 = map(y_motor, 1301, 1993, 0, 255);  // Map the values to forward motor speed
 digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(EN_A, motor_speed2);
  analogWrite(EN_B, motor_speed2);
}


// servo


if (x_servo == 0 && positionA < 180) {
    positionA = positionA + 2;
    servoA.write(positionA);
    digitalWrite(26,HIGH);
    delay(3);
  }


if (x_servo == 1993 && positionA > 0) {
    positionA = positionA - 2;
    servoA.write(positionA);
    delay(3);
  }


if (y_servo == 0 && positionB < 180) {
     positionB = positionB + 2;
    servoB.write(positionB);
    delay(3);
  }


if (y_servo == 1993 && positionB > 0) {
   positionB = positionB - 2;
    servoB.write(positionB);
    delay(3);
  }

if ( sw ==0) {
  positionA = 110;
  positionB = 80;
    servoA.write(positionA);
    servoB.write(positionB);
    delay(3);
  } 

 Serial.print("X_motor: ");
  Serial.print(x_motor);
  Serial.print(" | Y_motor: ");
  Serial.print(y_motor);
 
  Serial.print("   X_servo: ");
  Serial.print(x_servo);
  Serial.print(" | Y_servo : ");
  Serial.print(y_servo);
 Serial.print(" | sw : ");
  Serial.println(sw);
  
}
 
void setup() {
  Serial.begin(9600);
  // dc motor
  pinMode(EN_A, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN_B, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  //servo
  servoA.attach(16,500,2400);
  servoB.attach(4,500,2400);  
  servoA.write(positionA); 
  servoB.write(positionB);
  WiFi.mode(WIFI_STA); 
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}
