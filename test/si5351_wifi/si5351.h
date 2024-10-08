#include "Adafruit_SI5351.h"

#include <Wire.h>
#include <WiFi.h>     // Pour ESP32
#include <WiFiUdp.h>

// Paramètres WiFi
#define SSID "JONCKHEEREMOUNT"
#define PASSWORD "12345678"
#define LOCALPORT 4000
//#define CLOCKSIMULATOR 1


#define NUM_FREQUENCIES 3
// Output for AD and DEV
#define CLOCK_AD_OUTPUT 2
#define CLOCK_DEC_OUTPUT 0

// PLL managing AD and DEC
#define CLOCK_AD_PLL SI5351_PLL_A
#define CLOCK_DEC_PLL SI5351_PLL_B

#define NUM_COMMANDS 16
// PIN For orientation 
#define DIR_AD_PIN 15
#define DIR_DEC_PIN 16
#define DIR_COU1  25
#define DIR_COU2 26

// List of available commands
void setLunar(void);
void setSidereal(void);
void setSolar(void);
void x1(void);
void x2(void);
void x4(void);
void x16(void);
void ad_stop(void);
void ad_plus(void);
void ad_minus(void);
void dec_stop(void);
void dec_minus(void);
void dec_plus(void);

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

//SI5351 Configuration parameters
typedef struct {
    uint8_t pll_mult;    // a
    uint32_t pll_num;    // b
    uint32_t pll_denom;  // c
    uint32_t ms_divider; // d
    uint32_t ms_num;     // e
    uint32_t ms_denom;   // f
    uint8_t r_div;       // R
} FrequencyParams;

// Si5351 divider
si5351RDiv_t r_div = SI5351_R_DIV_64;
int freq_index = 0;

// List of base frequencies form mode
FrequencyParams ADFrequencies[NUM_FREQUENCIES] = {
    // Fréquence 1
    {24, 23607, 997652, 697, 0, 1, SI5351_R_DIV_64}, // "sidereal"
    // Fréquence 2
    {24, 6293, 234048, 699, 0, 1, SI5351_R_DIV_64}, // "solar"
    // Fréquence 3
    {24, 4303, 526562, 723, 0, 1, SI5351_R_DIV_64}  // "lunar"
};


// wifi Command structure
typedef void (*CommandFunction)();  // Définir un type pour le pointeur de fonction

typedef struct {
    const char* command;
    CommandFunction function;
} Command;

char *frequencyName = ["solar","sidereal","lunar"]

Command commands[NUM_COMMANDS] = {
    {"AD+",ad_plus},
    {"AD-",ad_minus},
    {"AD",ad_stop},
    {"DEC+",dec_plus},
    {"DEC-",dec_minus},
    {"DEC",dec_stop},
    {"sidereal", setSidereal},
    {"lunar", setLunar},
    {"solar", setSolar},
    {"x1",x1},
    {"x2",x2},
    {"x4",x4},
    {"x16",x16},
    {"COU+",cou_plus},
    {"COU-",cou_minus},
    {"COUSTOP",cou_stop},

};

