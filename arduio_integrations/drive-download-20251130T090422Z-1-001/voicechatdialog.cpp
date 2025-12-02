#include "voicechatdialog.h"
#include <QHBoxLayout>
#include <QMessageBox>

VoiceChatDialog::VoiceChatDialog(QWidget *parent)
    : QDialog(parent), m_voice(new VoiceToText(this))
{
    setWindowTitle("Voice Chat");
    setMinimumSize(520, 380);

    QVBoxLayout *main = new QVBoxLayout(this);

    lblStatus = new QLabel("Statut: inactif");
    main->addWidget(lblStatus);

    txtTranscript = new QTextEdit();
    txtTranscript->setReadOnly(true);
    txtTranscript->setPlaceholderText("Transcription en temps réel...");
    main->addWidget(txtTranscript, 1);

    QHBoxLayout *btnLayout = new QHBoxLayout();

    btnStart = new QPushButton("Démarrer");
    btnStop = new QPushButton("Arrêter");
    btnClose = new QPushButton("Fermer");

    btnLayout->addWidget(btnStart);
    btnLayout->addWidget(btnStop);
    btnLayout->addStretch();
    btnLayout->addWidget(btnClose);

    main->addLayout(btnLayout);

    connect(btnStart, &QPushButton::clicked, this, &VoiceChatDialog::onStartClicked);
    connect(btnStop, &QPushButton::clicked, this, &VoiceChatDialog::onStopClicked);
    connect(btnClose, &QPushButton::clicked, this, &VoiceChatDialog::close);

    connect(m_voice, &VoiceToText::statusChanged, this, &VoiceChatDialog::onStatusChanged);
    connect(m_voice, &VoiceToText::interimMessageReceived, this, &VoiceChatDialog::onInterim);
    connect(m_voice, &VoiceToText::finalMessageReceived, this, &VoiceChatDialog::onFinal);
}

VoiceChatDialog::~VoiceChatDialog()
{
    if (m_voice) {
        m_voice->stop();
        delete m_voice;
    }
}

void VoiceChatDialog::onStartClicked()
{
    if (!m_voice->hasApiKey()) {
        QMessageBox::warning(this, "Voice Chat", "Aucune clé API configurée pour la reconnaissance vocale.\n\nVeuillez définir une clé Hume via setApiKey() ou config.");
        return;
    }

    if (!m_voice->start()) {
        QMessageBox::critical(this, "Voice Chat", "Impossible de démarrer l'enregistrement.");
        return;
    }
}

void VoiceChatDialog::onStopClicked()
{
    m_voice->stop();
}

void VoiceChatDialog::onStatusChanged(const QString &status)
{
    lblStatus->setText("Statut: " + status);
}

void VoiceChatDialog::onInterim(const QString &text)
{
    // Affiche la transcription intermédiaire (remplace la dernière ligne)
    txtTranscript->append(QString("(interim) %1").arg(text));
}

void VoiceChatDialog::onFinal(const QString &role, const QString &text)
{
    Q_UNUSED(role);
    txtTranscript->append(QString("%1").arg(text));
}
