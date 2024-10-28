#pragma once

#include <memory>

#include <newleaf/events/event.h>
#include <newleaf/state/state.h>

namespace fb {

class MenuState : public nl::State {
  public:
    MenuState();

    virtual void on_attach() override final;
    virtual void on_detach() override final;
    virtual void on_event(nl::Event& e) override final;

    static std::unique_ptr<nl::State> create();
};
}
