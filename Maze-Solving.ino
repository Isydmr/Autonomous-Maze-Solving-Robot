// maze


#define PWM1  6
#define YON1  7
#define PWM2  5                         
#define YON2  4  
#define ILERI HIGH
#define GERI LOW
   
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

  onMesafe = analogRead(sensorpin);
  int ref = 350;
 

  int P = 2;
  int hata = ref - onMesafe;

  int cosku = P*hata;


  int yon;

  if(cosku <0){
    yon = GERI;
    cosku = -cosku;
  }
  else{
    yon = ILERI;
  }

  if(cosku > 255) cosku = 255;
  
  digitalWrite(YON1, yon);
  digitalWrite(YON2, yon);

  analogWrite(PWM1, cosku);
  analogWrite(PWM2, cosku);
  

 // sensor read 
 // filtreleme
  Serial.println(onMesafe);            // prints the value of the sensor to the serial monitor
  delay(400);                    // wait for this much time before printing next value  
}

