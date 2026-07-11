#include <obs-module.h>
#include <obs-frontend-api.h>
#include <QPointer>

#include "tihiy-multistream-dock.hpp"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("tihiy-multistream-pro", "en-US")

static QPointer<TihiyMultistreamDock> gDockWidget;

bool obs_module_load(void)
{
    blog(LOG_INFO, "[TiHiY MultiStream Pro] native plugin loading");

    obs_frontend_push_ui_translation(obs_module_get_string);
    gDockWidget = new TihiyMultistreamDock();
    obs_frontend_add_dock_by_id("tihiy_multistream_pro", "TiHiY MultiStream Pro", gDockWidget.data());
    obs_frontend_pop_ui_translation();

    blog(LOG_INFO, "[TiHiY MultiStream Pro] dock registered");
    return true;
}

void obs_module_unload(void)
{
    blog(LOG_INFO, "[TiHiY MultiStream Pro] native plugin unloading");
    if (gDockWidget) {
        delete gDockWidget;
        gDockWidget = nullptr;
    }
}

const char *obs_module_description(void)
{
    return "TiHiY MultiStream Pro: native OBS plugin for separate RTMP outputs with different bitrate/resolution.";
}
