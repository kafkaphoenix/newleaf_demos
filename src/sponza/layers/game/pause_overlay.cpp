#include "pause_overlay.h"

#include <newleaf/application/application.h>

namespace spz {

PauseOverlay::PauseOverlay() : Layer("pause_overlay") {}

void PauseOverlay::on_attach() { nl::Application::get().pause(true); }

void PauseOverlay::on_detach() { nl::Application::get().pause(false); }

void PauseOverlay::on_event(nl::Event& e) {
  nl::EventDispatcher dispatcher(e);
  dispatcher.dispatch<nl::KeyPressedEvent>(BIND_EVENT(on_key_pressed));
}

bool PauseOverlay::on_key_pressed(nl::KeyPressedEvent& e) {
  // repeat event if key is held down
  if (e.is_repeating())
    return false;

  if (e.get_key() == nl::Key::P) {
    m_enabled = false;
    on_detach();
    return true;
  }

  return false;
}

std::unique_ptr<nl::Layer> PauseOverlay::create() { return std::make_unique<PauseOverlay>(); }
}