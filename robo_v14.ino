//#include <PID_v1.h>
#include <SharpIR.h>

#define SAGHIZ  6
#define SAGM 7

#define SOLHIZ  5
#define SOLM 4

#define ILERI HIGH
#define GERI LOW

#define ENCSOL 3
#define ENCSAG 2

#define DUZGIT 0
#define SOLADON 1
#define SAGADON 2
#define GERIDON 3

#define ALTMOTORSINIRI_SAG 91
#define ALTMOTORSINIRI_SOL 70

#define USTMOTORSINIRI_SAG 235
#define USTMOTORSINIRI_SOL 170

byte cosku_sol = 70;
byte cosku_sag = 91;
bool yon_sol = ILERI;
bool yon_sag = ILERI;
int enc_sag = 0;
int enc_sol = 0;
bool rcs = true; //DÖNÜŞ KONTROL SİSTEMİ
int istikamet = 1;

void setup() {
  // put your setup code here, to run once:
  pinMode(SOLM, OUTPUT);            // Sol motor
  pinMode(SAGM, OUTPUT);// Sag motor
  pinMode(ENCSAG, INPUT);
  pinMode(ENCSOL, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:


  while (rcs) {
    Serial.println("Once:");
    Serial.println(enc_sag);
    Serial.println(enc_sol);
    Serial.println("------------\n\n");
    sagadon(4);
    Serial.println("Sonra:");
    Serial.println(enc_sag);
    Serial.println(enc_sol);
    Serial.println("------------\n\n");
  }
  control();



}

void control() {
  delay(1000);
  istikamet =1;
  cosku_sol = 70;
  cosku_sag = 91;
  enc_sag = 0;
  enc_sol = 0;
  rcs = true;
}
void birkareilerle(int adim) {
  sayacoku(ENCSOL);
  sayacoku(ENCSAG);
  if (enc_sag >= adim && enc_sol >= (adim + 1)) {
    dur();
    rcs = false;
  } else {
//    if (cosku_sol != USTMOTORSINIRI_SOL) {
//      cosku_sol += 10;
//      cosku_sag += 13;
//    }
    digitalWrite(SOLM, yon_sol);
    digitalWrite(SAGM, yon_sag);
    analogWrite(SOLHIZ,  cosku_sol);
    analogWrite(SAGHIZ,  cosku_sag);
  }


}
void soladon(int adim) {
  yon_sol = !yon_sol;
  sayacoku(ENCSOL);
  sayacoku(ENCSAG);
  if (enc_sag >= adim + 2 && enc_sol >= adim) {
    dur();
    rcs = false;
  } else {
    digitalWrite(SOLM, yon_sol);
    digitalWrite(SAGM, yon_sag);
    analogWrite(SOLHIZ, cosku_sol);
    analogWrite(SAGHIZ, cosku_sag);
  }
  yon_sol = !yon_sol;
}

void sagadon(int adim) {
  yon_sag = !yon_sag;
  sayacoku(ENCSOL);
  sayacoku(ENCSAG);
  if (enc_sag >= adim && enc_sol >= adim + 2) {
    dur();
    rcs = false;
  } else {
    digitalWrite(SOLM, yon_sol);
    digitalWrite(SAGM, yon_sag);
    analogWrite(SOLHIZ, cosku_sol);
    analogWrite(SAGHIZ, cosku_sag);
  }
  yon_sag = !yon_sag;
}

void geridon(int adim) {
  soladon(adim);
  soladon(adim);
}


void sayacoku(int a) {
  int tmp = digitalRead(a);
  delay(30);
  if (tmp != digitalRead(a)) {
    switch (a) {
      case ENCSOL:
        enc_sol++;
        break;
      case ENCSAG:
        enc_sag++;
        break;
    }
  }
}


void dur() {
//  while (cosku_sol != ALTMOTORSINIRI_SOL) {
    cosku_sol = 0;
    cosku_sag = 0;
    digitalWrite(SOLM, ILERI);
    digitalWrite(SAGM, ILERI);
    analogWrite(SOLHIZ,  cosku_sol);
    analogWrite(SAGHIZ,  cosku_sag);
//  }
}
