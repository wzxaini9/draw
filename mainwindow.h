#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "paintarea.h"
#include <QScrollArea>
#include <QComboBox>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void M_New();
    void M_Open();
    bool M_FileSave();
    bool M_FileSaveAs();
    void M_Copyright();
    void creatColorComboBox(QComboBox *comboBox);
protected:
    void changeEvent(QEvent *e);
    void cloesEvent(QCloseEvent *);
private:
    Ui::MainWindow *ui;
    PaintArea *area;
    QScrollArea *scrollArea;
    bool M_isSaved;
    QString M_curFile;
    bool M_maybeSave();
    bool M_saveFile(QString FileName);
private slots:
    void on_action_triggered();

    void on_action_O_triggered();

    void on_action_S_triggered();

    void on_action_A_triggered();

    void on_action_X_triggered();

    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_action_4_triggered();

    void on_action_5_triggered();

    void on_action_6_triggered();

    void on_action_7_triggered();

    void on_action_10_triggered();

    void on_action_13_triggered();

    void on_action_14_triggered();

    void on_action_15_triggered();

    void on_comboBox_currentIndexChanged(const QString shape);

    void on_comboBox_2_currentIndexChanged(const QString style);

    void on_spinBox_valueChanged(int width);

    void on_penColorComboBox_currentIndexChanged(int index);

    void on_brushColorComboBox_currentIndexChanged(int index);
};

#endif // MAINWINDOW_H
