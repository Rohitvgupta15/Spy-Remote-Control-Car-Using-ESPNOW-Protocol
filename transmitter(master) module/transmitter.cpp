#include <esp_now.h>
#include <WiFi.h>

int x_motor_key = 32;
int y_motor_key = 33;
int x_servo_key = 34;
int y_servo_key = 35;
int switch_sw   = 26;

uint8_t broadcastAddress1[] = {0x24, 0xA1, 0x60, 0x2D, 0xCA, 0x6D};

String success;

typedef struct struct_message {

    int a;
    int b;
    int c;
    int d;
    int e;
} struct_message;
struct_message datatosend;

esp_now_peer_info_t peerInfo;
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Delivery Status of: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? " Send Successfully" : " Fail to send");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}
 
void setup() {
  Serial.begin(9600);
 pinMode ( x_motor_key, INPUT) ;
 pinMode ( y_motor_key, INPUT) ;
 pinMode (x_servo_key, INPUT) ;
 pinMode (y_servo_key, INPUT) ;
 pinMode (switch_sw, INPUT);
  WiFi.mode(WIFI_STA);
             
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
   peerInfo.channel = 0; 
  peerInfo.encrypt = false;
   
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

}
 
void loop() {
  struct_message datatosend;
int x_motor_pos = analogRead ( x_motor_key) ; 
int y_motor_pos = analogRead ( y_motor_key) ; 

int x_servo_pos = analogRead ( x_servo_key) ; 
int y_servo_pos = analogRead ( y_servo_key) ; 
int sw          = digitalRead( switch_sw  ) ;
   datatosend.a =  map(x_motor_pos, 0, 4095, 0, 1993); 
   datatosend.b =  map(y_motor_pos, 0, 4095, 0, 1993); 
   datatosend.c =  map(x_servo_pos, 0, 4095, 0, 1993); 
   datatosend.d =  map(y_servo_pos, 0, 4095, 0, 1993); 
   datatosend.e =  sw;
 
  Serial.print("X_motor: ");
  Serial.print(datatosend.a);
  Serial.print(" | Y_motor: ");
  Serial.print(datatosend.b);
 
  Serial.print("   X_servo: ");
  Serial.print(datatosend.c);
  Serial.print(" | Y_servo : ");
  Serial.print(datatosend.d);
  
  Serial.print(" | switch: ");
  Serial.println(datatosend.e);


   delay(10);
    
  esp_err_t result = esp_now_send(0, (uint8_t *) &datatosend, sizeof(struct_message));
   
  if (result == ESP_OK) {
    Serial.println("Sent Successfullt");
  }
  else {
    Serial.println("Getiing Error while sending the data");
}

}
