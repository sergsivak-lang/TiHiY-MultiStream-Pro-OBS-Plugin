#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QTextEdit>
#include <QDialog>
#include <QString>
#include <QProcess>
#include <QComboBox>

#include <obs.hpp>

struct TihiyTargetUi {
    QCheckBox *enabled = nullptr;
    QLineEdit *server = nullptr;
    QLineEdit *key = nullptr;
    QSpinBox *width = nullptr;
    QSpinBox *height = nullptr;
    QSpinBox *fps = nullptr;
    QSpinBox *videoBitrate = nullptr;
    QSpinBox *audioBitrate = nullptr;
    QPushButton *start = nullptr;
    QPushButton *stop = nullptr;
    QCheckBox *autoOpenChat = nullptr;
    QLineEdit *chatUrl = nullptr;
    QPushButton *openChat = nullptr;
};

struct TihiyOutputHandle {
    obs_output_t *output = nullptr;
    obs_service_t *service = nullptr;
    obs_encoder_t *video = nullptr;
    obs_encoder_t *audio = nullptr;
};

class TihiyMultistreamDock : public QWidget {
    Q_OBJECT
public:
    explicit TihiyMultistreamDock(QWidget *parent = nullptr);
    ~TihiyMultistreamDock() override;

private slots:
    void startYouTube();
    void startTwitch();
    void startCustom();
    void startAll();
    void stopYouTube();
    void stopTwitch();
    void stopCustom();
    void stopAll();
    void applyRecommendedSettings();
    void saveSettingsClicked();
    void openYouTubeSettings();
    void openTwitchSettings();
    void openCustomSettings();
    void openRutonySettings();
    void startRutonyManual();
    void stopRutony();
    void openYouTubeChat();
    void openTwitchChat();
    void openCustomChat();

private:
    TihiyTargetUi youtube_;
    TihiyTargetUi twitch_;
    TihiyTargetUi custom_;

    TihiyOutputHandle youtubeOut_;
    TihiyOutputHandle twitchOut_;
    TihiyOutputHandle customOut_;

    QTextEdit *log_ = nullptr;
    QCheckBox *twitchSafeCpu_ = nullptr;
    QPushButton *applyRecommendedButton_ = nullptr;
    QPushButton *saveSettingsButton_ = nullptr;
    QPushButton *youtubeSettingsButton_ = nullptr;
    QPushButton *twitchSettingsButton_ = nullptr;
    QPushButton *customSettingsButton_ = nullptr;
    QPushButton *openChatsButton_ = nullptr;
    QPushButton *rutonySettingsButton_ = nullptr;
    QPushButton *startRutonyButton_ = nullptr;
    QPushButton *stopRutonyButton_ = nullptr;

    QComboBox *rutonyMode_ = nullptr;
    QLineEdit *rutonyPath_ = nullptr;
    QLineEdit *rutonyArgs_ = nullptr;
    QLineEdit *rutonySteamAppId_ = nullptr;
    QLineEdit *rutonyCustomCommand_ = nullptr;
    QCheckBox *rutonyAutoStart_ = nullptr;
    QProcess *rutonyProcess_ = nullptr;
    bool rutonyDetachedLaunch_ = false;

    QDialog *youtubeDialog_ = nullptr;
    QDialog *twitchDialog_ = nullptr;
    QDialog *customDialog_ = nullptr;
    QDialog *rutonyDialog_ = nullptr;

    QDialog *makeTargetDialog(const QString &title, TihiyTargetUi &ui,
                              const QString &server, const QString &defaultChatUrl,
                              int width, int height, int fps,
                              int vbr, int abr);
    QDialog *makeRutonyDialog();

    void showTargetDialog(QDialog *dialog);
    void openChatForTarget(const QString &name, TihiyTargetUi &ui, bool manual);
    void launchRutony(bool manual);
    void appendLog(const QString &message);
    bool startTarget(const QString &name, TihiyTargetUi &ui, TihiyOutputHandle &handle);
    void stopTarget(const QString &name, TihiyOutputHandle &handle);
    void releaseTarget(TihiyOutputHandle &handle);
    void loadSettings();
    void saveSettings();
};
