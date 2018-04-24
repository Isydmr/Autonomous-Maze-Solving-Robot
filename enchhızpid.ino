#define SAGHIZ  6
#define SAGM 7

#define SOLHIZ  5
#define SOLM 4

#define GERI LOW
#define ILERI HIGH

int sensorpin =0;                 // analog pin used to connect the sharp sensor
int onMesafe = 0;                 // variable to store the values from sensor(initially zero)
int cosku_sol=60;
int cosku_sag=60;
int on_uz=0;
int sag_uz=0;
int sol_uz=0;
int yon_sol = GERI;
int yon_sag = ILERI;
const byte interruptPin_sag = 2;
const byte interruptPin_sol = 3;

volatile long sol_tik = 0;
volatile long sag_tik = 0;


void setup(){
  Serial.begin(9600);
  pinMode(SOLM, OUTPUT);            // Sol motor
  pinMode(SAGM, OUTPUT);            // Sag motor
  pinMode(interruptPin_sag, INPUT_PULLUP);
  pinMode(interruptPin_sol, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin_sag), enc_tik_sag, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptPin_sol), enc_tik_sol, CHANGE);
  /* attachInterrupt(digitalPinToInterrupt(interruptPin_sag_geri), enc_tik_sag_geri, HIGH);
  attachInterrupt(digitalPinToInterrupt(interruptPin_sol_geri), enc_tik_sol_geri, HIGH);*/
}
  
  
void loop(){
  cosku_sol=60;
  cosku_sag=60;
  Serial.print("sag_tik:");
  Serial.println(sag_tik);
  Serial.print("sol_tik:");
  Serial.println(sol_tik); 
  Serial.println("--------------\n\n");    
  pidM2(60,SAGM);
  pidM2(60,SOLM);
  digitalWrite(SAGM, yon_sag);
  analogWrite(SAGHIZ, cosku_sag);
  digitalWrite(SOLM, yon_sol);
  analogWrite(SOLHIZ, cosku_sol);
//  delay(1000);
  Serial.print("sag:");
  Serial.println(vitesse(SAGM));
  Serial.print("sol:");
  Serial.println(vitesse(SOLM));
//  cosku_sol=0;
//  cosku_sag=0;
//  digitalWrite(SAGM, yon_sag);
//  analogWrite(SAGHIZ, cosku_sag);
//  digitalWrite(SOLM, yon_sol);
//  analogWrite(SOLHIZ, cosku_sol);
//  delay(1000);
//  

  
}
void enc_tik_sag() {
  sag_tik++;
}

void enc_tik_sol() {
  sol_tik++;
}

double vitesse(int motor){
  unsigned long tmp_time;
  double my_vitesse ;
  double timedif;
  int tmp_tik;
  int my_delay = 20;
  sol_tik = 0;
  sag_tik = 0;
  switch(motor){
    case SAGM:
      tmp_tik = sag_tik;
      tmp_time = millis();
      delay(my_delay);
      timedif = millis()-tmp_time;
      my_vitesse = (sag_tik - tmp_tik)/(timedif/my_delay);
      return my_vitesse;
      break;
    
    case SOLM:
      tmp_tik = sol_tik;
      tmp_time = millis();
      delay(my_delay);
      timedif = millis()-tmp_time;
      my_vitesse = (sol_tik - tmp_tik)/(timedif/my_delay);
      return my_vitesse;
      break;
  }
}
unsigned long now ;
  double timeChange ;
  unsigned long lastTime;
  
  double Output;
  double errSum, lastErr;
  double kp=2, ki=5, kd=2;
  double dErr;
  double error;
int pidM2(double Setpoint,int motor){
  
  double Input = vitesse(motor);
  switch(motor){
    case SOLM:
      now = millis();
   timeChange = (double)(now - lastTime);

   error = Setpoint - Input;
   errSum += (error * timeChange);
   dErr = (error - lastErr) / timeChange;
   Output = kp * error + ki * errSum + kd * dErr;
    if(error > 0){
        cosku_sol += Output;
 
      }
      else{
        cosku_sol -= Output;         
      }
   /*Remember some variables for next time*/
   lastErr = error;
   lastTime = now;
    
    break;
    
    case SAGM:
    now = millis();
   timeChange = (double)(now - lastTime);

   error = Setpoint - Input;
   errSum += (error * timeChange);
   dErr = (error - lastErr) / timeChange;
   Output = kp * error + ki * errSum + kd * dErr;
    if(error > 0){
        cosku_sag += Output;
 
      }
      else{
        cosku_sag -= Output;         
      }
   /*Remember some variables for next time*/
   lastErr = error;
   lastTime = now;
    
    break;
  }
  
}
//int pidM(double x,int motor){
//  double p = 1.5;
//  double hata;
//  switch(motor){
//    case SOLM:
//      hata = x - vitesse(SOLM);
//      if(hata > 0){
//        cosku_sol += hata * p;
// 
//      }
//      else{
//        cosku_sol += (-1)*hata*p;         
//      }
//        
//    break;
//    
//    case SAGM:
//    hata = x - vitesse(SAGM);
//      if(hata > 0){
//        cosku_sag += hata * p;
// 
//      }
//      else{
//        cosku_sag += (-1)*hata*p;         
//      }
//    break;
//  }
//  
//}


/*
void enc_tik_sag_geri() {
  if(yon_sag == GERI) {sag_tik--;}
}
void enc_tik_sol_geri() {
  if(yon_sol == GERI) {sol_tik--;}
}
*/
