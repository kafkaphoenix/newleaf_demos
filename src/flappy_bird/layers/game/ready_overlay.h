#pragma once

#include <memory>

#include <newleaf/events/event.h>
#include <newleaf/events/key_event.h>
#include <newleaf/state/layer.h>

namespace fb {

class ReadyOverlay : public nl::Layer {
  public:
    ReadyOverlay();

    virtual void on_attach() override final;
    virtual void on_detach() override final;
    virtual void on_event(nl::Event& e) override final;

    static std::unique_ptr<nl::Layer> create();

  private:
    bool on_key_pressed(nl::KeyPressedEvent& e);
};
}
