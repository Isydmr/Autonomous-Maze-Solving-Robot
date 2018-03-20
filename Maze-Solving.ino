#define SAGHIZ  6
#define SAGM 7 

#define SOLHIZ  5
#define SOLM 4

#define ILERI HIGH
#define GERI LOW

#include "QTRSensors.h"
#include "SharpIR.h"

#include <stdlib.h>
#include <Arduino.h>

SharpIR S1(A0,1080); //sol
SharpIR S2(A2,1080);
SharpIR S3(A1,1080);//sag


struct Konum{
int  x;
int  y; 
bool sagduvar;
bool solduvar;
bool onduvar;
bool arkaduvar;
int  referans;
};

int sayac=0;

int sensorpin =0;                 // analog pin used to connect the sharp sensor
int onMesafe = 0;                 // variable to store the values from sensor(initially zero)
int cosku_sol=150;
int cosku_sag=200;
int on_uz=0;
int sag_uz=0;
int sol_uz=0;
int yon_sol=HIGH,yon_sag=HIGH;
int d_sol,d_sag;

const byte interruptPin_sag = 3;
const byte interruptPin_sol = 2; 
volatile long sol_tik = 0;
volatile long sag_tik = 0;


void setup(){
  pinMode(SOLM, OUTPUT);            // Sol motor
  pinMode(SAGM, OUTPUT);            // Sag motor
  pinMode(interruptPin_sag, INPUT_PULLUP);
  pinMode(interruptPin_sol, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin_sag), enc_tik_sag, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptPin_sol), enc_tik_sol, CHANGE);
  Serial.begin(9600);
}

void loop(){
  cosku_sol=150;
cosku_sag=200;
sagadon(yon_sol,yon_sag, cosku_sol,cosku_sag);
dur();
delay(2000);
 }


void enc_tik_sag() {
  if(yon_sag == ILERI) {sag_tik++;}
  else {sag_tik--;}}

void enc_tik_sol() {
  if(yon_sol == ILERI) {sol_tik++;}
  else {sol_tik--;} }
  

void dur(){ // Parametre almıyor
  digitalWrite(SOLM, HIGH);
  digitalWrite(SAGM, HIGH);

  analogWrite(SOLHIZ, 0);
  analogWrite(SAGHIZ, 0);
}

void ilerigit(int solyon,int sagyon,int solcosku,int sagcosku){
  
  digitalWrite(SOLM, solyon);
  digitalWrite(SAGM, sagyon);

  analogWrite(SOLHIZ, solcosku);
  analogWrite(SAGHIZ, sagcosku);
  delay(55);
  dur();
  delay(55);
}
void soladon(int solyon,int sagyon,int solcosku,int sagcosku){
         
  digitalWrite(SOLM, solyon);
  digitalWrite(SAGM, sagyon);

  analogWrite(SOLHIZ, 0);
  analogWrite(SAGHIZ, sagcosku);
  delay(370);
  
}

void sagadon(int solyon,int sagyon,int solcosku,int sagcosku){
         
  digitalWrite(SOLM, solyon);
  digitalWrite(SAGM, sagyon);

  analogWrite(SOLHIZ, solcosku);
  analogWrite(SAGHIZ, 0);
  delay(385);
}
  


int mesafe_hata(int dist1, int dist2){
 int hata = dist1-dist2;
  return hata;
}

bool on(){
  on_uz = S1.distance();
  if(on_uz>4)return false;
  else return true;
  
}
bool sag(){
  sag_uz = S2.distance();
  if(sag_uz>4)return false;
  else return true;
}
bool sol(){
  sol_uz = S3.distance();
  if(sol_uz>4)return false;
  else return true;
}
