#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

#define VERT_PIN A0
#define HORZ_PIN A1
#define SEL_PIN  2
#define LED_PIN  4

int x = 6, xx=6;
int y = 0;
int z = 0;
int matrixx = 0;
int matrixy = 0;
int time = 0;
int selx=0;

char a[]="abcde";
char f[]="fghij";
char k[]="klmno";
char p[]="pqrst";
char u[]="uvxyz";


void setup() {
  lcd.begin(16, 2);
  pinMode(VERT_PIN, INPUT);
  pinMode(HORZ_PIN, INPUT);
  pinMode(SEL_PIN, INPUT);
  pinMode(4, OUTPUT);
 
 // indice 0 = spazio, indice 1 = 'a', ecc..
 draw();
}

// the loop function runs over and over again forever
void loop() {
  
}


void draw(){
  while(z==0){
  if(z==1)  {break;}
  lcd.setCursor(6,0);
  lcd.print("afkpu");
  lcd.setCursor(6,2);
  lcd.print("05.(+");
  int horz = analogRead(HORZ_PIN);
  int vert = analogRead(VERT_PIN);
  if (vert < 300) {
    y = 2; //basso
    delay(time);
  }
  if (vert > 700) {
    y = 0; //alto
    delay(time);
  }
  if (horz > 700) {
    x += -1; //sinistra
    delay(time);
  }
  if (horz < 300) {
    x += 1; //destra
    delay(time);
  }
  if (digitalRead(SEL_PIN) == HIGH) {
    z=1;
    delay(time);
  }
  if(x>=11) x=6;
  if(x<=5) x=10;
  delay(100);
  lcd.setCursor(x,y);
  lcd.print(" ");
  delay(100);
  }
   subroutine();
}


void subroutine(){
  digitalWrite(LED_PIN, HIGH);
  lcd.setCursor(6,2); lcd.print("         ");
  lcd.setCursor(6,0); lcd.print("         ");
  while(z==1){
    if(x==6 && y==0){lcd.setCursor(6,0); lcd.print("abcde");}
    if(x==7 && y==0){lcd.setCursor(6,0); lcd.print("fghij");}
    if(x==8 && y==0){lcd.setCursor(6,0); lcd.print("klmno");}
    if(x==9 && y==0){lcd.setCursor(6,0); lcd.print("pqrst");}
    if(x==10 && y==0){lcd.setCursor(6,0); lcd.print("uvxyz");}

    if(x==6 && y==2){lcd.setCursor(6,2); lcd.print("01234");}
    if(x==7 && y==2){lcd.setCursor(6,2); lcd.print("56789");}
    if(x==8 && y==2){lcd.setCursor(6,2); lcd.print(".,;:");}
    if(x==9 && y==2){lcd.setCursor(6,2); lcd.print("()[]{}");}
    if(x==10 && y==2){lcd.setCursor(6,2); lcd.print("+-/\$%&");}


  int horz = analogRead(HORZ_PIN);
  if (horz > 700) {
    xx += -1; //sinistra
    delay(time);
  }
  if (horz < 300) {
    xx += 1; //destra
    delay(time);
  }
  if (digitalRead(SEL_PIN) == HIGH) {
    z=1;
    delay(time);
  }
    if (digitalRead(SEL_PIN) == HIGH) {z==0; matrixx=x; matrixy=y; selx=xx;}


  delay(100);
  lcd.setCursor(xx,y);
  lcd.print(" ");
  delay(100);
  }
}