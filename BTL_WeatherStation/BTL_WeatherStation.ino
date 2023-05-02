#include <FreqCount.h>
#include <Adafruit_BMP085.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 6
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial mySerial(9,10); //chân TX, RX cho virtual terminal
LiquidCrystal_I2C lcd(0x38, 20, 8); //khai báo lcd

Adafruit_BMP085 bmp;

unsigned long count =0 , rain, speedWind;
float hume =0;
volatile byte interruption = LOW, aux = HIGH;
int contador, fila = 0, columna = 0;
float direction_send ;
int sensorPin = A0;
char direction[4];
String data;

//khai báo các phương thức
void printToLcdAndSendData();
void readDirection();
void readHumedad();
void SUBIR_PANTALLA();
void BAJAR_PANTALLA();
void readRainAndWind();

void setup(){
  dht.begin();
  Serial.begin(9600);
  mySerial.begin(9600);
  FreqCount.begin(1000);
  lcd.init();
  lcd.backlight();

  //các kết nối ngắt, làm tăng tín hiệu ở chân đầu vào
  pinMode(3, INPUT);
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(3), SUBIR_PANTALLA, RISING);
  attachInterrupt(digitalPinToInterrupt(2), BAJAR_PANTALLA, RISING);

  //khởi tạo bmp
  if(!bmp.begin()){
    // mySerial.println("Khong the tim thay BMP085 sensor, kiem tra ket nói");
    while(1){}
  }
  pinMode(4, OUTPUT);
}

void loop(){
  readRainAndWind();
  readDirection();
  readHumedad();
  
  if(aux != interruption){
    lcd.clear();
  }

  printToLcdAndSendData();
  Serial.println(data);
  mySerial.println("Send data to server successful");
  mySerial.print("Data: ");
  mySerial.println(data);
}

void readDirection(){
  int veleta = 0;
  veleta = analogRead(sensorPin);
  //phân loại hướng gió
  // mySerial.println(veleta);
  if (veleta == 238){
    strcpy(direction, "N"); //North = 0
    direction_send = 0;
  }
  if (veleta == 618){
    strcpy(direction, "NNE"); //North - north east
    direction_send = 22.5;
  }
  if (veleta == 563){
    strcpy(direction, "NE"); //north 
    direction_send = 45 ;
  }
  if (veleta == 940){
    strcpy(direction, "ENE"); //east north east
    direction_send = 67.5;
  }
  if (veleta == 931){
    strcpy(direction, "E"); //east
    direction_send = 90;
  }
  if (veleta == 958){
    strcpy(direction, "EES"); //east south east
    direction_send = 112.5;
  }
  if (veleta == 839){
    strcpy(direction, "SE"); //south east
    direction_send = 135;
  }
  if (veleta == 897){
    strcpy(direction, "SSE"); //south south east
    direction_send = 157.5;
  }
  if (veleta == 737){
    strcpy(direction, "S"); // south
    direction_send = 180;
  }
  if (veleta == 779){
    strcpy(direction, "SSW"); //south south west
    direction_send = 202.5;
  }
  if (veleta == 394){
    strcpy(direction, "SW"); //south west
    direction_send = 225;
  }
  if (veleta == 424){
    strcpy(direction, "WWS"); //wes tsouth west
    direction_send = 247.5;
  }
  if (veleta == 79){
    strcpy(direction, "W"); //west
    direction_send = 270;
  }
  if (veleta == 196){
    strcpy(direction, "WWN");
    direction_send = 292.5;
  }
  if (veleta == 137){
    strcpy(direction, "NW");
    direction_send = 315;
  }
  if (veleta == 321){
    strcpy(direction, "NNW");
    direction_send = 337.5;
  }
}

void readHumedad(){
  // delay(1000);
  hume = dht.readHumidity();
  mySerial.println(hume);
}

//ngắt kết nối ở chân 3 để giảm thông tin hiển thị
void SUBIR_PANTALLA() {
  interruption = !interruption;
  contador = contador - 1;
}

//ngắt kết nối ở chân 2 để tăng thông tin hiển thị
void BAJAR_PANTALLA() {
  interruption = !interruption;
  contador = contador + 1;
}

void readRainAndWind(){
  digitalWrite(4, HIGH);
  delay(3000);
  if (FreqCount.available()) {
    count = FreqCount.read();
  }

  if (count >= 0 && count < 5)
    rain = 0;
  if (count >= 25 && count < 30)
    rain = 10;
  if (count >= 54 && count < 60)
    rain = 20;
  if (count >= 80 && count < 85)
    rain = 30;
  if (count >= 109 && count < 115)
    rain = 40;
  if (count >= 135 && count < 140)
    rain = 50;
  if (count >= 164 && count < 170)
    rain = 60;
  if (count >= 190 && count < 195)
    rain = 70;
  if (count >= 218 && count < 223)
    rain = 80;
  if (count >= 245 && count < 250)
    rain = 90;
  if (count >= 273 && count < 278)
    rain = 100;

  //function to read the anemometer
  digitalWrite(4, LOW);
  delay(3000);
  if (FreqCount.available()) {
    speedWind = FreqCount.read();
    speedWind = speedWind * 2.5;
  }    
  
}

void printToLcdAndSendData(){
  lcd.clear();
  data = "";
  aux = interruption;
  fila = contador;

  // print the temperature
  data += String(bmp.readTemperature()) +",";
  if (fila >= 0 && fila < 4) {
    if (fila == 2 || fila == 3)
      columna = -4;
    else
      columna = 0;

    lcd.setCursor(columna, fila);
    lcd.print("Temperat=");
    lcd.print(bmp.readTemperature());
    lcd.print("*C");
    
  }

  // print the pressure
  data += String(bmp.readPressure()) +",";
  fila = contador + 1;
  if (fila >= 0 && fila < 4) {
    if (fila == 2 || fila == 3)
      columna = -4;
    else
      columna = 0;

    lcd.setCursor(columna, fila);
    lcd.print("Press=");
    lcd.print(bmp.readPressure());
    lcd.print("Pa");

  }

  // print the altitude
  fila = contador + 2;
  if (fila >= 0 && fila < 4) {
    if (fila == 2 || fila == 3)
      columna = -4;
    else
      columna = 0;

    lcd.setCursor(columna, fila);
    lcd.print("Altit=");
    lcd.print(bmp.readAltitude());
    lcd.print("m");
  }

   // print the speed of the wind
  data += String(speedWind) +",";
  fila = contador + 3;
  if (fila >= 0 && fila < 4) {
    if (fila == 2 || fila == 3)
      columna = -4;
    else
      columna = 0;

    lcd.setCursor(columna, fila);
    lcd.print("speedWind=");
    lcd.print(speedWind);
    lcd.print("km/h");
  }

  // print the rain
  data += String(rain) +",";
  fila = contador + 4;
  if (fila >= 0 && fila < 4) {
    if (fila == 2 || fila == 3)
      columna = -4;
    else
      columna = 0;

    lcd.setCursor(columna, fila);

    lcd.print("rain=");
    lcd.print(rain);
    lcd.print("mm/Hr");
  }

  // print the direction
  data += String(direction_send) +",";
  fila = contador + 5;
  if (fila >= 0 && fila < 4) {
    if (fila == 2 || fila == 3)
      columna = -4;
    else
      columna = 0;

    lcd.setCursor(columna, fila);
    lcd.print("direction=");
    lcd.print(direction);
  }

  // print the humidity
  data += String(hume) ;
  fila = contador + 6;
  if (fila >= 0 && fila < 4) {
    if (fila == 2 || fila == 3)
      columna = -4;
    else
      columna = 0;
    lcd.setCursor(columna, fila);
    lcd.print("humedad=");
    lcd.print(hume);
    lcd.print("%");
  }
  delay(500);
}

