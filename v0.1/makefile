PREFIX := D:/usr/local/src/animation_player/v0.1
LOCAL_PREFIX := D:/usr/local

CC := cl
CL := link
LB := lib

L_PREFIX = /LIBPATH:
NVL_PREFIX = -L

CC_OPTIONS := /nologo /std:c++latest /EHsc /MD
CL_OPTIONS := /NOLOGO /SUBSYSTEM:CONSOLE /ENTRY:mainCRTStartup 
NVL_OPTIONS :=
#/NODEFAULTLIB:libcmt.lib

ARGS ?= 

DLL_PATHS :=
IPATHS := -I$(PREFIX)/include
LPATHS := $(L_PREFIX)$(PREFIX)/lib
NVLPATHS := $(NVL_PREFIX)$(PREFIX)/lib
LIBS := Shell32.lib OpenGL32.lib Gdiplus.lib

# GLFW dependency
DLL_PATHS := $(DLL_PATHS);$(LOCAL_PREFIX)/packages/glfw-3.3.3/glfw-3.3.3.bin.WIN64/lib-vc2019
IPATHS := $(IPATHS) -I$(LOCAL_PREFIX)/packages/glfw-3.3.3/glfw-3.3.3.bin.WIN64/include
LPATH = $(LOCAL_PREFIX)/packages/glfw-3.3.3/glfw-3.3.3.bin.WIN64/lib-vc2019
LPATHS := $(LPATHS) $(L_PREFIX)$(LPATH)
NVLPATHS := $(NVLPATHS) $(NVL_PREFIX)$(LPATH)
LIBS := $(LIBS) glfw3.lib

# GLEW dependency
DLL_PATHS := $(DLL_PATHS);$(LOCAL_PREFIX)/packages/glew-2.1.0-win32/glew-2.1.0/bin/Release/x64
IPATHS := $(IPATHS) -I$(LOCAL_PREFIX)/packages/glew-2.1.0-win32/glew-2.1.0/include
LPATH := $(LOCAL_PREFIX)/packages/glew-2.1.0-win32/glew-2.1.0/lib/Release/x64
LPATHS := $(LPATHS) $(L_PREFIX)$(LPATH)
NVLPATHS := $(NVLPATHS) $(NVL_PREFIX)$(LPATH)
LIBS := $(LIBS) glew32.lib

# freeglut dependency
DLL_PATHS := $(DLL_PATHS);$(LOCAL_PREFIX)/packages/freeglut/bin
IPATHS := $(IPATHS) -I$(LOCAL_PREFIX)/packages/freeglut/include
LPATH := $(LOCAL_PREFIX)/packages/freeglut/lib
LPATHS := $(LPATHS) $(L_PREFIX)$(LPATH)
NVLPATHS := $(NVLPATHS) $(NVL_PREFIX)$(LPATH)
LIBS := $(LIBS) freeglutd.lib

# stb_truetype dependency (header only)
IPATHS := $(IPATHS) -I$(LOCAL_PREFIX)/packages/stb_truetype

# SDL2 dependency
DLL_PATHS := $(DLL_PATHS);$(LOCAL_PREFIX)/packages/sdl2-devel-visualcpp/SDL2-devel-2.0.14-VC/SDL2-2.0.14/lib/x64
IPATHS := $(IPATHS) -I$(LOCAL_PREFIX)/packages/sdl2-devel-visualcpp/SDL2-devel-2.0.14-VC/SDL2-2.0.14/include
LPATH := $(LOCAL_PREFIX)/packages/sdl2-devel-visualcpp/SDL2-devel-2.0.14-VC/SDL2-2.0.14/lib/x64
LPATHS := $(LPATHS) $(L_PREFIX)$(LPATH)
NVLPATHS := $(NVLPATHS) $(NVL_PREFIX)$(LPATH)
LIBS := $(LIBS) SDL2.lib

# SDL2_TTF dependency
DLL_PATHS := $(DLL_PATHS);$(LOCAL_PREFIX)/packages/SDL2_ttf-devel-2.0.15-VC/SDL2_ttf-2.0.15/lib/x64
IPATHS := $(IPATHS) -I$(LOCAL_PREFIX)/packages/SDL2_ttf-devel-2.0.15-VC/SDL2_ttf-2.0.15/include
LPATH := $(LOCAL_PREFIX)/packages/SDL2_ttf-devel-2.0.15-VC/SDL2_ttf-2.0.15/lib/x64
LPATHS := $(LPATHS) $(L_PREFIX)$(LPATH)
NVLPATHS := $(NVLPATHS) $(NVL_PREFIX)$(LPATH)
LIBS := $(LIBS) SDL2_ttf.lib

# SDL2_Image dependency
DLL_PATHS := $(DLL_PATHS);$(LOCAL_PREFIX)/packages/SDL2_image-devel-VC/SDL2_image-2.0.5/lib/x64
IPATHS := $(IPATHS) -I$(LOCAL_PREFIX)/packages/SDL2_image-devel-VC/SDL2_image-2.0.5/include
LPATH := $(LOCAL_PREFIX)/packages/SDL2_image-devel-VC/SDL2_image-2.0.5/lib/x64
LPATHS := $(LPATHS) $(L_PREFIX)$(LPATH)
NVLPATHS := $(NVLPATHS) $(NVL_PREFIX)$(LPATH)
LIBS := $(LIBS) SDL2_image.lib

# json dependency
IPATHS := $(IPATHS) -I$(LOCAL_PREFIX)/packages/json/include/single_include/nlohmann

# glm dependency
IPATHS := $(IPATHS) -I$(LOCAL_PREFIX)/packages/glm-0.9.9.8/glm/glm

# shader tools dependency
IPATHS := $(IPATHS) -I$(LOCAL_PREFIX)/src/shader_tools/v0.2/include
LPATH := $(LOCAL_PREFIX)/src/shader_tools/v0.2/lib
LPATHS := $(LPATHS) $(L_PREFIX)$(LPATH)
NVLPATHS := $(NVLPATHS) $(NVL_PREFIX)$(LPATH)
LIBS := $(LIBS) shader_tools.lib

# UI tools dependency
# IPATHS := $(IPATHS) -I$(LOCAL_PREFIX)/src/ui_tools/v0.2/include
# LPATH := $(LOCAL_PREFIX)/src/ui_tools/v0.2/lib
# LPATHS := $(LPATHS) $(L_PREFIX)$(LPATH)
# NVLPATHS := $(NVLPATHS) $(NVL_PREFIX)$(LPATH)
# LIBS := $(LIBS) ui_tools.lib

src/%.cpp:
	@$(CC) -c $@ $(CC_OPTIONS) $(IPATHS) /Fo$(PREFIX)/obj/$*.obj

src/%.cu:
	@nvcc $@ $(CC_OPTIONS) $(IPATHS) -o $(PREFIX)/obj/$*.obj

LINK:
	@$(CL) $(wildcard $(PREFIX)/obj/*.obj) $(CL_OPTIONS) $(LPATHS) $(LIBS) /OUT:$(PREFIX)/bin/main.exe

NVLINK:
	@nvcc $(wildcard $(PREFIX)/obj/*.obj) $(NVL_OPTIONS) $(NVLPATHS) $(LIBS) -o $(PREFIX)/bin/main.exe

RUN:
	@powershell -Command "\$$env:PATH += \"$(DLL_PATHS)\"; $(PREFIX)/bin/main.exe $(ARGS)" 