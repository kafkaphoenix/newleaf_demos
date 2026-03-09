#pragma once

#include <memory>

#include <newleaf/events/event.h>
#include <newleaf/state/state.h>
#include <newleaf/utils/time.h>

namespace fb {

class GameState : public nl::State {
  public:
    GameState();

    void on_attach() override final;
    void on_detach() override final;
    void on_update(const nl::Time& ts) override final;
    void on_event(nl::Event& e) override final;

    static std::unique_ptr<nl::State> create();
};
}
