#include <Wire.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

SoftwareSerial SerialESP(D7, D8);   // (RX, TX)

//------------------------ Set Wifi ---------------------------
#define FIREBASE_HOST "volumesampah-default-rtdb.firebaseio.com"                                  
#define FIREBASE_AUTH "fyoaS1NVqTlOpIP1uOU81ul9RkQLYKonhfmKnK4U"                                  
#define WIFI_SSID "CA2019"
#define WIFI_PASSWORD "Tekkom2019"
 

String dataIn;
String dt[10];
int i;
boolean parsing = false;

float jarak1, jarak2;

void setup() {
  // put your setup code here, to run once:
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  Serial.begin(9600);
  SerialESP.begin(9600);
  setWifi();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("check");
  fireBase();
  while (SerialESP.available() > 0) {

    char inChar = (char)SerialESP.read();
    dataIn += inChar;

    if (inChar == '\n') {
      parsing = true;
    }

  }
  if (parsing) {
    parsingData();
    parsing = false;
    dataIn = "";

  }


}

//****************************** fungsi **************************
void setWifi() {
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}

void parsingData() {
  int j = 0;

  //kirim data yang telah diterima sebelumnya
  Serial.print("data masuk : ");
  Serial.println(dataIn);

  //inisialisasi variabel, (reset isi variabel)
  dt[j] = "";
  //proses parsing data
  for (i = 1; i < dataIn.length(); i++) {
    //pengecekan tiap karakter dengan karakter (#) dan (,)
    if ((dataIn[i] == '#') || (dataIn[i] == ','))
    {
      //increment variabel j, digunakan untuk merubah index array penampung
      j++;
      dt[j] = "";     //inisialisasi variabel array dt[j]
    }
    else
    {
      //proses tampung data saat pengecekan karakter selesai.
      dt[j] = dt[j] + dataIn[i];
    }
  }
  jarak1 = dt[0].toFloat();
  jarak2 = dt[1].toFloat();
  jarak1 = map(jarak1, 42, 0, 0, 100);
  jarak2 = map(jarak2, 42, 0, 0, 100);
  jarak1 = constrain(jarak1, 0, 100);
  jarak2 = constrain(jarak2, 0, 100);


  //kirim data hasil parsing
  Serial.print("data 1 : ");
  Serial.println(jarak1);
  Serial.print("data 2 : ");
  Serial.println(jarak2);

  // kirim data ke firebase
  Firebase.setFloat("IOT/jarak1", jarak1);
  // handle error
  if (Firebase.failed()) {
    Serial.print("setting /jarak1 failed:");
    Serial.println(Firebase.error());
    return;
  }
  Firebase.setFloat("IOT/jarak2", jarak2);
  // handle error
  if (Firebase.failed()) {
    Serial.print("setting /Arus failed:");
    Serial.println(Firebase.error());
    return;
  }
}


void fireBase() {
  String dataB1 = Firebase.getString("IOT/Box1");
  String dataB2 = Firebase.getString("IOT/Box2");
  Serial.println("Data BOX1 = ");
  Serial.println(dataB1);
  Serial.println("Data BOX2 = ");
  Serial.println(dataB2);

  if (dataB1 == "true") {
    digitalWrite(D5, 1);
    digitalWrite(D2, 1);
  }
  if (dataB1 == "false") {
    digitalWrite(D5, 0);
    digitalWrite(D2, 0);
  }
  if (dataB2 == "true") {
    digitalWrite(D6, 1);
    digitalWrite(D3, 1);
  }
  if (dataB2 == "false") {
    digitalWrite(D6, 0);
    digitalWrite(D3, 0);
  }
}
