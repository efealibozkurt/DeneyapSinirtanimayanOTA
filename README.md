# DeneyapSinirtanimayaOTA Arduino Kütüphanesi

![Arduino](https://img.shields.io/badge/Arduino-Uyumlu-00979D?style=for-the-badge&logo=arduino)
![Platform](https://img.shields.io/badge/Platform-ESP32-E7352C?style=for-the-badge&logo=espressif)
![Lisans](https://img.shields.io/badge/Lisans-MIT-yellow.svg?style=for-the-badge)

**DeneyapSinirtanimayaOTA**, ESP32 tabanlı geliştirme kartları (özellikle **Deneyap Kart**) için GitHub üzerinden OTA (Over-the-Air) yazılım güncellemesini son derece basitleştiren bir Arduino kütüphanesidir. Bu kütüphane sayesinde, cihazlarınıza fiziksel olarak bağlanmadan, dünyanın neresinde olursanız olun internet üzerinden kolayca yeni yazılım yükleyebilirsiniz.

## Öne Çıkan Özellikler

-   🌍 **Uzaktan Güncelleme:** Cihazınız sizden kilometrelerce uzakta olsa bile, tek gereken bir WiFi bağlantısı.
-   ⚙️ **Kolay Kurulum:** Sadece birkaç satır kod ile projenize OTA özelliği ekleyin.
-   📺 **OLED Ekran Desteği:** SSD1306 OLED ekranlar üzerinden güncelleme durumu hakkında anlık geri bildirim alın.
-   🖲️ **Esnek Buton Kontrolü:** Güncellemeleri manuel tetiklemek için Deneyap Kart üzerindeki `GPKEY` veya harici bir butonu kullanın.
-   🌐 **Çevrimdışı (Offline) Mod:** WiFi bağlanamadığında cihazınızın çalışmaya devam etmesini sağlayın.
-   🔒 **Güvenli:** Özel (private) repolar için GitHub Access Token kullanımını destekler.

## Gereksinimler

-   **Donanım:**
    -   ESP32 tabanlı bir geliştirme kartı (Deneyap Kart ile test edilmiştir).
    -   SSD1306 I2C OLED Ekran (128x64).
-   **Yazılım:**
    -   Arduino IDE.
    -   Arduino IDE için ESP32 kart yöneticisi.
    -   `Adafruit GFX Library`
    -   `Adafruit SSD1306 Library`

## Kurulum

1.  **ZIP Olarak İndirme:**
    -   Bu reponun son sürümünü `.zip` olarak indirin (Code > Download ZIP).
    -   Arduino IDE'de `Taslak > Kütüphaneyi Dahil Et > .ZIP Kitaplığı Ekle` menüsünü takip ederek indirdiğiniz `.zip` dosyasını seçin.
2.  **Manuel Kurulum:**
    -   Bu repoyu klonlayın veya indirin.
    -   Klasörü, Arduino kütüphane klasörünüze kopyalayın (genellikle `Belgeler/Arduino/libraries`).
    -   Arduino IDE'yi yeniden başlatın.

## Kullanım

```cpp
#include <DeneyapSinirtanimayaOTA.h>

// Ayarlarınızı buraya girin...
const char* WIFI_SSID = "SENIN_WIFI_AGININ_ADI";
const char* WIFI_PASSWORD = "SENIN_WIFI_SIFREN";
// ...diğer ayarlar

// Kütüphaneden bir nesne oluşturun
DeneyapSinirtanimayaOTA ota(WIFI_SSID, WIFI_PASSWORD, ...);

void setup() {
  // WiFi bağlanamazsa bile kodun devam etmesi için "true" parametresini verin.
  ota.baslat(true); 
}

void loop() {
  // Varsayılan olarak GPKEY butonunu dinler.
  ota.dongu();
}
