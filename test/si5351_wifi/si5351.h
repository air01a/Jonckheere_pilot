#include <Wire.h>
#include "Adafruit_SI5351.h"
#include <WiFi.h>     // Pour ESP32
#include <WiFiUdp.h>

// Paramètres WiFi
//const char *ssid = "ArduinoAP";
//const char *password = "12345678";
#define SSID "ARDUINOAP"
#define PASSWORD "12345678"
#define LOCALPORT 4000

#define NUM_FREQUENCIES 3
#define CLOCK_OUTPUT 2
#define CLOCK_PLL SI5351_PLL_A

#define NUM_COMMANDS 7

void setLunar(void);
void setSidereal(void);
void setSolar(void);
void x1(void);
void x2(void);
void x4(void);
void x16(void);

// Configuration de l'IP statique
IPAddress local_IP(192, 168, 1, 1);        // IP fixe
IPAddress gateway(192, 168, 1, 1);         // Passerelle
IPAddress subnet(255, 255, 255, 0);        // Masque de sous-réseau

// Configuration UDP
WiFiUDP Udp;
char incomingPacket[255];  // Buffer pour les paquets entrants
WiFiServer tcpServer(LOCALPORT);        // Serveur TCP écoutant sur le port 4000
WiFiClient tcpClient;              // Client TCP

Adafruit_SI5351 clockgen = Adafruit_SI5351();

typedef struct {
    uint8_t pll_mult;    // a
    uint32_t pll_num;    // b
    uint32_t pll_denom;  // c
    uint32_t ms_divider; // d
    uint32_t ms_num;     // e
    uint32_t ms_denom;   // f
    uint8_t r_div;       // R
} FrequencyParams;
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


Command commands[NUM_COMMANDS] = {
    {"sidereal", setSidereal},
    {"lunar", setLunar},
    {"solar", setSolar},
    {"x2",x2},
    {"x1",x1},
    {"x4",x4},
    {"x16",x16}
};

