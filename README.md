## Setup SDK

Download nRF-util from: https://www.nordicsemi.com/Products/Development-tools/nRF-Util/Download#infotabs

Then run:
```
nrfutil  install device
nrfutil  install sdk-manager
nrfutil sdk-manager search
# Get latest stable version and replace it below
nrfutil sdk-manager install v2.9.1

# During install it will show where the SDK is installed
# [00:01:41] ###### 100% [Download toolchain v2.9.1] Toolchain downloaded
# [00:00:12] ###### 100% [Unpack toolchain v2.9.1] Toolchain unpacked to /opt/nordic/ncs/tmp/.tmpRNvpOV
# [00:00:00] ###### 100% [Install toolchain v2.9.1] Toolchain installed at /opt/nordic/ncs/toolchains/b8efef2ad5
# [00:01:01] ##----  20% [Download SDK v2.9.1] Downloading

```

## Building


```
# Extend path to nordic toolchain
export PATH=/opt/nordic/ncs/toolchains/f8037e9b83/bin:$PATH

# Source SDK
source /opt/nordic/ncs/v2.6.1/zephyr/zephyr-env.sh

# build
mkdir build && cd build
cmake .. -DBOARD_ROOT=. -DBOARD=hextree_glitchtag
make
```
