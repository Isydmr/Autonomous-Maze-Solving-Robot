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
int d_sol,d_sag,d_on;

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
  
  d_sol=S1.distance()-8;
  d_sag=S3.distance()-8;
  d_on =S2.distance()-9;
  yon_sol=HIGH; yon_sag=HIGH;
  
  cosku_sol=150;
  cosku_sag=200;
    Serial.print("sol:");

  Serial.println(d_sol);
  Serial.print("sag:");

    Serial.println(d_sag);
  Serial.println("-----------");
  //int tik2cm=2;
  //int istenen_konum = 19; // CM CINSINDEN
  //int ref = istenen_konum / tik2cm; // encoder tık cınsınden
  float P = 3;
  if( sagbos(d_sag) ){
    sagadon(yon_sol, yon_sag, cosku_sol, cosku_sag);
  }
  else if( solbos(d_sol)&& !sagbos(d_sag)  ){
    soladon(yon_sol, yon_sag, cosku_sol, cosku_sag);
  }
 //delay(150);
    for(int i=0;i<8;i++){
     d_sol=S1.distance()-8;
     d_sag=S3.distance()-8;
    yon_sol=HIGH; yon_sag=HIGH;
    cosku_sol=150;
    cosku_sag=200;
    if(mesafe_hata(d_sol,d_sag) > 1){
    cosku_sol = cosku_sol - P * ( mesafe_hata(d_sol,d_sag) );
    ilerigit(yon_sol, yon_sag, cosku_sol, cosku_sag);
    } 
    else if(mesafe_hata(d_sol,d_sag) < -1){
    cosku_sag = cosku_sag  - P * ( mesafe_hata(d_sag,d_sol) );
    ilerigit(yon_sol, yon_sag, cosku_sol, cosku_sag);
    }
    else{  
    ilerigit(yon_sol, yon_sag, cosku_sol, cosku_sag);
    }
    delay(50);
  }

    //delay(1000);

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
  delay(365);

}

int mesafe_hata(int dist1, int dist2){
 int hata = dist1-dist2;
  return hata;
}

bool onbos(){
  on_uz = S1.distance();
  if(on_uz>15)return true;
  else return false;
  
}
bool sagbos(int sag_uz){
  if(sag_uz>15)return true;
  else{
    return false;
  }
}
bool solbos(int sol_uz){
  if(sol_uz>15)return true;
  else return false;
}
