███╗   ███╗ ██████╗ ███╗   ██╗ ██████╗ ██╗     ██╗████████╗██╗  ██╗
████╗ ████║██╔═══██╗████╗  ██║██╔═══██╗██║     ██║╚══██╔══╝██║  ██║
██╔████╔██║██║   ██║██╔██╗ ██║██║   ██║██║     ██║   ██║   ███████║
██║╚██╔╝██║██║   ██║██║╚██╗██║██║   ██║██║     ██║   ██║   ██╔══██║
██║ ╚═╝ ██║╚██████╔╝██║ ╚████║╚██████╔╝███████╗██║   ██║   ██║  ██║
╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝ ╚══════╝╚═╝   ╚═╝   ╚═╝  ╚═╝

  ░▒▓█ _obelisk_ █▓▒░  
  The central hub of **MONOLITH** project.
  It's your desktop clock, calendar, and gateway to connected devices.  

  ───────────────────────────────────────────────────────────────  
  ▓ OVERVIEW  
  **_obelisk_** is the main control system of MONOLITH, powered by an  
  **ESP32** and **STM32F103**.  
  It serves as a desktop clock, calendar, weather display,  
  and the communication bridge for peripheral devices like _vertex_.  
  The **ESP32** handles USART-to-WebSocket bridging,  
  while the **STM32** drives core functionalities.  

  ───────────────────────────────────────────────────────────────  
  ▓ HARDWARE  
  ▪ **MCUs**: STM32F103 (Main), ESP32 (Bridge)  
  ▪ **DISPLAYS**: 20x4 LCD  
  ▪ **INPUTS**: Temperature & Humidity Sensors, Buttons  
  ▪ **OUTPUTS**: Buzzer, Backlight  
  ▪ **INTERFACE**: USART, WebSocket (via ESP32)  
  ▪ **POWER**: USB / External 5V  

  ───────────────────────────────────────────────────────────────  
  ▓ FEATURES  
  ▪ Real-time clock with calendar & weather display  
  ▪ WebSocket-based remote access via ESP32  
  ▪ USART communication with **_vertex_** and other devices  
  ▪ Expandable with additional sensors and outputs  
  ▪ Designed for **low-latency, high-reliability** operation  

  ───────────────────────────────────────────────────────────────  
  ▓ BUILD & FLASH  
  Compile **STM32** firmware with **ARM GCC**  
  Flash using **stlink**  

  make &&
  make flash

  Compile **ESP32** firmware with **ESP-IDF**  
  Flash using **esptool**  

  idf.py build &&
  idf.py flash

  ───────────────────────────────────────────────────────────────  
  ▓ USART COMMANDS  
  _obelisk_ listens for USART commands and relays data to connected devices.  
  Sample command format:  

  CMD:TIME:GET            ; Request current time  
  CMD:WEATHER:UPDATE      ; Fetch new weather data  
  CMD:VERTEX:SET_MODE:2   ; Set lighting mode on _vertex_  

  ───────────────────────────────────────────────────────────────  
  ▓ FINAL WORDS  
  This is more than a clock.
  This is **_obelisk_**—the heart of MONOLITH.  
