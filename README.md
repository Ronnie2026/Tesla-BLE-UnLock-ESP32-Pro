# Tesla BLE Unlock ESP32 Pro

车内接近检测 → 本地人脸比对与活体检测 → Tesla BLE 解锁。设备不提供启动或驾驶指令；驾驶仍由车主使用独立钥匙完成。

## v0.1 交付范围

**这是可运行的离线工程底座，不是已能解锁车辆的固件。**

- 已实现：平台无关 C++ 状态机、8 秒识别超时、15 秒冷却、识别会话编号隔离、急停锁存、桌面模拟和自动测试。
- 已搭建：ESP-IDF / ESP32-S3 工程、GitHub Actions 主机测试与固件编译模板（尚未启用）。
- 待接入：具体雷达/GPIO、摄像头与本地模型、真实活体算法、可信识别通信、BLE 传输与协议库、加密密钥存储和配对。
- 固件启动为 Disabled，未配置 GPIO、未开启 BLE；模拟识别成功只得到 DryRun，不会发送车辆命令。JSON 配置仅为后续契约草案。

## 立即运行（无硬件）

需要 CMake 3.16+、C++17 编译器。

```sh
cmake -S . -B build-host -DHOST_BUILD=ON
cmake --build build-host
ctest --test-dir build-host --output-on-failure
./build-host/unlock_sim
```

模拟器演示靠近、模拟识别通过、关闭输出、急停，不加载真实人脸或车辆资料。

## ESP32-S3 构建

安装并激活 ESP-IDF 5.4.2 后：

```sh
idf.py set-target esp32s3
idf.py build
# 确认实物板型、USB 串口及供电后，再执行：
# idf.py -p <PORT> flash monitor
```

首版 firmware 仅每 20 ms 调用状态机定时器，输出驱动是日志占位。刷机不会自动启用识别或解锁。GPIO 和 Flash/PSRAM 参数以已购板卡为准，目前不假设 N8R8 配置。

## 文档

- [系统架构、硬件候选、模块与时序](docs/architecture.md)
- [Driver 配对、密钥与权限边界](docs/pairing-security.md)
- [分阶段开发与验收](docs/development.md)
- [参考源码与许可](docs/references.md)

后续需要：已购主控/摄像头/雷达的准确型号与接线图、车型/年份/车机软件版本、车窗贴膜情况、常供电来源。VIN 和公钥配对现场提供，私钥不通过聊天或 GitHub 传递。

## 许可

当前自有代码沿用仓库原有 MIT。未复制或链接 tesla-ble；若后续集成该 AGPL-3.0 库，必须先处理组合软件的许可与源码提供义务，不能直接宣称组合固件仍仅适用 MIT。

## CI 启用状态

当前 GitHub OAuth 凭据缺少 workflow 权限，仓库首版将工作流保存在 `docs/ci/github-actions.yml.example`。有相应权限后，将其复制到 `.github/workflows/ci.yml` 即可启用。当前已通过本地主机编译/测试；ESP-IDF 固件编译尚未实测，不能视为通过。
