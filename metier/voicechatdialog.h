#ifndef VOICECHATDIALOG_H
#define VOICECHATDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include "voicechat.h" // existing VoiceToText class

class VoiceChatDialog : public QDialog
{
    Q_OBJECT
public:
    explicit VoiceChatDialog(QWidget *parent = nullptr);
    ~VoiceChatDialog();

private slots:
    void onStartClicked();
    void onStopClicked();
    void onStatusChanged(const QString &status);
    void onInterim(const QString &text);
    void onFinal(const QString &role, const QString &text);

private:
    VoiceToText *m_voice;
    QPushButton *btnStart;
    QPushButton *btnStop;
    QPushButton *btnClose;
    QTextEdit *txtTranscript;
    QLabel *lblStatus;
};

#endif // VOICECHATDIALOG_H
