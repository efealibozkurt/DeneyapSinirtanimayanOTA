/******************************************************************
 * DeneyapSinirtanimayaOTA.cpp
 * Sürüm - v2.0
 * EFE ALÝ BOZKURT 2025 'SADECE HAYAL ET'
 ******************************************************************/

#include "DeneyapSinirtanimayaOTA.h"

DeneyapSinirtanimayaOTA::DeneyapSinirtanimayaOTA(const char* ssid, const char* password, const char* githubToken, 
                                               String versionUrl, String binUrl, String currentVersion) {
  // Kurucu fonksiyondan ekranla ilgili tüm kodlar kaldýrýldý.
  _ssid = ssid;
  _password = password;
  _githubToken = githubToken;
  _firmware_version_url = versionUrl;
  _firmware_bin_url = binUrl;
  _current_version = currentVersion;
}

// YENÝ: Callback fonksiyonunu kaydeder.
void DeneyapSinirtanimayaOTA::onStatus(OtaStatusCallback callback) {
  _statusCallback = callback;
}

// YENÝ: Durum mesajlarýný gönderen merkezi fonksiyon.
void DeneyapSinirtanimayaOTA::_sendUpdateStatus(const char* message) {
  // Mesajý her zaman Seri Port'a yazdýr.
  Serial.println(message);
  // Eðer kullanýcý bir callback fonksiyonu tanýmladýysa, onu da çaðýr.
  if (_statusCallback != nullptr) {
    _statusCallback(message);
  }
}

void DeneyapSinirtanimayaOTA::baslat(bool offlineModaIzinVer) {
  Serial.begin(115200);
  
  _sendUpdateStatus(("Cihaz Basliyor...\nOTA v" + _current_version).c_str());
  delay(1500);

  WiFi.mode(WIFI_STA);
  WiFi.begin(_ssid, _password);
  _sendUpdateStatus("Wi-Fi'ye baglaniliyor...");

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (retries++ > 30) {
      if (offlineModaIzinVer) {
        _sendUpdateStatus("HATA: WiFi baglantisi yok!\nOffline modda devam ediliyor.");
        delay(2000);
        break; 
      } else {
        _sendUpdateStatus("HATA: WiFi agina baglanilamadi!\nYeniden baslatiliyor...");
        delay(2000);
        ESP.restart();
      }
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    _sendUpdateStatus(("Baglanti OK!\nIP: " + WiFi.localIP().toString()).c_str());
    delay(1500);
    _githubKontrolu();
  }
}

// dongu() ve diðer fonksiyonlar _sendUpdateStatus kullanacak þekilde güncellendi.
// ... (Kodun geri kalaný ayný mantýkta çalýþýr, sadece _ekranaYaz yerine _sendUpdateStatus kullanýlýr)

void DeneyapSinirtanimayaOTA::dongu() {
  if (digitalRead(GPKEY) == 0) {
    if (millis() - _lastButtonPressTime > _debounceDelay) {
      _lastButtonPressTime = millis();
      _githubKontrolu();
    }
  }
}

void DeneyapSinirtanimayaOTA::dongu(int customButtonPin) {
  if (digitalRead(customButtonPin) == LOW) {
    if (millis() - _lastButtonPressTime > _debounceDelay) {
      _lastButtonPressTime = millis();
      _githubKontrolu();
    }
  }
}

bool DeneyapSinirtanimayaOTA::_yeniSurumVarMi(String serverVersion) {
  // Bu fonksiyon deðiþmedi
  int cMajor = 0, cMinor = 0, sMajor = 0, sMinor = 0;
  sscanf(_current_version.c_str(), "%d.%d", &cMajor, &cMinor);
  sscanf(serverVersion.c_str(), "%d.%d", &sMajor, &sMinor);
  Serial.printf("Karsilastirma: Cihaz(%d.%d), Sunucu(%d.%d)\n", cMajor, cMinor, sMajor, sMinor);
  if (sMajor > cMajor) return true;
  if (sMajor == cMajor && sMinor > cMinor) return true;
  return false;
}

void DeneyapSinirtanimayaOTA::_guncellemeyiUygula() {
  _sendUpdateStatus("Indirme basladi...");
  HTTPClient http;
  http.begin(_firmware_bin_url);
  http.setTimeout(30000);
  http.addHeader("Authorization", "token " + String(_githubToken));
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK && Update.begin(http.getSize())) {
    Update.writeStream(*http.getStreamPtr());
    if (Update.end(true)) {
      _sendUpdateStatus("Guncelleme BASARILI!\nCihaz yeniden baslatiliyor...");
      delay(2000);
      ESP.restart();
    }
  } else {
    _sendUpdateStatus(("OTA Hatasi: Indirme basarisiz!\nKod: " + String(httpCode)).c_str());
    delay(2000);
  }
  http.end();
}

void DeneyapSinirtanimayaOTA::_githubKontrolu() {
  if (WiFi.status() != WL_CONNECTED) {
    _sendUpdateStatus("OTA Hatasi: WiFi baglantisi yok!");
    delay(2000);
    return;
  }
  
  _sendUpdateStatus("Guncellemeler kontrol ediliyor...");
  HTTPClient http;
  String unique_url = _firmware_version_url + "?cb=" + String(millis());
  http.begin(unique_url);
  http.addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  http.addHeader("Authorization", "token " + String(_githubToken));
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    String versionTag = "[OTA-VERSION:";
    int startIndex = payload.indexOf(versionTag);
    if (startIndex != -1) {
      startIndex += versionTag.length();
      int endIndex = payload.indexOf(']', startIndex);
      if (endIndex != -1) {
        String serverVersion = payload.substring(startIndex, endIndex);
        serverVersion.trim();
        if (_yeniSurumVarMi(serverVersion)) {
          _sendUpdateStatus(("Yeni surum bulundu: v" + serverVersion + "\nIndiriliyor...").c_str());
          _guncellemeyiUygula();
        } else {
          _sendUpdateStatus(("Yazilim guncel.\nMevcut Surum: v" + _current_version).c_str());
          delay(2000);
        }
      }
    }
  } else {
    _sendUpdateStatus("OTA Hatasi: Surum dosyasi okunamadi.");
    delay(2000);
  }
  http.end();
}
