#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>

#define RST_PIN 9  // Pin de reset
#define SS_PIN 10  // Pin del SS (Slave Select)

MFRC522 mfrc522(SS_PIN, RST_PIN); // Inicialitza el lector RFID

// WiFi
const char* ssid = "nom_de_la_teva_xarxa_wifi";
const char* password = "contrasenya_de_la_teva_xarxa_wifi";

// Email
const char* host = "smtp.gmail.com";
const int port = 465;
const char* emailFrom = "tucorreu@gmail.com";
const char* passwordEmail = "la_teu_contrasenya_de_correu";
const char* emailTo = "destinatari@gmail.com";
const char* subject = "Accés Autoritzat";

WiFiClient client;

void setup() {
  Serial.begin(9600); // Inicialitza la comunicació serial
  SPI.begin(); // Inicialitza la comunicació SPI
  mfrc522.PCD_Init(); // Inicialitza el lector RFID

  // Connecta't a la xarxa WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connectant a la xarxa WiFi...");
  }
  Serial.println("Connectat a la xarxa WiFi");
}

void loop() {
  // Intenta llegir una targeta RFID present
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Verifica si l'UID coincideix amb l'autoritzat
    if (checkUidAuthorization(mfrc522.uid.uidByte, mfrc522.uid.size)) {
      sendEmail(subject, "La targeta RFID autoritzada ha estat utilitzada.");
      delay(1000); // Espera 1 segon abans de tornar a llegir
    } else {
      Serial.println("Accés no autoritzat.");
    }
  }
}

// Funció per verificar si l'UID coincideix amb l'autoritzat
bool checkUidAuthorization(byte *uid, byte uidSize) {
  // Aquí pots implementar la teva lògica per verificar l'UID autoritzat
  // Per exemple, comparar l'UID amb un UID autoritzat emmagatzemat en una matriu
  return true; // Retorna true si és autoritzat, false si no ho és
}

// Funció per enviar un correu electrònic
void sendEmail(const char* subject, const char* message) {
  client.connect(host, port);
  client.println("EHLO example.com");
  client.println("AUTH LOGIN");
  client.println(base64::encode(emailFrom));
  client.println(base64::encode(passwordEmail));
  client.println("MAIL FROM:<" + String(emailFrom) + ">");
  client.println("RCPT TO:<" + String(emailTo) + ">");
  client.println("DATA");
  client.println("From: " + String(emailFrom));
  client.println("To: " + String(emailTo));
  client.println("Subject: " + String(subject));
  client.println();
  client.println(message);
  client.println(".");
  client.println("QUIT");
  client.stop();
}