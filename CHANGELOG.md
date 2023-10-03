# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- Firmware - USB device descriptor now has correct SemVer in bcdDevice version field
- Firmware - Implement PCM1802 data streaming and head switch GPIO sampling
- Doc - Add example scope trace for 28.6 MHz clock to build and install guide

## [0.2.0] - 2023-10-02

### Added

- Doc - Initial build and install guide
- Firmware - Implement UAC2 device to control Si5351 clock generator, only dummy data returned, PCM1802 is not yet used
- Mechanical - Add a simple 3D printable box to contain the clock generator

### Fixed

- PCB - Clock generator main board - Rev C (3.3V from PCM1802 erroneously driven by Pi Pico, should be open)

## [0.1.0] - 2023-09-24

### Added

- Firmware - Setup fixed 40 Mhz output
- Firmware - Add Si5351 Adafruit library and convert it to Pi Pico SDK C from C++ Arduino
- Firmware - Firmware build with pipeline and packaging

### Fixed

- PCB - Clock generator main board - Rev B (IO VDD for Si5351 should be 3.3V instead of VSys)

## [0.0.1] - 2023-09-16

### Added

- PCB - VT610ex clock generator adapter - Rev B
- PCB - Clock generator main board - Rev A
- Initial documentation

[unreleased]: https://gitlab.com/wolfre/cxadc-clock-generator-audio-adc/-/compare/v0.2.0...main
[0.2.0]: https://gitlab.com/wolfre/cxadc-clock-generator-audio-adc/-/compare/v0.1.0...v0.2.0
[0.1.0]: https://gitlab.com/wolfre/cxadc-clock-generator-audio-adc/-/compare/v0.0.1...v0.1.0
[0.0.1]: https://gitlab.com/wolfre/cxadc-clock-generator-audio-adc/-/tree/v0.0.1
