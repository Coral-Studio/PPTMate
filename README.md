# PPTMate
A simple tool which can save the copies of  Powerpoint files to a certain folder while opening them.

## Module

### PPTMate

- Copy the Powerpoint file to a certain folder while open it.
- You can choose whether put the file into a subfolder named with date. Change this setting in `config.ini`.

### PPTMateStartup

*Better to use TDM-GCC 9.2 to compile `PPTMateStartup.cpp`*

**WARNING: This function involves registry modification and is hard reversible. So it will be not added into release. Please make sure what you are doing while using it.**

- Register to associate the opening methods of `.pptx` and `.ppt` files.
- Turn off the function of Powerpoint detecting whether itself is the default opening method.

## Todo

- Add more command line options in future.

## Release Note

**0.4.1** Now the program will automatically recognize the location of Powerpoint executable file. You can choose to override by editing `config.ini`.

**0.4.0** Depart configurations into `config.ini`. Add a switch in configuration to control whether create a subfolder named by the date & Big format standardization.

**0.3.1** Increase opening speed and fix some bugs.

**0.3.0** Fix the problem of command-line interface flashing.

**0.2.1** Save the file in the subfolders with the date and change the icon.

**0.1.0** PPTMate came out!