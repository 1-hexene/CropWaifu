Import("env")
import subprocess
import sys
import os
from os import path

env = DefaultEnvironment()

def merge_firmware():
    version = env.get("CPPDEFINES", [])
    sw_ver = version[2][1].split('"',1)[1].split("\\")[0]

    output_dir = f"output/{sw_ver}"
    os.makedirs(output_dir, exist_ok=True)

    output_file_name = f"CropWaifu_{sw_ver}.bin"
    output_file = path.join(output_dir, output_file_name)

    if os.path.exists(output_file):
        print("########## WARNING #########\n OVERWRITING EXISTED FILE")

    cmd = [
        "esptool.py",
        "--chip", "esp32c3",
        "merge_bin",
        "--flash_mode", "dio",
        "--flash_size", "4MB",
        "-o", output_file,
        '0x0000', ".pio/build/CropWaifu/bootloader.bin",
        '0x8000',  ".pio/build/CropWaifu/partitions.bin",
        '0x10000', ".pio/build/CropWaifu/firmware.bin"
    ]

    print(cmd)
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

env.AddPostAction("$BUILD_DIR/firmware.bin", pre_upload_action)
