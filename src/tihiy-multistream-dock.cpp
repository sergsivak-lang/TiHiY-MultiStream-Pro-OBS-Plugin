#include "tihiy-multistream-dock.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QLabel>
#include <QDateTime>
#include <QSettings>
#include <QDialogButtonBox>
#include <QSizePolicy>

#include <obs-module.h>

static constexpr const char *VIDEO_ENCODER_ID_NVENC = "obs_nvenc_h264_tex";
static constexpr const char *VIDEO_ENCODER_ID_X264 = "obs_x264";
static constexpr const char *AUDIO_ENCODER_ID = "ffmpeg_aac";
static constexpr const char *OUTPUT_ID = "rtmp_output";
static constexpr const char *SERVICE_ID = "rtmp_common";

static QPushButton *makeBigButton(const QString &text)
{
    auto *button = new QPushButton(text);
    button->setMinimumHeight(30);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    return button;
}

TihiyMultistreamDock::TihiyMultistreamDock(QWidget *parent) : QWidget(parent)
{
    auto *main = new QVBoxLayout(this);
    main->setContentsMargins(8, 8, 8, 8);
    main->setSpacing(6);

    auto *title = new QLabel("<b>TiHiY MultiStream Pro</b>");
    title->setAlignment(Qt::AlignCenter);
    main->addWidget(title);

    youtubeDialog_ = makeTargetDialog("YouTube 2K60", youtube_, "rtmps://a.rtmps.youtube.com/live2", 2560, 1440, 60, 24000, 160);
    twitchDialog_ = makeTargetDialog("Twitch 1080p60", twitch_, "rtmp://live.twitch.tv/app", 1920, 1080, 60, 6000, 160);
    customDialog_ = makeTargetDialog("Custom RTMP", custom_, "", 1920, 1080, 60, 8000, 160);

    auto *startStopGrid = new QGridLayout();
    auto *startAllButton = makeBigButton("Start All");
    auto *stopAllButton = makeBigButton("Stop All");
    startAllButton->setToolTip("Start enabled outputs in order: YouTube, Twitch, Custom.");
    stopAllButton->setToolTip("Stop all active outputs.");
    startStopGrid->addWidget(startAllButton, 0, 0);
    startStopGrid->addWidget(stopAllButton, 0, 1);
    startStopGrid->setColumnStretch(0, 1);
    startStopGrid->setColumnStretch(1, 1);
    main->addLayout(startStopGrid);

    auto *platformGrid = new QGridLayout();
    youtubeSettingsButton_ = makeBigButton("YouTube");
    twitchSettingsButton_ = makeBigButton("Twitch");
    customSettingsButton_ = makeBigButton("Custom");
    saveSettingsButton_ = makeBigButton("Save settings");
    applyRecommendedButton_ = makeBigButton("Recommended settings");

    youtubeSettingsButton_->setToolTip("Open YouTube stream settings.");
    twitchSettingsButton_->setToolTip("Open Twitch stream settings.");
    customSettingsButton_->setToolTip("Open custom RTMP settings.");
    saveSettingsButton_->setToolTip("Save servers, stream keys, bitrate and enabled flags.");
    applyRecommendedButton_->setToolTip("Apply YouTube 2K60 + Twitch 1080p60 recommended preset.");

    platformGrid->addWidget(youtubeSettingsButton_, 0, 0);
    platformGrid->addWidget(twitchSettingsButton_, 0, 1);
    platformGrid->addWidget(customSettingsButton_, 0, 2);
    platformGrid->addWidget(saveSettingsButton_, 1, 0);
    platformGrid->addWidget(applyRecommendedButton_, 1, 1, 1, 2);
    platformGrid->setColumnStretch(0, 1);
    platformGrid->setColumnStretch(1, 1);
    platformGrid->setColumnStretch(2, 1);
    main->addLayout(platformGrid);

    twitchSafeCpu_ = new QCheckBox("Twitch safe 1080 fix");
    twitchSafeCpu_->setToolTip("Stable Twitch 1080p mode when OBS canvas is 2560x1440.");
    twitchSafeCpu_->setChecked(true);
    main->addWidget(twitchSafeCpu_);

    auto *hint = new QLabel("Click YouTube / Twitch / Custom to open platform settings.");
    hint->setWordWrap(true);
    hint->setAlignment(Qt::AlignCenter);
    main->addWidget(hint);

    log_ = new QTextEdit();
    log_->setReadOnly(true);
    log_->setMinimumHeight(70);
    log_->setMaximumHeight(110);
    log_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    main->addWidget(log_);

    connect(youtube_.start, &QPushButton::clicked, this, &TihiyMultistreamDock::startYouTube);
    connect(twitch_.start, &QPushButton::clicked, this, &TihiyMultistreamDock::startTwitch);
    connect(custom_.start, &QPushButton::clicked, this, &TihiyMultistreamDock::startCustom);
    connect(youtube_.stop, &QPushButton::clicked, this, &TihiyMultistreamDock::stopYouTube);
    connect(twitch_.stop, &QPushButton::clicked, this, &TihiyMultistreamDock::stopTwitch);
    connect(custom_.stop, &QPushButton::clicked, this, &TihiyMultistreamDock::stopCustom);
    connect(startAllButton, &QPushButton::clicked, this, &TihiyMultistreamDock::startAll);
    connect(stopAllButton, &QPushButton::clicked, this, &TihiyMultistreamDock::stopAll);

    connect(applyRecommendedButton_, &QPushButton::clicked, this, &TihiyMultistreamDock::applyRecommendedSettings);
    connect(saveSettingsButton_, &QPushButton::clicked, this, &TihiyMultistreamDock::saveSettingsClicked);
    connect(youtubeSettingsButton_, &QPushButton::clicked, this, &TihiyMultistreamDock::openYouTubeSettings);
    connect(twitchSettingsButton_, &QPushButton::clicked, this, &TihiyMultistreamDock::openTwitchSettings);
    connect(customSettingsButton_, &QPushButton::clicked, this, &TihiyMultistreamDock::openCustomSettings);

    loadSettings();
    appendLog("Compact scalable panel ready. Open a platform to edit settings.");
}

TihiyMultistreamDock::~TihiyMultistreamDock()
{
    stopAll();
    releaseTarget(youtubeOut_);
    releaseTarget(twitchOut_);
    releaseTarget(customOut_);
}

QDialog *TihiyMultistreamDock::makeTargetDialog(const QString &title, TihiyTargetUi &ui,
                                                const QString &server, int width, int height, int fps,
                                                int vbr, int abr)
{
    auto *dialog = new QDialog(this);
    dialog->setWindowTitle(title + " settings");
    dialog->setMinimumWidth(420);

    auto *outer = new QVBoxLayout(dialog);
    outer->setContentsMargins(10, 10, 10, 10);
    outer->setSpacing(8);

    auto *label = new QLabel("<b>" + title + "</b>");
    outer->addWidget(label);

    auto *layout = new QFormLayout();
    layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    ui.enabled = new QCheckBox("Enabled");
    ui.enabled->setChecked(title != "Custom RTMP");

    ui.server = new QLineEdit(server);
    ui.key = new QLineEdit();
    ui.key->setEchoMode(QLineEdit::Password);
    ui.key->setPlaceholderText("Stream key");

    ui.width = new QSpinBox(); ui.width->setRange(320, 7680); ui.width->setValue(width);
    ui.height = new QSpinBox(); ui.height->setRange(240, 4320); ui.height->setValue(height);
    ui.fps = new QSpinBox(); ui.fps->setRange(24, 120); ui.fps->setValue(fps);
    ui.videoBitrate = new QSpinBox(); ui.videoBitrate->setRange(500, 100000); ui.videoBitrate->setValue(vbr);
    ui.audioBitrate = new QSpinBox(); ui.audioBitrate->setRange(64, 320); ui.audioBitrate->setValue(abr);

    layout->addRow(ui.enabled);
    layout->addRow("Server", ui.server);
    layout->addRow("Stream key", ui.key);
    layout->addRow("Width", ui.width);
    layout->addRow("Height", ui.height);
    layout->addRow("FPS", ui.fps);
    layout->addRow("Video bitrate Kbps", ui.videoBitrate);
    layout->addRow("Audio bitrate Kbps", ui.audioBitrate);
    outer->addLayout(layout);

    auto *rowButtons = new QHBoxLayout();
    ui.start = makeBigButton("Start");
    ui.stop = makeBigButton("Stop");
    rowButtons->addWidget(ui.start);
    rowButtons->addWidget(ui.stop);
    outer->addLayout(rowButtons);

    auto *closeButtons = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(closeButtons, &QDialogButtonBox::rejected, dialog, &QDialog::hide);
    outer->addWidget(closeButtons);

    return dialog;
}

void TihiyMultistreamDock::showTargetDialog(QDialog *dialog)
{
    if (!dialog)
        return;
    dialog->show();
    dialog->raise();
    dialog->activateWindow();
}

void TihiyMultistreamDock::openYouTubeSettings() { showTargetDialog(youtubeDialog_); }
void TihiyMultistreamDock::openTwitchSettings() { showTargetDialog(twitchDialog_); }
void TihiyMultistreamDock::openCustomSettings() { showTargetDialog(customDialog_); }

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

    const bool twitchSafe = (name == "Twitch" && twitchSafeCpu_ && twitchSafeCpu_->isChecked());
    const char *videoEncoderId = twitchSafe ? VIDEO_ENCODER_ID_X264 : VIDEO_ENCODER_ID_NVENC;

    obs_data_t *vSettings = obs_data_create();
    obs_data_set_int(vSettings, "bitrate", ui.videoBitrate->value());
    obs_data_set_string(vSettings, "rate_control", "CBR");
    obs_data_set_int(vSettings, "keyint_sec", 2);
    obs_data_set_string(vSettings, "profile", "high");

    if (twitchSafe) {
        obs_data_set_string(vSettings, "preset", "veryfast");
        appendLog(name + ": Twitch safe 1080 fix enabled, using x264 fallback for scaled 1080p output.");
    } else {
        obs_data_set_string(vSettings, "preset", "p5");
        obs_data_set_string(vSettings, "tuning", "hq");
        obs_data_set_string(vSettings, "multipass", "qres");
        obs_data_set_bool(vSettings, "lookahead", false);
        obs_data_set_bool(vSettings, "psycho_aq", true);
        obs_data_set_int(vSettings, "bframes", 2);
    }

    handle.video = obs_video_encoder_create(videoEncoderId, (name + " Video").toUtf8().constData(), vSettings, nullptr);
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

static void setTargetValues(TihiyTargetUi &ui, const QString &server, int width, int height, int fps, int vbr, int abr)
{
    ui.server->setText(server);
    ui.width->setValue(width);
    ui.height->setValue(height);
    ui.fps->setValue(fps);
    ui.videoBitrate->setValue(vbr);
    ui.audioBitrate->setValue(abr);
}

void TihiyMultistreamDock::applyRecommendedSettings()
{
    youtube_.enabled->setChecked(true);
    twitch_.enabled->setChecked(true);
    custom_.enabled->setChecked(false);
    setTargetValues(youtube_, "rtmps://a.rtmps.youtube.com/live2", 2560, 1440, 60, 24000, 160);
    setTargetValues(twitch_, "rtmp://live.twitch.tv/app", 1920, 1080, 60, 6000, 160);
    if (twitchSafeCpu_)
        twitchSafeCpu_->setChecked(true);
    appendLog("Recommended preset applied: YouTube 2K60 + Twitch 1080p60 safe mode.");
}

void TihiyMultistreamDock::saveSettingsClicked()
{
    saveSettings();
    appendLog("Settings saved locally. Stream keys are stored in this Windows user profile.");
}

void TihiyMultistreamDock::saveSettings()
{
    QSettings s("TiHiY-DED", "TiHiYMultiStreamPro");
    auto saveTarget = [&s](const QString &prefix, TihiyTargetUi &ui) {
        s.setValue(prefix + "/enabled", ui.enabled->isChecked());
        s.setValue(prefix + "/server", ui.server->text());
        s.setValue(prefix + "/key", ui.key->text());
        s.setValue(prefix + "/width", ui.width->value());
        s.setValue(prefix + "/height", ui.height->value());
        s.setValue(prefix + "/fps", ui.fps->value());
        s.setValue(prefix + "/vbr", ui.videoBitrate->value());
        s.setValue(prefix + "/abr", ui.audioBitrate->value());
    };
    saveTarget("youtube", youtube_);
    saveTarget("twitch", twitch_);
    saveTarget("custom", custom_);
    if (twitchSafeCpu_)
        s.setValue("twitchSafeCpu", twitchSafeCpu_->isChecked());
}

void TihiyMultistreamDock::loadSettings()
{
    QSettings s("TiHiY-DED", "TiHiYMultiStreamPro");
    auto loadTarget = [&s](const QString &prefix, TihiyTargetUi &ui) {
        ui.enabled->setChecked(s.value(prefix + "/enabled", ui.enabled->isChecked()).toBool());
        ui.server->setText(s.value(prefix + "/server", ui.server->text()).toString());
        ui.key->setText(s.value(prefix + "/key", ui.key->text()).toString());
        ui.width->setValue(s.value(prefix + "/width", ui.width->value()).toInt());
        ui.height->setValue(s.value(prefix + "/height", ui.height->value()).toInt());
        ui.fps->setValue(s.value(prefix + "/fps", ui.fps->value()).toInt());
        ui.videoBitrate->setValue(s.value(prefix + "/vbr", ui.videoBitrate->value()).toInt());
        ui.audioBitrate->setValue(s.value(prefix + "/abr", ui.audioBitrate->value()).toInt());
    };
    loadTarget("youtube", youtube_);
    loadTarget("twitch", twitch_);
    loadTarget("custom", custom_);
    if (twitchSafeCpu_)
        twitchSafeCpu_->setChecked(s.value("twitchSafeCpu", true).toBool());
}

void TihiyMultistreamDock::startYouTube() { startTarget("YouTube", youtube_, youtubeOut_); }
void TihiyMultistreamDock::startTwitch() { startTarget("Twitch", twitch_, twitchOut_); }
void TihiyMultistreamDock::startCustom() { startTarget("Custom", custom_, customOut_); }

void TihiyMultistreamDock::startAll()
{
    saveSettings();
    appendLog("Start All requested. Starting enabled outputs in safe order...");

    bool startedAny = false;

    if (youtube_.enabled->isChecked())
        startedAny = startTarget("YouTube", youtube_, youtubeOut_) || startedAny;

    if (twitch_.enabled->isChecked())
        startedAny = startTarget("Twitch", twitch_, twitchOut_) || startedAny;

    if (custom_.enabled->isChecked())
        startedAny = startTarget("Custom", custom_, customOut_) || startedAny;

    if (!startedAny)
        appendLog("Start All: no enabled output started. Check Enabled flags, server and stream keys.");
}

void TihiyMultistreamDock::stopYouTube() { stopTarget("YouTube", youtubeOut_); }
void TihiyMultistreamDock::stopTwitch() { stopTarget("Twitch", twitchOut_); }
void TihiyMultistreamDock::stopCustom() { stopTarget("Custom", customOut_); }

void TihiyMultistreamDock::stopAll()
{
    stopTarget("YouTube", youtubeOut_);
    stopTarget("Twitch", twitchOut_);
    stopTarget("Custom", customOut_);
}
