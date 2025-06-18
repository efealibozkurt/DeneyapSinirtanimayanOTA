/******************************************************************
 * DeneyapSinirtanimayaOTA.cpp
 * Sürüm - v1.0
 * EFE ALÝ BOZKURT 2025 'SADECE HAYAL ET'
 ******************************************************************/

#include "DeneyapSinirtanimayaOTA.h"

DeneyapSinirtanimayaOTA::DeneyapSinirtanimayaOTA(const char* ssid, const char* password, const char* githubToken, 
                                               String versionUrl, String binUrl, String currentVersion)
: _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {
  _ssid = ssid;
  _password = password;
  _githubToken = githubToken;
  _firmware_version_url = versionUrl;
  _firmware_bin_url = binUrl;
  _current_version = currentVersion;
}

void DeneyapSinirtanimayaOTA::baslat(bool offlineModaIzinVer) {
  Serial.begin(115200);
  Wire.begin();
  if(!_display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("OLED Ekran baslatilamadi."));
    for(;;);
  }

  _ekranaYaz("Cihaz Basliyor", "OTA v" + _current_version);
  delay(1500);

  WiFi.mode(WIFI_STA);
  WiFi.begin(_ssid, _password);
  _ekranaYaz("Wi-Fi'ye", "Baglaniliyor...");

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (retries++ > 30) {
      if (offlineModaIzinVer) {
        Serial.println("\nWiFi baglantisi kurulamadi. Offline modda devam ediliyor.");
        _ekranaYaz("HATA", "WiFi baglantisi", "yok!", "Offline mod...");
        delay(2000);
        break; 
      } else {
        _ekranaYaz("HATA", "WiFi agina", "baglanilamadi!", "Yeniden baslatiliyor...");
        delay(2000);
        ESP.restart();
      }
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    _ekranaYaz("Baglanti OK!", "IP:", WiFi.localIP().toString());
    delay(1500);
    _githubKontrolu();
  }
}

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

void DeneyapSinirtanimayaOTA::_ekranaYaz(String l1, String l2, String l3, String l4) {
  _display.clearDisplay();
  _display.setTextSize(1);
  _display.setTextColor(SSD1306_WHITE);
  _display.setCursor(0, 0);
  _display.println(l1); _display.println(l2); _display.println(l3); _display.println(l4);
  _display.display();
}

bool DeneyapSinirtanimayaOTA::_yeniSurumVarMi(String serverVersion) {
  int cMajor = 0, cMinor = 0, sMajor = 0, sMinor = 0;
  sscanf(_current_version.c_str(), "%d.%d", &cMajor, &cMinor);
  sscanf(serverVersion.c_str(), "%d.%d", &sMajor, &sMinor);
  Serial.printf("Karsilastirma: Cihaz(%d.%d), Sunucu(%d.%d)\n", cMajor, cMinor, sMajor, sMinor);
  if (sMajor > cMajor) return true;
  if (sMajor == cMajor && sMinor > cMinor) return true;
  return false;
}

void DeneyapSinirtanimayaOTA::_guncellemeyiUygula() {
  _ekranaYaz("Indirme basladi", "Sunucu'dan...");
  HTTPClient http;
  http.begin(_firmware_bin_url);
  http.setTimeout(30000);
  http.addHeader("Authorization", "token " + String(_githubToken));
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK && Update.begin(http.getSize())) {
    Update.writeStream(*http.getStreamPtr());
    if (Update.end(true)) {
      _ekranaYaz("Guncelleme", "BASARILI!", "Yeniden", "Baslatiliyor...");
      delay(2000);
      ESP.restart();
    }
  } else {
    _ekranaYaz("OTA Hatasi", "Indirme basarisiz", "Kod: " + String(httpCode));
    delay(2000);
  }
  http.end();
}

void DeneyapSinirtanimayaOTA::_githubKontrolu() {
  if (WiFi.status() != WL_CONNECTED) {
    _ekranaYaz("OTA Hatasi", "WiFi baglantisi", "yok!");
    delay(2000);
    return;
  }
  
  Serial.println("Guncellemeler kontrol ediliyor...");
  _ekranaYaz("Guncelleme", "Kontrol ediliyor...");
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
          _ekranaYaz("Yeni Surum!", "v" + serverVersion, "Indiriliyor...");
          _guncellemeyiUygula();
        } else {
          _ekranaYaz("OTA v" + _current_version, "Yazilim Guncel");
          Serial.println("Yaziliminiz guncel.");
          delay(2000);
        }
      }
    }
  } else {
    _ekranaYaz("OTA Hatasi", "Surum dosyasi", "okunamadi.");
    delay(2000);
  }
  http.end();
}
