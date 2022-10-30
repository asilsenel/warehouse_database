#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAddNew_clicked();

    void on_radioButtonTR_clicked();

    void on_radioButtonEN_clicked();
    void cleanLineEdits();

    void on_btnAdd_clicked();

    void on_btnSearch_clicked();

    void on_btnDimin_clicked();

    void on_btnChange_clicked();

    void on_btnDelete_clicked();

    void on_btnClear_clicked();

    void on_btnSearchProject_clicked();


    void on_btnSearchProject_2_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
