#include <memory>

#include <newleaf/application/application.h>
#include <newleaf/application/entrypoint.h>
#include <newleaf/graphics/render_api.h>
#include <newleaf/logging/log_manager.h>
#include <newleaf/serializers/sSettings.h>
#include <newleaf/settings/settings_manager.h>
#include <newleaf/state/states_manager.h>
#include <newleaf/utils/get_default_roaming_path.h>

#include "components/register.h"
#include "states/menu_state.h"

namespace fb {

class FlappyBird : public nl::Application {
  public:
    FlappyBird(std::unique_ptr<nl::SettingsManager>&& s, nl::CLArgs&& args)
      : nl::Application(std::move(s), std::move(args)) {
      nl::RenderAPI::set_clear_color(m_settings_manager->clear_color);
      nl::RenderAPI::set_clear_depth(m_settings_manager->clear_depth);
      APP_TRACE("registering app components...");
      register_components();
      APP_TRACE("loading initial state...");
      m_states_manager->push_state(MenuState::create());
      APP_TRACE("state loaded!");
    }

    ~FlappyBird() override { APP_WARN("deleting Flappy Bird application"); }
};

}

nl::Application* nl::create(nl::CLArgs&& args) {
  auto settings_manager = load_settings("FlappyBird");
  LogManager::set_engine_logger_level(settings_manager->engine_log_level);
  LogManager::set_engine_logger_flush_level(
    settings_manager->engine_flush_level);
  LogManager::set_app_logger_level(settings_manager->app_log_level);
  LogManager::set_app_logger_flush_level(settings_manager->app_flush_level);
  LogManager::toggle_engine_logger(settings_manager->enable_engine_logger);
  LogManager::toggle_app_logger(settings_manager->enable_app_logger);

  if (not settings_manager->logfile_path.empty()) {
    LogManager::create_file_logger(
      (get_default_roaming_path("FlappyBird") / settings_manager->logfile_path)
        .string());
  }

  if (not settings_manager->backtrace_logfile_path.empty()) {
    LogManager::create_backtrace_logger(
      (get_default_roaming_path("FlappyBird") /
       settings_manager->backtrace_logfile_path)
        .string(),
      settings_manager->enable_engine_backtrace_logger,
      settings_manager->enable_app_backtrace_logger);
  }

  APP_INFO("loading settings...");
  APP_INFO("initializating Flappy Bird application");
  return new fb::FlappyBird(std::move(settings_manager), std::move(args));
}