#include <SIBA.h>
#include <Servo.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN D6     // DHT11이 연결된 핀
#define DHTTYPE DHT11  // DHT타입을 DHT11로 정의한다

DHT_Unified dht(DHTPIN, DHTTYPE);
//대기 시간 변수
uint32_t delayMS;
Servo myservo;
int sensorPin = 0;
int motorPin = D5;
/*
* Environment information:
* development is 1
* production is 0
*/
#define ENV 1
SIBA siba;
//define your router information
const char* ssid = "IoT-hub"; //your SIBA hub ssid
const char* pwd = "raspberry"; //your SIBA hub password
//your device's key for authentication
const char* hw_auth_key = "658b53e0c0634d08bae9e22dc8d6691a";
//your device's name (this name will be bluetooth alias)
const char* dev_name = "home-window";
int window=0;
int auto_mode=1;
long prev=millis();
byte open_lock=0;
byte close_lock=0;
void window_open(){
  if(!open_lock){
    open_lock=1;
    if(!window){
      window=1;
      for (int angle = 15; angle < 130; angle++)
      {
          myservo.write(angle);    //angle(각도)값으로 서보모터 제어
          delay(10);                    //delay로 각도의 변화 속도를 조절
      }
      siba.set_state("window",window,1);
    }
    open_lock=0;
  }
}
void window_close(){
  if(!close_lock){
    close_lock=1;
    if(window){
      window=0;
      for (int angle = 130; angle > 15; angle--)
      {
          myservo.write(angle);    //angle(각도)값으로 서보모터 제어
          delay(10);                    //delay로 각도의 변화 속도를 조절
      }
      siba.set_state("window", window,0);
    }
    close_lock=0;
  }
}
/*
* box id: 0, 
* button index: 0
* button event code: 0
*/
size_t action_0(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    window_open();
    return result;
}
/*
* box id: 0, 
* button index: 1
* button event code: 1
*/
size_t action_1(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    window_close();
    return result;
}
/*
* box id: 0, 
* button index: 2
* button event code: 2
*/
size_t action_2(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    
    return result;
}
/*
* box id: 1, 
* button index: 0
* button event code: 3
*/
//자동
size_t action_3(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    if(!auto_mode){
      auto_mode=1;
      siba.set_state("auto", auto_mode,0);
    }
    return result;
}
/*
* box id: 1, 
* button index: 1
* button event code: 4
*/
//수동
size_t action_4(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    if(auto_mode){
      auto_mode=0;
      siba.set_state("auto", auto_mode,0);
    }
    return result;
}
void add_ctrl_cmd_group() {
       
    siba.add_event(0, action_0);   
    siba.add_event(1, action_1);   
    siba.add_event(2, action_2);   
    siba.add_event(3, action_3);   
    siba.add_event(4, action_4);
}
void add_sensing_group() {
    
}
void init_device_state() {
    /* define device state model
    * example) siba.init_state("key", value);
    * ----------------------------------------------
    */
    siba.init_state("window", window,1);
    siba.init_state("auto", auto_mode,1);
    siba.init_state("temp", getTemperature(0),1);
}
void setup() {
    Serial.begin(115200); //board's baud rate
    /* put your other setup code here 
    * ----------------------------------------------
    */
    myservo.attach(motorPin);
    dht.begin();
    sensor_t sensor;
    dht.humidity().getSensor(&sensor);
    //최소한의 대기시간을 얻어 온다.
    delayMS = sensor.min_delay / 1000;  
    if(delayMS<5000) delayMS = 5000;
    myservo.write(15);
    delay(2000);
    /* ---------------------------------------------*/
    add_ctrl_cmd_group(); //add all control command
    add_sensing_group(); //add all sensing event
    siba.init_regist(init_device_state);
    //connect SIBA IoT platform
    #if ENV
    siba.init(ssid, pwd, hw_auth_key);
    #else
    siba.init(hw_auth_key, dev_name);
    #endif
}
void loop() {
    //keep alive your device and SIBA platform
    siba.verify_connection();
    if(millis()-prev>delayMS){
      getTemperature(1);
      prev=millis();
    }
}
int getTemperature(){
  int temp = 0;
  
  sensors_event_t event;  
  //온도값 얻기
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    //얻기 실패했다면...
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature: ");
    //실제 온도 값
    Serial.print(event.temperature);
    Serial.println(" *C");
    temp = (int)event.temperature;
    siba.set_state("temp",temp,1)
  }

  if(flag && auto_mode){
    if(event.temperature > 26){ //열기
      window_open();
    } 
    else{ //닫기
      window_close();
    }
  }

  return temp;
}
