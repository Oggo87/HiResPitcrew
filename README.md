# High Resolution Pitcrews for Grand Prix 4

A DLL to overcome the original limitation of 400 vertices for pitcrew meshes in Grand Prix 4

## Description

Grand Prix 4 has been extensively modded for years, and hi-resolution material meshes and textures have become the standard. One limitation has remained for years: pitcrew meshes cannot have more than 400 vertices in the original game.
This DLL finally overcomes this limitation, allowing pitcrew high resolution pitcrew meshes to be used. 

## Getting Started

### Prerequisites

* Grand Prix 4
* CSM by ZaZ or GP4 Memory Access by Carl_gpgames

### Installing

#### GP4 Memory Access

* Add an entry pointing to `HiResPitcrew.dll`

#### CSM

* Copy `HiResPitcrew.dll` into your mod Data folder (or any of its subfolders of your choice)
* In the main `.csm` file of you mod, `[Files]` section, add an entry to the `InjectDLL` key 
  
  Example: 
  ```ini
  InjectDll=Files\HiResPitcrew.dll
  ```

## Authors

Diego "Öggo" Noriega

## Version History

* 0.1
    * Pre-Release
