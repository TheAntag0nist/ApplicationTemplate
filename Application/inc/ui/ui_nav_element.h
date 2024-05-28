#ifndef UI_NAV_ELEMENT_H
#define UI_NAV_ELEMENT_H
#include <ui/ui_core.h>

namespace ui_nav_element {
    bool begin_child_ex(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags);
    bool begin_child(const char* str_id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags flags = 0);
    bool begin_child(ImGuiID id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags flags = 0);
    void end_child();
};

#endif