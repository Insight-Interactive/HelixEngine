#include "HelixEdPCH.h"


#include "misc/cpp/imgui_stdlib.cpp"
#if defined HE_WIN32 || defined HE_WIN64
#include "backends/imgui_impl_win32.cpp"
#endif

#define IMGUI_IMPLEMENTATION
#include "imgui.cpp"
#include "imgui_demo.cpp"
#include "imgui_draw.cpp"
#include "imgui_tables.cpp"
#include "imgui_widgets.cpp"