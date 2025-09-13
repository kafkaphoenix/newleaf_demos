#pragma once

#include <newleaf/application/application.h>
#include <newleaf/events/event.h>
#include <newleaf/events/key_event.h>
#include <newleaf/state/states_manager.h>

#include "states/game_state.h"

namespace fb {

inline bool onMenuKeyPressed(nl::KeyPressedEvent& e) {
  // repeat event if key is held down
  if (e.is_repeating())
    return false;

  if (e.get_key() == nl::Key::Enter) {
    nl::Application::get().get_states_manager().push_state(GameState::create());
    return true;
  }

  return false;
}

inline void menu_input_handler(nl::Event& e) {
  nl::EventDispatcher dispatcher(e);

  dispatcher.dispatch<nl::KeyPressedEvent>(BIND_STATIC_EVENT(onMenuKeyPressed));
}
}