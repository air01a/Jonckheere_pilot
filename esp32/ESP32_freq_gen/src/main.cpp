#include "params.h"


/***********************************************************/
/*                   Manage si5351 command                 */
/***********************************************************/
void set_frequency(si5351PLL_t pll, uint8_t clock, FrequencyParams params, si5351RDiv_t r_div) {
#ifndef CLOCKSIMULATOR
  clockgen.setupPLL(pll,params.pll_mult,params.pll_num,params.pll_denom);
  clockgen.setupMultisynth(clock,pll, params.ms_divider, params.ms_num, params.ms_denom);
  clockgen.setupRdiv(clock, r_div);
  clockgen.enableOutputs(true);
#endif
  Serial.println("New frequency set"); Serial.println("");
}

void setADFrequencies(si5351RDiv_t rdiv){
  FrequencyParams params = ADFrequencies[freq_index];
  set_frequency(CLOCK_AD_PLL, CLOCK_AD_OUTPUT,  params, rdiv);
}

void setDECFrequencies(si5351RDiv_t rdiv){
  FrequencyParams params = ADFrequencies[freq_index];
  set_frequency(CLOCK_DEC_PLL, CLOCK_DEC_OUTPUT, params, rdiv);
}


//**********************************************************
//*                    Manage mode command                  
//**********************************************************
void setLunar(char * response)
{
  freq_index=2;
  setADFrequencies(SI5351_R_DIV_64);
    strcpy(response, "OK");

}

void setSidereal(char * response)
{
  freq_index=1;
  setADFrequencies(SI5351_R_DIV_64);
    strcpy(response, "OK");

}

void setSolar(char * response)
{
  freq_index=0;
  setADFrequencies(SI5351_R_DIV_64);
    strcpy(response, "OK");

}

/***********************************************************/
/*                    Manage speed command                 */
/***********************************************************/
void x1(char * response)
{
  r_div = SI5351_R_DIV_64;
  strcpy(response, "OK");
}

void x2(char * response)
{
  r_div = SI5351_R_DIV_32;
    strcpy(response, "OK");

}

void x4(char * response)
{
  r_div = SI5351_R_DIV_16;
    strcpy(response, "OK");

}

void x16(char * response)
{
  r_div = SI5351_R_DIV_4;
    strcpy(response, "OK");

}


void getParams(char *response) {
  const char* speed;

  switch(r_div) {
    case SI5351_R_DIV_4 :
      speed="_x16";
      break;
    case SI5351_R_DIV_16 :
      speed="_x4";
      break;
    case SI5351_R_DIV_32:
      speed="_x2";
      break;
    default:
      speed="_x1";
      break;

  }
  strcpy(response, "OK_");
  strcpy(response+3, frequencyName[freq_index]);
  strcpy(response+strlen(response),speed);
  
}
/***********************************************************/
/*               Manage button press command               */
/***********************************************************/
void ad_stop(char * response) {
  digitalWrite(DIR_AD_PIN,HIGH);
  setADFrequencies(SI5351_R_DIV_64);
    strcpy(response, "OK");

}

void ad_plus(char * response) {
  digitalWrite(DIR_AD_PIN,HIGH);
  setADFrequencies(r_div);
    strcpy(response, "OK");

}

void ad_minus(char * response) {
  digitalWrite(DIR_AD_PIN,LOW);
  setADFrequencies(r_div);
    strcpy(response, "OK");

}


void dec_stop(char * response) {
  //set_frequency(SI5351_R_DIV_64);
    strcpy(response, "OK");

}

void dec_plus(char * response) {
 // set_frequency();
  digitalWrite(DIR_DEC_PIN,HIGH);
  setDECFrequencies(r_div);
    strcpy(response, "OK");


}

void dec_minus(char * response) {
  digitalWrite(DIR_DEC_PIN,LOW);
  setDECFrequencies(r_div);
 // set_frequency();
   strcpy(response, "OK");

}

/***********************************************************/
/*                    Manage all command                   */
/***********************************************************/
void executeCommand(const char* incomingPacket, char * response) {
  Serial.println(incomingPacket);
    for (int i = 0; i < NUM_COMMANDS; i++) {
        if (strcmp(incomingPacket, commands[i].command) == 0) {
            Serial.println(commands[i].command);
            commands[i].function(response);
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
  char response[50];

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
  setSidereal(response);

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
  char response[100]; 
  // Vérification des paquets UDP entrants
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // Lecture du paquet
    int len = Udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
    }
    Serial.printf("Reçu %d bytes: %s\n", len, incomingPacket);
    executeCommand(incomingPacket, response);
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write((const uint8_t *)response, strlen(response));
    Udp.endPacket();
  }

  tcpClient = tcpServer.available();  // Vérifie si un client TCP est connecté
  if (tcpClient) {
        Serial.println("Client TCP connecté.");
        if (tcpClient.available()) {
            int len = tcpClient.readBytesUntil('\n', incomingPacket, sizeof(incomingPacket) - 1);
            incomingPacket[len] = 0;  // Terminer la chaîne
            Serial.printf("Reçu %d bytes via TCP: %s\n", len, incomingPacket);

            // Exécuter la commande correspondante (traitement TCP)
            executeCommand(incomingPacket, response);
            tcpClient.write(response); 
      }
  }
}