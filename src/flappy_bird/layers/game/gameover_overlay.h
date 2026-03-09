#pragma once

#include <memory>

#include <newleaf/events/event.h>
#include <newleaf/events/key_event.h>
#include <newleaf/state/layer.h>

namespace fb {

class GameoverOverlay : public nl::Layer {
  public:
    GameoverOverlay();

    void on_attach() override final;
    void on_detach() override final;
    void on_event(nl::Event& e) override final;

    static std::unique_ptr<nl::Layer> create();

  private:
    bool on_key_pressed(nl::KeyPressedEvent& e);
};
}