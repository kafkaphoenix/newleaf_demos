#pragma once

#include <entt/entt.hpp>
#include <newleaf/application/application.h>
#include <newleaf/events/event.h>
#include <newleaf/events/window_event.h>
#include <newleaf/graphics/render_manager.h>
#include <newleaf/settings/settings_manager.h>

namespace fb {

inline bool on_window_closed(nl::WindowCloseEvent& e) {
  nl::Application::get().close();
  return true;
}

inline bool on_window_focused(nl::WindowFocusEvent& e) {
  auto& app = nl::Application::get();
  app.minimize(false);
  if (app.should_restore_pause()) {
    app.pause(true);
    app.restore_pause(false);
  } else {
    app.pause(false);
  }
  return true;
}

inline bool on_window_unfocused(nl::WindowLostFocusEvent& e) {
  auto& app = nl::Application::get();
  app.minimize(true);
  if (app.is_paused()) {
    app.restore_pause(true);
  } else {
    app.pause(true);
  }
  return true;
}

inline bool on_window_resized(nl::WindowResizeEvent& e, entt::registry& registry) {
  auto& app = nl::Application::get();
  auto& settings_manager = app.get_settings_manager();

  app.get_render_manager().on_window_resized(e.get_width(), e.get_height());

  if (not settings_manager.fullscreen) { // when resizing with the mouse
    settings_manager.window_w = e.get_width();
    settings_manager.window_h = e.get_height();
  }

  return true;
}

inline void window_handler(nl::Event& e, entt::registry& registry) {
  nl::EventDispatcher dispatcher(e);

  dispatcher.dispatch<nl::WindowCloseEvent>(BIND_STATIC_EVENT(on_window_closed));
  dispatcher.dispatch<nl::WindowFocusEvent>(BIND_STATIC_EVENT(on_window_focused));
  dispatcher.dispatch<nl::WindowLostFocusEvent>(BIND_STATIC_EVENT(on_window_unfocused));
  dispatcher.dispatch<nl::WindowResizeEvent>(BIND_STATIC_EVENT(on_window_resized, registry));
}
}