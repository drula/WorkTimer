#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum class State
    {
        Initial,
        Started,
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *_ui;
    State _state;

    void setState(State state);
    void setDefaultValues();

private slots:
    void onStartButton();
};

#endif // MAINWINDOW_H
