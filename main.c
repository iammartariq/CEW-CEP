#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <curl/curl.h>
#include "monitoring_system.h"
#include <cjson/cJSON.h>

// Callback function for libcurl
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    strncat(userdata, (char *)ptr, size * nmemb);
    return size * nmemb;
}

void fetch_environmental_data(const char *url, char *response, size_t max_len) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize CURL\n");
        exit(EXIT_FAILURE);
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "CURL error: %s\n", curl_easy_strerror(res));
        exit(EXIT_FAILURE);
    }

    curl_easy_cleanup(curl);
}

void process_data(const char *raw_data, float *temperature, float *humidity) {
    char *temp_ptr = strstr(raw_data, "\"temp\":");
    char *humidity_ptr = strstr(raw_data, "\"humidity\":");

    if (temp_ptr && humidity_ptr) {
        sscanf(temp_ptr, "\"temp\":%f", temperature);
        sscanf(humidity_ptr, "\"humidity\":%f", humidity);
    } else {
        fprintf(stderr, "Failed to parse JSON data\n");
        exit(EXIT_FAILURE);
    }
}


void save_raw_data(const char *filename, const char *raw_data) {
    FILE *file = fopen(filename, "r"); // Open file in read mode
    char *buffer = NULL;
    long file_size = 0;
    cJSON *existing_data = NULL;
    cJSON *new_entry = NULL;

    // Check if the file exists and is not empty
    if (file) {
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        if (file_size > 0) {
            buffer = malloc(file_size + 1);
            fread(buffer, 1, file_size, file);
            buffer[file_size] = '\0';
            existing_data = cJSON_Parse(buffer); // Parse existing JSON
            free(buffer);
        }

        fclose(file);
    }

    // If no existing data or invalid JSON, initialize a new array
    if (!existing_data || !cJSON_IsArray(existing_data)) {
        existing_data = cJSON_CreateArray();
    }

    // Parse new raw data
    new_entry = cJSON_Parse(raw_data);
    if (!new_entry) {
        fprintf(stderr, "Failed to parse new raw data as JSON\n");
        exit(EXIT_FAILURE);
    }

    // Add the new entry to the array
    cJSON_AddItemToArray(existing_data, new_entry);

    // Write updated JSON back to the file
    file = fopen(filename, "w"); // Open file in write mode
    if (!file) {
        perror("Error opening file for saving raw data");
        cJSON_Delete(existing_data);
        exit(EXIT_FAILURE);
    }

    // Write the updated JSON array to the file
    char *updated_json = cJSON_Print(existing_data);
    fprintf(file, "%s", updated_json);
    fclose(file);

    // Free memory
    cJSON_Delete(existing_data);
    free(updated_json);
}


void save_processed_data_csv(const char *filename, float temperature, float humidity) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file for processed data");
        exit(EXIT_FAILURE);
    }

    // Write CSV header
    fprintf(file, "Temperature (°C),Humidity (%%)\n");
    // Write data
    fprintf(file, "%.2f,%.2f\n", temperature, humidity);
    fclose(file);
}

// Function to check for alerts based on thresholds
void check_for_alert(float temperature, float humidity) {
    bool alert_triggered = false;
    char alert_message[256];

    // Check temperature threshold
    if (temperature > ALERT_THRESHOLD_TEMP) {
        snprintf(alert_message, sizeof(alert_message), "Temperature Alert! Temp: %.2f°C exceeds %.2f°C", temperature, ALERT_THRESHOLD_TEMP);
        alert_triggered = true;
    }

    // Check humidity threshold
    if (humidity > ALERT_THRESHOLD_HUMIDITY) {
        snprintf(alert_message, sizeof(alert_message), "Humidity Alert! Humidity: %.2f%% exceeds %.2f%%", humidity, ALERT_THRESHOLD_HUMIDITY);
        alert_triggered = true;
    }

    // Trigger alert if needed
    if (alert_triggered) {
        char command[512];
        snprintf(command, sizeof(command), "zenity --warning --text='%s'", alert_message);
        system(command);
    }
}

int main() {
    char raw_data[8192] = {0};
    float temperature = 0.0, humidity = 0.0;

    fetch_environmental_data(API_URL, raw_data, sizeof(raw_data));
    save_raw_data("raw_data.json", raw_data);

    process_data(raw_data, &temperature, &humidity);
    save_processed_data_csv("processed_data.csv", temperature, humidity);

    check_for_alert(temperature, humidity);

    return 0;
}
