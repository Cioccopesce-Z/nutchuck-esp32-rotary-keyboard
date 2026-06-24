#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
//#include <BleKeyboard.h>

//BleKeyboard bleKeyboard("ESP32-C3 Keyboard", "ESP32", 100);

// ===== PIN =====
#define JOY_X 2     // D0
#define JOY_Y 3     // D1
#define BTN_NEXT 21 // D6
#define BTN_SELECT 20 // D7

// ===== PARAMETRI =====
#define SAMPLES 8
#define DEADZONE 120  // mV circa centro joystick
#define CENTER 1650   // mV centro joystick

// ===== GHIERA =====
char ring[8][7] = {
  {'A','B','C','è','.',';',':'},  // ↑
  {'E','F','D','(',')','[',']'},  // →
  {'I','H','G','{','}','+','-'},  // ↓
  {'M','N','J','L','K','*','/'},  // ←
  {'O','P','S','R','Q','@','#'},  // ↗
  {'T','U','V','X','Y','=','?'},  // ↘
  {' ','Z','0','1','2','3','4'},  // ↙
  {',','4','5','6','7','8','9'}   // ↖
};

// ===== VARIABILI =====
int ringIndex = 0;
int lastDir = -1;

Adafruit_MPU6050 mpu;


// ===== FUNZIONI UTILI =====
void handleMouse() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  int moveX = g.gyro.z * 3;
  int moveY = g.gyro.x * 3;

  if (abs(moveX) < 3) moveX = 0;
  if (abs(moveY) < 3) moveY = 0;

  if (moveX != 0 || moveY != 0) {
    Serial.print("[MOUSE MOVE] X=");
    Serial.print(moveX);
    Serial.print(" Y=");
    Serial.println(moveY);
    //bleKeyboard.mouseMove(moveX, moveY);
  }
}


int readAxis(int pin){
  long sum = 0;
  for(int i=0;i<SAMPLES;i++){
    sum += analogReadMilliVolts(pin);
    delayMicroseconds(200);
  }
  return sum / SAMPLES;
}

int getDirection(int x, int y){
  int dx = x - CENTER;
  int dy = y - CENTER;
  
  if(abs(dx)<DEADZONE && abs(dy)<DEADZONE) return -1; // centro, nessuna direzione

  // Determina ottante (0..7)
  float angle = atan2(-dy, dx) * 180.0 / 3.14159; // -180..180
  if(angle < 0) angle += 360;

  // Mappa angolo a direzione 0..7
  int dir = ((int)((angle + 22.5)/45)) % 8;
  return dir;
}

int ringSize(int dir){
  for(int i=0;i<5;i++){
    if(ring[dir][i] == 0) return i;
  }
  return 5;
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);

  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);

  Wire.begin(); // SDA, SCL
  //bleKeyboard.begin();
  //Serial.println("Bluetooth Keyboard avviata");


  if (!mpu.begin()) {
    Serial.println("MPU6050 NON TROVATO");
    while (1);
  }

  Serial.println("Ghiera + MPU6050 pronti");
}


// ===== LOOP =====
void loop() {
  start: ;

  //if (!bleKeyboard.isConnected()) goto start;

  int x = readAxis(JOY_X);
  int y = readAxis(JOY_Y);

  int dir = getDirection(x, y);
  if(dir != -1 && dir != lastDir){
    ringIndex = 0;
    lastDir = dir;
  }

  bool nextPressed = digitalRead(BTN_NEXT) == LOW;
  bool selectPressed = digitalRead(BTN_SELECT) == LOW;

  if(nextPressed && dir != -1){
    ringIndex = (ringIndex + 1) % ringSize(dir);
    delay(200);
  }

  if (digitalRead(BTN_NEXT) == LOW && digitalRead(BTN_SELECT) == LOW) {

    int i = 0;

    // finché entrambi sono premuti
    while (digitalRead(BTN_NEXT) == LOW && digitalRead(BTN_SELECT) == LOW) {
      delay(1);
      i++;

      if (i >= 400) {          // pressione lunga
        Serial.println("ENTER");
        //bleKeyboard.write(KEY_RETURN);
        // aspetta rilascio
        while (digitalRead(BTN_NEXT) == LOW || digitalRead(BTN_SELECT) == LOW);
        delay(100);
        dir =-1;
        goto start;
      }
    }

  
    Serial.println("BACKSPACE");
    //bleKeyboard.write(KEY_BACKSPACE);
    delay(100);
    dir =-1;
  }




  if(selectPressed && dir != -1){
    char c = ring[dir][ringIndex];
    Serial.print("[KEY] ");
    Serial.println(c);
    //bleKeyboard.print(c); 
    delay(300);
  }

  if(dir != -1){
    Serial.print("Dir:");
    Serial.print(dir);
    Serial.print(" Lettera:");
    Serial.println(ring[dir][ringIndex]);
  }

  handleMouse();   // <<< MPU sempre attivo
  //bleKeyboard.releaseAll();
  delay(50);
}

