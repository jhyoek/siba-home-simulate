#include <SIBA.h>
#include <Servo.h>    //서보모터 라이브러리 include
/*
* Environment information:
* development is 1
* production is 0
*/
#define ENV 1
#define DOOR_CLOSE 100
#define DOOR_OPEN 5
SIBA siba;
//define your router information
const char* ssid = "IoT-hub"; //your SIBA hub ssid
const char* pwd = "raspberry"; //your SIBA hub password
//your device's key for authentication
const char* hw_auth_key = "7ae00928f83646b4bf87e5cc52febc66";
//your device's name (this name will be bluetooth alias)
const char* dev_name = "home-door";
Servo microServo;    //서보모터 객체 선언
const int servoPin = D6;    //서보모터 제어핀 할당
int door_state=1;
/*
* box id: 0, 
* button index: 0
* button event code: 0
*/
size_t action_0(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    //문열기
    if(!door_state){
      door_state=1;
      siba.set_state("door",door_state,1);
      for (int angle = 5; angle < 100; angle++)
      {
        microServo.write(angle);    //angle(각도)값으로 서보모터 제어
        delay(10);                    //delay로 각도의 변화 속도를 조절
      } 
    }
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
    if(door_state){
      door_state=0;
      siba.set_state("door",door_state,1);
      for (int angle = 100; angle > 5; angle--)
      {
          microServo.write(angle);    //angle(각도)값으로 서보모터 제어
          delay(10);                    //delay로 각도의 변화 속도를 조절
      }
    }
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
* box id: 10, 
* button index: 0
* button event code: 3
*/
size_t action_3(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    char arr[11];
    data[0].value.toCharArray(arr, data[0].value.length()+1);
    Serial.println(arr);
    siba.set_state("password", arr, 1);
    return result;
}
void add_ctrl_cmd_group() {
    siba.add_event(0, action_0);   
    siba.add_event(1, action_1);   
    siba.add_event(2, action_2);   
    siba.add_event(3, action_3);
}
void add_sensing_group() {
}
void init_device_state() {
    /* define device state model
    * example) siba.init_state("key", value);
    * ----------------------------------------------
    */
    siba.init_state("door",door_state,1);
    siba.init_state("password","123456",1);
}
void setup() {
    Serial.begin(115200); //board's baud rate
    /* put your other setup code here 
    * ----------------------------------------------
    */
    microServo.attach(servoPin);    //서보모터 초기화
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
}
