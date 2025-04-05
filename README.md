# Earthing Health Monitoring System



https://github.com/user-attachments/assets/37b62411-7779-48db-ba10-77b12e59e71d



A sensor-based system designed to monitor the **health of earthing systems** in electrical poles. This project detects and reports key parameters like **earth resistance**, **leakage current**, and **earthing continuity**, ensuring safety and compliance in electrical infrastructure.

## 🔍 Project Overview

The system continuously monitors:
- ⚡ **Leakage Current**: To detect insulation failures or current escaping to the ground.
- 🌍 **Earth Resistance**: Using the Fall of Potential method to check the effectiveness of the earthing system.
- 🔗 **Continuity of Earthing**: Ensures the earthing wire is not broken or disconnected.

This information is displayed on an **LCD** and optionally sent to a **central monitoring system** for real-time analysis and alerts.

## 🛠️ Hardware Used

- **ESP32 / Arduino UNO** (controller)
- **ADS1015** – 12-bit ADC for precision voltage measurement
- **ACS712** – Current sensor for leakage current detection
- **16x4 LCD** – To display real-time readings
- **Electrodes / Probes** – For measuring earth resistance using Fall of Potential method
- **Resistors, Connectors, Breadboard/PCB, Power Supply**

## 📐 Methodology

### 1. **Earth Resistance Measurement**
- Uses the **Fall of Potential Method** (3-point method)
- Formula: `ρ = 2πa (V/I)`, where:
  - `ρ` = soil resistivity
  - `a` = distance between voltage and current electrodes
  - `V` = measured voltage
  - `I` = injected current

### 2. **Leakage Current Monitoring**
- ACS712 outputs voltage proportional to the current
- ADC reads the sensor output
- Current = (ADC voltage - Offset) / Sensitivity

### 3. **Continuity Check**
- A small test voltage is applied and the return signal is checked
- If there's a break, the voltage doesn't return → continuity is broken

## 📊 Output

- **LCD Displays:**
  - Earth resistance (in ohms)
  - Leakage current (in mA)
  - Continuity status (OK / Fault)
- **Optional:** Data can be transmitted to a remote server or control room for logging and alerts (e.g., via LoRa, Wi-Fi, GSM)

## 💡 Applications

- Electrical utility poles in urban and rural areas
- Renewable energy systems (solar farms, wind turbines)
- Industrial safety monitoring
- Smart grids and substations

## 🚀 Getting Started

### Prerequisites
- Arduino IDE or PlatformIO
- Libraries:
  - `Wire.h` (I2C)
  - `Adafruit_ADS1X15.h` (for ADS1015)
  - `LiquidCrystal_I2C.h` (for 16x4 LCD)

### Uploading Code
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/earthing-health-monitor.git
   cd earthing-health-monitor
