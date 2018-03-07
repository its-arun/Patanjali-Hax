# Patanjali Hax
Macintosh Skin Changer for Counter-Strike: Global Offensive.

## Get the injector here:
https://github.com/scen/osxinj

## Usage
```
1. Download latest build from Release tab (https://github.com/its-arun/Patanjali-Hax/releases/tag/1.0.0)
2. Open terminal and CD to the Patanjali Hax folder
3. Type sudo ./osxinj csgo_osx libPatanjali.dylib
```

### Add alias for injecting hack in .bashrc
```
1. Open Terminal and execute nano .bashrc
2. Type alias patanjali="cd /Users/arun/Downloads/Patanjali/ && sudo ./osxinj csgo_osx64 libPatanjali.dylib"
3. Press ctrl+x , Y , return key
4. Relaunch terminal
5. Use patanjali command to inject cheat
```


## Change Skins
```
1. Clone this repository and open Patanjali.xcodeproj with XCode
2. Edit skinchanger.cpp on line 15
3. Find paintkit id from https://www.csgozone.net/database
4. Build and inject with osxinj as sudo
```
