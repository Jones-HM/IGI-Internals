# Project I.G.I Internal

[Project I.G.I](https://en.wikipedia.org/wiki/Project_I.G.I.) Internal is Project to access internal methods of the game by using **DLL injection method** into the game and calling [IGI Natives methods](https://github.com/IGI-Research-Devs/IGI-Internals-Data/blob/main/IGI-Natives.json) to modify/alter the behaviour of the game.</br>
This was originally intended for research project to understand the game mechanics and how developers worked hard to create them.</br>
So don't use this project to misuse the game's identity and source of original game devs.</br>

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
