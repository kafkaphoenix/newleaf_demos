#pragma once

#include <memory>

#include <newleaf/state/layer.h>

namespace fb {

class MenuBackgroundLayer : public nl::Layer {
  public:
    MenuBackgroundLayer();

    virtual void on_attach() override final;
    virtual void on_detach() override final;

    static std::unique_ptr<nl::Layer> create();
};
}
