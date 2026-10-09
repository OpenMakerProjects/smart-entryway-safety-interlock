# Architecture

ESP32 ADC1 GPIO34 samples an LDR divider every 100 ms. The Interlock policy requires valid non-rail light ≥500 and MQTT connectivity for 3000 ms before explicit ARM. GPIO25 indicates permission through a 330 Ω green LED. OLED I2C21/22 shows state. There is no actuator or certified safety function.

See the README, circuit SVG and firmware for the complete behavior. Historical seed files are retained; PlatformIO builds only firmware/main.cpp.
