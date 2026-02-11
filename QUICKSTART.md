# ⚡ Quick Start - Path Pulse

Get your blind assistance system running in **30 minutes**!

## 📦 What You Need

- [ ] ESP32 board
- [ ] 3× HC-SR04 ultrasonic sensors  
- [ ] Power bank
- [ ] Jumper wires
- [ ] Smartphone

## 🔌 Step 1: Wire It (10 min)

```
Front Sensor:
  VCC  → ESP32 5V
  TRIG → GPIO 27
  ECHO → GPIO 26
  GND  → ESP32 GND

Right Sensor:
  VCC  → ESP32 5V
  TRIG → GPIO 33
  ECHO → GPIO 32
  GND  → ESP32 GND

Back Sensor:
  VCC  → ESP32 5V
  TRIG → GPIO 12
  ECHO → GPIO 14
  GND  → ESP32 GND
```

##💻 Step 2: Upload Code (10 min)

1. Install Arduino IDE
2. Add ESP32 board support
3. Open `src/blind_assist.ino`
4. Select: Tools → Board → ESP32 Dev Module
5. Select your port
6. Click Upload (→)

## 📱 Step 3: Connect Phone (5 min)

1. Power on ESP32
2. Phone Settings → Wi-Fi
3. Connect to: **BlindAssist**
4. Password: **12345678**

## 🌐 Step 4: Open App (5 min)

**Option A:** Use your app
**Option B:** Open browser → `http://192.168.4.1/alert`

## ✅ Test It!

1. Power on device
2. Connect phone
3. Wave hand in front → Should say "Obstacle front, very close..."
4. Remove hand → Should say "Path clear"

## 🎉 Done!

You now have a working blind assistance system!

**Next Steps:**
- Read full [README.md](README.md)
- Watch [Demo Video](demo/performance_video.mp4)
- Customize settings
- Build enclosure

---

**Troubleshooting:**

Can't connect? → Check ESP32 is powered on
No readings? → Verify wiring matches pins
App not working? → Try browser first

**Need Help?** See full [README.md](README.md) troubleshooting section
