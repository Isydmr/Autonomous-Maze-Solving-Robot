#define SAGHIZ  5
#define SAGM 4

#define SOLHIZ  6
#define SOLM 7

#define GERI LOW
#define ILERI HIGH

int sensorpin =0;                 // analog pin used to connect the sharp sensor
int onMesafe = 0;                 // variable to store the values from sensor(initially zero)
int cosku_sol=0;
int cosku_sag=0;
int on_uz=0;
int sag_uz=0;
int sol_uz=0;
int yon_sol,yon_sag;
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
  birkare();
 

  
}
void enc_tik_sag() {
  sag_tik++;
}

void enc_tik_sol() {
  sol_tik++;
}
void birkare(){
  yon_sag=GERI;
  yon_sol=ILERI;
  sol_tik=0;
  sag_tik=0;
  cosku_sol=60;
  while(sag_tik < 20 ){
    cosku_sag=60;
    digitalWrite(SAGM, yon_sag);
    analogWrite(SAGHIZ, cosku_sag);
    digitalWrite(SOLM, yon_sol);
    analogWrite(SOLHIZ, cosku_sol);
    if(sol_tik==20)break;
  }
  
  while(sol_tik < 20 ){
    cosku_sol=60;
    
  }
  
  if(sag_tik==20) 
  if(sol_tik==20) cosku_sol=0;

  
  delay(50);
}
/*
void enc_tik_sag_geri() {
  if(yon_sag == GERI) {sag_tik--;}
}

void enc_tik_sol_geri() {
  if(yon_sol == GERI) {sol_tik--;}
}
*/
