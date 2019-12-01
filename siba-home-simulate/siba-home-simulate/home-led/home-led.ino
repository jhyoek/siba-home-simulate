#include <SIBA.h>
#define ROOM1    D1 //현관
#define ROOM2    D5 //거실
#define ROOM3    D6 //안방
#define ROOM4    D7 //화장실
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
const char* hw_auth_key = "c5b92f3ba7d14877a19a0e6c6335c3ff";
//your device's name (this name will be bluetooth alias)
const char* dev_name = "home-LED";
int room1 = 1;
int room2 = 1;
int room3 = 1;
int room4 = 1;
/*
* box id: 0, 
* button index: 0
* button event code: 0
*/
size_t action_0(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
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
    return result;
}
/*
* box id: 1, 
* button index: 0
* button event code: 3
*/
size_t action_3(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    if(!room1){
      room1=1;
      siba.set_state("room1", room1,1);
      digitalWrite (ROOM1, LOW); // 릴레이 ON
    }
    return result;
}
/*
* box id: 1, 
* button index: 1
* button event code: 5
*/
size_t action_5(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    if(!room2){
      room2=1;
      siba.set_state("room2", room2,1);
      digitalWrite (ROOM2, LOW); // 릴레이 ON
    }
    return result;
}
/*
* box id: 1, 
* button index: 2
* button event code: 6
*/
size_t action_6(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    if(!room3){
      room3=1;
      siba.set_state("room3", room3,1);
      digitalWrite (ROOM3, LOW); // 릴레이 ON
    }
    return result;
}
/*
* box id: 1, 
* button index: 3
* button event code: 7
*/
size_t action_7(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    if(!room4){
      room4=1;
      siba.set_state("room4", room4,1);
      digitalWrite (ROOM4, LOW); // 릴레이 ON
    }
    return result;
}
/*
* box id: 1, 
* button index: 4
* button event code: 11
*/
size_t action_11(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    action_3(before,data,data_length);
    action_5(before,data,data_length);
    action_6(before,data,data_length);
    action_7(before,data,data_length);
    return result;
}
/*
* box id: 2, 
* button index: 0
* button event code: 4
*/
size_t action_4(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    if(room1){
      room1=0;
      siba.set_state("room1", room1,1);
      digitalWrite (ROOM1, HIGH);
    }
    return result;
}
/*
* box id: 2, 
* button index: 1
* button event code: 8
*/
size_t action_8(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    if(room2){
      room2=0;
      siba.set_state("room2", room2,1);
      digitalWrite (ROOM2, HIGH);
    }
    return result;
}
/*
* box id: 2, 
* button index: 2
* button event code: 9
*/
size_t action_9(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    if(room3){
      room3=0;
      siba.set_state("room3", room3,1);
      digitalWrite (ROOM3, HIGH);
    }
    return result;
}
/*
* box id: 2, 
* button index: 3
* button event code: 10
*/
size_t action_10(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    if(room4){
      room4=0;
      siba.set_state("room4", room4,1);
      digitalWrite (ROOM4, HIGH);
    }
    return result;
}
/*
* box id: 2, 
* button index: 4
* button event code: 12
*/
size_t action_12(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    action_4(before,data,data_length);
    action_8(before,data,data_length);
    action_9(before,data,data_length);
    action_10(before,data,data_length);
    return result;
}
void add_ctrl_cmd_group() {
    siba.add_event(0, action_0);   
    siba.add_event(1, action_1);   
    siba.add_event(3, action_3);   
    siba.add_event(5, action_5);   
    siba.add_event(6, action_6);   
    siba.add_event(7, action_7);   
    siba.add_event(11, action_11);   
    siba.add_event(4, action_4);   
    siba.add_event(8, action_8);   
    siba.add_event(9, action_9);   
    siba.add_event(10, action_10);   
    siba.add_event(12, action_12);
}
void add_sensing_group() {
}
void init_device_state() {
    /* define device state model
    * example) siba.init_state("key", value);
    * ----------------------------------------------
    */
    siba.init_state("room1", room1,1);
    siba.init_state("room2", room2,1);
    siba.init_state("room3", room3,1);
    siba.init_state("room4", room4,1);
}
void setup() {
    Serial.begin(115200); //board's baud rate
    /* put your other setup code here 
    * ----------------------------------------------
    */
    pinMode(ROOM1,OUTPUT);         // 릴레이 제어 1번핀을 출력으로 설정
    pinMode(ROOM2,OUTPUT);         // 릴레이 제어 2번핀을 출력으로 설정
    pinMode(ROOM3,OUTPUT);         // 릴레이 제어 3번핀을 출력으로 설정
    pinMode(ROOM4,OUTPUT);
    digitalWrite (ROOM1, LOW); // 릴레이 ON
    digitalWrite (ROOM2, LOW); // 릴레이 ON
    digitalWrite (ROOM3, LOW); // 릴레이 ON
    digitalWrite (ROOM4, LOW); // 릴레이 ON
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
