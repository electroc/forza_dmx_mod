
#include <TeensyDMX.h>
#include <elapsedMillis.h>
#include <Flash.h>

#define KW_PIN 17
#define WW_PIN 16

#define FAN_PIN 5

namespace teensydmx = ::qindesign::teensydmx;

teensydmx::Receiver dmxRx{Serial1};

elapsedMillis timeOutElapsed;
elapsedMillis timeInElapsed;

unsigned int fan_time_in   =  3000;
unsigned int fan_time_out  =  3000;


unsigned int pwm_freq      =  20000;
byte pwm_resolution        =     12;
byte fan_threshold         =     80;
unsigned int dmx_start_adr =     1;

uint8_t dmx_dimm           =     0;
uint8_t dmx_fade           =     0;

uint8_t last_dmx_dimm           =     0;
uint8_t last_dmx_fade           =     0;


uint8_t fade_invert        =     0;
uint8_t dimm_invert        =     0;

int fade1                  =     0;
int fade2                  =     0;
float quotient;

unsigned int fadex         =     0;
unsigned int fadey         =     0;

unsigned int fadex_new         =     0;
unsigned int fadey_new         =     0;


bool fan_on             = false;
bool fan_off            = false;
bool fan_is_on          = false;

uint8_t x                     =     0;

bool odd                = false;

FLASH_ARRAY(unsigned int, pwmTable,100,135,140,146,151,156,161,167,172,178,183,189,194,200,206,211,217,223,229,235,240,246,252,258,265,271,277,283,289,296,302,308,315,321,328,335,341,348,355,361,368,375,382,389,396,403,411,418,425,432,440,447,455,462,470,477,485,493,501,508,516,524,532,541,549,557,565,574,582,590,599,608,616,625,634,643,652,660,670,679,688,697,706,716,725,735,744,754,764,774,783,793,803,814,824,834,844,855,865,876,886,897,908,919,930,941,952,963,974,986,997,1009,1020,1032,1044,1055,1067,1079,1092,1104,1116,1129,1141,1154,1166,1179,1192,1205,1218,1231,1244,1258,1271,1285,1298,1312,1326,1340,1354,1368,1382,1397,1411,1426,1440,1455,1470,1485,1500,1515,1531,1546,1562,1577,1593,1609,1625,1641,1657,1674,1690,1707,1724,1740,1757,1775,1792,1809,1827,1844,1862,1880,1898,1916,1934,1953,1971,1990,2009,2028,2047,2066,2085,2105,2125,2144,2164,2185,2205,2225,2246,2266,2287,2308,2329,2351,2372,2394,2416,2438,2460,2482,2504,2527,2550,2573,2596,2619,2643,2666,2690,2714,2738,2762,2787,2812,2837,2862,2887,2912,2938,2964,2990,3016,3042,3069,3095,3122,3150,3177,3204,3232,3260,3288,3317,3345,3374,3403,3432,3462,3491,3521,3551,3582,3612,3643,3674,3705,3736,3768,3800,3832,3865,3897,3930,3963);


void setup() {

  Serial.begin(115200);

  pinMode(FAN_PIN,OUTPUT);
  
  analogWriteResolution(pwm_resolution);
  analogWriteFrequency(KW_PIN, pwm_freq);
  analogWriteFrequency(WW_PIN, pwm_freq);

  dmxRx.begin();

  digitalWrite(FAN_PIN,HIGH);
  delay(2000);
  digitalWrite(FAN_PIN,LOW);
}


void loop() {

  odd =! odd;

  dmx_dimm = dmxRx.get(dmx_start_adr);
  dmx_fade = dmxRx.get(dmx_start_adr+1);
 
  if(dmx_dimm != last_dmx_dimm || dmx_fade != last_dmx_fade ){
      
    last_dmx_dimm = dmx_dimm;
    last_dmx_fade = dmx_fade;
 
    //dimm_invert = ~dmx_dimm;
    fade_invert = ~dmx_fade;
    
    quotient = float(dmx_dimm) / 255;
    
    fade1 = dmx_fade * quotient;     // kaltweiß 
    fade2 = fade_invert * quotient;  // warmweiß 
  
    fadex = pwmTable[fade1];
    fadey = pwmTable[fade2];
    
     
    analogWrite(KW_PIN,fadex);
    analogWrite(WW_PIN,fadey);
 }

/*
  for(int x=4;x<100;x++)
    {
      analogWrite(KW_PIN,x);
      delay(1);
    }
  for(int x=100;x>=4;x--)
    {
      analogWrite(KW_PIN,x);
      delay(1);
    }  
*/  
 
  if(!fan_is_on){    
    if( (fade1+fade2) > fan_threshold && !fan_on) { 
      fan_on=true;
      timeInElapsed=0;
      //Serial.println("fan soll eingeschalten werden");
    }
    if( (fade1+fade2) < fan_threshold) fan_on=false;
    
    if(timeInElapsed > fan_time_in && fan_on){
      //Serial.println("fan ist ein");
      digitalWriteFast(FAN_PIN,HIGH);
      fan_on=false;
      fan_is_on=true;
    }
  }
  if(fan_is_on){
    if( (fade1+fade2) < fan_threshold && !fan_off){
      fan_off=true;
      timeOutElapsed=0;
      //Serial.println("fan soll ausgeschalten werden");
    }
    if( (fade1+fade2) > fan_threshold) fan_off=false;
    
    if(timeOutElapsed > fan_time_out && fan_off){
      //Serial.println("fan ist aus");
      digitalWriteFast(FAN_PIN,LOW);
      fan_off=false;
      fan_is_on=false;
    }
  }

} 
