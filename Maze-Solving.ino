#define SOLHIZ  6
#define SOLM  7

#define SAGHIZ  5                         
#define SAGM  4  

#define ILERI HIGH
#define GERI LOW

void ilerigit();

struct Konum{
int  x;
int  y; 
bool sagduvar;
bool solduvar;
bool onduvar;
bool arkaduvar;
int  referans;
};

int sensorpin =0;                 // analog pin used to connect the sharp sensor
int onMesafe = 0;                 // variable to store the values from sensor(initially zero)


void setup() 
{ 
    Serial.begin(9600);               // starts the serial monitor
    pinMode(YON1, OUTPUT);   
    pinMode(YON2, OUTPUT); 
} 

void loop() 
{ 
  int value;
  int yon;
  
  onMesafe = analogRead(sensorpin);
  int ref = 350;
 

  int P = 2;
  int hata = ref - onMesafe;
  int cosku = P*hata;

  if(cosku <0){
    yon = GERI;
    cosku = -cosku;
  }
  else{
    yon = ILERI;
  }

  if(cosku > 255) cosku = 255;
  
  digitalWrite(SOLM, yon);
  digitalWrite(SAGM, yon);

  analogWrite(SOLHIZ, cosku);
  analogWrite(SAGHIZ, cosku);
  

 // sensor read 
 // filtreleme
  Serial.println(onMesafe);            // prints the value of the sensor to the serial monitor
  delay(400);                    // wait for this much time before printing next value  
}
/* */
void ilerigit(){

}
