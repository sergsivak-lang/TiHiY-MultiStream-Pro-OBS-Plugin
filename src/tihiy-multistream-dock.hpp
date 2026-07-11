#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QTextEdit>
#include <QGroupBox>
#include <QString>

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
    void stopYouTube();
    void stopTwitch();
    void stopCustom();
    void stopAll();

private:
    TihiyTargetUi youtube_;
    TihiyTargetUi twitch_;
    TihiyTargetUi custom_;

    TihiyOutputHandle youtubeOut_;
    TihiyOutputHandle twitchOut_;
    TihiyOutputHandle customOut_;

    QTextEdit *log_ = nullptr;

    QGroupBox *makeTargetBox(const QString &title, TihiyTargetUi &ui,
                             const QString &server, int width, int height, int fps,
                             int vbr, int abr);

    void appendLog(const QString &message);
    bool startTarget(const QString &name, TihiyTargetUi &ui, TihiyOutputHandle &handle);
    void stopTarget(const QString &name, TihiyOutputHandle &handle);
    void releaseTarget(TihiyOutputHandle &handle);
};
