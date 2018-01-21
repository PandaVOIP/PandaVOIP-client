# Qt Creator and Qt

The project uses the Qt Creator IDE for development. Set up is pretty simple once everything else is installed.

## Download and Install

Simply download the open source version of Qt Creator from their website: https://www.qt.io/download

### Windows Install and Setup

Install:

1. Download the open source version unified online installer from: https://www.qt.io/download
2. Run the installer.
3. At the "Select Components" stage, navigate to Qt -> Qt 5.9.3 and select MinGW 5.3.0 32 bit and hit next. Nothing else will need to be changed.
4. Everything should be installed now.

Setup: 

1. Open the PandaVOIP-client project.
2. On the premade kit for the project, hit manage.
3. For that kit, change the Compiler to the standalone install of MinGW (`C:\MinGW\bin`).
4. Hit configure project.
5. Once any other background processes are done, the project should be able to be ran.
