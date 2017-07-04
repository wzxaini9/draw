#include "paintarea.h"
#include <QPainter>
#include <QPrintDialog>
#include <QPrinter>
PaintArea::PaintArea()
{
    image = QImage(640,480,QImage::Format_RGB32);
    backColor = qRgb(255,255,255);
    image.fill(backColor);
    modified = false;
    scale = 1;
    shear = 0;
    angle = 0;
    penColor = Qt::black;
    brushColor = Qt::black;
    penWidth = 1;
    penStyle = Qt::SolidLine;
    curShape = None;
    isDrawing = false;
}

void PaintArea::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    painter.scale(scale,scale);
    if(isDrawing)
    {
        painter.drawImage(0,0,tempImage);
    }
    else
    {
    if(angle)
    {
        QImage copyImage = image;
        QPainter pp(&copyImage);
        QPointF center(copyImage.width()/2.0,copyImage.height()/2.0);
        painter.translate(center);
        painter.rotate(angle);
        painter.translate(-center);
        pp.drawImage(0,0,image);
        image = copyImage;
        angle = 0;
    }
    if(shear)
    {
        QImage copyImage = image;
        QPainter pp(&copyImage);
        pp.shear(shear,shear);
        pp.drawImage(0,0,image);
        image = copyImage;
        shear = 0;
    }
    painter.drawImage(0,0,image);
    }
}

void PaintArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        lastPoint = event->pos();
        isDrawing = true;
    }
}

void PaintArea::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()&Qt::LeftButton)
    {
        endPoint = event->pos();
        if(curShape == None)
        {
            isDrawing = false;
            paint(image);
        }
        else
        {
            tempImage = image;
            paint(tempImage);
        }
    }
}

void PaintArea::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        endPoint = event->pos();
        isDrawing = false;
        paint(image);
    }
}

void PaintArea::paint(QImage &theImage)
{
    QPainter pp(&theImage);
    QPen pen = QPen();
    pen.setColor(penColor);
    pen.setStyle(penStyle);
    pen.setWidth(penWidth);
    QBrush brush = QBrush(brushColor);
    pp.setPen(pen);
    pp.setBrush(brush);
    int x,y,w,h;
    x = lastPoint.x()/scale;
    y = lastPoint.y()/scale;
    w = endPoint.x()/scale - x;
    h = endPoint.y()/scale - y;
    switch(curShape)
    {
    case None:
        {
            pp.drawLine(lastPoint/scale,endPoint/scale);
            lastPoint = endPoint;
            break;
        }
    case Line:
        {
            pp.drawLine(lastPoint/scale,endPoint/scale);
            break;
        }
    case Rectangle:
        {
            pp.drawRect(x,y,w,h);
            break;
        }
    case Ellipse:
        {
            pp.drawEllipse(x,y,w,h);
            break;
        }
    }
    update();
    modified = true;
}

void PaintArea::setImageSize(int width, int height)
{
    QImage newImage(width,height,QImage::Format_RGB32);
    image = newImage;
    update();
}

void PaintArea::setImageColor(QColor color)
{
    backColor = color.rgb();
    image.fill(backColor);
    update();
}

bool PaintArea::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = image;
    if(visibleImage.save(fileName,fileFormat))
    {
        modified = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool PaintArea::openImage(const QString &fileName)
{
    QImage loadedImage;
    if(!loadedImage.load(fileName))
    {
        return false;
    }
    QSize newSize = loadedImage.size();
    setImageSize(newSize.width(),newSize.height());
    image =loadedImage;
    modified = false;
    update();
    return true;
}

QSize PaintArea::getImageSize()
{
    return image.size()*scale;
}

void PaintArea::M_print()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *printDialog = new QPrintDialog;
    if(printDialog->exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(),Qt::KeepAspectRatio);
        painter.setViewport(rect.x(),rect.y(),size.width(),size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0,0,image);
    }
}

void PaintArea::M_zoomIn()
{
    scale*= 1.1;
    update();
}

void PaintArea::M_zoomOut()
{
    scale/= 1.1;
    update();
}

void PaintArea::M_zoom_1()
{
    scale = 1;
    shear = 0;
    update();
}

void PaintArea::M_Shear1()
{
    shear += 0.1;
    update();
}

void PaintArea::M_Shear2()
{
    shear -= 0.1;
    update();
}

void PaintArea::M_Clear()
{
    image.fill(backColor);
    update();
}

void PaintArea::M_CRotate()
{
    angle+=10;
    update();
}

void PaintArea::M_ARotate()
{
    angle-=10;
    update();
}

void PaintArea::M_setPenStyle(Qt::PenStyle style)
{
    penStyle = style;
}

void PaintArea::M_setPenWidth(int width)
{
       penWidth = width;
}

void PaintArea::M_setPenColor(QColor color)
{
    penColor = color;
}

void PaintArea::M_setBrushColor(QColor color)
{
    brushColor = color;
}

void PaintArea::M_setShape(ShapeType shape)
{
    curShape = shape;
}
//Function: 按水平轴或垂直轴作镜像翻转。

//Parameters:

// bIsHorizon: 按水平轴翻转为true;反之则为false;

//void ImageViewer::filpImage(bool bIsHorizon)

//{

//image = image.mirrored(bIsHorizon,!bIsHorizon);

// imageLabel->setPixmap(QPixmap::fromImage(image));

//}

