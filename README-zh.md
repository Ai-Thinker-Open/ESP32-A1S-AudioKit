# 安信可音频开发板 SDK 框架

 [EN](./README.md)| 中文


- GitHub：https://github.com/Ai-Thinker-Open/ESP32-A1S-AudioKit.git
- Gitee 码市：https://gitee.com/xuhongv/AiThinker-esp32-A1s-AudioKit

本框架是基于乐鑫音频框架 esp-adf 上二次开发并遵循官方开源协议，如有侵犯，请联系我们。

## 总览

本框架同样适合乐鑫的官方开发板比如esp-lyart，还以最全面的方式轻松添加功能，从简单到复杂开发音频应用程序：

- 音乐播放器或录音机支持音频格式，例如MP3，AAC，FLAC，WAV，OGG，OPUS，AMR，TS，EQ，Downmixer，Sonic，ALC，G.711 ...
- 从以下来源播放音乐：HTTP，HLS（HTTP实时流），SPIFFS，SDCARD，A2DP来源，A2DP接收器，HFP ...
- 集成媒体服务，例如：DLNA，VoIP ...
- 网络广播
- 语音识别以及与Alexa，DuerOS等在线服务的集成...

## 使用ESP-ADF进行开发

### 快速开始

**注意**：如果您是第一次接触ESP32和ESP-IDF，请参考我们的 安信可一体化环境IDE搭建；

克隆 AiThinker-esp32-A1s-AudioKit 仓库，设置ADF_PATH路径和IDF路径，然后以与使用ESP-IDF相同的方式编译，刷新和监视应用程序。


开始克隆，尤其带子模块克隆：
``` bash
git clone --recursive https://gitee.com/xuhongv/AiThinker-esp32-A1s-AudioKit.git
``` 

之后注意设置的IDF路径为仓库下的 idf 目录，比如：

``` bash
export ADF_PATH="~/esp32-a1s-adf"
export IDF_PATH="~/esp32-a1s-adf/esp-idf"
```
开始编译并烧录到开发板，比如：

``` bash
cd esp-adf/ai-examples/get-started/play_mp3
make menuconfig
make flash monitor
``` 