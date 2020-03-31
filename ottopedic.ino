#include <ESP8266WiFi.h>                                                    // esp8266 library
#include <FirebaseArduino.h>       
#include "Wire.h"
#include "mpu6050rotatey.h"

#define FIREBASE_HOST "***********.firebaseio.com"                          // the project name address from firebase id
#define FIREBASE_AUTH "***************************************"            // the secret key generated from firebase

#define WIFI_SSID "**********"                                             // input your home or public wifi name 
#define WIFI_PASSWORD "**************"                                    //password of wifi ssid                                           
String x="";
int angle,no_of_times;
int leds=0;
bool suc=false;
double delta[3] = {0};
double rot[3] = {0};
double set_point_x = 0;
double set_point_y = 0;
bool success = false;

void read_mpu(){
  // takes time of ~1 sec to average 100 samples of reading
  // set the value of delta

  for(int i = 0; i < 3; i++) rot[i] = 0;
    
  int total_sample = 100;
  int sampling_interval = 500;  // in us
  
  for(int i=0; i<=100; i++){
    calcRotation(); // read from MPU
    rot[0] += angleX;
    rot[1] += angleY;
    rot[2] += angleZ;
    delayMicroseconds(sampling_interval);
  }

  for(int i = 0; i < 3; i++) rot[i] /= total_sample;

  //Serial.println("Delta values are: ");
  delta[0] = rot[0] - set_point_x;
  //Serial.println(delta[0]);
  delta[1] = rot[1] - set_point_y;
  //Serial.println(delta[0]);

  // store last position
  lastAngleX = rot[0];
  lastAngleY = rot[1];
  lastAngleZ = rot[2];
}

void setup() {
  Serial.begin(9600);
  delay(1000);                
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            
  read_mpu();
  set_point_x = rot[0];
  set_point_y = rot[1];
  Serial.println(set_point_x);
  Serial.println(set_point_y);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                              
  Firebase.set("LED_STATUS",0);
  if(leds==1)
  {
    angle=Firebase.getString("ANGLE").toInt();
    no_of_times=Firebase.getString("NO").toInt();
    suc=true;
    Serial.print("Angle");
    Serial.println(angle);
    Serial.print("No of times");
    Serial.println(no_of_times);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!suc)
  {
    while(leds==0)
    {
      leds=Firebase.getInt("LED_STATUS");
      Serial.print("Led status = ");
      Serial.println(leds);
    }
    angle=Firebase.getString("ANGLE").toInt();
    no_of_times=Firebase.getString("NO").toInt();
    Serial.print("Angle = ");
    Serial.println(angle);
    Serial.print("No of times = ");
    Serial.println(no_of_times);
    suc=true;
  }
}
