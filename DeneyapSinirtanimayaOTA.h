/******************************************************************
 * DeneyapSinirtanimayaOTA.h
 * S�r�m - v2.0 (Esnek Ekran Deste�i)
 * EFE AL� BOZKURT 2025 'SADECE HAYAL ET'
 ******************************************************************/

#ifndef DeneyapSinirtanimayaOTA_h
#define DeneyapSinirtanimayaOTA_h

#include "Arduino.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

// Callback fonksiyonunun imzas�n� tan�ml�yoruz.
// Kullan�c� bu yap�da bir fonksiyon yazarak k�t�phanenin mesajlar�n� yakalayabilecek.
typedef void (*OtaStatusCallback)(const char* statusMessage);

class DeneyapSinirtanimayaOTA {
  public:
    // Kurucu fonksiyon sadele�tirildi, art�k ekranla ilgili bir parametre alm�yor.
    DeneyapSinirtanimayaOTA(const char* ssid, const char* password, const char* githubToken, 
                           String versionUrl, String binUrl, String currentVersion);

    // K�t�phaneyi ba�lat�r.
    void baslat(bool offlineModaIzinVer = false);

    // K�t�phanenin d�ng�s�n� �al��t�r�r.
    void dongu();
    void dongu(int customButtonPin);
    
    // YEN�: Kullan�c�n�n kendi durum g�sterme fonksiyonunu kaydetmesi i�in.
    void onStatus(OtaStatusCallback callback);

  private:
    // Ayar de�i�kenleri (de�i�iklik yok)
    const char* _ssid;
    const char* _password;
    const char* _githubToken;
    String _firmware_version_url;
    String _firmware_bin_url;
    String _current_version;

    // Dahili zamanlama de�i�kenleri (de�i�iklik yok)
    unsigned long _lastButtonPressTime = 0;
    const unsigned long _debounceDelay = 2000;

    // YEN�: Callback fonksiyonunu tutacak olan i�aret�i
    OtaStatusCallback _statusCallback = nullptr;

    // �zel fonksiyonlar (i�erikleri de�i�ti)
    void _sendUpdateStatus(const char* message);
    bool _yeniSurumVarMi(String serverVersion);
    void _guncellemeyiUygula();
    void _githubKontrolu();
};

#endif
