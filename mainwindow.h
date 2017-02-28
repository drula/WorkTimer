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

// resources
const QString IconRc(":/data/images/sitting-rabbit.png");
const QString SoundRc(":/data/sounds/shipsbell.wav");
const QString TranslationRc(":/data/translations/mainwindow_by.qm");

//! temporary values, should be taken from settings
//! class values?
const QTime DefaultTotalTime(0, 5); //! (8, 0);
const QTime DefaultWorkTime(0, 3); //! (6, 0);
const QTime DefaultRestTime(0, 2); //! (2, 0);
const QTime DefaultWorkSessionTime(0, 0, 30); //! (0, 25);
const QTime DefaultRestSessionTime(0, 0, 5); //! (0, 5);


class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum class State {
        Initial,
        Work,
        Rest,
    };

    //! static?
    const QString Start = tr("Start");
    const QString Finish = tr("Finish");
    const QString Work = tr("Work");
    const QString Rest = tr("Rest");
    const QString CurrentWorkSessionTime = tr("Current work session time");
    const QString CurrentRestSessionTime = tr("Current rest session time");
    const QString NextWorkSessionTime = tr("Next work session time");
    const QString NextRestSessionTime = tr("Next rest session time");

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *_ui;
    State _state;
    QTimeLine _timeLine;

    QPalette _timePalette; // black time text
    QPalette _timeUpPalette; // red time text

    int _secsToAddToCurrentTime;
    void setCurrentTimeUp(bool isTimeUp);

    int _secsToAddToWorkTime;
    void setWorkTimeUp(bool isTimeUp);

    int _secsToAddToRestTime;
    void setRestTimeUp(bool isTimeUp);

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
