
#include "spi_display.h"
#include "lv_conf.h"
#include "lvgl_demo_widgets.h"
#include "lvgl_touch.h"
#include "Phone_gui.h"
#include <stdio.h>
#include "wifi_config.h"

#include <lvgl.h>

#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/binary_info.h>
#include <pico/time.h>
#include <hardware/spi.h>
#include <pico/cyw43_arch.h>

uint32_t Country = CYW43_COUNTRY_USA; // Append country to the end (replace USA if not USA)
uint32_t AuthType = CYW43_AUTH_WPA2_AES_PSK; // Other Auth types: CYW43_AUTH_OPEN, CYW43_AUTH_WPA_TKIP_PSK, CYW43_AUTH_WPA2_MIXED_PSK

/*Return the elapsed milliseconds since startup.
 *It needs to be implemented by the user*/
uint32_t cs122_get_millis(void) {
    return to_ms_since_boot(get_absolute_time());
}

static uint8_t buffer[OLEDRGB_WIDTH * OLEDRGB_HEIGHT / 10];

/*Copy the rendered image to the screen. */
void cs122_flush_cb_direct(lv_display_t * disp, const lv_area_t * area, uint8_t * px_buf) {
    ucr::bcoe::SPIDisplay *spi_display = reinterpret_cast<ucr::bcoe::SPIDisplay *>(lv_display_get_user_data(disp));
	uint32_t i = 0;
	for (uint32_t y = area->y1; y <= area->y2; y++) {
		for(uint32_t x = area->x1; x <= area->x2; x++) {
			uint32_t px_buf_idx = x * 2 + y * (spi_display->getWidth() * 2);
		    buffer[i++] =  (px_buf[px_buf_idx+1] & 0xE0) | ((px_buf[px_buf_idx+1] & 0x7) << 2) | (px_buf[px_buf_idx] & 0x1f) >> 3;
		}
	}

    /*Show the rendered image on the display*/
    spi_display->drawBitmap(area->x1, area->y1, area->x2, area->y2, buffer);

    /*Indicate that the buffer is available.
     *If DMA were used, call in the DMA complete interrupt*/
    lv_display_flush_ready(disp);
}

/*It needs to be implemented by the user*/
void cs122_flush_cb_partial(lv_display_t * disp, const lv_area_t * area, uint8_t * px_buf) {
	uint32_t size = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);

    /*Show the rendered image on the display*/
    ucr::bcoe::SPIDisplay *spi_display = reinterpret_cast<ucr::bcoe::SPIDisplay *>(lv_display_get_user_data(disp));
    spi_display->drawBitmap(2 * area->x1, area->y1, 2 * area->x2+1, area->y2, px_buf);

    /*Indicate that the buffer is available.
     *If DMA were used, call in the DMA complete interrupt*/
    lv_display_flush_ready(disp);
}

// void initWifi(const char *ssid, const char *password, uint32_t country, uint32_t auth)
// {
//     // Initialize onboard wifi chip with specified country settings
//     // Variations:
//     // cyw43_arch_init () - initialize with default country
//     if (cyw43_arch_init_with_country(country))
//     {
//         printf("Wi-Fi init failed\n");
//         return;
//     }

//     printf("Wi-Fi initialized\n");
    
//     // Enable client mode
//     // Variations:
//     // cyw43_arch_enable_ap_mode(ssid, password, auth) - enables server mode (access point)
//     cyw43_arch_enable_sta_mode();

//     // Connect to a network or timeout in 10 seconds.
//     // Variations:
//     // cyw43_arch_wifi_connect_timeout_ms(ssid, password, auth, waitTime)
//     // cyw43_arch_wifi_connect_async(ssid, password, auth)
//     if (cyw43_arch_wifi_connect_blocking(ssid, password, auth))
//     {
//         printf("Wi-Fi connection failed\n");
//         return;
//     }

//     printf("Wi-Fi connected\n");
// }

int main(void) {
    // Init drivers
	stdio_init_all();
	cyw43_arch_init();
    adc_init();

    // Wifi
    //initWifi(ssidName, ssidPassword, Country, AuthType);

    ucr::bcoe::SPIDisplay spi_display(480, 272, 10000000, 20);
	spi_display.begin();
	spi_display.clear();
    // lv_phone_gui_widgets();
    ucr::bcoe::cs::cs122::LVGL_DemoWidgets app(&spi_display, cs122_flush_cb_partial, cs122_get_millis);
    // lv_phone_gui_widgets();
    touch_init(26, 21, 27, 22);
    app.run();
}