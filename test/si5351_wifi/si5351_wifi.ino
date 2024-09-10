#include "si5351.h"



void set_frequency() {
  FrequencyParams params = frequencies[freq_index];
#ifndef CLOCKSIMULATOR
  clockgen.setupPLL(CLOCK_PLL,params.pll_mult,params.pll_num,params.pll_denom);
  clockgen.setupMultisynth(CLOCK_OUTPUT,CLOCK_PLL, params.ms_divider, params.ms_num, params.ms_denom);
  clockgen.setupRdiv(CLOCK_OUTPUT, r_div);
  clockgen.enableOutputs(true);
#endif
  Serial.println("New frequency set"); Serial.println("");

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
  //set_frequency();
}

void x2(void)
{
  r_div = SI5351_R_DIV_32;
  //set_frequency();
}

void x4(void)
{
  r_div = SI5351_R_DIV_16;
  //set_frequency();
}

void x16(void)
{
  r_div = SI5351_R_DIV_4;
  //set_frequency();
}

void ad_stop(void) {
  
}

void ad_plus(void) {
  set_frequency();
}

void ad_minus(void) {
  set_frequency();
}

void ad_plus(void) {
  set_frequency();
}

void dec_stop() {

}

void dec_plus(void) {
  set_frequency();
}

void dec_minus(void) {
  set_frequency();
}


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

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
  Serial.begin(57600);
  Serial.println("Si5351 Clockgen Test"); Serial.println("");


#ifndef CLOCKSIMULATOR
  /* Initialise the sensor */
  if (clockgen.begin() != ERROR_NONE)
  {
    /* There was a problem detecting the IC ... check your connections */
    Serial.print("Ooops, no Si5351 detected ... Check your wiring or I2C ADDR!");
   // while(1);
  }
#endif

  Serial.println("OK!");
  setSidereal();

  // Configuration de l'IP statique pour le point d'accès
  if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
    Serial.println("Erreur lors de la configuration de l'IP statique");
  }

  // Initialisation du WiFi en mode AP (point d'accès)
  Serial.println();
  Serial.println("Configuration de l'Access Point...");
  WiFi.softAP(SSID, PASSWORD);

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Initialisation de l'écoute UDP
  Udp.begin(LOCALPORT);
  Serial.printf("Maintenant en écoute sur l'UDP port %d\n", LOCALPORT);

  tcpServer.begin();
  Serial.printf("Maintenant en écoute sur l'TCP port %d\n", LOCALPORT);


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

  tcpClient = tcpServer.available();  // Vérifie si un client TCP est connecté
  if (tcpClient) {
        Serial.println("Client TCP connecté.");
        if (tcpClient.available()) {
            int len = tcpClient.readBytesUntil('\n', incomingPacket, sizeof(incomingPacket) - 1);
            incomingPacket[len] = 0;  // Terminer la chaîne
            Serial.printf("Reçu %d bytes via TCP: %s\n", len, incomingPacket);

            // Exécuter la commande correspondante (traitement TCP)
            executeCommand(incomingPacket);
      }
  }
}