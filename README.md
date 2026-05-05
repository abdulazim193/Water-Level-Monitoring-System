#  Water Level Monitoring System

![Arduino](https://img.shields.io/badge/Arduino-Uno-blue.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

An automatic water level control system that prevents tank overflow and pump dry-running using Arduino, analog sensor, I2C LCD, and relay module.

##  Demo


##  Features

- **Real-time monitoring** – 0–100% water level displayed on LCD
- **Automatic pump control** – Stops pump at 100% full
- **Dry-run protection** – Pump stays OFF when level < 20%
- **Manual override** – Push button to force pump ON/OFF
- **Visual alerts** – Bulb/LED indicates pump status
- **Low cost** – Built with components under $20

##  Components

| Component | Quantity |
|-----------|----------|
| Arduino Uno/Nano | 1 |
| Water level sensor (3-probe) | 1 |
| I2C LCD 16x2 | 1 |
| Single-channel relay | 1 |
| Push button | 1 |
| Bulb/LED (indicator) | 1 |
| Jumper wires | 20 |
| Breadboard | 1 |

##  Circuit Connections

| Arduino Pin | Connected To |
|-------------|--------------|
| 5V | Sensor VCC, LCD VCC, Relay VCC |
| GND | Sensor GND, LCD GND, Relay GND, Switch GND |
| A0 | Sensor signal (analog out) |
| A4 (SDA) | LCD SDA |
| A5 (SCL) | LCD SCL |
| D7 | Relay IN (controls pump) |
| D8 | Push button (internal pull-up) |

##  Code

Full code available in [`WLMS.ino`](./WLMS.ino)

### Key Logic

```cpp
// Map sensor value (0-1023) to percentage
int percent = map(analogRead(sensorPin), 0, 1023, 0, 100);

// Auto control
if (percent >= 100) digitalWrite(relayPin, LOW);  // Full -> OFF
if (percent <= 20) digitalWrite(relayPin, LOW);   // Low -> OFF
