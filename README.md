# Project I.G.I Internal

[Project I.G.I](https://en.wikipedia.org/wiki/Project_I.G.I.) Internal is Project to access internal methods of the game by using **DLL injection method** into the game and calling [IGI Natives methods](https://github.com/IGI-Research-Devs/IGI-Internals-Data/blob/main/IGI-Natives.json) to modify/alter the behaviour of the game.</br>
This was originally intended for research project to understand the game mechanics and how developers worked hard to create them.</br>
So don't use this project to misuse the game's identity and source of original game devs.</br>

## Pre-Requisite
- **General section.**
- [DLL File](https://en.wikipedia.org/wiki/Dynamic-link_library) - This project is DLL file rather than standard application.</br>
- [DLL Injection](https://en.wikipedia.org/wiki/DLL_injection) - This project needs DLL injection into _IGI_ game.</br>
- [Hooking](https://en.wikipedia.org/wiki/Hooking) - This project uses [Minhook](https://github.com/TsudaKageyu/minhook) for API calls.</br>
- [GTLibc](https://github.com/haseeb-heaven/GTLibc) -C/C++ library for interactig with Game.</br>
- [Pattern Scanning](https://memoryhackers.org/konular/external-internal-pattern-scanning-guide.95460/) - This project use pattern memory scanning to check Game/Player is loaded properly.</br>
- **Game specific section.**
- [IGI Graphs Structure](https://github.com/IGI-Research-Devs/IGI-Research-Data/blob/main/Research/GRAPH/Graph-Structure.txt) - Project IGI 1 Graph structure data.
- [IGI 3D Modes](https://github.com/IGI-Research-Devs/IGI-Research-Data/blob/main/Research/Natives/IGI-Models.txt) - Project IGI uses 3D models in 
form of _MEF_ (**M**esh **E**xternal **F**ile).
- [IGI Camera View](https://www.researchgate.net/figure/Definition-of-pitch-roll-and-yaw-angle-for-camera-state-estimation_fig15_273225757) - IGI use game Camera called [Viewport](https://en.wikipedia.org/wiki/Viewport) to display the game.
- **Native section.**
- Natives Invoker - Native invoker is a technology to invoke native methods from Game using their **Hash** with its **Handler**. 

## Building DLL project.
Open this project in your favourite IDE _(Visual Studio)_ and build it and your output will be **Release/Debug** folder depends which configuration you choosed and you will find file _IGI-Internals-Release.dll_ so download your favourite DLL injector make sure its **x86(32-bit)** compatible otherwise injection wouldn't work or you can use recommended [IGI-Injector](https://github.com/IGI-Research-Devs/IGI-Injector) to inject DLL.

## Building project for IGI Editor.
The project could be build for [IGI Editor](https://github.com/IGI-Research-Devs/IGI1Editor) the only thing we need to change is **Features.cpp** file we have to update with **Features file for Editor** which could be found here [Features_Editor.cpp](https://github.com/IGI-Research-Devs/IGI_Internal/blob/master/IGI_Internal/Features_Editor.cpp)


## Modifying this project.
You can modify the project the only file you need to focus on is **Features file** which could be found here [Features.cpp](https://github.com/IGI-Research-Devs/IGI_Internal/blob/master/IGI_Internal/Features.cpp) in _DllMainLoop()_ method go to _MENU_SCREEN_INGAME_ section add you logic for Adding/Removing Buildings/Weapons/A.I etc into the game.</br>

There are shown 5 examples into _Features.cpp_ file.
- Enable Debug mode.
- Restart game.
- Weapon pickup.
- Frames setting
- Humanplayer load.

## Adding new hashes for Natives.
Lets say you found new hash for Native now how to add them into project and use them.
So you have to follow the steps.
1. First Add your Hash to _Natives.hpp_ class like this 
```cpp
  MY_FIRST_NATIVE = 0x00402F90
```
2. Go to Natives folder and open _NativeHelper.hpp_ file and in any relevant section add its definition.
```cpp
  NATIVE_DECL void MY_FIRST_NATIVE_LOAD() { NATIVE_INVOKE<Void>((Void)HASH::MY_FIRST_NATIVE); }
```
3. Now go to _Features.cpp_ class and use it. 
```cpp
  		// Native method.
		else if (GT_HotKeysPressed(VK_CONTROL, VK_F1)) {
			MY_FIRST_NATIVE_LOAD();
		}
```

# This section requires to be updated.
## IGI-Internals Docs

## Resource Section.

### Loading predefined resource.

```cpp
string resource = "LANGUAGE:objectives.res";
auto res_addr = RESOURCE::LOAD(resource);
LOG_INFO("Resource '%s' loaded at %p",resource,res_addr);
```

### Loading custom resource.

```cpp
string resource = "LOCAL:my_resource.extension"; //Place your resource in game Local/root directory.
auto res_addr = RESOURCE::LOAD(resource);
LOG_INFO("Resource '%s' loaded at %p",resource,res_addr);
```

### Unloading a resource.

```cpp
string resource = "MISSION:sounds/sounds.qvm";
RESOURCE::UNLOAD(resource);
```

### Unloading resources.

```cpp
std::vector<string> res_list{"STATUSSCREEN:status.res","LANGUAGE:messages.res","LOCAL:menusystem/missionsprites.res"};
RESOURCE::UNLOAD(res_list);
```

### Unpack a resource .

```cpp
string resource = "LOCAL:menusystem/ingamemenu.qvm";
auto resource_addr = RESOURCE::UNPACK(resource.c_str());
```

### Flush a resource.

```cpp
string resource = "LOCAL:language/ENGLISH/menusystem.res";
auto resource_addr = RESOURCE::FIND(resource.c_str());
RESOURCE::FLUSH(resource_addr);
```

### Finding resource address.

```cpp
string resource = "LOCAL:missions/location0/level1/terrain/terrain.qvm";
auto resource_addr = RESOURCE::FIND(resource.c_str());
LOG_INFO("Resource '%s' loaded at address : %p",resource,resource_addr);
```

### Checking resource loaded.

```cpp
string resource = "MISSION:AI/2216.qvm";
bool is_loaded = RESOURCE::IS_LOADED(resource);
if(is_loaded) LOG_INFO("Resource is loaded"); 
```

### Saving all resource information.

```cpp
RESOURCE::ANIMATION_INFO_SAVE("IGI_Animations.txt");
RESOURCE::FONT_INFO_SAVE("IGI_Fonts.txt");
RESOURCE::SOUND_INFO_SAVE("IGI_Sound.txt");
RESOURCE::MATERIAL_INFO_SAVE("IGI_Material.txt");
RESOURCE::LIGHTMAP_INFO_SAVE("IGI_Lightmap.txt");
RESOURCE::OBJECT_INFO_SAVE("IGI_Object.txt");
RESOURCE::RESOURCE_INFO_SAVE("IGI_Resource.txt");
RESOURCE::TERRAIN_INFO_SAVE("IGI_Terrain.txt");
RESOURCE::TEXTURE_INFO_SAVE("IGI_Texture.txt");
RESOURCE::GRAPHICS_2D_INFO_SAVE("IGI_2D_Graphics.txt");
RESOURCE::GRAPHICS_3D_INFO_SAVE("IGI_3D_Graphics.txt");
```

## Resource - MEF Models Section.

### Finding MEF Model Name.

```cpp
string model = "435"; //Watertower Id. or provide full id like '435_01_1'
string model_name = RESOURCE::MEF_FIND_MODEL_NAME(model);
LOG_INFO("Model name: '%s'",model_name.c_str());
```

### Finding MEF Model Id.

```cpp
string model = "Watertower";
string model_id = RESOURCE::MEF_FIND_MODEL_ID(model,false);
LOG_INFO("Model id: '%s'",model_id.c_str()); // Output: Model id: '435'
```

### Finding MEF Model Full Id.

```cpp
string model = "Watertower";
string model_id = RESOURCE::MEF_FIND_MODEL_ID(model);
LOG_INFO("Model id: '%s'",model_id.c_str()); // Output: Model id: '435_01_1'
```

### Removing MEF Model by Id

```cpp
string model = "435";
RESOURCE::MEF_REMOVE_MODEL(model);
```

### Removing MEF Model by Full Id

```cpp
string model = "435_01_1";
RESOURCE::MEF_REMOVE_MODEL(model);
```

### Removing MEF Model by Name

```cpp
string model = "Watertower";
RESOURCE::MEF_REMOVE_MODEL(model);
```

### Removing MEF Models.

```cpp
std::vector<string> models_list{"Watertower","Watchtower","Barracks"};
RESOURCE::MEF_REMOVE_MODELS(models_list);
```

### Restoring MEF Models.

```cpp
RESOURCE::MEF_RESTORE_MODELS();
```

### Extracting MEF Models.

```cpp
RESOURCE::MEF_EXTRACT();
```

## Script Section. [QSC= 'Q' Script, QAS = 'Q' Assembler Script]

### Parse script file. [QSC to QAS Conversion.]
```cpp
string qsc_file = "LOCAL:config.qsc"; //Input file.
string qas_file = "LOCAL:config.qas"; //Output file.
int status = SCRIPT::PARSE(qsc_file, qas_file); //Status '0' success, 'Non-zero' error.
```

### Assemble script file. [QAS to QVM Conversion.]
```cpp
string qas_file = "LOCAL:config.qas"; //Input file.
string qvm_file = "LOCAL:config.qvm"; //Output file.
int status = SCRIPT::ASSEMBLE(qas_file, qvm_file); //Status '0' success, 'Non-zero' error.
```

### Compile script file.[QSC to QVM Conversion.]
```cpp
string qsc_file = "LOCAL:objects.qsc";//Place 'objects.qsc' file game root/local directory.
SCRIPT::COMPILE(qsc_file); //Output: "objects.qvm".
```

### Cleanup script file.
```cpp
string q_file = "LOCAL:objects.qas";
SCRIPT::CLEANUP(q_file);
```

## Script Section. - [QVM = 'Q' Virtual Machine].

### Loading qvm file.

```cpp
string qvm_file = "MISSION:sounds/sounds.qvm";
auto qvm_addr = QVM::LOAD(qvm_file);
```

### Reading qvm file.

```cpp
string qvm_file = "LOCAL:humanplayer/humanplayer.qvm";
int status = QVM::READ(qvm_file); //Status '0' success, 'Non-zero' error.
```

### Cleanup qvm file.

```cpp
string qvm_file = "LOCAL:common/sounds/sounds.qvm";
auto qvm_addr = QVM::LOAD(qvm_file);
int status = QVM::READ((int)qvm_addr); //Overloaded method 'QVM::READ(int)'
if(status == 0) QVM::CLEANUP(qvm_addr);
```

## Camera section.

### Enabling Free camera with controls

```cpp
Camera::Controls controls;

controls.UP(VK_SPACE); //Key event - Camera Up. (Z-Axis)
controls.DOWN(VK_MENU); //Key event - Camera Down. (Z-Axis)
controls.LEFT(VK_LEFT); //Key event - Camera Left. (Y-Axis)
controls.RIGHT(VK_RIGHT); //Key event - Camera Right. (Y-Axis)
controls.FORWARD(VK_UP); //Key event - Camera Forward. (X-Axis)
controls.BACKWARD(VK_DOWN); //Key event - Camera Backward. (X-Axis)
controls.CALIBRATE(VK_BACK); //Key event - Camera Calibrate. [Reset to X-Axis]
controls.QUIT(VK_RETURN); //Key event - Quit Free cam.
controls.AXIS_OFF(0.5f); //Offset value to move Camera from Axis.

CAMERA::FREECAM(controls);
```
