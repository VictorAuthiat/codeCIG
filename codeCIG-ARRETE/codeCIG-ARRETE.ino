
//#include <Countimer.h>
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include "EEPROMAnything.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Stepper.h>
#include "data.h"
#include "Countimer.h"



#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


#if (SSD1306_LCDHEIGHT != 32)
#error(“Height incorrect, please fix Adafruit_SSD1306.h!”);
#endif


#include <avr/pgmspace.h>
#define menubaseinfo_HEIGHT   32
#define menubaseinfo_WIDTH    128
#define menubasereg_HEIGHT       32
#define menubasereg_WIDTH        128
#define menubaseopen_HEIGHT     32
#define menubaseopen_WIDTH      128
#define menubase_HEIGHT   32
#define menubase_WIDTH    128
#define parametres2_HEIGHT   32
#define parametres2_WIDTH    128
#define menub_HEIGHT   32
#define menub_WIDTH    128
#define menuc_HEIGHT   32
#define menuc_WIDTH    128

#define para23_HEIGHT   32
#define para23_WIDTH    128
#define para22_HEIGHT   32
#define para22_WIDTH    128

Countimer tUp;
Countimer tDown;
Countimer tDown2;
Countimer tNone;
const int stepsPerRevolution = 250;
Stepper myStepper(stepsPerRevolution, 11, 9, 10, 8);
int cs = 0 ;
int tep =0;
int tpsin = 0;
int tps = 0;
int tp = 0;
int cig = 0 ;
int CIGR = 0;
int CIGF= 0;
int tpsmin = 0;
const int buttonA = 3;
const int buttonB = 2;
unsigned int buttonStateB = 0;      
unsigned int buttonStateA = 0;       
int rak = 0 ;
unsigned int mode=0;

bool interrupt = false;
bool doing_menu = false;

static const byte settings_version = B10000011;
bool fix_first = 1;




void setup()
{
 
      moteurepen1();
  byte read_settings_version;
  EEPROM_readAnything(0, read_settings_version);
 
  if(settings_version == read_settings_version) 
  {  
      EEPROM_readAnything(15, cs);
  EEPROM_readAnything(10, CIGR);
   EEPROM_readAnything(11, cig);
    EEPROM_readAnything(12, tpsmin);
  EEPROM_writeAnything(13, tpsin);
  
  
  }
  else                                         
  {
    EEPROM_writeAnything(0, settings_version);
    EEPROM_writeAnything(15, cs);
    EEPROM_writeAnything(10, CIGR);
    EEPROM_writeAnything(11, cig);
  EEPROM_writeAnything(12, tpsmin);
    EEPROM_writeAnything(13, tpsin);
  
  
  }
 
myStepper. setSpeed ( 60 ) ;
Serial.begin(9600);
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.display();
delay(3000);
display.clearDisplay();
//CIGR =0 ;
  
Wire.begin();
 
 moteurepen1();

 pinMode(3, INPUT); //Set pin for time/date mode button to input
digitalWrite(3, HIGH); //Turn on pullup resistors

pinMode(2, INPUT); //Set pin for time/date set button to input
digitalWrite(2, HIGH); //Turn on pullup resistors
     
 tDown2.setCounter(0, 0, 45, tDown2.COUNT_DOWN, tDown2Complete);
    // Call print_time2() method every 1s.
 tDown2.setInterval(print_time3, 1000);  //1000
    
// tNone.setInterval(print_none, 2000);
tUp.setCounter(24, 0, 0, tUp.COUNT_UP, tUpComplete);
    // Call print_time1() method every 1s.
  tUp.setInterval(print_time1, 1000);
}

//==========================================================//

void loop()


{  
  display.display();
  delay(10);
  display.clearDisplay();
      
       if ((digitalRead(3)) && (digitalRead(2)))
    {  //CIGARETTE DE SECOURS

Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  delay(500);



cig = 0;

display.clearDisplay();
display.setTextSize(1);
display.setTextColor(BLACK,WHITE);
display.setCursor(20,10);
display.print("cig de secours");
  
  
  display.display();
  delay(15000);
    cs = cs + 1 ;
   // EEPROM_readAnything(15, cs);
  //EEPROM_readAnything(10, CIGR);
      // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  delay(500);
     mode=0;
      }
    
 else if( (digitalRead(3)) && (digitalRead(2) == LOW) ) 
    {
      
      if (mode == 0) 
      {   //choisirpara
        mode = 1;
      }
    else if (mode == 1) 
        {  //parametres
        mode = 2;
        }
    else if (mode == 2) 
        { //parametres choisir cig dans le paquet
       cig=0;
        mode = 3;
        }
                                         else if (mode == 3) 
                                 {    
                                  
          
    EEPROM_writeAnything(11, cig);
  mode = 2;

                                  
                                 }
    else if (mode == 4) 
        {  mode = 5;
        }
                                          else if (mode == 5) 
                                 {    tpsmin = tp   ;           
                                  //cig dans le paquet

  EEPROM_writeAnything(12, tpsmin);
  mode = 2;                                     //temps entre chaque clope
                                 }

     else if (mode == 6)
        {
          mode = 0;
        }
     else if (mode == 7) {
      
    if (tps==0 && tpsin == 0) {
        
        mode=15;
        
        
    }
 
     else mode = 0;
     
     
     
     
     }
      
      
     else if (mode == 8) {
     mode =9;
      }
     else if (mode == 9) {
     mode =0;
      }
       EEPROM_writeAnything(10, CIGR);
     EEPROM_writeAnything(11, cig);  
        EEPROM_writeAnything(12, tpsmin);
     
}

    else if ( (digitalRead(2)) && (digitalRead(3) == LOW) )
    {
     

         if (mode == 0) 
        {
        mode = 10;
       
          }
    else if (mode == 1) 
       {
        mode = 7;
        }

    else if (mode == 2) 
       {
        mode = 4;
        }
             else if (mode == 3) 
       {              if ( cig <= 20) {
                   cig = cig + 1 ;
                   }
                if  (cig == 21){
                                  cig = 0;
}


                     
                                  
                      } 

       
    else if (mode == 4) 
       {
        mode =6;
       }
                             else if (mode == 5) 
       { 
        if ( tp <= 6) {
                   tp = tp + 1 ;
                   }
        if  (tp == 7){
                                  
              tp = 0;
                                  }
                                                            //temps entre chaque clope
       }
    else if (mode == 6) 
       {
        mode =2;
       }
    else if (mode == 7) 
       {
        mode =8;
       }
    else if (mode == 8) 
       {
        mode =1;
       }
    else if (mode == 9) 
       {
        mode =0;
       }    
    else if (mode == 10) 
       {
          CIGR = cig;
        EEPROM_writeAnything(10, CIGR);
        mode =11;
       }

    else if (mode == 11) 
       {
        mode =0;
       }
    
    
    
    }
 
 
  
  
 updateScreen();

//==========================================================//

}      

//==========================================================//




static void updateScreen(void) 
{
  tUp.run();
tDown.run();
 if (mode == 0)
  {
    go2();
   display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);

display.setCursor(75,0);
display.print(tDown.getCurrentTime());

display.drawBitmap(
    (display.width()  - menubase_WIDTH ) ,
    (display.height() - menubase_HEIGHT) ,
    menubase, menubase_WIDTH, menubase_HEIGHT, 1);
  display.display();
  
  
  }
   
  
  else if (mode == 1)
  {

    
   display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);

display.setCursor(75,0);
display.print(tDown.getCurrentTime());

display.drawBitmap(
    (display.width()  - menubasereg_WIDTH ) ,
    (display.height() - menubasereg_HEIGHT) ,
    menubasereg, menubasereg_WIDTH, menubasereg_HEIGHT, 1);
  display.display();


  }
  
  else if (mode == 2)
  {
//parametres choisir cig

    
   display.clearDisplay();
   display.drawBitmap(
    (display.width()  - parametres2_WIDTH ) ,
    (display.height() - parametres2_HEIGHT) ,
    parametres2, parametres2_WIDTH, parametres2_HEIGHT, 1);
  display.display();

  }
  
else if (mode == 3)
  {    
   
//parametre cig
if (cig == 0 ){
display.clearDisplay();
  display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (10,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Cig dans le paquet:");
  display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (60,15); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("0");
display.display();

}
else if (cig ==1){

display.clearDisplay();
display.setTextSize (1); 
  display.setTextColor (WHITE);
  display.setCursor (10,0); 
  display.print ("Cig dans le paquet:");
display.setTextSize (2); 
  display.setTextColor (WHITE);
  display.setCursor (60,15); 
  display.print ("1");
display.display();
}
else if (cig == 2){

display.clearDisplay();
display.setTextSize (1); 
  display.setTextColor (WHITE);
  display.setCursor (10,0); 
  display.print ("Cig dans le paquet:");
display.setTextSize (2); 
  display.setTextColor (WHITE);
  display.setCursor (60,15);
  display.print ("2");
display.display();
}

 else if (cig == 3){

display.clearDisplay();
display.setTextSize (1); 
  display.setTextColor (WHITE);
  display.setCursor (10,0); 
  display.print ("Cig dans le paquet:");
display.setTextSize (2); 
  display.setTextColor (WHITE);
  display.setCursor (60,15);
  display.print ("3");
display.display();
}




else if (cig == 4){
display.clearDisplay();

display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (10,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Cig dans le paquet:");
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (60,15); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("4");
display.display();
}



else if (cig == 5){
display.clearDisplay();

display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (10,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Cig dans le paquet:");
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (60,15); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("5");
display.display();
}



else if (cig == 6){
display.clearDisplay();

display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (10,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Cig dans le paquet:");
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (60,15); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("6");
display.display();
}




else if (cig == 7){
display.clearDisplay();

display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (10,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Cig dans le paquet:");
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (60,15); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("7");
display.display();
}




else if (cig == 8){
display.clearDisplay();

display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (10,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Cig dans le paquet:");
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (60,15); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("8");
display.display();
}



else if (cig == 9){

display.clearDisplay();

display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (10,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Cig dans le paquet:");
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (60,15); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("9");
display.display();
}

else if (cig == 10){
display.clearDisplay();

display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (10,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Cig dans le paquet:");
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (60,15); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("10");
display.display();
}



else if (cig == 11){
display.clearDisplay();

display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (10,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Cig dans le paquet:");
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (60,15); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("11");
display.display();
}




else if (cig == 12){

display.clearDisplay();

display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (10,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Cig dans le paquet:");
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (60,15); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("12");
display.display();
}

else if (cig == 13){
display.clearDisplay();


display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (10,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Cig dans le paquet:");
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (60,15); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("13");
display.display();
}

else if (cig == 14){
display.clearDisplay();

display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (10,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Cig dans le paquet:");
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (60,15); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("14");
display.display();
}


else if (cig == 15){
display.clearDisplay();


display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (10,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Cig dans le paquet:");
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (60,15); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("15");
display.display();
}




else if (cig == 16){

display.clearDisplay();

display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (10,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Cig dans le paquet:");
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (60,15); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("16");
display.display();
}



else if (cig == 17){
display.clearDisplay();


display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (10,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Cig dans le paquet:");
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (60,15); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("17");
display.display();
}

else if (cig == 18){

display.clearDisplay();

display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (10,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Cig dans le paquet:");
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (60,15); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("18");
display.display();
}


else if (cig == 19){
display.clearDisplay();
display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (10,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Cig dans le paquet:");
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (60,15); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("19");
display.display();
}


else if (cig == 20){

display.clearDisplay();
display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (10,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Cig dans le paquet:");
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (60,15); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("20");
display.display();
}

                      
EEPROM_writeAnything(11,cig);


  }
 else if (mode == 4)
  {
//parametres choisir temps 


    
   display.clearDisplay();
   display.drawBitmap(
    (display.width()  - para22_WIDTH ) ,
    (display.height() - para22_HEIGHT) ,
    para22, para22_WIDTH, para22_HEIGHT, 1);
   display.display();
  }
  
else if (mode == 5)
  {
//parametres tempsdisplay.clearDisplay();
if (tp == 0){
  //tDown2.setCounter(0, 0, 45, tDown2.COUNT_DOWN, tDown2Complete);
   //  tDown2.setInterval(print_time3, 1000);
    // Call print_time1() method every 1s.
// tNone.setInterval(print_none, 2000);
  tDown.setCounter(0, 0, 0, tDown.COUNT_DOWN, tDownComplete);
    // Call print_time2() method every 1s.
  tDown.setInterval(print_time2, 1000);
tNone.setInterval(print_none, 2000);
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (20,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Temps min :");
display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (30,20); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("0 minutes");
display.display();
}
  
else if (tp == 1){
 
  // tDown2.setCounter(0, 0, 45, tDown2.COUNT_DOWN, tDown2Complete);
   //  tDown2.setInterval(print_time3, 1000);
// tNone.setInterval(print_none, 2000);
  tDown.setCounter(0, 15, 0, tDown.COUNT_DOWN, tDownComplete);
    // Call print_time2() method every 1s.
  tDown.setInterval(print_time2, 1000);
tNone.setInterval(print_none, 2000);
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (20,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Temps min :");
display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (30,20); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("15 min");
display.display();
  }

else if (tp == 2){
 
 //  tDown2.setCounter(0, 0, 45, tDown2.COUNT_DOWN, tDown2Complete);
  //   tDown2.setInterval(print_time3, 1000);
// tNone.setInterval(print_none, 2000);
  tDown.setCounter(0, 30, 0, tDown.COUNT_DOWN, tDownComplete);
    // Call print_time2() method every 1s.
  tDown.setInterval(print_time2, 1000);
tNone.setInterval(print_none, 2000);
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (20,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Temps min :");
display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (30,20); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("30 min");
display.display();
  }

else if (tp == 3){

  // tDown2.setCounter(0, 0, 45, tDown2.COUNT_DOWN, tDown2Complete);
 //    tDown2.setInterval(print_time3, 1000);
 //tNone.setInterval(print_none, 2000);
  tDown.setCounter(0, 45, 0, tDown.COUNT_DOWN, tDownComplete);
    // Call print_time2() method every 1s.
  tDown.setInterval(print_time2, 1000);
tNone.setInterval(print_none, 2000);
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (20,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Temps min :");
display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (30,20); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("45 min");
display.display();
  }
else if (tp == 4){
 
 //  tDown2.setCounter(0, 0, 45, tDown2.COUNT_DOWN, tDown2Complete);
   //  tDown2.setInterval(print_time3, 1000);
//tNone.setInterval(print_none, 2000);
  tDown.setCounter(1, 0, 0, tDown.COUNT_DOWN, tDownComplete);
    // Call print_time2() method every 1s.
  tDown.setInterval(print_time2, 1000);
tNone.setInterval(print_none, 2000);
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (20,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Temps min :");
display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (30,20); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("1H00");
display.display();
  }
else if (tp == 5){
 //tDown2.setCounter(0, 0, 45, tDown2.COUNT_DOWN, tDown2Complete);
    // tDown2.setInterval(print_time3, 1000);
 //tNone.setInterval(print_none, 2000);
tDown.setCounter(1, 30, 0, tDown.COUNT_DOWN, tDownComplete);
 tDown.setInterval(print_time2, 1000);

tNone.setInterval(print_none, 2000);
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (20,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Temps min :");
display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (30,20); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("1H30");
display.display();
  }
else if (tp == 6){
  
 //  tDown2.setCounter(0, 0, 45, tDown2.COUNT_DOWN, tDown2Complete);
   //  tDown2.setInterval(print_time3, 1000);
 // tNone.setInterval(print_none, 2000);
 
  tDown.setCounter(2, 0, 0, tDown.COUNT_DOWN, tDownComplete);
    // Call print_time2() method every 1s.
  tDown.setInterval(print_time2, 1000);

tNone.setInterval(print_none, 2000);
display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (20,0); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Temps min :");
display.setTextSize (1); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
  display.setCursor (30,20); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("2H00");
display.display();
}
EEPROM_writeAnything(12, tpsmin);
EEPROM_writeAnything(13, tpsin);
}
else if (mode == 6)
  {
//parametres choisir retour

    
   display.clearDisplay();
   display.drawBitmap(
    (display.width()  - para23_WIDTH ) ,
    (display.height() - para23_HEIGHT) ,
    para23, para23_WIDTH, para23_HEIGHT, 1);
   display.display();
  }

  
  
else if (mode == 7)
  {
//menu base choisir ouvrir


    display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);

display.setCursor(75,0);
display.print(tDown.getCurrentTime());

display.drawBitmap(
    (display.width()  - menubaseopen_WIDTH ) ,
    (display.height() - menubaseopen_HEIGHT) ,
   menubaseopen,menubaseopen_WIDTH, menubaseopen_HEIGHT, 1);
  display.display();


  }
 else if (mode == 8)
  {
//menu base choisir info

    
   display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);

display.setCursor(75,0);
display.print(tDown.getCurrentTime());

display.drawBitmap(
    (display.width()  - menubaseinfo_WIDTH ) ,
    (display.height() - menubaseinfo_HEIGHT) ,
   menubaseinfo, menubaseinfo_WIDTH, menubaseinfo_HEIGHT, 1);
  display.display();


  }
  
  else if (mode == 9)
  {

//infos
 display.clearDisplay();
 
   
display.setTextSize(1);
display.setTextColor (BLACK, WHITE);
display.setCursor(70,0);
display.print("Cig-Arret");
display.setTextColor(WHITE);
display.setCursor(70,10);
display.print("version1");
display.setCursor(0,0);
display.print("CdS :");
cigsec();
//cig de secours eeprom
display.display ();
 
  }
 else if (mode == 10)
  {
//infos cig restantes /cig fumees /dernieres cig il y a...
 
   display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);

display.setCursor(75,20);
 display.print(tDown.getCurrentTime());
display.setCursor(75,0);

display.print(tUp.getCurrentTime());

display.drawBitmap(
    (display.width()  - menub_WIDTH ) ,
    (display.height() - menub_HEIGHT) ,
   menub, menub_WIDTH, menub_HEIGHT, 1);
  display.display();

  }

else if (mode == 11)
  { 
     
//infos cig restantes /cig fumees /dernieres cig il y a...
display.clearDisplay();
EEPROM_readAnything(15, cs);
  EEPROM_readAnything(10, CIGR);
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);


display.setTextSize(2);
display.setTextColor(WHITE);

EEPROM_readAnything(10, CIGR);

 if (CIGR==0) {
display.clearDisplay();
cigsec();
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);
display.setCursor(75,15);
display.setTextSize(2);
display.setTextColor(WHITE);
display.print("0");
display.display();

 }

else if (CIGR==1 ) {

display.clearDisplay();
cigsec();
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);
display.setTextSize(2);
display.setTextColor(WHITE);
  display.setCursor(75,15);
display.print("1");

display.display();

}
else if (CIGR==2) {
display.clearDisplay();
cigsec();
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);

display.setTextSize(2);
display.setTextColor(WHITE);
  display.setCursor(75,15);
display.print("2");
display.display();
}
else if (CIGR==3) {
display.clearDisplay();
cigsec();
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);

display.setTextSize(2);
display.setTextColor(WHITE);
  display.setCursor(75,15);
display.print("3");
display.display();

}
else if (CIGR==4) {
display.clearDisplay();
cigsec();
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);

display.setTextSize(2);
display.setTextColor(WHITE);
  display.setCursor(75,15);
display.print("4");
display.display();
}
else if (CIGR==5) {
display.clearDisplay();
cigsec();
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);

display.setTextSize(2);
display.setTextColor(WHITE);
  display.setCursor(75,15);
display.print("5");
display.display();
}
else if (CIGR==6) {
display.clearDisplay();
cigsec();
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);

display.setTextSize(2);
display.setTextColor(WHITE);
  display.setCursor(75,15);
display.print("6");
display.display();
}
else if (CIGR==7) {
display.clearDisplay();
cigsec();
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);

display.setTextSize(2);
display.setTextColor(WHITE);
  display.setCursor(75,15);
display.print("7");
display.display();
}
else if (CIGR==8) {
display.clearDisplay();
cigsec();
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);

display.setTextSize(2);
display.setTextColor(WHITE);
  display.setCursor(75,15);
display.print("8");
display.display();
}
else if (CIGR==9) {
display.clearDisplay();
cigsec();
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);

display.setTextSize(2);
display.setTextColor(WHITE);
  display.setCursor(75,15);
display.print("9");
display.display();
}

else if (CIGR==10) {
display.clearDisplay();
cigsec();
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);

display.setTextSize(2);
display.setTextColor(WHITE);
  display.setCursor(75,15);
display.print("10");
display.display();
}
else if (CIGR==11) {
display.clearDisplay();
cigsec();
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);

display.setTextSize(2);
display.setTextColor(WHITE);
  display.setCursor(75,15);
display.print("11");
display.display();
}
else if (CIGR==12) {
display.clearDisplay();
cigsec();
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);

display.setTextSize(2);
display.setTextColor(WHITE);
  display.setCursor(75,15);
display.print("12");
display.display();
}
else if (CIGR==13) {
display.clearDisplay();
cigsec();
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);

display.setTextSize(2);
display.setTextColor(WHITE);
  display.setCursor(75,15);
display.print("13");
display.display();
}
else if (CIGR==14) {
display.clearDisplay();
cigsec();
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);

display.setTextSize(2);
display.setTextColor(WHITE);
  display.setCursor(75,15);
display.print("14");
display.display();
}
else if (CIGR==15) {
display.clearDisplay();
cigsec();
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);

display.setTextSize(2);
display.setTextColor(WHITE);
  display.setCursor(75,15);
display.print("15");
display.display();
}


else if (CIGR==16) {
display.clearDisplay();
cigsec();
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);

display.setTextSize(2);
display.setTextColor(WHITE);
  display.setCursor(75,15);
display.print("16");
display.display();
}
else if (CIGR==17) {
display.clearDisplay();
cigsec();
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);

display.setTextSize(2);
display.setTextColor(WHITE);
  display.setCursor(75,15);
display.print("17");
display.display();
}
else if (CIGR==18) {
display.clearDisplay();
cigsec();
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);

display.setTextSize(2);
display.setTextColor(WHITE);
  display.setCursor(75,15);
display.print("18");
display.display();
}
else if (CIGR==19) {
display.clearDisplay();
cigsec();
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);

display.setTextSize(2);
display.setTextColor(WHITE);
  display.setCursor(75,15);
display.print("19");
display.display();
}
else if (CIGR==20) {
display.clearDisplay();
cigsec();
display.drawBitmap(
    (display.width()  - menuc_WIDTH ) ,
    (display.height() - menuc_HEIGHT) ,
   menuc, menuc_WIDTH, menuc_HEIGHT, 1);

display.setTextSize(2);
display.setTextColor(WHITE);
  display.setCursor(75,15);
display.print("20");
display.display();
}


 }

else if (mode == 15)
  {   
     moteurOpen();
          if (CIGR>0){
     CIGR= (cig - 1) ;
     }
    EEPROM_writeAnything(10, CIGR);
EEPROM_writeAnything(15, cs);
     display.clearDisplay();
      tDown2.run();
        tDown2.start();   
     
      display.setTextSize (1);
  display.setCursor ( 10,20);
  display.println("paquet ouvert");
   display.setCursor (10,0); 
   display.setTextSize (2);
   display.print(tDown2.getCurrentTime());
     //   go();  
        
display.display();

  
  }
}


void print_time3()
{
 //Serial.print("tDown2: ");
Serial.println(tDown2.getCurrentTime());
}
void print_time2()
{
//Serial.print("tDown: ");
//Serial.println(tDown.getCurrentTime());
}
void print_time1()
{
//Serial.print("tDown: ");
//Serial.println(tDown.getCurrentTime());
}
 void go(){
      if (rak == 0) {
        tUp.start();
      }
  else if ( rak > 0) { tUp.restart();}
   
    
    
 }
//==========================================================//
 void tDownComplete()
{
display.clearDisplay();
 tps= tps - 1 ;
  delay(500);
 
  display.display();

mode = 0;
rak = 1 ; 
}
 void tDown2Complete()
{  
moteurclose();
        display.clearDisplay();
  /*
  display.setTextSize (2); // définit la taille du texte sur 2 (grand)
  display.setTextColor (WHITE);
display.setCursor (5,10); // définit la position de début du texte sur column = 0 et row = 0
  display.print ("Fermeture!");
*/
  display.display();
tps=tps+1;
     
mode=0;

tUp.run();
tDown.run();
        tDown.start();
    go();
 

        
  //delay(4500);
 
      //     mode=0;
 // 


}
 
void print_none()
{
  Serial.print("tNone: millis(): ");
  Serial.println(millis());
}

//==========================================================//
static void moteurepen1(void)
{
    
Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
     Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
     
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  delay(500);   




  
}
void moteurclose(void)
{
      Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  delay(500);   
}

void tUpComplete()
{
 
}
static void go2 () {
if (tps > 0) {
      tDown.run();
    
    tDown.start();
    }}
//=======================================================
void cigsec () {
  if( cs == 0 ){
  
  display.setTextSize(2);
  display.setCursor(35,0);
  display.print ( "0");
}
else if ( cs == 1) {
display.setTextSize(2);
  display.setCursor(35,0);
  display.print ( "1");
  
}
else if ( cs == 2) {
display.setTextSize(2);
  display.setCursor(35,0);
  display.print ( "2");
  
}
else if ( cs == 3) {
display.setTextSize(2);
  display.setCursor(35,0);
  display.print ( "3");
  
}



else if ( cs == 4) {
display.setTextSize(2);
  display.setCursor(35,0);
  display.print ( "4");
  
}

else if ( cs == 5) {
display.setTextSize(2);
  display.setCursor(35,0);
  display.print ( "5");
  
}

else if ( cs == 6) {
display.setTextSize(2);
  display.setCursor(35,0);
  display.print ( "6");
  
}
else if ( cs == 7) {
display.setTextSize(2);
  display.setCursor(35,0);
  display.print ( "7");
  
}

else if ( cs == 8) {
display.setTextSize(2);
  display.setCursor(35,0);
  display.print ( "8");
  


  
}

}



void moteurOpen(void)
{
      Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  delay(500);   
}
//==========================================================//
