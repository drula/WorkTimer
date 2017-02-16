#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimeLine>
#include <QTime>
#include <QPalette>

namespace Ui
{
    class MainWindow;
}

//! temporary values, should be taken from settings
const QTime DefaultTotalTime(0, 5); //! (8, 0);
const QTime DefaultWorkTime(0, 3); //! (6, 0);
const QTime DefaultRestTime(0, 2); //! (2, 0);
const QTime DefaultWorkSessionTime(0, 0, 30); //! (0, 25);
const QTime DefaultRestSessionTime(0, 0, 5); //! (0, 5);

//! temporary values, should be taken from translation files
const QString Start("Start");
const QString Finish("Finish");
const QString Work("Work");
const QString Rest("Rest");
const QString CurrentWorkSessionTime("Current work session time");
const QString CurrentRestSessionTime("Current rest session time");
const QString NextWorkSessionTime("Next work session time");
const QString NextRestSessionTime("Next rest session time");

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum class State {
        Initial,
        Work,
        Rest,
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *_ui;
    State _state;
    QTimeLine _timeLine;

    QPalette _timePalette; // black time text
    QPalette _timeUpPalette; // red time text
    bool _isTimeUp;
    int _secsToAdd;
    void setTimeUp(bool isTimeUp);

// state transition functions
    void toInitial();
    void InitialToWork();
    void InitialToRest();
    void WorkToInitial();
    void WorkToRest();
    void RestToInitial();
    void RestToWork();

private slots:
    void onStartFinishButton();
    void onWorkRestButton();
    void onWorkRadioButton();
    void onRestRadioButton();
    void onTimeLineFrameChanged();
    void onTimeLineFinished();
};

#endif // MAINWINDOW_H
