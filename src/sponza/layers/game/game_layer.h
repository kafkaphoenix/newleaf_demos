#pragma once

#include <memory>

#include <newleaf/events/event.h>
#include <newleaf/events/key_event.h>
#include <newleaf/scene/scene_manager.h>
#include <newleaf/state/layer.h>
#include <newleaf/utils/time.h>

namespace spz {

class GameLayer : public nl::Layer {
  public:
    GameLayer();

    virtual void on_attach() override final;
    virtual void on_detach() override final;
    virtual void on_update(const nl::Time& ts) override final;
    virtual void on_event(nl::Event& e) override final;

    static std::unique_ptr<nl::Layer> create();

  private:
    bool on_key_pressed(nl::KeyPressedEvent& e);
    void register_systems(nl::SceneManager& scene_manager);
    void unregister_systems(nl::SceneManager& scene_manager);
    void create_entities(nl::SceneManager& scene_manager);
    void destroy_entities(nl::SceneManager& scene_manager);
};
}
