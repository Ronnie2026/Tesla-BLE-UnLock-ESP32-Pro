# 参考与依赖状态

核查日期：2026-10-20。外部接口会变化，真实接入必须重新核对。

- [Tesla 官方 vehicle-command](https://github.com/teslamotors/vehicle-command)：协议与官方实现参考。
- [yoziru/tesla-ble](https://github.com/yoziru/tesla-ble)：源码核查快照 `f7e288e026a19e6bfac0b207922086e57460d860`（idf_component 版本 6.3.0）。参考 include/vehicle.h、include/adapters.h、proto/keys.proto。AGPL-3.0，当前工程未复制/链接。
- [ESPHome 集成](https://github.com/yoziru/esphome-tesla-ble)：宿主集成参考，不直接套用充电配置为解锁。
- [HA 集成](https://github.com/yoziru/ha-tesla-ble)：后续可参考，本版未复用。
- [rabbithole](https://github.com/SebasPinto/rabbithole)：用户提供的实体钥匙参考，本版未复用/未验证。
- [tesla-key-esp32](https://github.com/0Bu/tesla-key-esp32)：用户提供的充电参考，本版未复用，不用其充电授权替代解锁授权。
- [ESP32-S3-DevKitC-1 硬件说明](https://documentation.espressif.com/esp-dev-kits/en/latest/esp32s3/esp32-s3-devkitc-1/user_guide_v1.0.html)。
- [DFRobot SEN0395](https://wiki.dfrobot.com/sen0395)。
- [Waveshare IMX462 IR-CUT Camera (A)](https://www.waveshare.com/wiki/IMX462_IR-CUT_Camera_%28A%29)。

硬件型号是候选，厂商描述不代表已经验证安装在本车玻璃后的效果。首版自有源码保持原 MIT LICENSE；后续第三方模型、库和固件分别做许可清单。
