# Smart Home System Project

## Overview
This project implements a smart home system that allows users to control various home devices and functionalities through a keypad interface. It includes features like setting passwords, logging in as Admin or Guest, controlling room appliances, managing temperature settings, and handling session timeouts.

## Components
- **Microcontroller:** Utilizes a microcontroller for system control and device communication.
- **Keypad:** Allows user input for authentication and device control.
- **EEPROM:** Stores password and system configuration data.
- **LCD Display:** Provides visual feedback and user prompts.
- **LEDs:** Indicate system status and device activation.
- **SPI Communication:** Enables communication between master and slave devices.

## Functionality
- **Login Authentication:** Admin and Guest login modes with password authentication.
- **Device Control:** Control and status monitoring of multiple rooms and devices (e.g., lights, TV, air conditioning).
- **Temperature Setting:** Set and control room temperatures.
- **Session Management:** Session timeout handling for security.

## How to Use
1. **Initialization:** Upon startup, the system initializes and displays a welcome message.
2. **First-time Setup:** If passwords are not set, the system prompts for Admin and Guest password setup.
3. **Login:** Users select login mode (Admin or Guest) and enter the corresponding password.
4. **Menu Navigation:** Use the keypad to navigate through room/device menus, select options, and control functionalities.
5. **Temperature Setting:** Access the temperature menu to set room temperatures.
6. **Session Timeout:** System automatically logs out users upon session timeout.

## Code Structure
- **`Master_App.c`:** Contains the main application logic.
- **`Master_App.h`:** Header file with function declarations and constants.
- **Functions:** Includes functions for keypad input, EEPROM data handling, menu navigation, device control, and session management.

## Usage Notes
- Ensure proper connections of hardware components (keypad, LCD, LEDs, etc.) to the microcontroller.
- Review the code comments for detailed explanations of functionality and implementation.
- Adjust timeout values or device configurations as needed within the code.
- Follow proper programming practices for extending or modifying the system.

## Dependencies
- This project may require specific hardware configurations or libraries based on the microcontroller and peripherals used.

