#include "tihiy-multistream-dock.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QDateTime>

#include <obs-module.h>

static constexpr const char *VIDEO_ENCODER_ID = "obs_nvenc_h264_tex";
static constexpr const char *AUDIO_ENCODER_ID = "ffmpeg_aac";
static constexpr const char *OUTPUT_ID = "rtmp_output";
static constexpr const char *SERVICE_ID = "rtmp_common";

TihiyMultistreamDock::TihiyMultistreamDock(QWidget *parent) : QWidget(parent)
{
    auto *main = new QVBoxLayout(this);

    auto *title = new QLabel("<b>TiHiY MultiStream Pro — Native OBS Plugin</b>");
    main->addWidget(title);

    main->addWidget(makeTargetBox("YouTube 2K60", youtube_, "rtmp://a.rtmp.youtube.com/live2", 2560, 1440, 60, 24000, 160));
    main->addWidget(makeTargetBox("Twitch 1080p60", twitch_, "rtmp://live.twitch.tv/app", 1920, 1080, 60, 6000, 160));
    main->addWidget(makeTargetBox("Custom RTMP", custom_, "", 1920, 1080, 60, 8000, 160));

    auto *buttons = new QHBoxLayout();
    auto *stopAllButton = new QPushButton("Stop All");
    buttons->addWidget(stopAllButton);
    buttons->addStretch();
    main->addLayout(buttons);

    log_ = new QTextEdit();
    log_->setReadOnly(true);
    log_->setMinimumHeight(120);
    main->addWidget(log_);

    connect(youtube_.start, &QPushButton::clicked, this, &TihiyMultistreamDock::startYouTube);
    connect(twitch_.start, &QPushButton::clicked, this, &TihiyMultistreamDock::startTwitch);
    connect(custom_.start, &QPushButton::clicked, this, &TihiyMultistreamDock::startCustom);
    connect(youtube_.stop, &QPushButton::clicked, this, &TihiyMultistreamDock::stopYouTube);
    connect(twitch_.stop, &QPushButton::clicked, this, &TihiyMultistreamDock::stopTwitch);
    connect(custom_.stop, &QPushButton::clicked, this, &TihiyMultistreamDock::stopCustom);
    connect(stopAllButton, &QPushButton::clicked, this, &TihiyMultistreamDock::stopAll);

    appendLog("Plugin UI ready. First test one platform only.");
}

TihiyMultistreamDock::~TihiyMultistreamDock()
{
    stopAll();
    releaseTarget(youtubeOut_);
    releaseTarget(twitchOut_);
    releaseTarget(customOut_);
}

QGroupBox *TihiyMultistreamDock::makeTargetBox(const QString &title, TihiyTargetUi &ui,
                                                const QString &server, int width, int height, int fps,
                                                int vbr, int abr)
{
    auto *box = new QGroupBox(title);
    auto *layout = new QFormLayout(box);

    ui.enabled = new QCheckBox("Enabled");
    ui.enabled->setChecked(title != "Custom RTMP");

    ui.server = new QLineEdit(server);
    ui.key = new QLineEdit();
    ui.key->setEchoMode(QLineEdit::Password);

    ui.width = new QSpinBox(); ui.width->setRange(320, 7680); ui.width->setValue(width);
    ui.height = new QSpinBox(); ui.height->setRange(240, 4320); ui.height->setValue(height);
    ui.fps = new QSpinBox(); ui.fps->setRange(24, 120); ui.fps->setValue(fps);
    ui.videoBitrate = new QSpinBox(); ui.videoBitrate->setRange(500, 100000); ui.videoBitrate->setValue(vbr);
    ui.audioBitrate = new QSpinBox(); ui.audioBitrate->setRange(64, 320); ui.audioBitrate->setValue(abr);

    auto *rowButtons = new QHBoxLayout();
    ui.start = new QPushButton("Start");
    ui.stop = new QPushButton("Stop");
    rowButtons->addWidget(ui.start);
    rowButtons->addWidget(ui.stop);

    layout->addRow(ui.enabled);
    layout->addRow("Server", ui.server);
    layout->addRow("Stream key", ui.key);
    layout->addRow("Width", ui.width);
    layout->addRow("Height", ui.height);
    layout->addRow("FPS", ui.fps);
    layout->addRow("Video bitrate Kbps", ui.videoBitrate);
    layout->addRow("Audio bitrate Kbps", ui.audioBitrate);
    layout->addRow(rowButtons);

    return box;
}

void TihiyMultistreamDock::appendLog(const QString &message)
{
    const QString line = QDateTime::currentDateTime().toString("HH:mm:ss") + "  " + message;
    if (log_)
        log_->append(line);
    blog(LOG_INFO, "[TiHiY MultiStream Pro] %s", line.toUtf8().constData());
}

bool TihiyMultistreamDock::startTarget(const QString &name, TihiyTargetUi &ui, TihiyOutputHandle &handle)
{
    if (!ui.enabled->isChecked()) {
        appendLog(name + ": skipped, not enabled.");
        return false;
    }

    if (ui.server->text().trimmed().isEmpty() || ui.key->text().trimmed().isEmpty()) {
        appendLog(name + ": server/key is empty.");
        return false;
    }

    if (handle.output) {
        appendLog(name + ": already created. Stop first.");
        return false;
    }

    obs_data_t *serviceSettings = obs_data_create();
    obs_data_set_string(serviceSettings, "server", ui.server->text().toUtf8().constData());
    obs_data_set_string(serviceSettings, "key", ui.key->text().toUtf8().constData());
    handle.service = obs_service_create(SERVICE_ID, name.toUtf8().constData(), serviceSettings, nullptr);
    obs_data_release(serviceSettings);

    obs_data_t *vSettings = obs_data_create();
    obs_data_set_int(vSettings, "bitrate", ui.videoBitrate->value());
    obs_data_set_string(vSettings, "rate_control", "CBR");
    obs_data_set_int(vSettings, "keyint_sec", 2);
    obs_data_set_string(vSettings, "preset", "p5");
    obs_data_set_string(vSettings, "profile", "high");
    handle.video = obs_video_encoder_create(VIDEO_ENCODER_ID, (name + " Video").toUtf8().constData(), vSettings, nullptr);
    obs_data_release(vSettings);

    obs_data_t *aSettings = obs_data_create();
    obs_data_set_int(aSettings, "bitrate", ui.audioBitrate->value());
    handle.audio = obs_audio_encoder_create(AUDIO_ENCODER_ID, (name + " Audio").toUtf8().constData(), aSettings, 0, nullptr);
    obs_data_release(aSettings);

    handle.output = obs_output_create(OUTPUT_ID, (name + " Output").toUtf8().constData(), nullptr, nullptr);

    if (!handle.service || !handle.video || !handle.audio || !handle.output) {
        appendLog(name + ": failed to create OBS output/encoder/service.");
        releaseTarget(handle);
        return false;
    }

    obs_encoder_set_video(handle.video, obs_get_video());
    obs_encoder_set_audio(handle.audio, obs_get_audio());

    // OBS supports scaled encoder output. FPS is usually global; for your presets both targets are 60 FPS.
    obs_encoder_set_scaled_size(handle.video, ui.width->value(), ui.height->value());

    obs_output_set_service(handle.output, handle.service);
    obs_output_set_video_encoder(handle.output, handle.video);
    obs_output_set_audio_encoder(handle.output, handle.audio, 0);

    if (!obs_output_start(handle.output)) {
        const char *error = obs_output_get_last_error(handle.output);
        appendLog(name + ": start failed: " + QString(error ? error : "unknown error"));
        releaseTarget(handle);
        return false;
    }

    appendLog(name + ": started " + QString::number(ui.width->value()) + "x" + QString::number(ui.height->value()) +
              " @ " + QString::number(ui.videoBitrate->value()) + " Kbps");
    return true;
}

void TihiyMultistreamDock::stopTarget(const QString &name, TihiyOutputHandle &handle)
{
    if (handle.output) {
        obs_output_stop(handle.output);
        appendLog(name + ": stop requested.");
    }
    releaseTarget(handle);
}

void TihiyMultistreamDock::releaseTarget(TihiyOutputHandle &handle)
{
    if (handle.output) { obs_output_release(handle.output); handle.output = nullptr; }
    if (handle.video) { obs_encoder_release(handle.video); handle.video = nullptr; }
    if (handle.audio) { obs_encoder_release(handle.audio); handle.audio = nullptr; }
    if (handle.service) { obs_service_release(handle.service); handle.service = nullptr; }
}

void TihiyMultistreamDock::startYouTube() { startTarget("YouTube", youtube_, youtubeOut_); }
void TihiyMultistreamDock::startTwitch() { startTarget("Twitch", twitch_, twitchOut_); }
void TihiyMultistreamDock::startCustom() { startTarget("Custom", custom_, customOut_); }
void TihiyMultistreamDock::stopYouTube() { stopTarget("YouTube", youtubeOut_); }
void TihiyMultistreamDock::stopTwitch() { stopTarget("Twitch", twitchOut_); }
void TihiyMultistreamDock::stopCustom() { stopTarget("Custom", customOut_); }

void TihiyMultistreamDock::stopAll()
{
    stopTarget("YouTube", youtubeOut_);
    stopTarget("Twitch", twitchOut_);
    stopTarget("Custom", customOut_);
}
