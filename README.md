# plugin_echo

luo9_bot C++ 插件示例

## 功能

- `/echo <text>` - 回显消息
- `/task start <name> <cron> [payload]` - 创建定时任务
- `/task end <name>` - 取消定时任务
- 支持私聊和群聊消息处理
- 支持元事件和通知处理
- 支持版本查询响应

## 快速开始

### 1. 安装编译器

#### Windows

**方式一：MSVC (推荐)**

安装 Visual Studio 2022 或 Build Tools for Visual Studio：
- 下载地址：https://visualstudio.microsoft.com/downloads/
- 安装时选择 "使用C++的桌面开发" 工作负载

**方式二：MinGW-w64**

必须使用 posix 线程模型版本：
- 下载地址：https://github.com/niXman/mingw-builds-binaries/releases
- 选择 `x86_64-*-posix-seh-ucrt` 版本
- 解压到目录（如 `G:\mingw64-posix`）

#### Linux

```bash
# Ubuntu/Debian
sudo apt install g++ cmake

# Arch Linux
sudo pacman -S gcc cmake
```

### 2. 配置编译器路径

编辑 `CMakePresets.json`，修改 MinGW 编译器路径：

```json
{
    "name": "mingw-posix",
    "cacheVariables": {
        "CMAKE_CXX_COMPILER": "你的mingw路径/bin/g++.exe",
        "CMAKE_C_COMPILER": "你的mingw路径/bin/gcc.exe"
    }
}
```

### 3. 编译

#### Windows + MSVC
```bash
cmake --preset msvc
cmake --build build --config Release
```

#### Windows + MinGW
```bash
cmake --preset mingw-posix
cmake --build build
```

#### Linux
```bash
mkdir build && cd build
cmake ..
make
```

### 4. 输出文件

- Windows: `build/bin/plugin_echo.dll`
- Linux: `build/lib/libplugin_echo.so`

## 项目结构

```
.
├── CMakeLists.txt        # CMake 配置
├── CMakePresets.json     # 编译器预设
├── .howmake              # 编译说明
├── README.md             # 本文件
├── include/              # 插件头文件
│   └── main.h
├── src/                  # 插件源码
│   ├── core.cpp
│   └── main.cpp
└── sdk/                  # SDK 文件（自动同步）
    ├── include/          # SDK 头文件
    └── lib/              # SDK 库文件
        ├── windows/
        └── linux/
```

## SDK 命名空间

```
luo9::
├── bus::       # 消息总线
├── bot::       # 机器人操作
├── command::   # 命令解析
├── payload::   # 载荷解析
├── message::   # 消息构建器
├── pattern::   # 模式匹配
└── version::   # 版本查询
```

## 常见问题

### Q: MinGW 编译报错 "undefined reference to pthread_*"

A: 必须使用 posix 线程模型的 MinGW，不能使用 win32 线程模型版本。

### Q: MSVC 编译报错 LNK4099 警告

A: 这是正常的，只是缺少调试信息（PDB），不影响功能。

### Q: 如何更新 SDK？

A: SDK 会在推送正式版本 tag 时自动同步，或在 GitHub Actions 手动触发同步。
