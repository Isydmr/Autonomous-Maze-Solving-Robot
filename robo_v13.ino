#define SAGHIZ  6
#define SAGM 7 

#define SOLHIZ  5
#define SOLM 4

#define ILERI HIGH
#define GERI LOW

#define DUZGIT 0
#define SOLADON 1
#define SAGADON 2
#define GERIDON 3

//#include "QTRSensors.h"
#include "SharpIR.h"

#include <stdlib.h>
#include <Arduino.h>

SharpIR S1(GP2Y0A21YK0F,A0); //sol
SharpIR S2(GP2YA41SK0F,A2);
SharpIR S3(GP2Y0A21YK0F,A1);//sag

struct Konum{
int  x;
int  y; 
bool sagduvar;
bool solduvar;
bool onduvar;
bool arkaduvar;
int  referans;
};

                // variable to store the values from sensor(initially zero)
int cosku_sol=170;
int cosku_sag=235;
int yon_sol=HIGH,yon_sag=HIGH;
int d_sol,d_sag,d_on;
int istikamet;
int oncelik = 0;




void setup(){
  pinMode(SOLM, OUTPUT);            // Sol motor
  pinMode(SAGM, OUTPUT);            // Sag motor
  Serial.begin(9600);
}

void loop(){

  yon_sol=HIGH; yon_sag=HIGH;
  
  //ONCELİK DGİSKENİ ONTANIMLI 0 DGERİ ALIR SAG ONCELİKLİ
  switch(oncelik%2){
  case 0 :  
      if(onbos()){
        istikamet = DUZGIT;
      }else if(sagbos()){
        istikamet = SAGADON;
      }else if(solbos()){
        istikamet = SOLADON;
      }else{
        istikamet = GERIDON;
      }
  break;
  case 1:
      if(onbos()){
        istikamet = DUZGIT;
      }else if(solbos()){
        istikamet = SOLADON;
      }else if(sagbos()){
        istikamet = SAGADON;
      }else{
        istikamet = GERIDON;
      }
  break;
  }

  
switch(istikamet){
  case DUZGIT :
      duz_git(100);
  break;
  case SOLADON :
      soladon();
  break;
  case SAGADON :
      sagadon();
  break;
  case GERIDON :
      geridon();
  break;
  }


 /*
   * FONKSYON ZEMİNDE ÇİZGİ GÖRÜRSE ÖNCLİĞİ DĞİŞİR.
   * if(qtrdeger){
    öncelik++;
  }
  */
}



void dur(int beklet){ // Parametre almıyor
  
  digitalWrite(SOLM, HIGH);
  digitalWrite(SAGM, HIGH);

  analogWrite(SOLHIZ, 0);
  analogWrite(SAGHIZ, 0);
  delay(beklet);
}


void duz_git(int tekrar){
  for(int i =0 ; i<tekrar; i++){
  d_sol=S1.getDistance()-8;
  d_sag=S3.getDistance()-8;
  d_on =S2.getDistance();
  mesafeyaz();
  //delay(200);
    cosku_sol=170;
   cosku_sag=235;
  int p = 1;
  int hata = mesafe_hata(d_sol,d_sag);
  //Serial.print("hata:");
  //Serial.println(hata);
  if(hata > 1){
//    cosku_sol = cosku_sol - p*hata;
    for(int i = 0; i<hata*p ; i++){
  cosku_sol--;
  ilerigit();   
      }
    //Serial.println("sol cosku:");
    //Serial.println(cosku_sol);
  }else if(hata < -1)
  {
//    cosku_sag = cosku_sag + p*hata;
    for(int i = 0; i<((-1*hata)*p); i++){
  cosku_sag--; 
  ilerigit();    
        }
    //Serial.println("sag cosku:");
    //Serial.println(cosku_sag);
  }
  else{
    ilerigit();
  }
  }
}


void ilerigit(){
  
  digitalWrite(SOLM, yon_sol);
  digitalWrite(SAGM, yon_sag);

  analogWrite(SOLHIZ, cosku_sol);
  analogWrite(SAGHIZ, cosku_sag);
  delay(100);
  dur(100);
}
void soladon(){
  yon_sol = !yon_sol;       
  digitalWrite(SOLM, yon_sol);
  digitalWrite(SAGM, yon_sag);

  analogWrite(SOLHIZ, cosku_sol);
  analogWrite(SAGHIZ, cosku_sag);
  delay(370);
  yon_sol = !yon_sol;
}

void sagadon(){
  yon_sag = !yon_sag;
  digitalWrite(SOLM, yon_sol);
  digitalWrite(SAGM, yon_sag);

  analogWrite(SOLHIZ, cosku_sol);
  analogWrite(SAGHIZ, cosku_sag);
  delay(365);
  yon_sag = !yon_sag;
}

void geridon(){
  yon_sag = !yon_sag;
  digitalWrite(SOLM, yon_sol);
  digitalWrite(SAGM, yon_sag);

  analogWrite(SOLHIZ, cosku_sol);
  analogWrite(SAGHIZ, cosku_sag);
  delay(720);
  yon_sag = !yon_sag;
}
int mesafe_hata(int dist1, int dist2){
 int hata = dist1-dist2;
  return hata;
}

bool onbos(){
int  on_uz = S2.getDistance();
  if(on_uz>5)return true;
  else return false;
  
}
bool sagbos(){
  int sag_uz=S3.getDistance()-8;
  if(sag_uz> 5 )return true;
  else{
    return false;
  }
}
bool solbos(){
  int sol_uz = S1.getDistance()-8;
  
  if(sol_uz > 5)return true;
  else return false;
}

void mesafeyaz(){
  Serial.print("sol:");
Serial.println(d_sol);
Serial.print("sag:");
Serial.println(d_sag);
Serial.print("On:");
Serial.println(d_on);
Serial.println("-----------");

}

