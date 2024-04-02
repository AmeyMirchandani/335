# What is this controller?
- This controller is an Arduino that is mounted on the wrist
- The user can use it to take remote control over the mouse/input of any bluetooth enabled device
- It is meant to be used for games (has features that account for common inputs in addition to the mouse), but can be reconfigured for anything

## Setup
- central.py is the file that must be run on the target device to pair with. It handles the software pairing
- Final_Project.ino is the C++ file that must be run in the Ardiuno IDE. It handles the hardware pairing

## Operation
- Motion sensor is used to toggle between constant *wasd* movement (swipe hand over sensor in one of the 4 directions)
- Tilting the device to the side will cause the gyroscope to register a roll change and start moving the mouse to that side as long as it is past the midway point (neutral). The further you are from the midway point, the faster the mouse moves
- Tilting the device upward or downward will cause the gyroscope to register a pitch change, which will move the mouse up and down as you tilt the device in each of those directions
- Reload input is mounted on the unit at the back
- Fire input is mounted on the right side
