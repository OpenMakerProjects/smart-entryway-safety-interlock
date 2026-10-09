# Wiring

Follow [the editable circuit](circuit-diagram.svg) and the exact pin table in [README](../README.md). Disconnect supplies before assembly. All grounds are common and GPIO is 3.3 V.

3V3 → LDR → GPIO34 junction → 10 kΩ → GND. OLED VCC3V3/GND/SDA21/SCL22. GPIO25 → 330 Ω → green LED anode; cathode GND. USB supplies the controller. No actuator or relay is connected.
