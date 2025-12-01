#include "clientmapwidget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QFont>
#include <QPen>
#include <QCoreApplication>
#include <QPainterPath>
#include <QWheelEvent>
#include <QMouseEvent>

ClientMapWidget::ClientMapWidget(QWidget *parent)
    : QWidget(parent),
      m_latitude(0.0),
      m_longitude(0.0),
      m_hasFix(false),
      m_hasBackground(false),
      m_zoom(1.0)
{
    setMinimumSize(400, 300);

    // Charger automatiquement une image de fond si elle existe
    // 1) d'abord à côté de l'exécutable (build Debug/Release)
    // 2) sinon à la racine du projet (un niveau au-dessus du dossier build)
    const QString appDir = QCoreApplication::applicationDirPath();
    QString imagePath = appDir + "/map_background.png";
    m_hasBackground = m_background.load(imagePath);
    if (!m_hasBackground) {
        imagePath = appDir + "/../map_background.png";
        m_hasBackground = m_background.load(imagePath);
    }
    if (!m_hasBackground) {
        imagePath = appDir + "/../../map_background.png";
        m_hasBackground = m_background.load(imagePath);
    }
    if (!m_hasBackground) {
        imagePath = appDir + "/../../../map_background.png";
        m_hasBackground = m_background.load(imagePath);
    }
}

void ClientMapWidget::setClientInfo(const QString &name,
                                    const QString &address,
                                    double latitude,
                                    double longitude,
                                    bool hasFix)
{
    m_name = name;
    m_address = address;
    m_latitude = latitude;
    m_longitude = longitude;
    m_hasFix = hasFix;
    update();
}

void ClientMapWidget::setBackgroundImage(const QString &filePath)
{
    m_hasBackground = m_background.load(filePath);
    update();
}

void ClientMapWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    // Fond global
    p.fillRect(rect(), QColor(15, 30, 50));

    // Zone de carte
    QRectF mapRect = rect().adjusted(20, 20, -20, -100);

    // --- Contenu zoomable (fond + grille + point) ---
    {
        QPainterPath clipPath;
        clipPath.addRoundedRect(mapRect, 10, 10);

        p.save();
        p.setClipPath(clipPath);

        // Appliquer le zoom autour du centre de la carte puis le pan (déplacement)
        p.translate(mapRect.center());
        p.scale(m_zoom, m_zoom);
        p.translate(-mapRect.center());
        p.translate(m_panOffset);

        // Fond de la carte (image ou couleur unie)
        if (m_hasBackground && !m_background.isNull()) {
            QPixmap scaled = m_background.scaled(mapRect.size().toSize(),
                                                 Qt::KeepAspectRatioByExpanding,
                                                 Qt::SmoothTransformation);
            p.drawPixmap(mapRect.topLeft(), scaled);
        } else {
            p.fillRect(mapRect, QColor(20, 60, 100));
        }

        // Grille
        p.setPen(QColor(80, 110, 140, 170));
        const int gridLines = 6;
        for (int i = 1; i < gridLines; ++i) {
            double x = mapRect.left() + i * mapRect.width() / gridLines;
            double y = mapRect.top() + i * mapRect.height() / gridLines;
            p.drawLine(QPointF(x, mapRect.top()), QPointF(x, mapRect.bottom()));
            p.drawLine(QPointF(mapRect.left(), y), QPointF(mapRect.right(), y));
        }

        // Point du client ou message "pas de GPS"
        if (m_hasFix) {
            double latNorm = (m_latitude + 90.0) / 180.0;   // -90..90 -> 0..1
            double lonNorm = (m_longitude + 180.0) / 360.0; // -180..180 -> 0..1

            double x = mapRect.left() + lonNorm * mapRect.width();
            double y = mapRect.bottom() - latNorm * mapRect.height();

            const double r = 7.0;
            const double accuracyRadius = r * 4.0;

            // Cercle de précision (zone d'incertitude)
            p.setBrush(QColor(255, 80, 80, 40));
            p.setPen(Qt::NoPen);
            p.drawEllipse(QPointF(x, y), accuracyRadius, accuracyRadius);

            // Point du client
            p.setBrush(QColor(255, 80, 80));
            p.setPen(Qt::NoPen);
            p.drawEllipse(QPointF(x, y), r, r);

            // Cible autour du point
            p.setPen(QColor(255, 160, 160));
            p.setBrush(Qt::NoBrush);
            p.drawEllipse(QPointF(x, y), r * 2, r * 2);
        } else {
            p.setPen(QColor(255, 200, 120));
            QFont infoFont("Segoe UI", 10, QFont::Normal);
            p.setFont(infoFont);
            p.drawText(mapRect, Qt::AlignCenter,
                       "Aucune coordonnée GPS disponible pour ce client.");
        }

        p.restore();
    }

    // Bordure de la carte (non zoomée)
    p.setPen(QColor(200, 200, 200));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(mapRect, 10, 10);

    // Titre et infos système (non zoomés)
    p.setPen(Qt::white);
    QFont titleFont("Segoe UI", 12, QFont::Bold);
    p.setFont(titleFont);
    QRectF titleRect(mapRect.left() + 10, mapRect.top() - 5, mapRect.width() * 0.6, 24);
    p.drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter, "Carte offline - position approximative");

    QFont sysFont("Segoe UI", 9);
    p.setFont(sysFont);
    QRectF sysRect(mapRect.left() + 10, mapRect.top() + 18, mapRect.width() * 0.6, 18);
    p.drawText(sysRect, Qt::AlignLeft | Qt::AlignVCenter, "Système : WGS84 - Latitude/Longitude");

    // Indicateur de zoom
    QString zoomText = QString("Zoom x%1").arg(m_zoom, 0, 'f', 1);
    QRectF zoomRect(mapRect.right() - 140, mapRect.top() - 5, 130, 24);
    p.drawText(zoomRect, Qt::AlignRight | Qt::AlignVCenter, zoomText);

    // Boussole (Nord)
    QPointF compassCenter(mapRect.left() + 40, mapRect.top() + 50);
    const double compassRadius = 16.0;
    p.setPen(QColor(200, 200, 200));
    p.setBrush(QColor(15, 30, 50, 220));
    p.drawEllipse(compassCenter, compassRadius, compassRadius);
    p.drawLine(QPointF(compassCenter.x(), compassCenter.y() + compassRadius - 4),
               QPointF(compassCenter.x(), compassCenter.y() - compassRadius + 4));
    QFont compassFont("Segoe UI", 8, QFont::Bold);
    p.setFont(compassFont);
    QRectF nRect(compassCenter.x() - 8, compassCenter.y() - compassRadius - 12, 16, 12);
    p.drawText(nRect, Qt::AlignCenter, "N");

    // Boutons de zoom + / -
    const double buttonSize = 26.0;
    const double buttonMargin = 12.0;
    m_zoomInRect = QRectF(mapRect.right() - buttonMargin - buttonSize,
                          mapRect.top() + buttonMargin,
                          buttonSize, buttonSize);
    m_zoomOutRect = QRectF(mapRect.right() - buttonMargin - buttonSize,
                           mapRect.top() + buttonMargin + buttonSize + 6.0,
                           buttonSize, buttonSize);

    QColor btnBg(25, 40, 70, 230);
    QColor btnBorder(200, 200, 200);
    p.setPen(btnBorder);
    p.setBrush(btnBg);
    p.drawRoundedRect(m_zoomInRect, 4, 4);
    p.drawRoundedRect(m_zoomOutRect, 4, 4);

    QFont btnFont("Segoe UI", 11, QFont::Bold);
    p.setFont(btnFont);
    p.setPen(Qt::white);
    p.drawText(m_zoomInRect, Qt::AlignCenter, "+");
    p.drawText(m_zoomOutRect, Qt::AlignCenter, "-");

    // Panneau d'informations en bas (inchangé)
    QRectF infoRect = rect().adjusted(20, height() - 80, -20, -20);
    p.setBrush(QColor(25, 40, 70));
    p.setPen(QColor(200, 200, 200));
    p.drawRoundedRect(infoRect, 8, 8);

    p.setPen(Qt::white);
    QFont infoFont("Segoe UI", 9);
    p.setFont(infoFont);

    QString line1 = QString("Client : %1").arg(m_name.isEmpty() ? "-" : m_name);
    QString line2 = QString("Adresse : %1").arg(m_address.isEmpty() ? "-" : m_address);
    QString line3;
    if (m_hasFix) {
        line3 = QString("Latitude : %1   Longitude : %2")
                    .arg(m_latitude, 0, 'f', 6)
                    .arg(m_longitude, 0, 'f', 6);
    } else {
        line3 = "GPS : aucune coordonnée enregistrée";
    }

    p.drawText(infoRect.adjusted(10, 8, -10, -8), Qt::AlignLeft | Qt::AlignTop, line1);
    p.drawText(infoRect.adjusted(10, 26, -10, -8), Qt::AlignLeft | Qt::AlignTop, line2);
    p.drawText(infoRect.adjusted(10, 44, -10, -8), Qt::AlignLeft | Qt::AlignTop, line3);
}

void ClientMapWidget::applyZoom(bool zoomIn)
{
    const double factor = 1.1; // 10% par cran de molette ou bouton

    if (zoomIn)
        m_zoom *= factor;
    else
        m_zoom /= factor;

    if (m_zoom < 0.5)
        m_zoom = 0.5;
    if (m_zoom > 5.0)
        m_zoom = 5.0;

    update();
}

void ClientMapWidget::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() != 0) {
        applyZoom(event->angleDelta().y() > 0);
        event->accept();
    } else {
        QWidget::wheelEvent(event);
    }
}

void ClientMapWidget::mousePressEvent(QMouseEvent *event)
{
    const QPointF pos = event->pos();

    if (m_zoomInRect.contains(pos)) {
        applyZoom(true);
        event->accept();
        return;
    }

    if (m_zoomOutRect.contains(pos)) {
        applyZoom(false);
        event->accept();
        return;
    }

    // Démarrer le pan si clic gauche dans la zone de carte
    if (event->button() == Qt::LeftButton) {
        QRectF mapRect = rect().adjusted(20, 20, -20, -100);
        if (mapRect.contains(pos)) {
            m_isPanning = true;
            m_lastPanPos = event->pos();
            setCursor(Qt::ClosedHandCursor);
            event->accept();
            return;
        }
    }

    QWidget::mousePressEvent(event);
}

void ClientMapWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isPanning && (event->buttons() & Qt::LeftButton)) {
        QPoint delta = event->pos() - m_lastPanPos;
        m_lastPanPos = event->pos();
        m_panOffset += delta;
        update();
        event->accept();
        return;
    }

    QWidget::mouseMoveEvent(event);
}

void ClientMapWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_isPanning) {
        m_isPanning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }

    QWidget::mouseReleaseEvent(event);
}
