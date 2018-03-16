#ifndef Roborobo_h
#define Roborobo_h

#ifdef ARDUINO
  #include "Arduino.h"
#endif
struct Konum{
byte x;
byte y; 
bool sagduvar;
bool solduvar;
bool onduvar;
bool arkaduvar;
int  referans;
};
class Hareket{

public:
void ilerigit();
void gerigit();
void solagit();
void sagagit();

void soladon();
void sagadon();
void arkayadon();

void hizlan();
void yavaslayarakdur();

};
class Sensor{

public:
//belirtilen yönlerde duvar olup olmadığını kontrol eder
bool on();
bool arka();
bool sag();
bool sol();

bool cizgi(); // zeminde çizgi görürse true döndürür
int top_cizgi(); //başlangıçtan itibaren geçilen çizgiyi döndürür

// tekerleriin kaçar tur döndüğünü hesaplar;  
float sag_tur();
float sol_tur();  



};

class Motor{
//motorları suren kütüphane bulunursa ayrıca eklenebilir
void cosku();
void fren();

};



#endif
