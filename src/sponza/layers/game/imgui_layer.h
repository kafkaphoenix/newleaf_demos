#pragma once

#include <memory>
#include <newleaf/state/layer.h>

namespace spz {

class ImGuiLayer : public nl::Layer {
  public:
    ImGuiLayer();

    virtual void on_imgui_update() override;

    static std::unique_ptr<nl::Layer> create();
};
}