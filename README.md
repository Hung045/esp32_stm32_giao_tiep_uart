# Mạng cảm biến 2 node đơn giản

## Tổng Quan

Dự án này xây dựng một **hệ thống cảm biến không dây** gồm 2 nút (node) chính:
- **Node 1 (ESP32)**: Cảm biến nhiệt độ MLX90614 + Kết nối WiFi/MQTT
- **Node 2 (STM32)**: Cảm biến khoảng cách VL53L0X + Điều khiển LED

Hai nút giao tiếp với nhau thông qua **UART** để trao đổi dữ liệu cảm biến.

---

## Cấu Trúc Dự Án

```
kiem_tra/
├── node1_esp_mlx90614/          # ESP32 Node - Cảm biến nhiệt độ
│   ├── platformio.ini           # Cấu hình PlatformIO
│   ├── src/
│   │   └── main.cpp             # Chương trình chính
│   ├── include/
│   └── lib/
│
├── node2_stm32_vl53l0x/         # STM32 Node - Cảm biến khoảng cách
│   ├── mdk/                     # Keil MDK v5 project files
│   └── user/
│       ├── incl/                # Header files
│       │   ├── delay.h
│       │   ├── i2c_reg.h
│       │   ├── led.h
│       │   ├── uart_reg.h
│       │   └── vl53l0x.h
│       └── src/                 # Source files
│           ├── delay.c
│           ├── i2c_reg.c
│           ├── led.c
│           ├── main.c
│           ├── uart_reg.c
│           └── vl53l0x.c
│
└── README.md                    # Tài liệu này
```

---

## Chi Tiết từng Node

### **Node 1: ESP32 + MLX90614 (Cảm Biến Nhiệt Độ)**

**Chức năng:**
- Đọc dữ liệu nhiệt độ từ cảm biến **MLX90614** (infrared)
- Kết nối **WiFi** đến router
- Kết nối **MQTT** để gửi dữ liệu lên server (IP: 172.11.214.24)
- Nhận dữ liệu khoảng cách từ Node 2 (STM32)
- Điều khiển LED (GPIO 2)

**Thư viện sử dụng:**
- `Adafruit_MLX90614` - Driver cảm biến nhiệt độ
- `WiFi` - Kết nối WiFi
- `PubSubClient` - MQTT client
- `Wire` - I2C communication

**Thông số WiFi:**
```
SSID: "Đã 6 còn đi wave"
Password: "66774953"
MQTT Server: 172.11.214.24
```

**Cấu hình Serial:**
- UART2 (GPIO 16, 17) - Giao tiếp với STM32

---

### **Node 2: STM32F103C8 + VL53L0X (Cảm Biến Khoảng Cách)**

**Chức năng:**
- Đọc dữ liệu khoảng cách từ cảm biến **VL53L0X** (LIDAR)
- Gửi dữ liệu sang Node 1 qua **UART1** (115200 baud)
- Nhận lệnh từ ESP32
- Điều khiển **LED PC13**:
  - Bật LED khi khoảng cách < 250mm
  - Tắt LED khi khoảng cách ≥ 250mm
- In log debug qua **UART2** đến Hercules (9600 baud)

**Module chính:**
- `VL53L0X`: Đọc cảm biến khoảng cách qua I2C
- `UART_reg`: Giao tiếp serial (UART1, UART2)
- `I2C_reg`: Giao tiếp I2C với VL53L0X
- `LED`: Điều khiển LED (PC13)
- `Delay`: Hàm delay

**Cấu hình Serial:**
- **UART1** (PA9, PA10) - 115200 baud - Kết nối ESP32
- **UART2** (PA2, PA3) - 9600 baud - Debug (Hercules)

**Cấu hình I2C:**
- **I2C1** (PB6, PB7) - VL53L0X

---

## 🚀 Hướng Dẫn Build & Chạy

### **Node 1 (ESP32) - PlatformIO**

1. **Cài đặt VSCode + PlatformIO**
   ```
   - Cài extension PlatformIO
   - Mở thư mục node1_esp_mlx90614
   ```

2. **Build project:**
   ```
   PlatformIO: Build
   ```

3. **Upload lên ESP32:**
   ```
   PlatformIO: Upload
   ```

4. **Theo dõi Serial Monitor:**
   ```
   Baudrate: 115200
   ```

---

### **Node 2 (STM32) - Keil MDK v5**

1. **Mở project trong Keil:**
   ```
   File → Open Project
   Chọn: node2_stm32_vl53l0x/mdk/stm32_to_esp32.uvprojx
   ```

2. **Build project:**
   ```
   Project → Build Target
   ```

3. **Flash vào STM32:**
   ```
   Flash → Download
   ```

4. **Monitor debug output:**
   - Mở **Hercules Terminal**
   - Kết nối COM port (UART2 của STM32)
   - Baudrate: 9600

---

## Kết Nối Phần Cứng

### **Node 1 - ESP32:**
| Component | Pin | Mô tả |
|-----------|-----|-------|
| MLX90614 SDA | GPIO 21 | I2C Data |
| MLX90614 SCL | GPIO 22 | I2C Clock |
| LED | GPIO 2 | Indicator |
| UART2 RX | GPIO 16 | Nhận từ STM32 |
| UART2 TX | GPIO 17 | Gửi đến STM32 |

### **Node 2 - STM32F103C8:**
| Component | Pin | Mô tả |
|-----------|-----|-------|
| VL53L0X SDA | PB7 | I2C Data |
| VL53L0X SCL | PB6 | I2C Clock |
| LED | PC13 | Indicator (bật < 250mm) |
| UART1 TX | PA9 | Gửi đến ESP32 |
| UART1 RX | PA10 | Nhận từ ESP32 |
| UART2 TX | PA2 | Debug output |
| UART2 RX | PA3 | Debug input |

---

## Thông Tin Cảm Biến

### **MLX90614 (ESP32)**
- **Kiểu**: Cảm biến nhiệt độ hồng ngoại
- **Giao tiếp**: I2C
- **Phạm vi**: -40°C đến +125°C
- **Độ chính xác**: ±0.5°C
- **Địa chỉ I2C**: 0x5A (mặc định)

### **VL53L0X (STM32)**
- **Kiểu**: Cảm biến khoảng cách LIDAR (ToF - Time of Flight)
- **Giao tiếp**: I2C
- **Phạm vi**: 30mm - 1200mm
- **Độ chính xác**: ±3%
- **Địa chỉ I2C**: 0x29 (mặc định)

---

## Debug & Xử Sự Cố

### **Node 1 - ESP32:**

1. **Không kết nối WiFi:**
   - Kiểm tra SSID & mật khẩu trong `main.cpp`
   - Kiểm tra antenna ESP32
   - Xem Serial Monitor để xem thông báo lỗi

2. **MQTT không kết nối:**
   - Ping server: `172.11.214.24`
   - Kiểm tra firewall
   - Xem loại lỗi (rc code) từ Serial Monitor

3. **MLX90614 không đọc được:**
   - Kiểm tra kết nối I2C (SDA, SCL)
   - Kiểm tra địa chỉ I2C (0x5A)
   - Kiểm tra pullup resistor (~4.7kΩ)

### **Node 2 - STM32:**

1. **Không thấy log trên Hercules:**
   - Kiểm tra baudrate (9600)
   - Kiểm tra COM port đúng
   - Kiểm tra kết nối TX/RX

2. **VL53L0X không đọc được:**
   - Kiểm tra I2C1 initialization
   - Kiểm tra kết nối SDA/SCL (PB7, PB6)
   - Xem log từ Hercules

3. **LED không bật:**
   - Kiểm tra PC13 có high khi khoảng cách < 250mm không
   - Kiểm tra kết nối LED

---

## 📚 Tài Liệu Tham Khảo

- [Adafruit MLX90614 Library](https://github.com/adafruit/Adafruit-MLX90614-Library)
- [PubSubClient Library](https://github.com/knolleary/pubsub_client)
- [VL53L0X Datasheet](https://www.st.com/resource/en/datasheet/vl53l0x.pdf)
- [ESP32 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/)
- [STM32F103 Reference Manual](https://www.st.com/resource/en/reference_manual/rm0008.pdf)

---

**Lần cập nhật gần nhất**: 20/3/2026