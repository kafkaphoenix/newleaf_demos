#pragma once

#include <memory>

#include <newleaf/events/event.h>
#include <newleaf/events/key_event.h>
#include <newleaf/state/layer.h>
#include <newleaf/utils/time.h>

namespace fb {

class GameLayer : public nl::Layer {
  public:
    GameLayer();

    virtual void on_detach() override final;
    virtual void on_update(const nl::Time& ts) override final;
    virtual void on_event(nl::Event& e) override final;

    static std::unique_ptr<nl::Layer> create();

  private:
    bool on_key_pressed(nl::KeyPressedEvent& e);
};
}
