import sys
import shutil

if __name__ == "__main__":
    args = sys.argv
    workspace = args[1]
    targetDir = args[2]

    shutil.copy2(f'{workspace}vendor/d3d12_sdk/bin/x64/D3D12Core.dll', f'{targetDir}')
    shutil.copy2(f'{workspace}vendor/d3d12_sdk/bin/x64/d3d12SDKLayers.dll', f'{targetDir}')
