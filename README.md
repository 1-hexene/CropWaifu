# CropWaifu

![GitHub last commit (branch)](https://img.shields.io/github/last-commit/1-hexene/CropWaifu/main?display_timestamp=committer&style=flat)
![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/1-hexene/cropwaifu/.github%2Fworkflows%2Fbuild.yml?branch=main)
![GitHub latest release](https://img.shields.io/github/v/release/1-hexene/CropWaifu)


[![License: AGPL v3](https://img.shields.io/badge/License-AGPL_v3-blue.svg)](https://www.gnu.org/licenses/agpl-3.0)
![Espressif](https://img.shields.io/badge/espressif-E7352C.svg?style=flat&logo=espressif&logoColor=white)
![Build with PlatformIO](https://img.shields.io/badge/build%20with-PlatformIO-orange?logo=platformio)


CropWaifu is a crop micro-climate analyzer and controller based on ESP32.

CropWaifu is a fork of CanWaifu.

## How to use?

Upper computer publish to topic `cropwaifu/control`

Message format （JSON）

```json
{
"messageID":1, // int
"boardID":1, //int (0-6),
"mode":1, //int (1 = Absolute control, 0 = Closed-loop control),
"fan":255, //PWM number (0-255, Only when mode = 1),
"led":255, //PWM number (0-255, Only when mode = 1),
"temperature":23.5, // degree Celcius(float),
"lightIntensity":450.0, // Luminance(float),
"timestamp":1234 //int
}
```

Lower computer publish to topic `cropwaifu/respond`

Lower computer's response format (JSON)

```json
{
"messageID":1, // int,
"boardID":1, // int (0-6),
"status":"OK",// OK or FAIL,
"timestamp":1235 // int
}
```

## Credits

![Github-sponsors](https://img.shields.io/badge/sponsor-30363D?style=for-the-badge&logo=GitHub-Sponsors&logoColor=#EA4AAA)
