#include <SIBA.h>
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
const char* hw_auth_key = "191c1d85482e468bb65865aa590950b9";
//your device's name (this name will be bluetooth alias)
const char* dev_name = "home-garage";
int trig = D6;
int echo = D5;
int piezoPin = D7;
int once = 0;
long prev= millis();
long sensing_prev= millis();
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
* box id: 4, 
* button index: 0
* button event code: 3
*/
size_t action_3(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    
    return result;
}
/*
* box id: 5, 
* button index: 0
* button event code: 5
*/
size_t action_5(size_t before, sb_dataset data[2], size_t data_length) {
    size_t result = 1;
    //define logic code in here
    
    return result;
}
void add_ctrl_cmd_group() {
       
    siba.add_event(1, action_1);   
    siba.add_event(3, action_3);   
    siba.add_event(5, action_5);
}
void add_sensing_group() {
}
void init_device_state() {
    /* define device state model
    * example) siba.init_state("key", value);
    * ----------------------------------------------
    */
    siba.init_state("car", 0,1);
    siba.init_state("auto", 1,1);
    //siba.init_state("dist", 0,1);
}
void setup() {
    Serial.begin(115200); //board's baud rate
    /* put your other setup code here 
    * ----------------------------------------------
    */
    //초음파 송신부-> OUTPUT, 초음파 수신부 -> INPUT
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    // 부저
    pinMode(piezoPin, OUTPUT);
    
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
    digitalWrite(trig, LOW);
    digitalWrite(echo, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    unsigned long duration = pulseIn(echo, HIGH);
    float distance = duration / 29.0 / 2.0;
    if(millis()-sensing_prev>3000){
      if(distance < 28)
      {
        // args(pinNum, note, duration)
        if(!once){
          once=1;
          tone(piezoPin, 262, 100);
          siba.set_state("car", 1,1);
        Serial.println("yes");
        }
      }
      else{
        if(once){
          once=0;
          siba.set_state("car", 0,1);
        }
        Serial.println("no");
      }
      Serial.println(distance);
      sensing_prev=millis();
    }
    if(millis()-prev>5000){
      int dist = (int)(distance/100);
      //siba.set_state("dist", dist,1);
      prev=millis();
    }
}
