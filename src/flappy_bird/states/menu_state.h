#pragma once

#include <memory>

#include <newleaf/events/event.h>
#include <newleaf/state/state.h>

namespace fb {

class MenuState : public nl::State {
  public:
    MenuState();

    void on_attach() override final;
    void on_detach() override final;
    void on_event(nl::Event& e) override final;

    static std::unique_ptr<nl::State> create();
};
}
