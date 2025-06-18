/******************************************************************
 * DeneyapSinirtanimayaOTA.h
 * Sürüm - v1.0
 * EFE ALÝ BOZKURT 2025 'SADECE HAYAL ET'
 ******************************************************************/

#ifndef DeneyapSinirtanimayaOTA_h
#define DeneyapSinirtanimayaOTA_h

#include "Arduino.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class DeneyapSinirtanimayaOTA {
  public:
    // Kurucu fonksiyon: Kütüphaneyi baþlatmak için tüm ayarlarý alýr.
    DeneyapSinirtanimayaOTA(const char* ssid, const char* password, const char* githubToken, 
                           String versionUrl, String binUrl, String currentVersion);

    // Kütüphaneyi baþlatýr. Offline moda izin verilip verilmeyeceði seçilebilir.
    void baslat(bool offlineModaIzinVer = false);

    // Kütüphanenin döngüsünü çalýþtýrýr.
    void dongu();                     // Varsayýlan: Deneyap Kart'ýn GPKEY butonunu dinler.
    void dongu(int customButtonPin);  // Seçenek: Harici bir butonu dinler.

  private:
    const char* _ssid;
    const char* _password;
    const char* _githubToken;
    String _firmware_version_url;
    String _firmware_bin_url;
    String _current_version;

    Adafruit_SSD1306 _display;
    #define SCREEN_WIDTH 128
    #define SCREEN_HEIGHT 64
    #define OLED_RESET -1

    unsigned long _lastButtonPressTime = 0;
    const unsigned long _debounceDelay = 2000;

    void _ekranaYaz(String l1, String l2 = "", String l3 = "", String l4 = "");
    bool _yeniSurumVarMi(String serverVersion);
    void _guncellemeyiUygula();
    void _githubKontrolu();
};

#endif
