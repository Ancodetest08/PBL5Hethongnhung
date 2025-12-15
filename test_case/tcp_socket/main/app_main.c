#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "app_priv.h"
#include "app_storage.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "lwip/sockets.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#define LIGHT_TCP_CLIENT            0
#define LIGHT_ESP_WIFI_SSID         "Chan Ga Sa Tac"
#define LIGHT_ESP_WIFI_PASS         "anktmt2026"
#define LIGHT_ESP_MAXIMUM_RETRY     5
#define HOST_IP                     "172.20.10.4"
#define PORT                        3333

#define WIFI_CONNECTED_BIT      BIT0
#define WIFI_FAIL_BIT           BIT1

static const char *TAG = "tcp socket";

static EventGroupHandle_t s_wifi_event_group = NULL;
static int s_retry_num = 0;

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < LIGHT_ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();

            s_retry_num++;

            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }

        ESP_LOGI(TAG, "connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;

        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));

        s_retry_num = 0;

        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

static void wifi_initialize(void) {
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));
}

static void wifi_station_initialize(void) {
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = LIGHT_ESP_WIFI_SSID,
            .password = LIGHT_ESP_WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_station_initialize finished.");

    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s", LIGHT_ESP_WIFI_SSID, LIGHT_ESP_WIFI_PASS);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s", LIGHT_ESP_WIFI_SSID, LIGHT_ESP_WIFI_PASS);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }
}

static esp_err_t esp_create_tcp_server(void) {
   int len;
   int keepAlive = 1;
   int keepIdle = 5;
   int keepInterval = 5;
   int keepCount = 3;
   char rx_buffer[128] = {0};
   char addr_str[32] = {0};
   esp_err_t err = ESP_FAIL;
   struct sockaddr_in server_addr;

   int listenfd = socket(AF_INET, SOCK_STREAM, 0);
   if (listenfd < 0) {
      ESP_LOGE(TAG, "create socket error");

      return err;
   }

   ESP_LOGI(TAG, "create socket success, listenfd : %d", listenfd);

   int opt = 1;
   int ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
   if (ret < 0) {
      ESP_LOGE(TAG, "Failed to set SO_REUSEADDR. Error %d", errno);

      goto exit;
   }

   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = INADDR_ANY;
   server_addr.sin_port = htons(PORT);

   ret = bind(listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
   if (ret < 0) {
      ESP_LOGE(TAG, "bind socket failed, socketfd : %d, errno : %d", listenfd, errno);

      goto exit;
   }

   ESP_LOGI(TAG, "bind socket success");

   ret = listen(listenfd, 1);
   if (ret < 0) {
      ESP_LOGE(TAG, "listen socket failed, socketfd : %d, errno : %d", listenfd, errno);

      goto exit;
   }

   ESP_LOGI(TAG, "listen socket success");

   while (1) {
      struct sockaddr_in source_addr;
      socklen_t addr_len = sizeof(source_addr);

      int sock = accept(listenfd, (struct sockaddr *)&source_addr, &addr_len);
      if (sock < 0) {
         ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);

         break;
      }

      setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(int));
      setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &keepIdle, sizeof(int));
      setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &keepInterval, sizeof(int));
      setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &keepCount, sizeof(int));

      if (source_addr.sin_family == PF_INET) {
         inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
      }

      ESP_LOGI(TAG, "Socket accepted ip address: %s", addr_str);

      do {
         len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
         if (len < 0) {
            ESP_LOGE(TAG, "Error occurred during receiving: errno %d", errno);
         } else if (len == 0) {
            ESP_LOGW(TAG, "Connection closed");
         } else {
            rx_buffer[len] = 0;

            ESP_LOGI(TAG, "Received %d bytes: %s", len, rx_buffer);
         }
      } while (len > 0);

      shutdown(sock, 0);

      close(sock);
   }

exit:
   close(listenfd);

   return err;
}

static esp_err_t esp_create_tcp_client(void) {
   esp_err_t err = ESP_FAIL;
   char *payload = "Open the light";
   struct sockaddr_in dest_addr;
   dest_addr.sin_addr.s_addr = inet_addr(HOST_IP);
   dest_addr.sin_family = AF_INET;
   dest_addr.sin_port = htons(PORT);

   int sock =  socket(AF_INET, SOCK_STREAM, 0);
   if (sock < 0) {
      ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);

      return err;
   }

   ESP_LOGI(TAG, "Socket created, connecting to %s:%d", HOST_IP, PORT);

   int ret = connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
   if (ret != 0) {
      ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);

      close(sock);

      return err;
   }

   ESP_LOGI(TAG, "Successfully connected");

   ret = send(sock, payload, strlen(payload), 0);
   if (ret < 0) {
      ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);

      goto exit;
   }

   err = ESP_OK;

exit:
   shutdown(sock, 0);

   close(sock);

   return err;
}

void app_main() {
    int i = 0;

    ESP_LOGE(TAG, "app_main");

    ESP_LOGI(TAG, "NVS Flash initialization");
    app_storage_init();

    ESP_LOGI(TAG, "Application driver initialization");
    app_driver_init();

    ESP_LOGI(TAG, "Wi-Fi initialization");
    wifi_initialize();

    ESP_LOGI(TAG, "Wi-Fi Station initialization");
    wifi_station_initialize();

#if LIGHT_TCP_CLIENT
    esp_create_tcp_client();
#else
    esp_create_tcp_server();
#endif

    while (1) {
        ESP_LOGI(TAG, "[%02d] Hello world!", i++);
        
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
