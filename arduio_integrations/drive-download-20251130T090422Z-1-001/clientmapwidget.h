#ifndef CLIENTMAPWIDGET_H
#define CLIENTMAPWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QRectF>
#include <QPointF>
#include <QPoint>

class QWheelEvent;
class QMouseEvent;

class ClientMapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientMapWidget(QWidget *parent = nullptr);

    void setClientInfo(const QString &name,
                       const QString &address,
                       double latitude,
                       double longitude,
                       bool hasFix);

    // Optionnel : définir une image de fond personnalisée pour la carte
    void setBackgroundImage(const QString &filePath);

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QString m_name;
    QString m_address;
    double m_latitude;
    double m_longitude;
    bool m_hasFix;

    QPixmap m_background;
    bool m_hasBackground;

    double m_zoom;  // facteur de zoom (1.0 = normal)
    QRectF m_zoomInRect;
    QRectF m_zoomOutRect;

    QPointF m_panOffset;   // décalage de la carte (pan)
    bool m_isPanning = false;
    QPoint m_lastPanPos;

    void applyZoom(bool zoomIn);
};

#endif // CLIENTMAPWIDGET_H
