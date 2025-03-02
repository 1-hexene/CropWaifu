Import("env")
import subprocess
import sys
import os
from os import path
from pathlib import Path

env = DefaultEnvironment()

# 合并配置参数
def merge_firmware():

    # 创建输出文件名（环境名_merged.bin）
    version = env.get("CPPDEFINES", [])
    print(version)
    hw_ver = version[2].split('_')[0]+version[2].split('_')[2]
    sw_ver = version[3][1].split('"',1)[1].split("\\")[0]

    output_dir = f"output/{sw_ver}"
    if not (os.path.exists(output_dir)):
        os.makedirs(output_dir)

    output_file_name = f"CanWaifu_{sw_ver}_{hw_ver}.bin"
    if os.path.exists(output_dir+'/'+output_file_name):
        print("########## WARNING #########\n OVERWRITING EXISTED FILE")

    output_file = path.join(output_dir, output_file_name)
    
    # 构建esptool命令参数
    cmd = [
        "esptool.py",
        "--chip", "esp32c3",
        "merge_bin",
        "--flash_mode", "dio",
        "--flash_size", "4MB",
        "-o", output_file,
         '0x0000', ".pio/build/CanWaifu/bootloader.bin",     # Bootloader偏移地址
         '0x8000',  ".pio/build/CanWaifu/partitions.bin",    # 分区表偏移地址
         '0x10000', ".pio/build/CanWaifu/firmware.bin",      # 应用程序偏移地址
         '0x210000',".pio/build/CanWaifu/spiffs.bin",         # SPIFFS文件系统偏移地址

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

    merge_firmware()

env.AddPostAction("$BUILD_DIR/spiffs.bin", pre_upload_action)