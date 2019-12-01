#include <SIBA.h>
#define LIMIT 400
/*
* Environment information:
* development is 1
* production is 0
*/
#define ENV 1
const int fan = D5;
int auto_mode=1;
int fan_st=0;
long prev=millis();
SIBA siba;
//define your router information
const char* ssid = "IoT-hub"; //your SIBA hub ssid
const char* pwd = "raspberry"; //your SIBA hub password
//your device's key for authentication
const char* hw_auth_key = "d41cf712a8964cecba72d7f0c23a4b61";
//your device's name (this name will be bluetooth alias)
const char* dev_name = "home-fan";
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
//fan on
size_t action_3(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    if(!fan_st){
      fan_st=1;
      digitalWrite(fan,HIGH);
      siba.set_state("fan",fan_st,1);
    }
    return result;
}
/*
* box id: 1, 
* button index: 1
* button event code: 4
*/
//fan off
size_t action_4(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    if(fan_st){
      fan_st=0;
      digitalWrite(fan,LOW);
      siba.set_state("fan",fan_st,1);
    }
    return result;
}
/*
* box id: 6, 
* button index: 0
* button event code: 5
*/
size_t action_5(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    if(auto_mode){
      auto_mode=0;
      siba.set_state("auto",auto_mode,1);
    }
    return result;
}
/*
* box id: 6, 
* button index: 1
* button event code: 6
*/
size_t action_6(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    if(!auto_mode){
      auto_mode=1;
      siba.set_state("auto",auto_mode,1);
    }
    return result;
}
void add_ctrl_cmd_group() {
    siba.add_event(0, action_0);   
    siba.add_event(1, action_1);   
    siba.add_event(2, action_2);   
    siba.add_event(3, action_3);   
    siba.add_event(4, action_4);   
    siba.add_event(5, action_5);   
    siba.add_event(6, action_6);
}
void add_sensing_group() {
}
void init_device_state() {
    /* define device state model
    * example) siba.init_state("key", value);
    * ----------------------------------------------
    */
    siba.init_state("auto",auto_mode,1);
    //siba.init_state("gas",0,1);
    siba.init_state("gas_st",0,1);
    siba.init_state("fan",fan_st,1);
}
void setup() {
    Serial.begin(115200); //board's baud rate
    /* put your other setup code here 
    * ----------------------------------------------
    */
    pinMode(fan, OUTPUT);
    digitalWrite(fan,LOW);
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
  if(auto_mode && millis()-prev>4000){
    int val=0;
    val=analogRead(0);//Read Gas value from analog 0
    //siba.set_state("gas",val,1);
    siba.set_state("gas_st",val,1);
    Serial.println(val);
    if(val >=LIMIT)
    {
      if(!fan_st){
        fan_st=1;
        digitalWrite(fan,HIGH);
        siba.set_state("fan",fan_st,1);
      }
    }
    else{ //off
      if(fan_st){
        fan_st=0;
        digitalWrite(fan,LOW);
        siba.set_state("fan",fan_st,1);
      }
    }
    prev=millis();
  }
}
