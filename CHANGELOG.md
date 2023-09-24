# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.1.0] - 2023-09-24

### Added

- FW - Setup fixed 40 Mhz output
- FW - Add Si5351 Adafruit library and convert it to Pi Pico SDK C from C++ Arduino
- FW - Firmware build with pipeline and packaging

### Fixed

- PCB - Clock generator main board - Rev B (IO VDD for Si5351 should be 3.3V instead of VSys)

## [0.0.1] - 2023-09-16

### Added

- PCB - VT610ex clock generator adapter - Rev B
- PCB - Clock generator main board - Rev A
- Initial documentation

[unreleased]: https://gitlab.com/wolfre/cxadc-clock-generator-audio-adc/-/compare/v0.1.0...main
[0.1.0]: https://gitlab.com/wolfre/cxadc-clock-generator-audio-adc/-/compare/v0.0.1...v0.1.0
[0.0.1]: https://gitlab.com/wolfre/cxadc-clock-generator-audio-adc/-/tree/v0.0.1
