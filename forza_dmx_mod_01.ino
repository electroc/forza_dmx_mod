
#include <TeensyDMX.h>
#include <elapsedMillis.h>
#include <Flash.h>

#define KW_PIN 16
#define WW_PIN 17

#define FAN_PIN 5

namespace teensydmx = ::qindesign::teensydmx;

teensydmx::Receiver dmxRx{Serial1};

elapsedMillis timeOutElapsed;
elapsedMillis timeInElapsed;

unsigned int fan_time_in = 5000;
unsigned int fan_time_out = 5000;


unsigned int pwm_freq = 20000;
byte pwm_resolution = 12;
byte fan_threshold = 90;
unsigned int dmx_start_adr = 1;

uint8_t dmx_dimm=0;
uint8_t dmx_fade=0;

uint8_t fade_invert=0;
uint8_t dimm_invert=0;

int fade1=0;
int fade2=0;

boolean fan_on=true;


byte x=0;

FLASH_ARRAY(unsigned int, pwmTable_W,0,6,6,7,7,8,8,9,9,10,10,10,11,11,11,12,12,12,12,13,13,13,14,14,14,15,15,15,16,16,17,17,18,18,19,19,20,20,21,21,22,22,23,23,24,24,25,25,26,27,27,28,29,29,30,31,31,32,33,34,35,36,36,37,38,39,40,41,42,43,44,45,47,48,49,50,51,53,54,55,57,58,59,61,62,64,66,67,69,71,72,74,76,78,80,82,84,86,88,90,92,95,97,99,102,104,107,110,112,115,118,121,124,127,130,133,137,140,143,147,151,154,158,162,166,170,174,179,183,188,192,197,202,207,212,217,223,228,234,240,246,252,258,264,271,278,285,292,299,306,314,322,330,338,346,355,363,372,382,391,401,411,421,431,442,453,464,476,487,500,512,525,538,551,565,579,593,608,623,638,654,670,687,704,721,739,757,776,795,815,835,856,877,899,921,944,967,991,1015,1041,1066,1093,1120,1148,1176,1205,1235,1266,1297,1329,1362,1396,1430,1466,1502,1539,1577,1616,1656,1697,1739,1783,1827,1872,1918,1966,2014,2064,2115,2168,2222,2277,2333,2391,2450,2511,2573,2636,2702,2769,2837,2907,2979,3053,3129,3206,3286,3367,3451,3536,3624,3713,3805,3899,3996,4095);


void setup() {

  Serial.begin(9600);

  pinMode(FAN_PIN,OUTPUT);
  
  analogWriteResolution(pwm_resolution);
  analogWriteFrequency(KW_PIN, pwm_freq);
  analogWriteFrequency(WW_PIN, pwm_freq);

  dmxRx.begin();

  digitalWrite(FAN_PIN,HIGH);
}


void loop() {


  
  dmx_dimm = dmxRx.get(dmx_start_adr);
  dmx_fade = dmxRx.get(dmx_start_adr+1);

  dimm_invert = ~dmx_dimm;
  fade_invert = ~dmx_fade;
    
  fade1 = dmx_fade;
  
  if((fade1-dimm_invert)>0){
    fade1 = fade1-dimm_invert;
  }
  else fade1=0;
  
  fade2 = fade_invert;

  if((fade2-dimm_invert)>0){
    fade2 = fade2-dimm_invert;
  }
  else fade2=0;

  /*
  if( (fade1+fade2) > fan_threshold ) { Serial.println("fan_on"); fan_on;}
  else if( timeOutElapsed > fan_time_out ){
    !fan_on;
    timeOutElapsed=0;
  }
  */
  
  analogWrite(KW_PIN,fade1);
  analogWrite(WW_PIN,fade2);

  Serial.print("fan_on: ");
  Serial.print(fan_on);
  Serial.print(" - dmx_dimm: ");
  Serial.print(dmx_dimm);
  Serial.print(" - dmx_fade: ");
  Serial.print(dmx_fade);
  Serial.print(" - dmx_dim_invert: ");
  Serial.print(dimm_invert);
  Serial.print(" - fade1: ");
  Serial.print(fade1);
  Serial.print(" - fade2: ");
  Serial.println(fade2);
  delay(50);
  
}

/*


  
///////////SINGLE COLD WHITE //////////////
  digitalWrite(FAN_PIN,HIGH);
  
  for(int x=1;x<250;x++){
    analogWrite(KW_PIN,x);
    delay(10);
  }
  
  delay(500);
  digitalWrite(FAN_PIN,LOW);
  
  for(int x=250;x>0;x--){
    analogWrite(KW_PIN,x);
    delay(10);
  }
  
  delay(500);
  digitalWrite(FAN_PIN,HIGH);

///////////SINGLE WARM WHITE ////////////// 

  for(int x=1;x<250;x++){
    analogWrite(WW_PIN,x);
    delay(10);
  }
  
  delay(500);
  digitalWrite(FAN_PIN,LOW);
  
  for(int x=250;x>0;x--){
    analogWrite(WW_PIN,x);
    delay(10);
  }
  
  delay(500);
  digitalWrite(FAN_PIN,HIGH);
///////////MULTI WHITE ////////////// 

  for(int x=1;x<125;x++){
    analogWrite(WW_PIN,x);
    delay(20);
  }

  byte y=125;
  for(int x=1;x<125;x++){
    analogWrite(KW_PIN,x);
    analogWrite(WW_PIN,y);
    y--;
    delay(20);
  }
  digitalWrite(FAN_PIN,LOW);
  delay(500);

  y=1;
  for(int x=125;x>0;x--){
    analogWrite(KW_PIN,x);
    analogWrite(WW_PIN,y);
    y++;
    delay(20);
  }
  
  delay(500);  
  digitalWrite(FAN_PIN,HIGH);
  
  for(int x=125;x>0;x--){
    analogWrite(WW_PIN,x);
    delay(20);
  }
  
  delay(500);  
  
}

*/
