# STM32L4 FreeRTOS and libopencm3 projects template

This template is a starting point to develop projects for the stm32l433.
It provides freertos + libopencm3 + cmake

## Build instructions

To build the blinky app inside a build directory:

```
git submodule update --init --recursive
mkdir build
cd build
cmake -DAPP_NAME=blinky ../src/
make
```

## Flashing

Use https://github.com/stlink-org/stlink
```
st-flash --reset write blinky.elf 0x8000000
```

## License

Licensed under either Apache License, Version 2.0 (see LICENSE or http://www.apache.org/licenses/LICENSE-2.0).
