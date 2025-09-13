#include <memory>

#include <newleaf/application/application.h>
#include <newleaf/application/entrypoint.h>
#include <newleaf/graphics/render_api.h>
#include <newleaf/logging/log_manager.h>
#include <newleaf/serializers/sSettings.h>
#include <newleaf/settings/settings_manager.h>
#include <newleaf/state/states_manager.h>
#include <newleaf/utils/get_default_roaming_path.h>

#include <states/game_state.h>

namespace spz {

class Sponza : public nl::Application {
  public:
    Sponza(std::unique_ptr<nl::SettingsManager>&& s, nl::CLArgs&& args)
      : nl::Application(std::move(s), std::move(args)) {
      nl::RenderAPI::set_clear_color(m_settings_manager->clear_color);
      APP_TRACE("loading initial state");
      m_states_manager->push_state(GameState::create());
      APP_TRACE("state loaded");
    }

    ~Sponza() override final { APP_WARN("stopping Sponza application"); }
};

}

nl::Application* nl::create(nl::CLArgs&& args) {
  auto settings_manager = load_settings("Sponza");

  // TODO this should be in the settings manager or a config file
  settings_manager->app_name = "Sponza";
  settings_manager->window_icon_path = "assets/common/textures/app_icon.png";
  settings_manager->cursor_icon_path = "assets/common/textures/cursor.png";
  settings_manager->default_texture_path = "assets/common/textures/default.png";
  settings_manager->active_scene = "root";
  settings_manager->active_scene_path = "assets/sponza/scenes/root.json";

  // TODO this should be in the settings manager methods?
  LogManager::set_engine_logger_level(settings_manager->engine_log_level);
  LogManager::set_engine_logger_flush_level(settings_manager->engine_flush_level);
  LogManager::set_app_logger_level(settings_manager->app_log_level);
  LogManager::set_app_logger_flush_level(settings_manager->app_flush_level);
  LogManager::toggle_engine_logger(settings_manager->engine_logger_enabled);
  LogManager::toggle_app_logger(settings_manager->app_logger_enabled);

  if (not settings_manager->logfile_path.empty()) {
    LogManager::create_file_logger((get_default_roaming_path("Sponza") / settings_manager->logfile_path).string());
  }

  if (not settings_manager->backtrace_logfile_path.empty()) {
    LogManager::create_backtrace_logger(
      (get_default_roaming_path("Sponza") / settings_manager->backtrace_logfile_path).string(),
      settings_manager->engine_backtrace_logger_enabled, settings_manager->app_backtrace_logger_enabled);
  }

  APP_INFO("loading settings");
  APP_INFO("initializating Sponza application");
  return new spz::Sponza(std::move(settings_manager), std::move(args));
}