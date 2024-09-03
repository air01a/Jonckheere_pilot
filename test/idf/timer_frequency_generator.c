#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "driver/gptimer.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "rom/gpio.h"


#define GPIO_OUTPUT_PIN       15  // GPIO utilisé pour générer le signal


#define LUNAR_TICKS 19736
#define SOLAR_TICKS 19066
#define SIDERAL_TICKS 19014

volatile bool toggle_flag = false;

static bool IRAM_ATTR timer_callback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {
    portDISABLE_INTERRUPTS();

    toggle_flag = !toggle_flag;
    gpio_set_level(GPIO_OUTPUT_PIN, toggle_flag);
    portENABLE_INTERRUPTS();

    // Nettoie la pile interruption
    return true; // true pour réactiver l'int
}

void init_timer() {
    // Configurer le GPIO en mode sortie
    gpio_pad_select_gpio(GPIO_OUTPUT_PIN);
    gpio_set_direction(GPIO_OUTPUT_PIN, GPIO_MODE_OUTPUT);

    // Configurer le timer
    gptimer_handle_t gptimer = NULL;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT, // Utilise l'horloge APB par défaut 80 Mhz
        .direction = GPTIMER_COUNT_UP,      // Compte ascendant
        .resolution_hz = 2*1000*1000,           // 2 MHz, requis pour la précision de 0,001 hz (2 * freq base pour gérer front montant / ascendant)
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));

    // Configurer l'alarme
    gptimer_alarm_config_t alarm_config = {
        .alarm_count = SIDERAL_TICKS,               // Déclenche l'alarme après le bon nombre de ticks
        .reload_count = 0,                  // Compte à partir de zéro après chaque alarme
        .flags.auto_reload_on_alarm = true,       // Redémarre automatiquement après l'alarme
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));

    // Enregistrer le callback pour l'alarme
    gptimer_event_callbacks_t cbs = {
        .on_alarm = timer_callback,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));
    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    // Démarrer le timer
    ESP_ERROR_CHECK(gptimer_start(gptimer));
}

void app_main(void) {
    // Initialise le timer
    init_timer();
    // Boucle principale, ne sert à rien
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000)); // Délai de 1 seconde
    }
}