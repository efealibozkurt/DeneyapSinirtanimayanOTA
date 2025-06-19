/******************************************************************
 * OTA Örnek Projesi - v2.0
 * Sürüm - v2.0
 * EFE ALİ BOZKURT 2025 'SADECE HAYAL ET'
 ******************************************************************/

/******************************************************************
 * Gelistirilmis OTA Ornegi
 * * Bu örnek, DeneyapSinirtanimayaOTA kütüphanesinin tüm özelliklerini
 * bir arada gösterir:
 * 1. Varsayılan olarak Seri Port'a durum mesajlarını yazar.
 * 2. İsteğe bağlı olarak OLED ekrana bildirim gönderir.
 * 3. İsteğe bağlı olarak harici bir buton ile güncellemeyi tetikler.
 * * NASIL KULLANILIR:
 * - OLED ekran kullanmak için: Kodun başındaki #define USE_OLED_DISPLAY 
 * satırının yorumunu ('//') kaldırın.
 * - Harici buton kullanmak için: #define USE_CUSTOM_BUTTON
 * satırının yorumunu kaldırın ve doğru pin numarasını yazın.
 * - Hiçbir değişiklik yapmazsanız, kütüphane varsayılan olarak Seri Port'u
 * ve Deneyap Kart'ın GPKEY butonunu kullanacaktır.
 ******************************************************************/

// Ana kütüphanemizi dahil ediyoruz.
#include <DeneyapSinirtanimayaOTA.h>

// --- İSTEĞE BAĞLI ÖZELLİKLERİ AKTİFLEŞTİRME ---

// OLED ekran kullanmak istiyorsanız bu satırın başındaki yorumu (//) kaldırın.
// #define USE_OLED_DISPLAY

// Harici bir buton kullanmak istiyorsanız bu satırın başındaki yorumu kaldırın
// ve 12 yerine kendi pin numaranızı yazın.
// #define USE_CUSTOM_BUTTON 12

// --- Kütüphane ve Donanım Kurulumu (Eğer aktifse) ---
#ifdef USE_OLED_DISPLAY
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
#endif

// =======================================================================
//                           KULLANICI AYARLARI
// =======================================================================
const char* WIFI_SSID = "SENIN_WIFI_AGININ_ADI";
const char* WIFI_PASSWORD = "SENIN_WIFI_SIFREN";
const char* GITHUB_TOKEN = "ghp_XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"; 
const String FIRMWARE_VERSION_URL = "https://raw.githubusercontent.com/kullanici/repo/main/version.txt";
const String FIRMWARE_BIN_URL = "https://raw.githubusercontent.com/kullanici/repo/main/firmware.bin";
const String FIRMWARE_VERSION = "1.0"; // Bu cihaza yüklenen yazılımın versiyonu

// =======================================================================
//                             NESNE TANIMLAMALARI
// =======================================================================

// OTA kütüphanemizden bir nesne her zaman oluşturulur.
DeneyapSinirtanimayaOTA ota(WIFI_SSID, WIFI_PASSWORD, GITHUB_TOKEN, FIRMWARE_VERSION_URL, FIRMWARE_BIN_URL, FIRMWARE_VERSION);

// OLED Ekran nesnesi sadece eğer kullanacaksak oluşturulur.
#ifdef USE_OLED_DISPLAY
  Adafruit_SSD1306 display(128, 64, &Wire, -1);
#endif

// =======================================================================
//               İSTEĞE BAĞLI CALLBACK FONKSİYONU
// =======================================================================

// Bu fonksiyon sadece eğer OLED ekran kullanacaksak gereklidir.
// Kütüphaneden gelen durum mesajlarını yakalayıp ekrana yazdırma işini yapar.
#ifdef USE_OLED_DISPLAY
void oledDurumGoster(const char* statusMessage) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(statusMessage); // Gelen mesajı doğrudan ekrana yaz.
  display.display();
}
#endif

// =======================================================================
//                             ANA PROGRAM
// =======================================================================

void setup() {
  Serial.begin(115200);
  Serial.println("\nProje Başlatılıyor...");

  // Eğer özel bir buton tanımlandıysa, pin modunu ayarla.
  #ifdef USE_CUSTOM_BUTTON
    pinMode(CUSTOM_UPDATE_PIN, INPUT_PULLUP);
    Serial.println("Ozel guncelleme butonu ayarlandi: Pin " + String(CUSTOM_UPDATE_PIN));
  #endif

  // Eğer OLED ekran kullanılacaksa, onu başlat ve kütüphaneye callback fonksiyonunu kaydet.
  #ifdef USE_OLED_DISPLAY
    Wire.begin();
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println(F("OLED Ekran baslatilamadi."));
    } else {
      Serial.println("OLED Ekran baslatildi ve callback olarak ayarlandi.");
      // Kütüphaneye, durum mesajları olduğunda bizim fonksiyonumuzu çağırmasını söylüyoruz.
      ota.onStatus(oledDurumGoster);
    }
  #endif

  // Kütüphaneyi başlat. "true" parametresi, WiFi bağlanamazsa offline modda devam etmesini sağlar.
  ota.baslat(true); 

  Serial.println("Kurulum tamamlandi. Ana donguye giriliyor...");
}

void loop() {
  // Hangi butonun kullanılacağını otomatik olarak seçer ve dinler.
  #ifdef USE_CUSTOM_BUTTON
    ota.dongu(CUSTOM_UPDATE_PIN); // Harici butonu dinle
  #else
    ota.dongu(); // Varsayılan olarak GPKEY'i dinle
  #endif

  // Buradan sonra kendi projenizin ana döngü kodları çalışabilir.
  // Örneğin her saniye sensör okuyup Seri Port'a yazdırabilirsiniz.
  // delay(1000);
  // Serial.println("Ana program calisiyor...");
}