#pragma once

#include <memory>

#include <newleaf/state/layer.h>

namespace fb {

class MenuButtonsLayer : public nl::Layer {
  public:
    MenuButtonsLayer();

    virtual void on_attach() override final;
    virtual void on_detach() override final;
    virtual void on_event(nl::Event& e) override final;

    static std::unique_ptr<nl::Layer> create();
};
}
