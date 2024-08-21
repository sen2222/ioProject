# `io` 项目编译与运行指南

[GitHub 仓库](https://github.com/sen2222/ioProject)

## 目录

- [编译项目](#编译项目)
- [运行项目](#运行项目)
- [项目结构](#项目结构)
- [注意事项](#注意事项)

## 编译项目

使用以下命令进行编译：

```bash
arm-linux-gcc ./io项目/src/* ./io项目/lib/* -I./io项目/inc -o ./io项目/build/io -lm -pthread
```

### 编译命令详解

- **`arm-linux-gcc`**：指定使用 ARM 架构的交叉编译器。
- **`./io项目/src/*`**：包含项目中 `src` 目录下的所有源文件。
- **`./io项目/lib/*`**：包含项目中 `lib` 目录下的所有库文件。
- **`-I./io项目/inc`**：指定包含头文件的路径。
- **`-o ./io项目/build/io`**：生成的可执行文件将保存到 `./io项目/build/` 目录中，文件名为 `io`。
- **`-lm`**：链接数学库 `libm`。
- **`-pthread`**：启用 POSIX 线程支持。

> **注意：** 确保 `arm-linux-gcc` 已正确安装，并且相关路径配置正确。

## 运行项目

编译成功后，生成的可执行文件位于 `./io项目/build/` 目录下。运行以下命令启动项目：

```bash
cd ./io项目/build/
./io
```

## 项目结构

项目目录结构如下所示：

```
.
├── 远程demo               # 远程演示相关代码目录
│   └── client.c           # 远程演示的客户端源文件
│
├── build                  # 构建输出目录
│   ├── io                 # 编译生成的 `io` 可执行文件
│   └── mplayer            # 编译生成的 `mplayer` 可执行文件
│
├── docs                   # 文档目录
│   ├── doctordata         # 医生相关数据文件
│   ├── log                # 日志文件
│   ├── pitdata            # 病人数据文件
│   └── usrdata            # 用户数据文件
│
├── inc                    # 头文件目录
│   ├── advertising.h      # 广告模块头文件
│   ├── font.h             # 字体处理模块头文件
│   ├── hospital.h         # 医院模块头文件
│   ├── img.h              # 图片处理模块头文件
│   ├── list.h             # 链表管理模块头文件
│   ├── login.h            # 登录模块头文件
│   ├── main.h             # 主程序头文件
│   ├── scoreboard.h       # 记分板模块头文件
│   ├── tools.h            # 工具函数头文件
│   └── touch.h            # 触摸屏处理模块头文件
│
├── lib                    # 库文件目录
│   └── libfont.a          # 字体处理静态库
│
├── readme.md              # 项目说明文件
│
├── rec                    # 资源文件目录（图片和视频）
│   ├── img                # 图片文件目录
│   │   ├── 1              # 目录 1 下的图片文件
│   │   │   ├── 1.bmp
│   │   │   ├── 2.bmp
│   │   │   ├── 3.bmp
│   │   │   ├── 4.bmp
│   │   │   ├── 5.bmp
│   │   │   ├── deguo.bmp
│   │   │   ├── sky.bmp
│   │   │   ├── ubuntu2.bmp
│   │   │   ├── wz1.bmp
│   │   │   ├── wz2.bmp
│   │   │   ├── wz3.bmp
│   │   │   ├── wz4.bmp
│   │   │   └── wz5.bmp
│   │   ├── bofan.bmp
│   │   ├── break.bmp
│   │   ├── hospital       # 医院相关图片目录
│   │   │   ├── a.bmp
│   │   │   ├── doctor0.bmp
│   │   │   ├── doctor1.bmp
│   │   │   ├── doctor2.bmp
│   │   │   ├── doctor3.bmp
│   │   │   ├── doctor4.bmp
│   │   │   ├── doctor5.bmp
│   │   │   └── pei.bmp
│   │   ├── keyboard.bmp
│   │   ├── menu.bmp
│   │   ├── win.bmp
│   │   └── yueqian.bmp
│   └── video              # 视频文件目录
│       ├── 1              # 目录 1 下的视频文件
│       │   ├── a1.mp4
│       │   ├── a2.mp4
│       │   ├── a3.mp4
│       │   ├── b1.mp4
│       │   ├── b2.mp4
│       │   ├── c1.mp4
│       │   └── c4.mp4
│       ├── login.mp4
│       └── open.mp4
│
├── script                 # 脚本目录
│   ├── download_img.sh    # 下载图片的脚本
│   ├── download_video.sh  # 下载视频的脚本
│   └── kill_all_mplayer.sh # 终止所有 mplayer 进程的脚本
│
└── src                    # 源代码目录
    ├── advertising.c      # 广告模块实现文件
    ├── hospital.c         # 医院模块实现文件
    ├── img.c              # 图片处理模块实现文件
    ├── libfont.a          # 字体库文件
    ├── login.c            # 登录模块实现文件
    ├── main.c             # 主程序实现文件
    ├── scoreboard.c       # 记分板模块实现文件
    ├── tools.c            # 工具函数实现文件
    └── touch.c            # 触摸屏处理模块实现文件

```

## 注意事项

- **依赖库**：请确保所有依赖的库文件都在 `lib/` 目录中，并且路径正确。
- **编译器路径**：确保系统中已经安装 `arm-linux-gcc` 并且路径已正确配置。
- **运行环境**：生成的 `io` 可执行文件仅能在 ARM 架构的设备或模拟器中运行。
