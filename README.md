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
