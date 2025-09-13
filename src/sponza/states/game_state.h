#pragma once

#include <memory>

#include <newleaf/events/event.h>
#include <newleaf/scene/scene_manager.h>
#include <newleaf/state/state.h>
#include <newleaf/state/states_manager.h>
#include <newleaf/utils/time.h>

namespace spz {

class GameState : public nl::State {
  public:
    GameState();

    virtual void on_attach() override final;
    virtual void on_detach() override final;
    virtual void on_update(const nl::Time& ts) override final;
    virtual void on_imgui_update() override final;
    virtual void on_event(nl::Event& e) override final;

    static std::unique_ptr<nl::State> create();

  private:
    void create_layers(nl::StatesManager& states_manager);
    void register_systems(nl::SceneManager& scene_manager);
    void unregister_systems(nl::SceneManager& scene_manager);
};
}
