
#include <Adafruit_NeoPixel.h>    //Libreria
#ifdef __AVR__
#include <avr/power.h>            // Required for 16 MHz Adafruit Trinket
#endif
#define PIN 6                     // Pin della striscia led
#define NUMPIXELS 300             // Numero di pixel della striscia led

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 20

int contatoreP1 = 0;              // Posizione sulla striscia led giocatore 1 (Rosso)
int contatoreP2 = 0;              // Posizione sulla striscia led giocatore 2 (Verde)
int pinPulsante = 12;             // Pin pulsante 1 (Rosso)
int pinPulsanteB = 10;            // Pin pulsante 2 (Verde)
int pinPulsanteC = 2;             // Pin pulsante reset
int statoPulsante = 0;            // Stato pulsante 1 (Rosso)
int statoPulsanteB = 0;           // Stato pulsante 2 (Verde)
int statoPulsanteC = 0;           // Stato pulsante reset
int lastButtonState = 0;          // Stato precedente pulsante 1 (Rosso)
int lastButtonStateB = 0;         // Stato precedente pulsante 2 (Verde)
int lastButtonStateC = 0;         // Stato precedente pulsante reset

void setup() {

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  pixels.begin();
  pinMode(pinPulsante, INPUT_PULLUP);       // Definisci stato pin 1 (Rosso)
  pinMode(pinPulsanteB, INPUT_PULLUP);      // Definisci stato pin 2 (Verde)
  pinMode(pinPulsanteC, INPUT_PULLUP);      // Definisci stato pin reset
  Serial.begin(9600);
  
}

void loop() {
  
  pixels.clear();
  statoPulsante = digitalRead(pinPulsante);         // Legge il valore HIGH/LOW pulsante 1 (Rosso)
  statoPulsanteB = digitalRead(pinPulsanteB);       // Legge il valore HIGH/LOW pulsante 2 (Verde)
  statoPulsanteC = digitalRead(pinPulsanteC);       // Legge il valore HIGH/LOW pulsante reset


  // PULSANTE 1 (Rosso)

  if (statoPulsante != lastButtonState) {           // Confronta statoPulsante con lastButtonState. TRUE se valore non uguale
    if (statoPulsante == HIGH) {                    // Se quindi il pulsante é HIGH
      contatoreP1++;                                // Giocatore avanta di un pixel sulla striscia led
      Serial.println("on");                         
      Serial.print("number of button pushes: ");
      Serial.println(contatoreP1);
      pixels.setPixelColor(contatoreP1, pixels.Color(255, 0, 0));     // Colore pixel giocatore 1 (Rosso)
      pixels.setPixelColor(contatoreP2, pixels.Color(0, 255, 0));     // Colore pixel giocatore 2 (Verde)
      pixels.show();                                                  // Mostra i piexel della striscia led
    } else {                                                          // Se quindi il pulsante é LOW
      Serial.println("off");
    }
    delay(50);
  }
  
lastButtonState = statoPulsante;


  // PULSANTE 2 (Verde)

  if (statoPulsanteB != lastButtonStateB) {
    if (statoPulsanteB == HIGH) {
      contatoreP2++;
      Serial.println("on");
      Serial.print("number of button2 pushes: ");
      Serial.println(contatoreP2);
      pixels.setPixelColor(contatoreP2, pixels.Color(0, 255, 0));
      pixels.setPixelColor(contatoreP1, pixels.Color(255, 0, 0));
      pixels.show();
    } else {
      Serial.println("off");
    }
    delay(50);
  }
  
lastButtonStateB = statoPulsanteB;

  
  // Se i due giocatori si trovano sullo stesso pixel della striscia led diventa giallo
  
  if ( contatoreP1 == contatoreP2) {                                      
    pixels.setPixelColor(contatoreP1, pixels.Color(255, 255, 0));       // Colorazione gialla giocatore 1
    pixels.setPixelColor(contatoreP2, pixels.Color(255, 255, 0));       // Colorazione gialla giocatore 2
    pixels.show();                                                      // Mostra i piexel della striscia led
  }


  // Se il giocatore 1 arriva alla fine dei led, la striscia led si accende del colore del giocatore e si ricomincia da 0

  if ( contatoreP1 == NUMPIXELS) {                          // Giocatore 1 (Rosso) arriva al numero totale pixel
    for (int i = 0; i < NUMPIXELS; i ++) {                  // Tutti i pixel si accendono a crescere
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));     // Colore rosso
      pixels.show();                                        // Mostra i piexel della striscia led
    }
  } else if (contatoreP1 > NUMPIXELS) {                     // Se giocatore 1 (Rosso) supera numero totale pixel
    contatoreP1 = 0;                                        // Giocatore 1 torna a pixel 0
    contatoreP2 = 0;                                        // Giocatore 2 torna a pixel 0
    pixels.setPixelColor(contatoreP1, pixels.Color(255, 0, 0));
    pixels.show();
  }
  

  // Se il giocatore 2 arriva alla fine dei led, la striscia led si accende del colore del giocatore e si ricomincia da 0
  
  if ( contatoreP2 == NUMPIXELS) {                          // Giocatore 2 (Verde) arriva al numero totale pixel
    for (int i = 0; i < NUMPIXELS; i ++) {
      pixels.setPixelColor(i, pixels.Color(0, 255, 0));     // Colore verde
      pixels.show();
    }
  } else if (contatoreP2 > NUMPIXELS) {                     // Se giocatore 2 (Verde) supera numero totale pixel
    contatoreP2 = 0;                                        // Giocatore 2 torna a pixel 0
    contatoreP1 = 0;                                        // Giocatore 1 torna a pixel 0
    pixels.setPixelColor(contatoreP2, pixels.Color(0, 255, 0));
    pixels.show();
  }


  // RESET
  
  if ( statoPulsanteC == 1 ) {                              // Se il pulsante reset é HIGH
    contatoreP1 = 0;                                        // Giocatore 1 (Rosso) torna a pixel 0
    contatoreP2 = 0;                                        // Giocatore 2 (Verde) torna a pixel 0
    Serial.println("on");
      Serial.print("number of button3 pushes: ");
      Serial.println(lastButtonStateC);
  } else {
      Serial.println("off");
  }
  
}
