# DeneyapSinirtanimayaOTA Arduino Kütüphanesi

![Arduino](https://img.shields.io/badge/Arduino-Uyumlu-00979D?style=for-the-badge&logo=arduino)
![Platform](https://img.shields.io/badge/Platform-ESP32-E7352C?style=for-the-badge&logo=espressif)
![Lisans](https://img.shields.io/badge/Lisans-MIT-yellow.svg?style=for-the-badge)

**DeneyapSinirtanimayaOTA**, ESP32 tabanlı geliştirme kartları (özellikle **Deneyap Kart**) için GitHub üzerinden OTA (Over-the-Air) yazılım güncellemesini son derece basitleştiren bir Arduino kütüphanesidir. Bu kütüphane sayesinde, cihazlarınıza fiziksel olarak bağlanmadan, dünyanın neresinde olursanız olun internet üzerinden kolayca yeni yazılım yükleyebilirsiniz.

## Öne Çıkan Özellikler

-   🌍 **Uzaktan Güncelleme:** Cihazınız sizden kilometrelerce uzakta olsa bile, güncelleme için tek gereken bir WiFi bağlantısıdır.
-   ⚙️ **Kolay Entegrasyon:** Projenize OTA yeteneği kazandırmak için `setup()` ve `loop()` fonksiyonlarına sadece birer satır kod eklemeniz yeterlidir.
-   📺 **OLED Ekran Desteği:** SSD1306 OLED ekranlar üzerinden güncelleme durumu hakkında (bağlantı, indirme, başarı, hata) anlık geri bildirim alabilirsiniz.
-   🖲️ **Esnek Buton Kontrolü:** Güncellemeleri manuel olarak tetiklemek için Deneyap Kart üzerindeki dahili `GPKEY` butonunu veya harici bağlayacağınız herhangi bir butonu kullanabilirsiniz.
-   🌐 **Çevrimdışı (Offline) Mod:** Başlangıçta WiFi bağlantısı kurulamadığında cihazınızın kilitlenmesini veya sürekli yeniden başlamasını engeller, projeniz çalışmaya devam eder.
-   🔒 **Güvenli Erişim:** Özel (private) GitHub repolarından güncelleme çekmek için Kişisel Erişim Token'ı (Personal Access Token) kullanımını destekler.

## Gereksinimler

-   **Donanım:**
    -   ESP32 tabanlı bir geliştirme kartı (Deneyap Kart ile test edilmiştir).
    -   SSD1306 I2C OLED Ekran (128x64).
-   **Yazılım:**
    -   Arduino IDE
    -   Arduino IDE için ESP32 Kart Yöneticisi
    -   `Adafruit GFX Library`
    -   `Adafruit SSD1306 Library`

## Kurulum

1.  **ZIP Kütüphanesi Olarak Ekleme (Tavsiye Edilen):**
    -   Bu reponun son sürümünü `Code > Download ZIP` yoluyla indirin.
    -   Arduino IDE'de `Taslak > Kütüphaneyi Dahil Et > .ZIP Kitaplığı Ekle` menüsünden indirdiğiniz `.zip` dosyasını seçin.
2.  **Manuel Kurulum:**
    -   Bu repoyu bilgisayarınıza klonlayın veya indirin.
    -   Klasörün tamamını Arduino kütüphane klasörünüze kopyalayın (genellikle `Belgeler/Arduino/libraries`).
    -   Arduino IDE'yi yeniden başlatın.

## Temel Kullanım

```cpp
#include <DeneyapSinirtanimayaOTA.h>

// --- AYARLARINIZI BURAYA GİRİN ---
const char* WIFI_SSID = "SENIN_WIFI_AGININ_ADI";
const char* WIFI_PASSWORD = "SENIN_WIFI_SIFREN";
const char* GITHUB_TOKEN = "ghp_XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"; 
const String FIRMWARE_VERSION_URL = "[https://raw.githubusercontent.com/kullanici/repo/main/version.txt](https://raw.githubusercontent.com/kullanici/repo/main/version.txt)";
const String FIRMWARE_BIN_URL = "[https://raw.githubusercontent.com/kullanici/repo/main/firmware.bin](https://raw.githubusercontent.com/kullanici/repo/main/firmware.bin)";
const String FIRMWARE_VERSION = "1.0"; // Bu cihaza yüklenen yazılımın mevcut versiyonu

// Kütüphaneden bir nesne oluşturun
DeneyapSinirtanimayaOTA ota(WIFI_SSID, WIFI_PASSWORD, GITHUB_TOKEN, 
                          FIRMWARE_VERSION_URL, FIRMWARE_BIN_URL, FIRMWARE_VERSION);

void setup() {
  // WiFi bağlanamazsa bile kodun devam etmesi için "true" parametresini verin.
  ota.baslat(true); 
}

void loop() {
  // Varsayılan olarak GPKEY butonunu dinler.
  ota.dongu();

  // Kendi kodlarınız buraya...
}
```
## GitHub Repo Hazırlığı (Kütüphaneyi Kullananlar İçin)
Bu kütüphanenin projenizdeki güncellemeleri çekebilmesi için, projenizin bulunduğu GitHub reposunu aşağıdaki gibi hazırlamanız gerekmektedir:

firmware.bin Dosyası: Arduino IDE'de Taslak > Derlenmiş Binary'i Çıkar seçeneği ile oluşturduğunuz .bin dosyasını reponuzun ana dizinine yükleyin. Bu dosya, cihaza yüklenecek olan asıl programdır.

Sürüm Kontrol Dosyası: Reponuzda basit bir metin dosyası (version.txt, README.md vb.) oluşturun. Kütüphane, bu dosyanın içinde yeni sürüm bilgisini arayacaktır. Sürüm bilgisi aşağıdaki özel formatta olmalıdır:

Proje hakkında bilgiler...
Son Sürüm: [OTA-VERSION:1.2]
Daha fazla bilgi...
Bu basit etiketli yapı, ESP32'nin belleğini yormadan sürüm bilgisini kolayca okumasını sağlar.

## Fonksiyonlar
DeneyapSinirtanimayaOTA(...): Kütüphaneyi kurar ve tüm ayarları alır.
baslat(bool offlineModaIzinVer): Cihazı başlatır. true parametresi, WiFi yoksa cihazın çevrimdışı çalışmasına izin verir. false veya boş bırakılırsa, bağlantı kuramayınca cihaz kendini yeniden başlatır.
dongu(): Varsayılan GPKEY butonunu dinler.
dongu(int customButtonPin): Belirttiğiniz harici butonu dinler.

## Lisans
Bu proje, MIT Lisansı altında dağıtılmaktadır. Detaylar için LICENSE dosyasına bakınız.
