#ifdef DEKI_EDITOR

#include <deki-editor/CustomEditor.h>
#include <deki-editor/EditorGUI.h>
#include <deki-editor/EditorRegistry.h>
#include "../TweenComponent.h"
#include "imgui.h"

namespace {

class TweenCustomEditor : public DekiEditor::CustomEditor
{
public:
    const char* GetComponentName() const override { return "TweenComponent"; }

    bool WantsInspectorOverride(DekiComponent* comp) override { return true; }

    void OnInspectorGUI(DekiComponent* comp) override
    {
        // Draw all default properties first
        DekiEditor::EditorGUI::Get().DrawDefaultInspector();

        // Show calculated total duration
        auto* tween = static_cast<TweenComponent*>(comp);
        ImGui::Spacing();
        ImGui::Separator();
        if (tween->loops == -1)
        {
            ImGui::TextDisabled("Total Duration: infinite");
        }
        else
        {
            int plays = tween->loops <= 1 ? 1 : tween->loops;
            float total = tween->delay + tween->duration * plays;
            ImGui::TextDisabled("Total Duration: %.2fs", total);
        }
    }
};

} // anonymous namespace

REGISTER_EDITOR(TweenCustomEditor)

#endif // DEKI_EDITOR
