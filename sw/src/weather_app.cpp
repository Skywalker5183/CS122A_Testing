// #include "../include/weather_app.h"

// static void back_pressed(void);

// static lv_style_t style_btn;
// static lv_style_t style_btn_pressed;
// static lv_color_filter_dsc_t color_filter;
// static lv_obj_t * label;
// static lv_obj_t *bg;
// static long kph_to_mph;
// static char *date_seperated = NULL;
// static char *time_seperated = NULL;
// static long temp_convert_to_F;
// static char *Current_data;
// char *temp;
// char *wind;
// char *time_;
// char* pst;
// CURL *curl;

// // Struct to hold response data
// struct Memory {
//     char *response; // a dynamically allocated string that stores the downloaded data.
//     size_t size; // the current size of the response string.
// };

// CURLcode res;
// struct Memory chunk;

// static size_t ResponseCallback(void *contents, size_t size, size_t nmemb, void *userp);
// char *extract_value(const char *src, const char *key);
// char *extract_current(const char *json);
// void split_datetime(const char *input, char **date_out, char **time_out);
// void get_weather_data();
// static void make_weather_app();
// char *utc_to_pst(const char *utc_time);

// static lv_color_t darken_cb(const lv_color_filter_dsc_t * dsc, lv_color_t color, lv_opa_t opa)
// {
//     LV_UNUSED(dsc); // Macro that simply tells the compiler: “I know this parameter exists, but I’m not using it.”
//     return lv_color_darken(color, opa); // Take the original color and darken it by the opacity amount
// }

// static void make_btn_styles(void)
// {
//     lv_style_init(&style_btn); // Creates an empty sytle object

//     lv_style_set_radius(&style_btn, 10); // Rounded corners

//     // opacity (opa) is a value from 0 to 255:
//     // 0 means fully transparent (invisible)
//     // 255 means fully opaque (solid)

//     lv_style_set_bg_opa(&style_btn, LV_OPA_COVER); // Background is fully opague / solid

//     lv_style_set_bg_color(&style_btn, lv_palette_lighten(LV_PALETTE_GREY, 3)); // Sets a light gray base color

//     // Adds a vertical gradient from light grey to darker grey.
//     lv_style_set_bg_grad_color(&style_btn, lv_palette_main(LV_PALETTE_GREY));
//     lv_style_set_bg_grad_dir(&style_btn, LV_GRAD_DIR_VER);

//     // Border
//     lv_style_set_border_color(&style_btn, lv_color_black());
//     lv_style_set_border_opa(&style_btn, LV_OPA_20);
//     lv_style_set_border_width(&style_btn, 2);

//     lv_style_set_text_color(&style_btn, lv_color_black());

//     // Pressed style (darken filter)
//     // When this style is active, run darken_cb() on every pixel
//     lv_color_filter_dsc_init(&color_filter, darken_cb);

//     // Empty style object for pressed state
//     lv_style_init(&style_btn_pressed);

//     // Apply the darken filter with 20% opacity
//     // To all colors in the button (background, border, text, images)
//     lv_style_set_color_filter_dsc(&style_btn_pressed, &color_filter);
//     lv_style_set_color_filter_opa(&style_btn_pressed, LV_OPA_20);
// }

// static void btn_event_cb(lv_event_t * e)
// {
//     // Returns the object that triggered the event which is the button in this case
//     lv_obj_t * btn = lv_event_get_target(e);
//     lv_event_code_t code = lv_event_get_code(e);    // Gets the event

//     // Apply darkening
//     if(code == LV_EVENT_PRESSED) {
//         lv_obj_clean(lv_scr_act());

//         get_weather_data();

//         // User clicked the weather app, so use weather_app_background
//         bg = lv_img_create(lv_scr_act());
//         lv_img_set_src(bg, &Weather_app_background);
//         lv_obj_move_background(bg);

//         lv_obj_set_pos(bg, 0, 0);
//         lv_obj_set_size(bg, LV_HOR_RES, LV_VER_RES);

//         make_weather_app();

//         make_Back_button(Back_button_width, Back_button_height, back_pressed);
//     }
// }

// void make_weather_button(void)
// {
//     make_btn_styles();

//     lv_obj_t* btn = lv_btn_create(lv_scr_act()); // Makes button
//     lv_obj_remove_style_all(btn);                // Removes all styles cause were making our own

//     lv_obj_t* img = lv_img_create(btn);          // Parent is the button
//     lv_img_set_src(img, &weather_logo);          // Image appears on top of button

//     lv_obj_center(img);                          // Centers image

//     lv_obj_set_pos(btn, 10, 10);
//     lv_obj_set_size(btn, button_width, button_height);

//     lv_obj_add_style(btn, &style_btn, 0);        // Apply's style to buttons default state(0)

//     lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL); // Button callback for events

//     label = lv_label_create(btn);     // Make label for button
//     lv_label_set_text(label, "");
//     lv_obj_center(label);             // Centers object inside parent object(btn)
// }

// static void back_pressed(void)
// {
//     free(date_seperated);
//     free(time_seperated);
//     free(temp);
//     free(wind);
//     free(time_);
//     free(Current_data);

//     // Cleanup
//     curl_easy_cleanup(curl);

//     // Free allocated memory
//     free(chunk.response);

//     curl_global_cleanup();

//     lv_obj_clean(lv_scr_act());

//     lv_phone_gui_widgets();
// }

// static void make_weather_app() {
//     lv_obj_t * parent = lv_screen_active();

//     // -------------------- Title Bar ----------------------- //
//     lv_obj_t * title_bar = lv_obj_create(parent); // Creates a new LVGL object inside parent
//     lv_obj_set_size(title_bar, LV_PCT(100), 30);
//     lv_obj_align(title_bar, LV_ALIGN_TOP_MID, 0, 0);
//     lv_obj_clear_flag(title_bar, LV_OBJ_FLAG_SCROLLABLE);

//     //Now add text:
//     lv_obj_t * title_label = lv_label_create(title_bar);
//     lv_label_set_text(title_label, "Weather");
//     lv_obj_center(title_label);

//     // Make Bar have no rounded corners
//     lv_obj_set_style_radius(title_bar, 0, 0);
//     lv_obj_set_style_bg_color(title_bar, lv_color_white(), 0);
//     lv_obj_set_style_text_color(title_bar, lv_color_black(), 0);

//     // Remove border
//     lv_obj_set_style_border_width(title_bar, 0, LV_PART_MAIN);

//     char buf[128];

//     // --------------- TIME ---------------
//     snprintf(buf, sizeof(buf),
//             "Time: %s\n",
//             pst);

//     lv_obj_t *Time_text = lv_label_create(lv_screen_active());
//     lv_label_set_text(Time_text, buf);
//     lv_obj_set_pos(Time_text, 5, 40);

//     // --------------- TEMP ---------------
//     snprintf(buf, sizeof(buf),
//             "Temperature: %s°C / %ld°F",
//             temp, temp_convert_to_F);

//     lv_obj_t *temp_text = lv_label_create(lv_screen_active());
//     lv_label_set_text(temp_text, buf);
//     lv_obj_set_pos(temp_text, 5, 60);

//     // --------------- WIND ---------------
//     snprintf(buf, sizeof(buf),
//             "Wind Speed: %ldmph\n",
//             kph_to_mph);

//     lv_obj_t *wind_text = lv_label_create(lv_screen_active());
//     lv_label_set_text(wind_text, buf);
//     lv_obj_set_pos(wind_text, 5, 80);
// }

// // Functions for Weather API
// void get_weather_data() {
//     chunk.response = malloc(1);  // Initialize memory
//     chunk.size = 0;             // No data yet

//     curl_global_init(CURL_GLOBAL_DEFAULT);
//     curl = curl_easy_init();

//     if (curl) {
//         // Temporarily disable certificate verification:
//         curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
//         curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

//         // Set URL of the API endpoint
//         char slug[] = "home";
//         char version[]= "draft";
//         char url[512];
//         snprintf(url, sizeof(url), "https://api.open-meteo.com/v1/forecast?latitude=33.953350&longitude=-117.396156&current=temperature_2m,wind_speed_10m");

//         // Print the URL
//         printf("URL: %s\n", url);

//         // initializing libcurl

//         // setting the URL
//         curl_easy_setopt(curl, CURLOPT_URL, url );

//         // Follow redirect
//         curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

//         // Set callback function to handle response data
//         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ResponseCallback);

//         // Pass the Memory struct to the callback function
//         curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

//         // Perform the HTTP GET request
//         res = curl_easy_perform(curl);

//         // Check for errors
//         if (res != CURLE_OK) {
//             fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
//         } else {
//             printf("Response data size: %zu\n", chunk.size);
//             printf("Response data: \n%s\n", chunk.response);
//         }

//         const char *API_Response = chunk.response;
//         Current_data = extract_current(chunk.response);

//         temp = extract_value(Current_data, "temperature_2m");
//         wind = extract_value(Current_data, "wind_speed_10m");
//         time_ = extract_value(Current_data, "time");

//         char* endptr;
//         // Convert strings to long integers using strtol
//         long temp_num_C = strtol(temp, &endptr, 10);
//         temp_convert_to_F = ( temp_num_C * 1.8 ) + 32;

//         printf("Temperature: %s°C / %ld°F\n", temp, temp_convert_to_F);

//         char* endptr1;
//         // Convert strings to long integers using strtol
//         long char_to_int = strtol(temp, &endptr1, 10);
//         kph_to_mph = char_to_int * 0.621371;

//         printf("Wind: %ldmph\n", kph_to_mph);

//         split_datetime(time_, &date_seperated, &time_seperated);

//         printf("Date: %s\n", date_seperated);

//         // UTC -> PST ==> PST = UTC - 8 hrs
//         pst = utc_to_pst(time_seperated);
//         printf("Time: %s\n", pst);
//     }
// }

// static size_t ResponseCallback(void *contents, size_t size, size_t nmemb, void *userp) {
//     // This computes the total size of the current chunk of data received by multiplying the size of one block (size) with the number of blocks (nmemb).
//     // For example, if the server sends 8 blocks of 256 bytes each, totalSize will be 8 * 256 = 2048 bytes.
//     size_t totalSize = size * nmemb;

//     // The userp pointer is cast to a struct Memory *. This struct was passed earlier in the main program and is used to accumulate the received data.
//     struct Memory *mem = (struct Memory *)userp;

//     printf(". %zu %zu\n", size, nmemb);

//     /*
//     Resizes the response buffer to accommodate the new data chunk:

//     mem->size: the current size of the buffer.
//     totalSize: The size of the new chunk.
//     +1: Space for the null-terminator (\0) to make it a valid C string.
//     realloc: dynamically reallocates memory for the response buffer.
//     If the allocation fails, realloc returns NULL, and the old memory remains valid.
//     */
//     char *ptr = realloc(mem->response, mem->size + totalSize + 1);

//     /*
//     If memory allocation fails (so ptr is NULL), print an error message and
//     return 0. Returning 0 signals libcurl to abort the transfer.
//     */
//     if (ptr == NULL) {
//         printf("Not enough memory to allocate buffer.\n");
//         return 0;
//     }

//     // assign the newly allocated memory back to the response pointer.
//     mem->response = ptr;

//     /*
//     copy the new chunk of data from contents into the buffer:
//     &(mem->response[mem->size]): The location in the buffer where the new data should be appended (end of the current data).
//     contents: The data received from the server.
//     totalSize: The size of the data to copy.
//     */
//     memcpy(&(mem->response[mem->size]), contents, totalSize);

//     // Increment the size of the response buffer to reflect the new total size after appending the new chunk.
//     mem->size += totalSize;

//     // Add a null-terminator at the end of the response buffer to make it a valid C string.
//     // This ensures that the response can be safely treated as a regular null-terminated string.
//     mem->response[mem->size] = '\0';

//     // Return the number of bytes processed (totalSize).
//     // This signals to libcurl that the data chunk was handled successfully.
//     return totalSize;
// }

// char *extract_value(const char *src, const char *key) {
//     // Build search pattern: "key":
//     char pattern[256];
//     snprintf(pattern, sizeof(pattern), "\"%s\":", key);

//     // Find the key in the source string
//     char *pos = strstr(src, pattern);
//     if (!pos) return NULL;  // key not found

//     pos += strlen(pattern); // move past "key":

//     // Skip spaces
//     while (*pos == ' ' || *pos == '\t') pos++;

//     // Determine if value is quoted or not
//     int quoted = (*pos == '"');
//     if (quoted) pos++; // skip opening quote

//     // Find end of value
//     const char *end = pos;
//     if (quoted) {
//         while (*end && *end != '"') end++;
//     } else {
//         while (*end && *end != ',' && *end != '}' && *end != ' ') end++;
//     }

//     // Allocate output string
//     size_t len = end - pos;
//     char *out = malloc(len + 1);
//     if (!out) return NULL;

//     memcpy(out, pos, len);
//     out[len] = '\0';

//     return out;
// }

// char *extract_current(const char *json) {
//     const char *key = "\"current\":";
//     char *pos = strstr(json, key);
//     if (!pos) return NULL;

//     // Move to the first '{'
//     pos = strchr(pos, '{');
//     if (!pos) return NULL;

//     const char *start = pos;
//     int brace_count = 0;

//     // Find matching closing brace
//     do {
//         if (*pos == '{') brace_count++;
//         else if (*pos == '}') brace_count--;
//         pos++;
//     } while (*pos && brace_count > 0);

//     if (brace_count != 0) return NULL; // malformed JSON

//     size_t len = pos - start;

//     char *out = malloc(len + 1);
//     if (!out) return NULL;

//     memcpy(out, start, len);
//     out[len] = '\0';

//     return out;
// }

// void split_datetime(const char *input, char **date_out, char **time_out) {
//     const char *T = strchr(input, 'T');
//     if (!T) {
//         *date_out = NULL;
//         *time_out = NULL;
//         return;
//     }

//     size_t date_len = T - input;
//     size_t time_len = strlen(T + 1);

//     // Allocate date string
//     *date_out = malloc(date_len + 1);
//     memcpy(*date_out, input, date_len);
//     (*date_out)[date_len] = '\0';

//     // Allocate time string
//     *time_out = malloc(time_len + 1);
//     memcpy(*time_out, T + 1, time_len);
//     (*time_out)[time_len] = '\0';
// }

// char *utc_to_pst(const char *utc_time) {
//     int hour, minute;

//     // Parse "HH:MM"
//     sscanf(utc_time, "%d:%d", &hour, &minute);

//     // Convert to total minutes
//     int total = hour * 60 + minute;

//     // Subtract 7 hours for PDT (UTC-7)
//     total -= 7 * 60;

//     // Wrap around if negative
//     if (total < 0)
//         total += 24 * 60;

//     // Convert back to HH:MM
//     int new_hour = total / 60;
//     int new_min  = total % 60;

//     char *out;
//     if(new_hour > 12) {
//         new_hour -= 12;

//         out = malloc(8); // "HH:MM" + null
//         snprintf(out, 8, "%02d:%02dpm", new_hour, new_min);

//     } else {
//         out = malloc(8); // "HH:MM" + null
//         snprintf(out, 8, "%02d:%02dam", new_hour, new_min);
//     }

//     return out;
// }
