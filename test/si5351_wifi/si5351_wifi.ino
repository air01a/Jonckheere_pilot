#include <Wire.h>
#include "Adafruit_SI5351.h"
#include <WiFi.h>     // Pour ESP32
#include <WiFiUdp.h>


Adafruit_SI5351 clockgen = Adafruit_SI5351();
// Paramètres WiFi
const char *ssid = "ArduinoAP";
const char *password = "12345678";

// Configuration de l'IP statique
IPAddress local_IP(192, 168, 1, 1);        // IP fixe
IPAddress gateway(192, 168, 1, 1);         // Passerelle
IPAddress subnet(255, 255, 255, 0);        // Masque de sous-réseau

// Configuration UDP
WiFiUDP Udp;
unsigned int localUdpPort = 4000;  // Port pour recevoir les paquets UDP
char incomingPacket[255];  // Buffer pour les paquets entrants

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/

void setLunar(void)
{
  clockgen.setupPLL(SI5351_PLL_A,24,4303,526562);
  clockgen.setupMultisynth(2, SI5351_PLL_A, 723, 0, 1);
  clockgen.setupRdiv(2, SI5351_R_DIV_64);
  clockgen.enableOutputs(true);
}

void setSidereal(void)
{
  clockgen.setupPLL(SI5351_PLL_A,24,23607,997652);
  clockgen.setupMultisynth(2, SI5351_PLL_A, 697, 0, 1);
  clockgen.setupRdiv(2, SI5351_R_DIV_64);
  clockgen.enableOutputs(true);
}

void setSolar(void)
{
  clockgen.setupPLL(SI5351_PLL_A,24,6293,234048);
  clockgen.setupMultisynth(2, SI5351_PLL_A, 699, 0, 1);
  clockgen.setupRdiv(2, SI5351_R_DIV_64);
  clockgen.enableOutputs(true);
}

void setup(void)
{
  Serial.begin(57600);
  Serial.println("Si5351 Clockgen Test"); Serial.println("");

  /* Initialise the sensor */
  if (clockgen.begin() != ERROR_NONE)
  {
    /* There was a problem detecting the IC ... check your connections */
    Serial.print("Ooops, no Si5351 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  Serial.println("OK!");
  setSidereal();

  // Configuration de l'IP statique pour le point d'accès
  if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
    Serial.println("Erreur lors de la configuration de l'IP statique");
  }

  // Initialisation du WiFi en mode AP (point d'accès)
  Serial.println();
  Serial.println("Configuration de l'Access Point...");
  WiFi.softAP(ssid, password);

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Initialisation de l'écoute UDP
  Udp.begin(localUdpPort);
  Serial.printf("Maintenant en écoute sur l'UDP port %d\n", localUdpPort);

}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void)
{
  // Vérification des paquets UDP entrants
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // Lecture du paquet
    int len = Udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
    }
    Serial.printf("Reçu %d bytes: %s\n", len, incomingPacket);

    // Vérification du contenu du paquet
    if (strcmp(incomingPacket, "sidereal") == 0) {
      Serial.println("Chaîne 'sidereal' reçue. Changement de fréquence...");
      // Changer la fréquence, par exemple 20 MHz
      setSidereal();
    } else if (strcmp(incomingPacket, "lunar") == 0) {
      setLunar();
    } else if (strcmp(incomingPacket, "solar") == 0){
      setSolar(); 
    }
  }
}