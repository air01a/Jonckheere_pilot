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


typedef struct {
    uint8_t pll_mult;    // a
    uint32_t pll_num;    // b
    uint32_t pll_denom;  // c
    uint32_t ms_divider; // d
    uint32_t ms_num;     // e
    uint32_t ms_denom;   // f
    uint8_t r_div;       // R
} FrequencyParams;

#define NUM_FREQUENCIES 3
#define CLOCK_OUTPUT 2
#define CLOCK_PLL SI5351_PLL_A

si5351RDiv_t r_div = SI5351_R_DIV_64;
int freq_index = 0;

FrequencyParams frequencies[NUM_FREQUENCIES] = {
    // Fréquence 1
    {24, 23607, 997652, 697, 0, 1, SI5351_R_DIV_64}, // "sidereal"
    // Fréquence 2
    {24, 6293, 234048, 699, 0, 1, SI5351_R_DIV_64}, // "solar"
    // Fréquence 3
    {24, 4303, 526562, 723, 0, 1, SI5351_R_DIV_64}  // "lunar"
};

typedef void (*CommandFunction)();  // Définir un type pour le pointeur de fonction

typedef struct {
    const char* command;
    CommandFunction function;
} Command;


/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/



void set_frequency() {
  FrequencyParams params = frequencies[freq_index];
  clockgen.setupPLL(CLOCK_PLL,params.pll_mult,params.pll_num,params.pll_denom);
  clockgen.setupMultisynth(CLOCK_OUTPUT,CLOCK_PLL, params.ms_divider, params.ms_num, params.ms_denom);
  clockgen.setupRdiv(CLOCK_OUTPUT, r_div);
  clockgen.enableOutputs(true);

}

void setLunar(void)
{
  freq_index=2;
  set_frequency();
}

void setSidereal(void)
{
  freq_index=1;
  set_frequency();
}

void setSolar(void)
{
  freq_index=0;
  set_frequency();
}

void x1(void)
{
  r_div = SI5351_R_DIV_64;
  set_frequency();
}

void x2(void)
{
  r_div = SI5351_R_DIV_32;
  set_frequency();
}

void x4(void)
{
  r_div = SI5351_R_DIV_16;
  set_frequency();
}

void x16(void)
{
  r_div = SI5351_R_DIV_4;
  set_frequency();
}

#define NUM_COMMANDS 7

Command commands[NUM_COMMANDS] = {
    {"sidereal", setSidereal},
    {"lunar", setLunar},
    {"solar", setSolar},
    {"x2",x2},
    {"x1",x1},
    {"x4",x4},
    {"x16",x16}
};

void executeCommand(const char* incomingPacket) {
  Serial.println(incomingPacket);
    for (int i = 0; i < NUM_COMMANDS; i++) {
        if (strcmp(incomingPacket, commands[i].command) == 0) {
            Serial.println(commands[i].command);
            commands[i].function();
            return;
        }
    }
    Serial.println("Chaîne inconnue reçue.");
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
    executeCommand(incomingPacket);
  }
}