- Sinh viên: Phạm Tuấn Minh 
- MSSV: 25112081
- Nhóm: 11
- Công cụ: PlatformIO (espressif32@6.9.0, board esp32dev, framework Arduino)
Bài 1
Đã làm: ap.cpp cho ESP32 phát Wi-Fi bằng WiFi.softAP(); sta.cpp cho ESP32 nối vào AP đó bằng WiFi.begin() và in IP.
Quan sát:
- AP: SSID nhóm đặt = <điền>; Serial in AP IP: 192.168.4.1.
- STA: Serial in dấu chấm rồi IP nhận được = <điền>.
Bài 2
Đã làm: ESP32 làm BLE server, tên thiết bị ESP32-BLE-NHOM-XX, một service + một characteristic (UUID service 4fafc201-…, characteristic beb5483e-…) chứa thông điệp riêng (MSSV), thêm PROPERTY_WRITE.
Quan sát: nRF Connect quét thấy tên nhóm; đọc characteristic được <giá trị>; ghi giá trị mới <giá trị ghi> rồi đọc lại thấy đã đổi (BLE phải dùng đúng UUID).
Bài 3
Đã làm: env mac in địa chỉ MAC; receiver.cpp đăng ký callback in số đếm; transmitter.cpp gửi struct GoiTin { int dem; } tới MAC của Receiver mỗi 2 giây. Cả hai kit để WIFI_STA, struct hai bên giống hệt.
Quan sát: Serial của Receiver in số đếm tăng dần.
Thử chéo nhóm: đổi MAC sang kit nhóm khác → kết quả <có/không nhận được>.
Bài 4
Đã làm: ESP32 nối Wi-Fi (STA), chạy WebServer cổng 80 với các route /, /on, /off, /toggle; trang hiển thị rõ LED đang BẬT/TẮT. LED nối GPIO2 qua điện trở 220 Ω.
Quan sát: mở IP của ESP32 trên điện thoại cùng mạng, bấm BẬT/TẮT thì LED đổi trạng thái tương ứng.

Sơ đồ đấu nối: docs/bai4_so_do.svg và docs/bai4.fzz (Fritzing). Ảnh/video: media/bai4/.
Bài 5
Đã làm: ESP32 nối Wi-Fi rồi nối broker <test.mosquitto.org hoặc IP Mosquitto trong LAN> cổng 1883; publish số đếm mỗi 2 giây vào bcse/th3/nhom-XX; subscribe bcse/th3/nhom-XX/led (nhận 1 bật LED, 0 tắt LED). Client id lấy từ MAC nên khác nhau giữa các kit; gọi mqtt.loop() liên tục trong loop().
Quan sát: app MQTT (subscribe bcse/th3/nhom-XX) thấy số đếm tăng dần; publish 1 / 0 vào topic .../led thì LED bật/tắt.