# 🎸 DIY-Guitar-Tuner-Arduino

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Arduino](https://img.shields.io/badge/Platform-Arduino-blue.svg)](https://www.arduino.cc/)

[🇺🇸 English](#-english) | [🇹🇷 Türkçe](#-türkçe)

---

## 🇺🇸 English

**DIY-Guitar-Tuner-Arduino** is an automated, smart tuning project that listens to guitar notes via a microphone and uses a stepper motor to adjust the tuning pegs automatically. It features high-precision **pitch detection** and a minimalist UI on an **SSD1306 OLED** display.

### 🌟 Key Features
* **Automatic Tuning:** Real-time motor control for hands-free tuning.
* **Precision Pitch Detection:** Zero-crossing frequency analysis algorithm.
* **Visual Guidance:** Intuitive inward-pointing arrows on a 0.96" OLED screen.
* **Compact Design:** Optimized for Arduino Nano/Uno platforms.

### 🛠️ Hardware List
* **Arduino Nano / Uno**
* **0.96" OLED Display** (I2C, **SSD1306** Driver)
* **28BYJ-48 Stepper Motor** + **ULN2003** Driver
* **Microphone Module** (Generic LM393 or **MAX9814** Amplifier)
* **3x LEDs** (Red, Green, Yellow) & 1x Button

### 🔌 Wiring (Pinout)

| Component | Arduino Pin |
| :--- | :--- |
| **Mic Out (Analog)** | `A0` |
| **OLED (SDA / SCL)** | `A4` / `A5` |
| **Button** | `D2` |
| **LEDs (Red / Green / Yellow)** | `D3` / `D4` / `D5` |
| **Motor (IN1 / IN3 / IN2 / IN4)** | `D8` / `D10` / `D9` / `D11` |

### 🤝 Acknowledgements
This project was developed during my internship at **Robotistan**.
You can find the detailed tutorial and project story here:
👉 [maker.robotistan.com/gitar-akort-cihazi](https://maker.robotistan.com/gitar-akort-cihazi/)

---

## 🇹🇷 Türkçe

**DIY-Guitar-Tuner-Arduino**, mikrofon aracılığıyla gitar notalarını algılayan ve bir step motor kullanarak burguları otomatik olarak ayarlayan akıllı bir akort projesidir. **SSD1306 OLED** ekran üzerinde hassas **perde algılama (pitch detection)** ve minimalist bir kullanıcı arayüzü sunar.

### 🌟 Temel Özellikler
* **Otomatik Akort:** El değmeden akort için gerçek zamanlı motor kontrolü.
* **Hassas Perde Algılama:** Zero-crossing frekans analizi algoritması.
* **Görsel Rehberlik:** 0.96" OLED ekranda sezgisel, içe bakan ok göstergeleri.
* **Kompakt Tasarım:** Arduino Nano/Uno platformları için optimize edilmiştir.

### 🛠️ Gerekli Malzemeler
* **Arduino Nano / Uno**
* **0.96" OLED Ekran** (I2C, **SSD1306** Sürücülü)
* **28BYJ-48 Step Motor** + **ULN2003** Sürücü
* **Mikrofon Modülü** (Standart LM393 veya daha iyi hassasiyet için **MAX9814**)
* **3x LED** (Kırmızı, Yeşil, Sarı) & 1x Buton

### 🔌 Bağlantı Şeması (Pinout)

| Bileşen | Arduino Pini |
| :--- | :--- |
| **Mikrofon Çıkış (Analog)** | `A0` |
| **OLED (SDA / SCL)** | `A4` / `A5` |
| **Buton** | `D2` |
| **LED'ler (Kırmızı / Yeşil / Sarı)**| `D3` / `D4` / `D5` |
| **Motor (IN1 / IN3 / IN2 / IN4)** | `D8` / `D10` / `D9` / `D11` |

### 🤝 Teşekkür
Bu proje **Robotistan** stajım süresince geliştirilmiştir.
Projenin detaylı yapım aşamalarını ve hikayesini buradan inceleyebilirsiniz:
👉 [maker.robotistan.com/gitar-akort-cihazi](https://maker.robotistan.com/gitar-akort-cihazi/)

---

**License:** MIT License  
**Developed by:** Salih Mete Alkan
