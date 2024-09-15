# Simple utils for arduino, esp-01 and other microcontrollers

This repository is mainly for my own use in my own Arduino or ESP-01 projects. 
People can use this if it makes them happy, but I cannot promise there aren't 
breaking changes at least for a month or two from today (August, 2024).

## Modules
- Logger: A utility module for logging messages and debugging information.
- Battery Monitor: A simple module for monitoring the battery level in a robot car project.

### Battery Manager
Make a separate intance of this class for each battery pack.

My plan is to:
- add a possibility for an interrupt pin to wake up the manager
- to make it possible to get the manager it self to actually "manage" the battery, 
    thus warn the main app if it's low or whatnot... 
- This really isn't a well thought through component, but there you go...

## boo