# Requirements (Tested)
* Visual Studio 2019
* Raspberry Pi 4

# How to clone repo
> git clone --recursive https://github.com/xXTurnerLP/SPDetector

# How to compile
This is a Visual Studio 2019 Project, when the project was generated there was a readme folder, that readme folder contains information how to setup VS to remotely compile and build.

# First steps
After cloning and building the project, binary should be on the RPi4 (Raspberry Pi 4). <br>
When you run the binary for the first time it will generate `config.ini` file, you need to open it with a text editor and edit the template to match your data. For more information read section **Config file**.

# Config file
id (string): `Replace "00000000-0000-0000-0000-000000000000" with your own UUID for that device/parking` <br>
name (string): `Name of the parking` <br>
name (string): `Street Address of the parking` <br>
GPS Lng (float): `GPS Longitute coordinate of parking` <br>
GPS Lat (float): `GPS Latitude coordinate of parking` <br>
total (int): `Total parking spots, int` <br>
type (string): `Type of parking "free"/"paid" and if paid the price is written in the format "paid X/h" or "paid X/entry" (if the parking is paid for only once when entering) where X is the money`