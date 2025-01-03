#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CITIES 10
#define CITY_NAME_SIZE 100
#define WEATHER_INFO_SIZE 1024

typedef struct {
    char city[CITY_NAME_SIZE];
    char weather_info[WEATHER_INFO_SIZE];
} WeatherData;

WeatherData weather_list[MAX_CITIES];
int weather_count = 0;

// Hava durumu verisi ekleme fonksiyonu
void add_weather_data(const char *city, const char *weather_info) {
    if (weather_count < MAX_CITIES) {
        strncpy(weather_list[weather_count].city, city, CITY_NAME_SIZE - 1);
        weather_list[weather_count].city[CITY_NAME_SIZE - 1] = '\0'; // Güvenlik için sonlandırma
        strncpy(weather_list[weather_count].weather_info, weather_info, WEATHER_INFO_SIZE - 1);
        weather_list[weather_count].weather_info[WEATHER_INFO_SIZE - 1] = '\0'; // Güvenlik için sonlandırma
        weather_count++;
    } else {
        printf("Maksimum şehir sayısına ulaşıldı.\n");
    }
}

// Verileri dosyaya kaydetme fonksiyonu
void save_weather_data_to_file() {
    FILE *file = fopen("./weather_data.txt", "w");
    if (file == NULL) {
        printf("Dosya açılamadı.\n");
        return;
    }

    for (int i = 0; i < weather_count; i++) {
        fprintf(file, "Şehir: %s\n", weather_list[i].city);
        fprintf(file, "Hava Durumu: %s\n", weather_list[i].weather_info);
        fprintf(file, "-------------------------\n");
    }

    fclose(file);
    printf("Veriler 'weather_data.txt' dosyasına başarıyla kaydedildi.\n");
}

int main() {
    // Hava durumu verilerini ekleyelim
    add_weather_data("Istanbul", "Bugün hava güneşli, sıcaklık 25°C.");
    add_weather_data("Ankara", "Bugün hava yağmurlu, sıcaklık 18°C.");
    add_weather_data("Izmir", "Bugün hava rüzgarlı, sıcaklık 22°C.");

    // Verileri dosyaya kaydedelim
    save_weather_data_to_file();

    return 0;
}