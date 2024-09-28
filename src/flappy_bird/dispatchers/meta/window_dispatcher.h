#pragma once

#include <entt/entt.hpp>
#include <newleaf/application/application.h>
#include <newleaf/components/graphics/cFBO.h>
#include <newleaf/components/meta/cUUID.h>
#include <newleaf/events/event.h>
#include <newleaf/events/window_event.h>
#include <newleaf/graphics/render_manager.h>
#include <newleaf/settings/settings_manager.h>

namespace fb {

inline bool on_window_closed(nl::WindowCloseEvent& e) {
  nl::Application::get().close();
  return true;
}

inline bool on_window_restored(nl::WindowRestoredEvent& e) { return true; }

inline bool on_window_minimized(nl::WindowMinimizedEvent& e) { return true; }

inline bool on_window_maximized(nl::WindowMaximizedEvent& e) { return true; }

inline bool on_window_focused(nl::WindowFocusEvent& e) {
  nl::Application::get().minimize(false);
  if (nl::Application::get().should_restore_pause()) {
    nl::Application::get().pause(true);
    nl::Application::get().restore_pause(false);
  } else {
    nl::Application::get().pause(false);
  }
  return true;
}

inline bool on_window_unfocused(nl::WindowLostFocusEvent& e) {
  nl::Application::get().minimize(true);
  if (nl::Application::get().is_paused()) {
    nl::Application::get().restore_pause(true);
  } else {
    nl::Application::get().pause(true);
  }
  return true;
}

inline bool on_window_moved(nl::WindowMovedEvent& e) { return true; }

inline bool on_window_resized(nl::WindowResizeEvent& e,
                              entt::registry& registry) {
  auto& app = nl::Application::get();
  auto& settings_manager = app.get_settings_manager();
  auto& render_manager = app.get_render_manager();

  render_manager.on_window_resized(e.get_width(), e.get_height());

  if (not settings_manager.fullscreen) { // when resizing with the mouse
    settings_manager.window_w = e.get_width();
    settings_manager.window_h = e.get_height();
  }

  if (not render_manager.get_framebuffers().empty()) {
    entt::entity fbo = registry.view<nl::CFBO, nl::CUUID>()
                         .front(); // TODO: support more than one?
    nl::CFBO& cfbo = registry.get<nl::CFBO>(fbo);
    render_manager.delete_framebuffer(cfbo.fbo);
    render_manager.add_framebuffer(std::string(cfbo.fbo), e.get_width(),
                                   e.get_height(), cfbo.attachment);
  }

  return true;
}

inline void window_dispatcher(nl::Event& e, entt::registry& registry) {
  nl::EventDispatcher dispatcher(e);

  dispatcher.dispatch<nl::WindowCloseEvent>(
    BIND_STATIC_EVENT(on_window_closed));
  dispatcher.dispatch<nl::WindowRestoredEvent>(
    BIND_STATIC_EVENT(on_window_restored));
  dispatcher.dispatch<nl::WindowMinimizedEvent>(
    BIND_STATIC_EVENT(on_window_minimized));
  dispatcher.dispatch<nl::WindowMaximizedEvent>(
    BIND_STATIC_EVENT(on_window_maximized));
  dispatcher.dispatch<nl::WindowResizeEvent>(
    BIND_STATIC_EVENT(on_window_resized, registry));
  dispatcher.dispatch<nl::WindowFocusEvent>(
    BIND_STATIC_EVENT(on_window_focused));
  dispatcher.dispatch<nl::WindowLostFocusEvent>(
    BIND_STATIC_EVENT(on_window_unfocused));
  dispatcher.dispatch<nl::WindowMovedEvent>(BIND_STATIC_EVENT(on_window_moved));
}
}