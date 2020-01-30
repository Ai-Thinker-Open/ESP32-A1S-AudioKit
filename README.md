
# Ai-Thinker AudioKit Board SDK


EN | [中文](./README-zh.md)


- GitHub：https://github.com/Ai-Thinker-Open/ESP32-A1S-AudioKit.git
- Gitee：https://gitee.com/xuhongv/AiThinker-esp32-A1s-AudioKit

This framework is based on the second development of the ESPRESSIF audio framework esp-adf and follows the official open source agreement

## Overview

The framework is also suitable for the company's official development board such as esp-lyart, which also easily adds functions in the most comprehensive way, from simple to complex development of audio applications

- Music player or recorder supports audio formats such as MP3, AAC, FLAC, WAV, OGG, OPUS, AMR, TS, EQ, Downmixer, Sonic, ALC, G.711...
- Play music from sources: HTTP, HLS(HTTP Live Streaming), SPIFFS, SDCARD,  A2DP-Source, A2DP-Sink, HFP ...
- Integrate Media services such as: DLNA, VoIP ...
- Internet Radio
- Voice recognition and integration with online services such as Alexa, DuerOS, ...

## Developing with the ESP-ADF

### Quick Start


You need [version 3.3.1 of ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/v3.3.1/versions.html) to provide the toolchain, the ESP32-LyraT board and headphone connected.

**Note:**  If this is your first exposure to ESP32 and ESP-IDF, proceed to [Getting Started](https://docs.espressif.com/projects/esp-idf/en/v3.3.1/get-started/index.html) documentation.

Clone the ESP-ADF repository, set up `ADF_PATH`, and then compile, flash and monitor applications in the same way as when working with ESP-IDF.

```
git clone --recursive https://github.com/Ai-Thinker-Open/ESP32-A1S-AudioKit.git
cd esp-adf/ai-examples/get-started/play_mp3
make menuconfig
make flash monitor
```

If you clone project without `--recursive` flag, please goto the `esp-adf` directory and run command `git submodule update --init` before doing anything.