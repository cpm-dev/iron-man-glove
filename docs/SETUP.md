# 🚀 Development Setup Guide

Complete guide for setting up your development environment for the Iron Man Glove project.

## Prerequisites

### Required Software

1. **Visual Studio Code**
   - Download: https://code.visualstudio.com/
   - Latest stable version recommended

2. **PlatformIO IDE Extension**
   - Install from VS Code Extensions (Ctrl+Shift+X)
   - Search for "PlatformIO IDE"
   - Click Install

3. **Git** (for cloning the repository)
   - Download: https://git-scm.com/
   - Verify installation: `git --version`

### Recommended Software

- **Arduino IDE** (optional, for testing components)
- **Serial Monitor Tool** (optional, PlatformIO has built-in)
- **Fritzing** (optional, for circuit diagrams)

## Quick Start

### 1. Clone the Repository

```bash
# Clone via HTTPS
git clone https://github.com/yourusername/ironman-glove.git

# Or clone via SSH
git clone git@github.com:yourusername/ironman-glove.git

# Navigate to project directory
cd ironman-glove
```

### 2. Open in VS Code

```bash
# Open current directory in VS Code
code .
```

Or use VS Code: File → Open Folder → Select `ironman-glove`

### 3. Let PlatformIO Initialize

When you first open the project:
1. PlatformIO will detect `platformio.ini`
2. It will automatically install:
   - AVR platform
   - Arduino framework
   - All required libraries
3. This takes 2-5 minutes on first run

**You'll see this in the status bar:**
```
PlatformIO: Installing dependencies...
```

### 4. Connect Arduino Nano

1. Plug in Arduino Nano via USB
2. Wait for drivers to install (Windows)
3. Check Device Manager (Windows) or `ls /dev/tty*` (Linux/Mac)
4. Note the COM port (e.g., COM3, /dev/ttyUSB0)

### 5. Build and Upload

**Option A: Using PlatformIO Toolbar**
1. Click PlatformIO icon in left sidebar (alien head)
2. Expand "nanoatmega328"
3. Click "Upload"

**Option B: Using Keyboard Shortcut**
- `Ctrl+Alt+U` (Windows/Linux)
- `Cmd+Shift+U` (Mac)

**Option C: Using Command Palette**
1. `Ctrl+Shift+P` (or `Cmd+Shift+P` on Mac)
2. Type "PlatformIO: Upload"
3. Press Enter

### 6. Open Serial Monitor

**After uploading, view debug output:**
1. PlatformIO icon → nanoatmega328 → Monitor
2. Or keyboard shortcut: `Ctrl+Alt+S`
3. Set baud rate to 115200 if needed

You should see:
```
=== Iron Man Glove Starting ===
Initializing LED Controller...
LED Controller initialized
Initializing Motion Detector...
MPU6050 Found!
=== System Ready ===
```

## Project Structure

```
ironman-glove/
│
├── .pio/                    # PlatformIO build files (auto-generated)
│   ├── build/              # Compiled binaries
│   └── libdeps/            # Downloaded libraries
│
├── .vscode/                # VS Code settings (auto-generated)
│   └── extensions.json    # Recommended extensions
│
├── src/                    # Source code
│   ├── main.cpp           # Entry point
│   ├── config.h           # Configuration
│   ├── motion_detector.h  # Motion detection header
│   ├── motion_detector.cpp # Motion detection implementation
│   ├── led_controller.h   # LED control header
│   └── led_controller.cpp  # LED control implementation
│
├── test/                   # Unit tests (optional)
│
├── docs/                   # Documentation
│   ├── WIRING.md          # Wiring guide
│   └── SETUP.md           # This file
│
├── platformio.ini         # PlatformIO configuration
├── .gitignore            # Git ignore rules
└── README.md             # Project overview
```

## PlatformIO Configuration Explained

**platformio.ini breakdown:**

```ini
[env:nanoatmega328]
platform = atmelavr           # AVR microcontroller platform
board = nanoatmega328        # Arduino Nano board definition
framework = arduino          # Use Arduino framework

monitor_speed = 115200       # Serial monitor baud rate
monitor_filters = colorize   # Colored serial output

lib_deps =                   # Libraries to auto-install
    adafruit/Adafruit MPU6050 @ ^2.2.4
    adafruit/Adafruit Sensor @ ^1.1.14
    adafruit/Adafruit BusIO @ ^1.14.5
    fastled/FastLED @ ^3.6.0

build_flags =                # Compiler flags
    -D DEBUG=1               # Enable debug mode
    -Wall                    # Show all warnings

upload_speed = 115200        # Upload baud rate
```

## Customizing Build Configuration

### Debug vs Release Builds

**Debug build** (default):
```bash
pio run -e nanoatmega328
```
- Enables serial debugging
- Larger binary size
- Slower execution

**Release build** (optimized):
```bash
pio run -e nanoatmega328_release
```
- Disables serial debugging
- Smaller binary
- Faster execution

### Changing Configuration

Edit `platformio.ini` to:

**Add libraries:**
```ini
lib_deps = 
    existing/library
    new/library @ version
```

**Change upload port:**
```ini
upload_port = COM3          # Windows
upload_port = /dev/ttyUSB0  # Linux
```

**Adjust optimization:**
```ini
build_flags = 
    -Os                     # Optimize for size
    -O2                     # Optimize for speed
```

## Common Development Tasks

### Rebuild from Scratch
```bash
pio run --target clean
pio run
```

### Upload without building
```bash
pio run --target upload
```

### View compiled size
```bash
pio run --target size
```

### Update libraries
```bash
pio pkg update
```

### List serial ports
```bash
pio device list
```

## Debugging

### Serial Debug Output

The code uses these macros (defined in config.h):
```cpp
DEBUG_PRINT(x)    // Print without newline
DEBUG_PRINTLN(x)  // Print with newline
```

**Enable/Disable in platformio.ini:**
```ini
build_flags = 
    -D DEBUG=1    # Enable
    -D DEBUG=0    # Disable
```

### Reading Debug Output

Open Serial Monitor and you'll see:
```
Pitch: 12.34
Pitch: 15.67
HAND RAISED - ACTIVATING!
*** ACTIVATING IRON MAN MODE ***
LED Activation started
Power-up complete, entering steady mode
```

### Common Debug Scenarios

**Sensor not detected:**
```
Failed to find MPU6050 chip
Check wiring:
  SDA -> A4
  SCL -> A5
  VCC -> 3.3V
  GND -> GND
```

**Normal operation:**
```
Pitch: 45.23
HAND RAISED - ACTIVATING!
Starting fade out
Fade out complete
```

## Testing Without Hardware

### Compiling without Upload

```bash
pio run --target build
```

This verifies:
- Code compiles successfully
- No syntax errors
- Libraries are compatible
- Binary fits in flash memory

### Using Simulators (Advanced)

PlatformIO supports simulation with SimAVR, but it's complex for beginners. Better to test with real hardware.

## Troubleshooting

### "Command not found: pio"

PlatformIO commands work in VS Code terminal automatically. If using external terminal:
- Windows: `%USERPROFILE%\.platformio\penv\Scripts\pio.exe`
- Mac/Linux: `~/.platformio/penv/bin/pio`

### Upload Fails

**Error: "Port not available"**
```bash
# List available ports
pio device list

# Specify port manually in platformio.ini
upload_port = COM3  # or /dev/ttyUSB0
```

**Error: "Permission denied" (Linux/Mac)**
```bash
# Add user to dialout group
sudo usermod -a -G dialout $USER

# Or use sudo (not recommended)
sudo pio run --target upload
```

### Libraries Won't Install

**Clear cache and retry:**
```bash
rm -rf .pio
pio run
```

### Out of Memory Errors

**Arduino Nano has limited RAM (2KB)**

Check memory usage:
```bash
pio run --target size
```

If RAM is >80% full:
- Reduce `NUM_LEDS`
- Disable DEBUG mode
- Optimize string usage
- Use `F()` macro for strings in flash

## Advanced Configuration

### Custom Build Flags

**Enable specific features:**
```ini
build_flags = 
    -D FEATURE_X=1
    -D MAX_VALUE=100
    -D USE_ALTERNATIVE_PINS
```

### Conditional Compilation

In code:
```cpp
#ifdef FEATURE_X
    // This code only compiles if FEATURE_X is defined
#endif
```

### Multiple Environments

**Different hardware configs:**
```ini
[env:prototype]
board = nanoatmega328
build_flags = -D LED_COUNT=5

[env:final]
board = nanoatmega328
build_flags = -D LED_COUNT=10
```

Build specific environment:
```bash
pio run -e prototype
pio run -e final
```

## VS Code Tips

### Recommended Extensions

1. **C/C++** (Microsoft) - IntelliSense
2. **PlatformIO IDE** - Already installed
3. **Better Comments** - Highlighted comments
4. **Error Lens** - Inline errors

### Keyboard Shortcuts

| Action | Shortcut |
|--------|----------|
| Build | `Ctrl+Alt+B` |
| Upload | `Ctrl+Alt+U` |
| Serial Monitor | `Ctrl+Alt+S` |
| Clean | `Ctrl+Alt+C` |
| Command Palette | `Ctrl+Shift+P` |

### IntelliSense Configuration

PlatformIO auto-configures IntelliSense, but if you see red squiggles:

1. `Ctrl+Shift+P`
2. "C/C++: Edit Configurations (JSON)"
3. PlatformIO creates `.vscode/c_cpp_properties.json` automatically

## Next Steps

1. ✅ Environment set up
2. ✅ Code compiling
3. ✅ Upload successful
4. → Wire up hardware (see WIRING.md)
5. → Test and calibrate
6. → Customize and experiment!

## Getting Help

- **PlatformIO Docs**: https://docs.platformio.org/
- **Arduino Reference**: https://www.arduino.cc/reference/
- **FastLED Guide**: https://github.com/FastLED/FastLED/wiki
- **Adafruit MPU6050**: https://learn.adafruit.com/mpu6050-6-dof-accelerometer-and-gyro

---

Happy coding! 🚀
