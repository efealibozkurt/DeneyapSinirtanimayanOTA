/******************************************************************
 * OTA Örnek Projesi - v1.0
 * Sürüm - v1.0
 * EFE ALİ BOZKURT 2025 'SADECE HAYAL ET'
 ******************************************************************/

#include <DeneyapSinirtanimayaOTA.h>

// =======================================================================
//                           KULLANICI AYARLARI
// =======================================================================

const char* WIFI_SSID = "SENIN_WIFI_AGININ_ADI";
const char* WIFI_PASSWORD = "SENIN_WIFI_SIFREN";
const char* GITHUB_TOKEN = "ghp_XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"; 
const String FIRMWARE_VERSION_URL = "https://raw.githubusercontent.com/KULLANICIADI/REPOİSMİ/main/README.md";
const String FIRMWARE_BIN_URL = "https://raw.githubusercontent.com/KULLANICIADI/REPOİSMİ/main/firmware.bin";
const String FIRMWARE_VERSION = "1.0";

// #define CUSTOM_UPDATE_PIN 12 // İSTEĞE BAĞLI GÜNCELLEME KONTROL PİNİ BELİRLE

// =======================================================================
//                             KÜTÜPHANE KURULUMU
// =======================================================================

DeneyapSinirtanimayaOTA ota(WIFI_SSID, WIFI_PASSWORD, GITHUB_TOKEN, FIRMWARE_VERSION_URL, FIRMWARE_BIN_URL, FIRMWARE_VERSION);

// =======================================================================
//                             ANA PROGRAM
// =======================================================================

void setup() {
  #ifdef CUSTOM_UPDATE_PIN
    pinMode(CUSTOM_UPDATE_PIN, INPUT_PULLUP);
  #endif

  // Kütüphaneyi başlat. "true" parametresi, WiFi bağlanamazsa
  // kodun çalışmaya devam etmesine (offline moda) izin verir.
  // "false" yaparsan veya boş bırakırsan, bağlanamazsa kendini yeniden başlatır.
  ota.baslat(true); 

  Serial.println("Kurulum tamamlandi. Ana donguye giriliyor...");
}

void loop() {
  #ifdef CUSTOM_UPDATE_PIN
    ota.dongu(CUSTOM_UPDATE_PIN);
  #else
    ota.dongu();
  #endif

  // Kendi kodların...
}