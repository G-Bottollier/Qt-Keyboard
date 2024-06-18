# Simple Qt Keyboard (No QML)

This repository contains a simple replica of the Qt Virtual Keyboard, designed for use with Qt versions that do not include the QML-based virtual keyboard, specifically targeting Qt 5.5.1. The project aims to provide a basic but functional virtual keyboard that can be integrated into Qt Widgets applications without the need for QML.

## Overview

The virtual keyboard is implemented in C++ and relies on the QtWidgets framework to render the keyboard and handle user interactions. This makes it suitable for projects that are based on the traditional QWidget system rather than the newer QML interface.

The keyboard supports multiple layouts, including QWERTY, AZERTY, and Numeric, which can be switched at runtime. It also features visual feedback for key presses and can be easily customized and extended.

## Contents

The repository includes two main files:

- `KeyboardManager.cpp`: The implementation file for the keyboard manager, handling the logic for key presses, layout switching, and UI updates.
- `KeyboardManager.h`: The header file for the keyboard manager, defining the class structure and public interfaces.

## Usage

To use the virtual keyboard in your project, simply include the `KeyboardManager.cpp` and `KeyboardManager.h` files in your project and ensure that your Qt environment is set up to use QtWidgets.

You can create an instance of the keyboard and add it to your application's UI like any other QWidget. The keyboard layout can be switched at runtime using the `SetKeyboardLayout` method.

## License

This code is free to use and is provided as a starting point for developers looking to integrate a virtual keyboard into their Qt Widgets applications. You are encouraged to modify, extend, and share your improvements to the code.

## Note

This project is a simple implementation and does not cover all the features and complexities of a full-fledged virtual keyboard. It is intended as a starting point and may require further development to meet the specific needs of your application.

## Acknowledgments

This code was created as a workaround for the absence of the QML-based Qt Virtual Keyboard in Qt 5.5.1. It is inspired by the functionality of the official Qt Virtual Keyboard but is implemented entirely using QtWidgets for compatibility with older versions of Qt.

This code was developed in half a day and is currently on pause before further upgrades.

