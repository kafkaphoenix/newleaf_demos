#pragma once

#include <memory>

#include <newleaf/state/layer.h>

namespace fb {

class MenuButtonsLayer : public nl::Layer {
  public:
    MenuButtonsLayer();

    void on_attach() override final;
    void on_detach() override final;
    void on_event(nl::Event& e) override final;

    static std::unique_ptr<nl::Layer> create();
};
}
