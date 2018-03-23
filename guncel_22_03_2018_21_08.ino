/* !eklenecek! 
-kapali yola girme yani -1 yap
-eger bir yol ayrimdan cıkmaz olmadan baskasına ugrarsa o zaman stackten cekip ona gitmeye çalış(-- yaparak)
-bir cikmaz buldugunda ayrim noktasindaki diger gitmedigin ayrimlara gitmen lazım.
*/

// YONLER
#define KUZEY 0
#define DOGU 1
#define GUNEY 2
#define BATI 3
#define SAGHIZ  5
#define SAGM  4

#define BOS 0
#define DUVAR 1
#define BILINMIYOR 1

#define SOLHIZ  6
#define SOLM  7

#define ILERI HIGH
#define GERI LOW

#include "QTRSensors.h"
#include "SharpIR.h"

#include <stdlib.h>
#include <Arduino.h>

SharpIR S1(GP2Y0A21YK0F,A0); //sol
SharpIR S2(GP2YA41SK0F,A2);
SharpIR S3(GP2Y0A21YK0F,A1);//sag

struct Robot {
  byte yon;
  byte x;
  byte y;

} robot;

struct cell {
  byte duvarlar[4]; // YUKARIDAKI DEFINE LARA GORE GIDIYOR
  byte deger;
  bool visited=false;
  // buraya flood fill sayıları da gelecek;
};

struct cell labirent[16][16];   //labirent[X][Y] // NORMALIN NEREDEYSE IKI KATI
/*YOL AYRIMI */

int yolayrim[10];  // Yol ayrimlarini tutcaz

int d,d_sol,d_sag,d_on,on = 0;


/*YOL AYRIMI */
/* */
//void()



int sayac = 0;

int sensorpin = 0;                // analog pin used to connect the sharp sensor
int onMesafe = 0;                 // variable to store the values from sensor(initially zero)
int cosku_sol = 0;
int cosku_sag = 0;
int on_uz = 0;
int sag_uz = 0;
int sol_uz = 0;
int yon_sol, yon_sag;
const byte interruptPin_sag = 2;
const byte interruptPin_sol = 3;
volatile long sol_tik = 0;
volatile long sag_tik = 0;
int kutudegeri=1;

void setup() {
  kutudegeri=1;
  initLabirent();
  robot.yon = KUZEY;  // KUZEYE BAKIYORUZ
  robot.x = 0; // SOLUMUZDA DA SAGIMIZDA DA LABIRENT OLABILIR
  robot.y = 0; // ARKAMIZDA LABIRENT OLMASI MUMKUN DEGIL

  labirent[robot.x][robot.y].duvarlar[GUNEY] = DUVAR;  // BASLANGICTA ARKAMIZ DOLU;
  DuvarlariLabirenteKaydet();  // SOL SAG ON LABIRENTE KAYDOLSUN
  labirent[robot.x][robot.y].visited = true;
  labirent[robot.x][robot.y].deger = kutudegeri;


  pinMode(SOLM, OUTPUT);            // Sol motor
  pinMode(SAGM, OUTPUT);            // Sag motor
  pinMode(interruptPin_sag, INPUT_PULLUP);
  pinMode(interruptPin_sol, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin_sag), enc_tik_sag, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptPin_sol), enc_tik_sol, CHANGE);
  Serial.begin(9600);
}

/* BURASI DOLACAK */
void loop() {
  yon_sol = HIGH;
  yon_sag = HIGH;
  /* sag -> sol -> on */
  //if(beyazcizgi)
  
  if( !onbos() && !solbos() && sagbos() ) {
    
    switch(robot.yon) {
      case KUZEY:
        labirent[robot.x+1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        break;
      case GUNEY:
        labirent[robot.x-1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        break;
      case BATI:
        labirent[robot.x][robot.y+1].deger = labirent[robot.x][robot.y].deger+1;
        break;
      case DOGU:
        labirent[robot.x][robot.y-1].deger = labirent[robot.x][robot.y].deger+1;
        break;
    }
    sagadon();
    labirent[robot.x][robot.y].visited = true;
  }

  else if( !onbos() && solbos() && !sagbos() ) {
    switch(robot.yon) {
      case KUZEY:
        labirent[robot.x-1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        break;
      case GUNEY:
        labirent[robot.x+1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        break;
      case BATI:
        labirent[robot.x][robot.y-1].deger = labirent[robot.x][robot.y].deger+1;
        break;
      case DOGU:
        labirent[robot.x][robot.y+1].deger = labirent[robot.x][robot.y].deger+1;
        break;
    }
    soladon();
    labirent[robot.x][robot.y].visited = true;
  }
  else if( onbos() && !solbos() && !sagbos() ) {

    switch(robot.yon) {
      case KUZEY:
        labirent[robot.x][robot.y+1].deger = labirent[robot.x][robot.y].deger+1;
        break;
      case GUNEY:
        labirent[robot.x][robot.y-1].deger = labirent[robot.x][robot.y].deger+1;
        break;
      case BATI:
        labirent[robot.x-1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        break;
      case DOGU:
        labirent[robot.x+1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        break;
    }
    labirent[robot.x][robot.y].visited = true;
  }
  else if( !onbos() && solbos() && sagbos() ) {
    
    switch(robot.yon) {
      case KUZEY:
        labirent[robot.x-1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        labirent[robot.x+1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        break;
      case GUNEY:
        labirent[robot.x-1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        labirent[robot.x+1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        break;
      case BATI:
        labirent[robot.x][robot.y-1].deger = labirent[robot.x][robot.y].deger+1;
        labirent[robot.x][robot.y+1].deger = labirent[robot.x][robot.y].deger+1;
        break;
      case DOGU:
        labirent[robot.x][robot.y+1].deger = labirent[robot.x][robot.y].deger+1;
        labirent[robot.x][robot.y-1].deger = labirent[robot.x][robot.y].deger+1;
        break;
    }
    labirent[robot.x][robot.y].visited = true;
    push_ayrim(labirent[robot.x][robot.y].deger);
    sagadon(); 
  }

  else if(onbos() && !solbos() && sagbos()) {
    
    switch(robot.yon) {
      case KUZEY:
        labirent[robot.x][robot.y+1].deger = labirent[robot.x][robot.y].deger+1;
        labirent[robot.x+1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        break;
      case GUNEY:
        labirent[robot.x-1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        labirent[robot.x][robot.y-1].deger = labirent[robot.x][robot.y].deger+1;
        break;
      case BATI:
        labirent[robot.x-1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        labirent[robot.x][robot.y+1].deger = labirent[robot.x][robot.y].deger+1;
        break;
      case DOGU:
        labirent[robot.x+1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        labirent[robot.x][robot.y-1].deger = labirent[robot.x][robot.y].deger+1;
        break;
    }
    push_ayrim(labirent[robot.x][robot.y].deger);
    labirent[robot.x][robot.y].visited = true;
    
  }

  else if(onbos() && solbos() && !sagbos()) {
    
    switch(robot.yon) {
      case KUZEY:
        labirent[robot.x][robot.y+1].deger = labirent[robot.x][robot.y].deger+1;
        labirent[robot.x-1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        break;
      case GUNEY:
        labirent[robot.x+1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        labirent[robot.x][robot.y-1].deger = labirent[robot.x][robot.y].deger+1;
        break;
      case BATI:
        labirent[robot.x-1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        labirent[robot.x][robot.y-1].deger = labirent[robot.x][robot.y].deger+1;
        break;
      case DOGU:
        labirent[robot.x+1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        labirent[robot.x][robot.y+1].deger = labirent[robot.x][robot.y].deger+1;
        break;
    }
    push_ayrim(labirent[robot.x][robot.y].deger);
    labirent[robot.x][robot.y].visited = true;
  }
 
  else if(onbos() && solbos() && sagbos()) {
    
    switch(robot.yon) {
      case KUZEY:
        labirent[robot.x][robot.y+1].deger = labirent[robot.x][robot.y].deger+1;
        labirent[robot.x+1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        labirent[robot.x-1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        break;
      case GUNEY:
        labirent[robot.x-1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        labirent[robot.x+1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        labirent[robot.x][robot.y-1].deger = labirent[robot.x][robot.y].deger+1;
        break;
      case BATI:
        labirent[robot.x-1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        labirent[robot.x][robot.y+1].deger = labirent[robot.x][robot.y].deger+1;
        labirent[robot.x][robot.y-1].deger = labirent[robot.x][robot.y].deger+1;
        break;
      case DOGU:
        labirent[robot.x+1][robot.y].deger = labirent[robot.x][robot.y].deger+1;
        labirent[robot.x][robot.y-1].deger = labirent[robot.x][robot.y].deger+1;
        labirent[robot.x][robot.y+1].deger = labirent[robot.x][robot.y].deger+1;
        break;
    }
    push_ayrim(labirent[robot.x][robot.y].deger);
    push_ayrim(labirent[robot.x][robot.y].deger);
    labirent[robot.x][robot.y].visited = true;
  }
  /* EGER CIKMAZ GORDUYSEN BU ELSE'E GIR */
  else{
      geridon();
      while( labirent[robot.x][robot.y].deger != pop_ayrim() ){
      switch(robot.yon) { 
      case KUZEY:
        if( labirent[robot.x][robot.y].deger-1 == labirent[robot.x][robot.y+1].deger){
            labirent[robot.x][robot.y].deger=-1;
        }
        else if( labirent[robot.x][robot.y].deger-1 == labirent[robot.x-1][robot.y].deger){
            labirent[robot.x][robot.y].deger=-1;
            soladon();
        } 
        else if( labirent[robot.x][robot.y].deger-1 == labirent[robot.x+1][robot.y].deger){
            labirent[robot.x][robot.y].deger=-1;
            sagadon();
        }
        break;
      case GUNEY:
        if( labirent[robot.x][robot.y].deger-1 == labirent[robot.x][robot.y-1].deger){
            labirent[robot.x][robot.y].deger=-1;
        }
        else if( labirent[robot.x][robot.y].deger-1 == labirent[robot.x+1][robot.y].deger){
            labirent[robot.x][robot.y].deger=-1;
            soladon();
        } 
        else if( labirent[robot.x][robot.y].deger-1 == labirent[robot.x-1][robot.y].deger){
            labirent[robot.x][robot.y].deger=-1;
            sagadon();
        }
        break;
      case BATI:
        if( labirent[robot.x][robot.y].deger-1 == labirent[robot.x][robot.y+1].deger){
           labirent[robot.x][robot.y].deger=-1;
           sagadon();
        }
        else if( labirent[robot.x][robot.y].deger-1 == labirent[robot.x+1][robot.y-1].deger){
            labirent[robot.x][robot.y].deger=-1;
            soladon();
        } 
        else if( labirent[robot.x][robot.y].deger-1 == labirent[robot.x-1][robot.y].deger){
            labirent[robot.x][robot.y].deger=-1;
        }
        break;
      case DOGU:
        if( labirent[robot.x][robot.y].deger-1 == labirent[robot.x][robot.y+1].deger){
           labirent[robot.x][robot.y].deger=-1;
           soladon();
        }
        else if( labirent[robot.x][robot.y].deger-1 == labirent[robot.x+1][robot.y-1].deger){
            labirent[robot.x][robot.y].deger=-1;
            sagadon();
        } 
        else if( labirent[robot.x][robot.y].deger-1 == labirent[robot.x+1][robot.y].deger){
            labirent[robot.x][robot.y].deger=-1;
        }
        break;
    }  
    }            
  }


  duz_git(30); //bir kare git
   labirent[robot.x][robot.y].deger=kutudegeri;
  //loopun bitişi
}




void enc_tik_sag() {
  if (yon_sag == ILERI) {
    sag_tik++;
  }
  else {
    sag_tik--;
  }
}
void enc_tik_sol() {
  if (yon_sol == ILERI) {
    sol_tik++;

  }
  else {
    sol_tik--;
  }
}

void dur(int beklet) { // Parametre alıyor, kac ms?

  digitalWrite(SOLM, HIGH);
  digitalWrite(SAGM, HIGH);

  analogWrite(SOLHIZ, 0);
  analogWrite(SAGHIZ, 0);
  delay(beklet);
}

void duz_git(int tekrar) {
  for (int i = 0 ; i < tekrar; i++) {
    d_sol = S1.getDistance() - 8;
    d_sag = S3.getDistance() - 8;
    d_on = S2.getDistance();
    if (d_sol > 5) d_sol = 5;
    if (d_sag > 5) d_sag = 5;
    mesafeyaz();
    //delay(200);
    cosku_sol = 170;
    cosku_sag = 235;
    int p = 3;
    int hata = mesafe_hata();
    //Serial.print("hata:");
    //Serial.println(hata);
    if (hata > 1) {
      //    cosku_sol = cosku_sol - p*hata;
      for (int i = 0; i < hata * p ; i++) {
        cosku_sol--;
        ilerigit();
      }
      //Serial.println("sol cosku:");
      //Serial.println(cosku_sol);
    } else if (hata < -1)
    {
      //    cosku_sag = cosku_sag + p*hata;
      for (int i = 0; i < ((-1 * hata)*p); i++) {
        cosku_sag--;
        ilerigit();
      }
      //Serial.println("sag cosku:");
      //Serial.println(cosku_sag);
    }
    else {
      ilerigit();
    }
  }
}


void ilerigit() {

  digitalWrite(SOLM, yon_sol);
  digitalWrite(SAGM, yon_sag);

  analogWrite(SOLHIZ, cosku_sol);
  analogWrite(SAGHIZ, cosku_sag);
  delay(100);
  dur(100);
}
void soladon() {
  yon_sol = !yon_sol;
  digitalWrite(SOLM, yon_sol);
  digitalWrite(SAGM, yon_sag);

  analogWrite(SOLHIZ, cosku_sol);
  analogWrite(SAGHIZ, cosku_sag);
  delay(370);
  SolaDonduk();
  yon_sol = !yon_sol;
}

void sagadon() {
  yon_sag = !yon_sag;
  digitalWrite(SOLM, yon_sol);
  digitalWrite(SAGM, yon_sag);

  analogWrite(SOLHIZ, cosku_sol);
  analogWrite(SAGHIZ, cosku_sag);
  delay(365);

  SagaDonduk();
  yon_sag = !yon_sag;
}

void geridon() {
  yon_sag = !yon_sag;
  digitalWrite(SOLM, yon_sol);
  digitalWrite(SAGM, yon_sag);

  analogWrite(SOLHIZ, cosku_sol);
  analogWrite(SAGHIZ, cosku_sag);
  delay(720);
  SagaDonduk(); SagaDonduk();
  yon_sag = !yon_sag;
}

int mesafe_hata() {
  int hata = S1.getDistance() - S3.getDistance();
  return hata;
}

bool onbos() {
  int  on_uz = S2.getDistance();
  if (on_uz > 5)return true;
  else return false;

}
bool sagbos() {
  int sag_uz = S3.getDistance() - 8;
  if (sag_uz == 1) sag_uz = 30;
  if (sag_uz > 5 )return true;
  else {
    return false;
  }
}
bool solbos() {
  int sol_uz = S1.getDistance() - 8;
  if (sol_uz == 1) sol_uz = 30;
  if (sol_uz > 5)return true;
  else return false;
}
void mesafeyaz() {
  Serial.print("sol:");
  Serial.println(d_sol);
  Serial.print("sag:");
  Serial.println(d_sag);
  Serial.print("On:");
  Serial.println(d_on);
  Serial.println("-----------");

}


/*aktarma */
void initLabirent() {
  for (int x = 0; x < 16 ; x++) {
    for (int y = 0; y < 16; y++) {
      for (d = 0; d < 4; d++) {
        labirent[x][y].duvarlar[d] = BILINMIYOR;
        labirent[x][y].deger = 0;

      }
    }
  }
}


void SolaDonduk() {  // SOLA DONME DURUMUNDA ROBOTUN YOUNU GUNCELLER
  robot.yon += 3;   // ASLINDA YONDEN 1 CIKARDIK, ama +3 de aynı yonu gosteriyor
  robot.yon %= 4;
}
void SagaDonduk() { // SAGA DONME DURUMUNDA ROBOTUN YONUNU GUNCELLER
  robot.yon += 1;
  robot.yon %= 4;
}

//tek başına çağırılmayacak.
void DuvarlariLabirenteKaydet() {

  if ( !onbos() ) { // ON TARAFTA DUVAR VAR
    labirent[robot.x][robot.y].duvarlar[robot.yon] = DUVAR; //LABIRENTTE ROBOTUN DURDUGU KONUMDA, BAKTIGI YONDE DUVAR VAR
    if (robot.yon == KUZEY) {
      labirent[robot.x][robot.y + 1].duvarlar[GUNEY] = DUVAR;
    }
    else if (robot.yon == GUNEY) {
      labirent[robot.x][robot.y - 1].duvarlar[KUZEY] = DUVAR;
    }
    else if (robot.yon == DOGU) {
      labirent[robot.x + 1][robot.y].duvarlar[BATI] = DUVAR;
    }
    else if (robot.yon == BATI) {
      labirent[robot.x - 1][robot.y].duvarlar[DOGU] = DUVAR;
    }
  }
  else {
    labirent[robot.x][robot.y].duvarlar[robot.yon] = BOS;
    if (robot.yon == KUZEY) {
      labirent[robot.x][robot.y + 1].duvarlar[GUNEY] = BOS;
    }
    else if (robot.yon == GUNEY) {
      labirent[robot.x][robot.y - 1].duvarlar[KUZEY] = BOS;
    }
    else if (robot.yon == DOGU) {
      labirent[robot.x + 1][robot.y].duvarlar[BATI] = BOS;
    }
    else if (robot.yon == BATI) {
      labirent[robot.x - 1][robot.y].duvarlar[DOGU] = BOS;
    }
  }

  if ( !solbos() ) { // SOL TARAFTA DUVAR VAR
    labirent[robot.x][robot.y].duvarlar[(robot.yon + 3) % 4] = DUVAR;
    if (robot.yon == KUZEY) {
      labirent[robot.x - 1][robot.y].duvarlar[DOGU] = DUVAR;
    }
    else if (robot.yon == GUNEY) {
      labirent[robot.x + 1][robot.y].duvarlar[BATI] = DUVAR;
    }
    else if (robot.yon == DOGU) {
      labirent[robot.x][robot.y + 1].duvarlar[GUNEY] = DUVAR;
    }
    else if (robot.yon == BATI) {
      labirent[robot.x][robot.y - 1].duvarlar[KUZEY] = DUVAR;
    }
  }

  else {
    labirent[robot.x][robot.y].duvarlar[(robot.yon + 3) % 4] = BOS;
    if (robot.yon == KUZEY) {
      labirent[robot.x - 1][robot.y].duvarlar[DOGU] = BOS;
    }
    else if (robot.yon == GUNEY) {
      labirent[robot.x + 1][robot.y].duvarlar[BATI] = BOS;
    }
    else if (robot.yon == DOGU) {
      labirent[robot.x][robot.y + 1].duvarlar[GUNEY] = BOS;
    }
    else if (robot.yon == BATI) {
      labirent[robot.x][robot.y - 1].duvarlar[KUZEY] = BOS;
    }
  }


  if ( !sagbos() ) { // SAG TARAFTA DUVAR VAR
    labirent[robot.x][robot.y].duvarlar[(robot.yon + 1) % 4] = DUVAR;
    if (robot.yon == KUZEY) {
      labirent[robot.x + 1][robot.y].duvarlar[BATI] = DUVAR;
    }
    else if (robot.yon == GUNEY) {
      labirent[robot.x - 1][robot.y].duvarlar[DOGU] = DUVAR;
    }
    else if (robot.yon == DOGU) {
      labirent[robot.x][robot.y - 1].duvarlar[KUZEY] = DUVAR;
    }
    else if (robot.yon == BATI) {
      labirent[robot.x][robot.y + 1].duvarlar[GUNEY] = DUVAR;
    }
    else {
      labirent[robot.x][robot.y].duvarlar[(robot.yon + 3) % 4] = BOS;
      if (robot.yon == KUZEY) {
        labirent[robot.x + 1][robot.y].duvarlar[BATI] = DUVAR;
      }
      else if (robot.yon == GUNEY) {
        labirent[robot.x - 1][robot.y].duvarlar[DOGU] = DUVAR;
      }
      else if (robot.yon == DOGU) {
        labirent[robot.x][robot.y - 1].duvarlar[KUZEY] = DUVAR;
      }
      else if (robot.yon == BATI) {
        labirent[robot.x][robot.y + 1].duvarlar[GUNEY] = DUVAR;
      }
    }
  }

  else {
    labirent[robot.x][robot.y].duvarlar[(robot.yon + 1) % 4] = BOS;
    if (robot.yon == KUZEY) {
      labirent[robot.x + 1][robot.y].duvarlar[BATI] = BOS;
    }
    else if (robot.yon == GUNEY) {
      labirent[robot.x - 1][robot.y].duvarlar[DOGU] = BOS;
    }
    else if (robot.yon == DOGU) {
      labirent[robot.x][robot.y - 1].duvarlar[KUZEY] = BOS;
    }
    else if (robot.yon == BATI) {
      labirent[robot.x][robot.y + 1].duvarlar[GUNEY] = BOS;
    }
  }


}

void IleriGittik() {
  if (robot.yon == KUZEY) robot.y++;
  if (robot.yon == GUNEY) robot.y--;
  if (robot.yon == DOGU) robot.x++;
  if (robot.yon == BATI) robot.x--;

  DuvarlariLabirenteKaydet();
}


void printLabirent() {    // BU DAHA BITMEDI
  for (int y = 15; y >= 0; y--) {
    for (int x = 0; x < 31 ; x++) {

      for (d = 0; d < 4; d++) {
        switch ( labirent[x][y].duvarlar[d] ) {
          case DUVAR:
            Serial.print("|");
            break;
          /*case BILINMIYOR:
            Serial.print("?");
            break;*/
          case BOS:
            Serial.print(" ");
            break;
        }
      }
    }
  }
}
void push_ayrim(int value ){
  yolayrim[on]=value;
  on++;
}

int pop_ayrim(){ //struct dondur, bos yeri goster
on--;
return yolayrim[on];
}
