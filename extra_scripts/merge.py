Import("env")
import subprocess
import sys
import os
from os import path
from pathlib import Path

env = DefaultEnvironment()

# 合并配置参数（根据实际情况修改）
def merge_firmware(env_dir):


    # 创建输出文件名（环境名_merged.bin）
    env_name = path.basename(env_dir)
    output_file = path.join(env_dir, f"{env_name}_merged.bin")
    
    # 构建esptool命令参数
    cmd = [
        "esptool.py",
        "--chip", "esp32c3",
        "merge_bin",
        "--flash_mode", "dio",
        "--flash_size", "4MB",
        "-o", output_file,
         '0x0000', ".pio/build/esp32-c3-devkitm-1/bootloader.bin",     # Bootloader偏移地址
         '0x8000',  ".pio/build/esp32-c3-devkitm-1/partitions.bin",    # 分区表偏移地址
         '0x10000', ".pio/build/esp32-c3-devkitm-1/firmware.bin",      # 应用程序偏移地址
         '0x210000',".pio/build/esp32-c3-devkitm-1/spiffs.bin",         # SPIFFS文件系统偏移地址

    ]
    
    print(cmd)
    # 执行合并命令
    try:
        print("\n========== Merging firmware ==========")
        result = subprocess.run(cmd, check=True, capture_output=True, text=True)
        print(result.stdout)
        print(f"\nSuccessfully created merged firmware: {output_file}")
    except subprocess.CalledProcessError as e:
        print(f"Merge failed: {e.stderr}")
        sys.exit(1)

def pre_upload_action(source, target, env):
    print("\n============ Start Post Build Script================")
    # 通过PlatformIO环境变量获取构建目录

    env_dir = ".pio/build/esp32-c3-devkitm-1"  # 修改为你的实际环境目录
    print("Warning: Using debug build directory")

    merge_firmware(env_dir)

env.AddPostAction("$BUILD_DIR/spiffs.bin", pre_upload_action)