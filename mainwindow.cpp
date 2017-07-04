#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "donewdialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(600,500);
    area = new PaintArea;
    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(area);
    scrollArea->widget()->setMinimumSize(800,600);
    setCentralWidget(scrollArea);
    M_isSaved = false;
    M_curFile = tr("未命名.png");
    creatColorComboBox(ui->penColorComboBox);
    creatColorComboBox(ui->brushColorComboBox);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch(e->type())
    {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
        default:
            break;
    }
}

void MainWindow::M_New()
{
    if(M_maybeSave())
    {
        DoNewDialog dlg;
        if(dlg.exec() == QDialog::Accepted)
        {
            int width = dlg.getWidth();
            int hetght = dlg.getHeight();
            area->setImageSize(width,hetght);
            scrollArea->widget()->resize(width,hetght);
            area->setImageColor(dlg.getBackColor());
            M_isSaved = false;
        }

    }
}

void MainWindow::M_Open()
{
    if(M_maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this,
                           tr("打开文件"), QDir::currentPath());
        if(!fileName.isEmpty())
        {
            area->openImage(fileName);
            scrollArea->widget()->resize(area->getImageSize());
            M_isSaved = true;
            M_curFile = fileName;
        }
    }
}
bool MainWindow::M_maybeSave()
{
    if(area->isModified())
    {
        QMessageBox::StandardButton box;
        box = QMessageBox::warning(this,tr("保存文件"),tr("图片已经改变，是否保存？"),
                                   QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(box == QMessageBox::Yes)
        {
            return M_FileSave();
        }
        else if(box == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}

bool MainWindow::M_FileSave()
{
    if(M_isSaved)
    {
        return M_saveFile(M_curFile);
    }
    else
        return M_FileSaveAs();
}

bool MainWindow::M_saveFile(QString FileName)
{
    if(area->saveImage(FileName,"png"))
    {
        M_isSaved = true;
        return true;
    }
    else
        return false;
}

bool MainWindow::M_FileSaveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("另存为"),M_curFile);
    if(fileName.isEmpty())
    {
        return false;
    }
    else
    {
        return M_saveFile(fileName);
    }
}

void MainWindow::M_Copyright()
{
    QMessageBox::information(NULL,"Copyright","Copyright @ wzx 2011<br>       0.8.9(138)",QMessageBox::Yes);
}

void MainWindow::on_action_triggered()
{
    M_New();
}

void MainWindow::on_action_O_triggered()
{
    M_Open();
}

void MainWindow::on_action_S_triggered()
{
    M_FileSave();
}

void MainWindow::on_action_A_triggered()
{
    M_FileSaveAs();
}

void MainWindow::on_action_X_triggered()
{
    if(M_maybeSave())
        qApp->quit();
}

void MainWindow::cloesEvent(QCloseEvent *event)
{
    if(M_maybeSave())
        qApp->quit();
    else event->ignore();
}

void MainWindow::on_action_5_triggered()
{
    area->M_zoomIn();
    scrollArea->widget()->resize(area->getImageSize());
}

void MainWindow::on_action_6_triggered()
{
    area->M_zoomOut();
    scrollArea->widget()->resize(area->getImageSize());
}

void MainWindow::on_action_7_triggered()
{
    area->M_zoom_1();
}


void MainWindow::on_action_10_triggered()
{
    area->M_Shear1();
}

void MainWindow::on_action_4_triggered()
{
    area->M_Shear2();
}

void MainWindow::on_action_13_triggered()
{
    area->M_Clear();
}

void MainWindow::on_action_2_triggered()
{
    area->M_CRotate();
}

void MainWindow::on_action_3_triggered()
{
    area->M_ARotate();
}

void MainWindow::on_action_14_triggered()
{
    ui->dockWidget->show();
}

void MainWindow::on_action_15_triggered()
{
   M_Copyright();
}

void MainWindow::creatColorComboBox(QComboBox *comboBox)
{
    QPixmap pix(16,16);
    QPainter painter(&pix);
    painter.fillRect(0,0,16,16,Qt::red);
    comboBox->addItem(QIcon(pix),tr("红色"),Qt::red);
    painter.fillRect(0,0,16,16,Qt::green);
    comboBox->addItem(QIcon(pix),tr("绿色"),Qt::green);
    painter.fillRect(0,0,16,16,Qt::blue);
    comboBox->addItem(QIcon(pix),tr("蓝色"),Qt::blue);
    painter.fillRect(0,0,16,16,Qt::yellow);
    comboBox->addItem(QIcon(pix),tr("黄色"),Qt::yellow);
    painter.fillRect(0,0,16,16,Qt::white);
    comboBox->addItem(QIcon(pix),tr("白色"),Qt::white);
    painter.fillRect(0,0,16,16,Qt::black);
    comboBox->addItem(QIcon(pix),tr("黑色"),Qt::black);
    comboBox->addItem(QIcon(),tr("无色"),Qt::transparent);
}

void MainWindow::on_comboBox_currentIndexChanged(const QString shape)
{
    if(shape == tr("无"))
        area->M_setShape(PaintArea::None);
    else if(shape == tr("直线"))
        area->M_setShape(PaintArea::Line);
    else if(shape == tr("矩形"))
        area->M_setShape(PaintArea::Rectangle);
    else if(shape == tr("椭圆"))
        area->M_setShape(PaintArea::Ellipse);
}

void MainWindow::on_comboBox_2_currentIndexChanged(const QString style)
{
    if(style == tr("实线"))
    {
        area->M_setPenStyle(Qt::SolidLine);
    }
    else if(style == tr("点线"))
    {
        area->M_setPenStyle(Qt::DotLine);
    }
}

void MainWindow::on_spinBox_valueChanged(int width)
{
    area->M_setPenWidth(width);
}

void MainWindow::on_penColorComboBox_currentIndexChanged(int index)
{
    QColor color = ui->penColorComboBox->itemData(index,Qt::UserRole).value<QColor>();
    area->M_setPenColor(color);
}


void MainWindow::on_brushColorComboBox_currentIndexChanged(int index)
{
    QColor color = ui->brushColorComboBox->itemData(index,Qt::UserRole).value<QColor>();
    area->M_setBrushColor(color);
}
