/******************************************************************
 * DeneyapSinirtanimayaOTA.h
 * Sürüm - v2.0 (Esnek Ekran Desteði)
 * EFE ALÝ BOZKURT 2025 'SADECE HAYAL ET'
 ******************************************************************/

#ifndef DeneyapSinirtanimayaOTA_h
#define DeneyapSinirtanimayaOTA_h

#include "Arduino.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

// Callback fonksiyonunun imzasýný tanýmlýyoruz.
// Kullanýcý bu yapýda bir fonksiyon yazarak kütüphanenin mesajlarýný yakalayabilecek.
typedef void (*OtaStatusCallback)(const char* statusMessage);

class DeneyapSinirtanimayaOTA {
  public:
    // Kurucu fonksiyon sadeleþtirildi, artýk ekranla ilgili bir parametre almýyor.
    DeneyapSinirtanimayaOTA(const char* ssid, const char* password, const char* githubToken, 
                           String versionUrl, String binUrl, String currentVersion);

    // Kütüphaneyi baþlatýr.
    void baslat(bool offlineModaIzinVer = false);

    // Kütüphanenin döngüsünü çalýþtýrýr.
    void dongu();
    void dongu(int customButtonPin);
    
    // YENÝ: Kullanýcýnýn kendi durum gösterme fonksiyonunu kaydetmesi için.
    void onStatus(OtaStatusCallback callback);

  private:
    // Ayar deðiþkenleri (deðiþiklik yok)
    const char* _ssid;
    const char* _password;
    const char* _githubToken;
    String _firmware_version_url;
    String _firmware_bin_url;
    String _current_version;

    // Dahili zamanlama deðiþkenleri (deðiþiklik yok)
    unsigned long _lastButtonPressTime = 0;
    const unsigned long _debounceDelay = 2000;

    // YENÝ: Callback fonksiyonunu tutacak olan iþaretçi
    OtaStatusCallback _statusCallback = nullptr;

    // Özel fonksiyonlar (içerikleri deðiþti)
    void _sendUpdateStatus(const char* message);
    bool _yeniSurumVarMi(String serverVersion);
    void _guncellemeyiUygula();
    void _githubKontrolu();
};

#endif
