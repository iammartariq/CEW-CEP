#ifndef MONITORING_SYSTEM_H
#define MONITORING_SYSTEM_H

#define API_URL "http://api.openweathermap.org/data/2.5/weather?q=London&appid=78ea6250b58c486bd0b1a8a3f39dc58b&units=metric"
#define ALERT_THRESHOLD_TEMP 30.0  // Example threshold for temperature
#define ALERT_THRESHOLD_HUMIDITY 70.0

void fetch_environmental_data(const char *url, char *response, size_t max_len);
void process_data(const char *raw_data, float *temperature, float *humidity);
void save_raw_data(const char *filename, const char *raw_data);
void save_processed_data_csv(const char *filename, float temperature, float humidity);
void check_for_alert(float temperature, float humidity);

#endif
