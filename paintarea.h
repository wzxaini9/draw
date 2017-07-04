#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
class PaintArea : public QWidget
{
public:
    PaintArea();
    void setImageSize(int width,int height);
    void setImageColor(QColor color);
    bool isModified() const {return modified;}
    bool saveImage(const QString &fileName,const char*fileFormat);
    bool openImage(const QString &fileName);
    QSize getImageSize();
    void M_print();
    void M_zoomIn();
    void M_zoomOut();
    void M_zoom_1();
    void M_Clear();
    void M_Shear1();
    void M_Shear2();
    void M_CRotate();
    void M_ARotate();
    void M_setPenStyle(Qt::PenStyle style);
    void M_setPenWidth(int width);
    void M_setPenColor(QColor color);
    void M_setBrushColor(QColor color);
    enum ShapeType
    {
        None,
        Line,
        Rectangle,
        Ellipse
    };
    void M_setShape(ShapeType shape);
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paint(QImage& theImage);
private:
    QImage image;
    QRgb backColor;
    QPoint lastPoint,endPoint;
    bool modified;
    qreal scale;
    qreal shear;
    int angle;
    QColor penColor;
    QColor brushColor;
    int penWidth;
    Qt::PenStyle penStyle;
    ShapeType curShape;
    QImage tempImage;
    bool isDrawing;
};

#endif // PAINTAREA_H
