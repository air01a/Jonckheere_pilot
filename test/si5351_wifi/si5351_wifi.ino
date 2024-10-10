#include "si5351.h"


/***********************************************************/
/*                   Manage si5351 command                 */
/***********************************************************/
void set_frequency(si5351PLL_t pll, uint8_t clock, FrequencyParams params, int rdiv) {
#ifndef CLOCKSIMULATOR
  clockgen.setupPLL(pll,params.pll_mult,params.pll_num,params.pll_denom);
  clockgen.setupMultisynth(clock,pll, params.ms_divider, params.ms_num, params.ms_denom);
  clockgen.setupRdiv(clock, r_div);
  clockgen.enableOutputs(true);
#endif
  Serial.println("New frequency set"); Serial.println("");
}

void setADFrequencies(int rdiv){
  FrequencyParams params = ADFrequencies[freq_index];
  set_frequency(CLOCK_AD_PLL, CLOCK_AD_OUTPUT,  params, rdiv)
}

void setDECFrequencies(int rdiv){
  FrequencyParams params = ADFrequencies[freq_index];
  set_frequency(CLOCK_DEC_PLL, CLOCK_DEC_OUTPUT, params, rdiv)
}


//**********************************************************
//*                    Manage mode command                  
//**********************************************************
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

/***********************************************************/
/*                    Manage speed command                 */
/***********************************************************/
void x1(void)
{
  r_div = SI5351_R_DIV_64;
}

void x2(void)
{
  r_div = SI5351_R_DIV_32;
}

void x4(void)
{
  r_div = SI5351_R_DIV_16;
}

void x16(void)
{
  r_div = SI5351_R_DIV_4;
}

/***********************************************************/
/*               Manage button press command               */
/***********************************************************/
void ad_stop(void) {
  digitalWrite(DIR_AD_PIN,HIGH);
  setADFrequencies(SI5351_R_DIV_64);
}

void ad_plus(void) {
  digitalWrite(DIR_AD_PIN,HIGH);
  setADFrequencies(r_div);
}

void ad_minus(void) {
  digitalWrite(DIR_AD_PIN,LOW);
  setADFrequencies(r_div);
}


void dec_stop() {
  //set_frequency(SI5351_R_DIV_64);

}

void dec_plus(void) {
 // set_frequency();
  digitalWrite(DIR_DEC_PIN,HIGH);
  setDECFrequencies(r_div);


}

void dec_minus(void) {
  digitalWrite(DIR_DEC_PIN,LOW);
  setDECFrequencies(r_div);
 // set_frequency();
}


/***********************************************************/
/*               Manage Coupole                            */
/***********************************************************/

void cou_stop() {
  //set_frequency(SI5351_R_DIV_64);
  digitalWrite(DIR_COU1,LOW);
    digitalWrite(DIR_COU2,LOW);

}

void cou_plus(void) {
 // set_frequency();
  digitalWrite(DIR_COU1,HIGH);
    digitalWrite(DIR_COU2,LOW);

}

void dec_minus(void) {
  digitalWrite(DIR_COU1,LOW);
    digitalWrite(DIR_COU2,HIGH);
}



/***********************************************************/
/*                    Manage all command                   */
/***********************************************************/
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
    for(int i=0;i<NUM_COMMANDS;i++) {
      print(commands[i]); 
    }
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

  pinMode(DIR_AD_PIN, OUTPUT);  // Configure le PIN comme une sortie
  pinMode(DIR_DEC_PIN, OUTPUT);  // Configure le PIN comme une sortie

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