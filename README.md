Plant Generator
---------------

Generates 3D models of plants using L-Systems.

Screenshots
-----------
<img src="https://raw.githubusercontent.com/jpbetz/plant-generator/master/screenshots/tree1.png">

Requirements
------------

* Unreal Engine 4.4.1+
* Xcode 6.0.1+

How to Build
------------

* Open project in Xcode (usually in ~/Documents/UnrealProjects/).
* In Xcode the build command to "<ProjectName>Editor - Mac" and set the target to "My Mac"
* Run the project,  this should open Unreal Engine

To test:

* Inside Unreal Engine, Launch the IOS application

To build:

* Inside Unreal Engine, run "Build" from the toolbar
* Inside Unreal Engine, run "File -> Package Project -> IOS"
* Drag and drop the ".ipa" file produced by Unreal Engine in the IOS folder into iTunes
* Sync any IOS device with iTunes (check that the app is listed to be installed under the Apps tab if it does not show up)
