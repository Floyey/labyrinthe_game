#if defined(ENERGIA) // LaunchPad MSP430, Stellaris and Tiva, Experimeter Board FR5739 specific
#include "Energia.h"
#else
#error Platform not defined
#endif
#include <SPI.h>
#include <LCD_screen.h>
#include <LCD_screen_font.h>
#include <LCD_utilities.h>
#include <Screen_HX8353E.h>
#include <Terminal12e.h>
#include <Terminal6e.h>
#include <Terminal8e.h>
Screen_HX8353E myScreen;

uint16_t x, y, x00, y00, i, k;
uint32_t chrono;
long x1_l1, x2_l1, y1_l1, y2_l1;
long x1_l2, x2_l2, y1_l2, y2_l2;
long x_A, y_A;
long x_D, y_D;
const int xpin = 23;
const int ypin = 24;
int button2 = 32;
int button1 = 33;
int buzzer = 40;

void setup()
{
  Serial.begin(9600;
  
  pinMode(buzzer, OUTPUT); // Pin du Buzzer
  pinMode(button2, INPUT_PULLUP); // Pin du bouton 2
  pinMode(button1, INPUT_PULLUP); // Pin du bouton 1

  myScreen.begin();
  x00 = 0;
  y00 = 0;
  i = 1;
  k = 0;

  debut();
}

void debut()
{
  while(digitalRead(button2) == HIGH || digitalRead(button1) == HIGH)
  {
    myScreen.gText(35, 50, "Labyrinthe", whiteColour);
    myScreen.gText(1, 100, "Appuyez sur un bouton", whiteColour);
    myScreen.gText(1, 110, "pour commencer.", whiteColour);
    
    if(digitalRead(button2) == LOW || digitalRead(button1) == LOW)
    {
      break;
    }
  }
  chrono = 0;
  myScreen.clear();
}

void fin()
{
  myScreen.clear();
  while(digitalRead(button2) == HIGH || digitalRead(button1) == HIGH)
  {
    myScreen.gText(20, 50, "Felicitations !", whiteColour);
    myScreen.gText(20, 60, "Vous avez mis :", whiteColour);
    myScreen.gText(40, 70, String(chrono) + " ms", whiteColour);
    myScreen.gText(1, 100, "Appuyez sur le bouton", whiteColour);
    myScreen.gText(1, 110, "pour continuer.", whiteColour);

    if(digitalRead(button2) == LOW || digitalRead(button1) == LOW)
    {
      break;
    }
  }
  chrono = 0;
  i++;
  myScreen.clear();
}

void loop()
{
  x = map(analogRead(xpin), 1200, 2800, -120, 150);
  y = map(analogRead(ypin), 1200, 2800, 150, -100);
  if (x < 1)      x = 1;
  if (x > 128)    x = 128;
  if (y < 1)      y = 1;
  if (y > 128)    y = 128;
        
  myScreen.dRectangle(x00-1, y00-1, 4, 4, blackColour); // Intérieur du carré joueur
  myScreen.dRectangle(x-1, y-1, 4, 4, blueColour);      // Extérieur du carré joueur
  x00 = x;
  y00 = y;

  myScreen.rectangle(0, 8, 127, 127, orangeColour);             // Délimitation de l'espace de jeu
  myScreen.gText(0, 0, "Level " + String(i), whiteColour);      // Affichafe du niveau
  myScreen.gText(55, 0, "Mort(s) : " + String(k), whiteColour); // Affichage du nombre de morts
  
  myScreen.line(30, 8, 30, 100, orangeColour);  // Apparition d'une ligne
  myScreen.line(95, 127, 95, 30, orangeColour); // Apparition d'une ligne
  myScreen.gText(110, 110, "A", whiteColour);   // Apparition du point d'arrivée
  myScreen.gText(10, 15, "D", whiteColour);     // Apparition du point de départ

  /* RESET EN CAS DE COLLISION + COMTPEUR DE MORT(S) */
  if(((x + 3) == 30 && (y + 3) <= 100) || (x == 30 && y <= 100) || ((x + 3) == 95 && (y + 3) >= 30) || (x == 95 && y >= 30)) // Collision barre
  {
    tone(buzzer, 500, 500); // Émets un son
    myScreen.clear();   // Éteint l'écran
    k++;                // Actualisation du nombre de mort
    delay(500);
  }
  if((x + 3) <= 0 || x <= 0 || (x + 3) >= 127 || x >= 127 || (y + 3) <= 8 || y <= 8 ||(y + 3) >= 127 || y >= 127) // Collision rectangle
  {
    tone(buzzer, 500, 500); // Émets un son
    myScreen.clear();   // Éteint l'écran
    k ++;               // Actualisation du nombre de mort
    delay(500);
  }

  chrono = millis();
  
  /* AFFICHAGE ET ACTUALISATION DU CHRONO */
  if(((x + 3) == 110 && (y + 3) ==110) || (x == 110 && y == 110)) // Point d'arrivée
  {
    fin();
  }
}
