#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSound>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

    _ui->setupUi(this);
    _timeLine.setCurveShape(QTimeLine::LinearCurve);

    _timePalette = _ui->currentSessionTimeEdit->palette();
    _timeUpPalette = _timePalette;
    _timeUpPalette.setColor(QPalette::Text, Qt::red);

    bool connected = false;

    connected = connect(_ui->startFinishButton, &QPushButton::clicked, this, &MainWindow::onStartFinishButton);
    Q_ASSERT(connected);

    connected = connect(&_timeLine, &QTimeLine::frameChanged, this, &MainWindow::onTimeLineFrameChanged);
    Q_ASSERT(connected);

    connected = connect(&_timeLine, &QTimeLine::finished, this, &MainWindow::onTimeLineFinished);
    Q_ASSERT(connected);

    connected = connect(_ui->workRadioButton, &QRadioButton::clicked, this, &MainWindow::onWorkRadioButton);
    Q_ASSERT(connected);

    connected = connect(_ui->restRadioButton, &QRadioButton::clicked, this, &MainWindow::onRestRadioButton);
    Q_ASSERT(connected);

    connected = connect(_ui->workRestButton, &QPushButton::clicked, this, &MainWindow::onWorkRestButton);
    Q_ASSERT(connected);

    toInitial();
}

void MainWindow::toInitial()
{
    _ui->totalTimeEdit->setEnabled(true);
    _ui->totalTimeEdit->setTime(DefaultTotalTime);

    _ui->workTimeEdit->setEnabled(true);
    _ui->workTimeEdit->setTime(DefaultWorkTime);

    _ui->restTimeEdit->setEnabled(true);
    _ui->restTimeEdit->setTime(DefaultRestTime);

    _ui->startFinishButton->setText(Start);
    _ui->startFinishButton->setEnabled(true);

    _ui->workRadioButton->setEnabled(true);
    _ui->restRadioButton->setEnabled(true);
    _ui->workRadioButton->setChecked(true);
    onWorkRadioButton();

    _ui->currentSessionTimeLabel->clear();
    _ui->currentSessionTimeEdit->setEnabled(false);
    _ui->currentSessionTimeEdit->clear();

    _ui->nextSessionTimeEdit->setEnabled(true);

    _ui->workRestButton->setEnabled(false);
    _ui->workRestButton->setText(Work);

    setCurrentTimeUp(false);
    setWorkTimeUp(false);
    setRestTimeUp(false);

    _state = State::Initial;
}

void MainWindow::InitialToWork()
{
    _ui->startFinishButton->setText(Finish);
    _ui->workRadioButton->setEnabled(false);
    _ui->restRadioButton->setEnabled(false);

    _ui->currentSessionTimeLabel->setText(CurrentWorkSessionTime);
    _ui->currentSessionTimeEdit->setTime(_ui->nextSessionTimeEdit->time());
    _ui->currentSessionTimeEdit->setEnabled(true);
    _ui->currentSessionTimeEdit->setReadOnly(true);

    _ui->nextSessionLabel->setText(NextRestSessionTime);
    _ui->nextSessionTimeEdit->setTime(DefaultRestSessionTime);

    _ui->workRestButton->setText(Rest);
    _ui->workRestButton->setEnabled(true);

    int secs = - _ui->workTimeEdit->time().secsTo(QTime(0, 0));
    _timeLine.setDuration(secs * 1000);
    _timeLine.setFrameRange(0, secs);
    _timeLine.start();

    _state = State::Work;
}

void MainWindow::InitialToRest()
{
    _ui->startFinishButton->setText(Finish);
    _ui->workRadioButton->setEnabled(false);
    _ui->restRadioButton->setEnabled(false);

    _ui->currentSessionTimeLabel->setText(CurrentRestSessionTime);
    _ui->currentSessionTimeEdit->setTime(_ui->nextSessionTimeEdit->time());
    _ui->currentSessionTimeEdit->setEnabled(true);
    _ui->currentSessionTimeEdit->setReadOnly(true);

    _ui->nextSessionLabel->setText(NextWorkSessionTime);
    _ui->nextSessionTimeEdit->setTime(DefaultWorkSessionTime);

    _ui->workRestButton->setText(Work);
    _ui->workRestButton->setEnabled(true);

    int secs = - _ui->workTimeEdit->time().secsTo(QTime(0, 0));
    _timeLine.setDuration(secs * 1000);
    _timeLine.setFrameRange(0, secs);
    _timeLine.start();

    _state = State::Rest;
}

void MainWindow::WorkToInitial()
{
    _timeLine.stop();

    _ui->startFinishButton->setText(Start);
    _ui->workRadioButton->setEnabled(true);
    _ui->restRadioButton->setEnabled(true);

    setCurrentTimeUp(false);
    setWorkTimeUp(false);
    setRestTimeUp(false);

    _state = State::Initial;
}

void MainWindow::WorkToRest()
{
    _ui->currentSessionTimeLabel->setText(CurrentRestSessionTime);
    _ui->currentSessionTimeEdit->setTime(_ui->nextSessionTimeEdit->time());

    setCurrentTimeUp(false);

    _ui->nextSessionLabel->setText(NextWorkSessionTime);
    _ui->nextSessionTimeEdit->setTime(DefaultWorkSessionTime);

    _ui->workRestButton->setText(Work);

    _state = State::Rest;
}

void MainWindow::RestToInitial()
{
    _timeLine.stop();

    _ui->startFinishButton->setText(Start);
    _ui->workRadioButton->setEnabled(true);
    _ui->restRadioButton->setEnabled(true);

    setCurrentTimeUp(false);
    setWorkTimeUp(false);
    setRestTimeUp(false);

    _state = State::Initial;
}

void MainWindow::RestToWork()
{
    _ui->currentSessionTimeLabel->setText(CurrentWorkSessionTime);
    _ui->currentSessionTimeEdit->setTime(_ui->nextSessionTimeEdit->time());

    setCurrentTimeUp(false);

    _ui->nextSessionLabel->setText(NextRestSessionTime);
    _ui->nextSessionTimeEdit->setTime(DefaultRestSessionTime);

    _ui->workRestButton->setText(Rest);

    _state = State::Work;
}

void MainWindow::onStartFinishButton()
{
    switch (_state) {
    case State::Initial:
        if (_ui->workRadioButton->isChecked())
            InitialToWork();
        else if (_ui->restRadioButton->isChecked())
            InitialToRest();

        break;

    case State::Work:
        WorkToInitial();
        break;

    case State::Rest:
        RestToInitial();
        break;
    }
}

void MainWindow::onWorkRestButton()
{
    switch (_state) {
    case State::Work: WorkToRest(); break;
    case State::Rest: RestToWork(); break;
    default: break;
    }
}

void MainWindow::onWorkRadioButton()
{
    _ui->nextSessionLabel->setText(NextWorkSessionTime);
    _ui->nextSessionTimeEdit->setTime(DefaultWorkSessionTime);
}

void MainWindow::onRestRadioButton()
{
    _ui->nextSessionLabel->setText(NextRestSessionTime);
    _ui->nextSessionTimeEdit->setTime(DefaultRestSessionTime);
}

void MainWindow::onTimeLineFrameChanged()
{
    QTime currentSessionTime = _ui->currentSessionTimeEdit->time();
    if (currentSessionTime == QTime(0, 0)) {
        QSound::play(SoundRc);
        setCurrentTimeUp(true);
    }

    _ui->currentSessionTimeEdit->setTime(currentSessionTime.addSecs(_secsToAddToCurrentTime));

    switch (_state) {
    case State::Work: {
        QTime workTime = _ui->workTimeEdit->time();
        if (workTime == QTime(0, 0))
            setWorkTimeUp(true);

        _ui->workTimeEdit->setTime(workTime.addSecs(_secsToAddToWorkTime));
    }

        break;

    case State::Rest: {
        QTime restTime = _ui->restTimeEdit->time();
        if (restTime == QTime(0, 0))
            setRestTimeUp(true);

        _ui->restTimeEdit->setTime(restTime.addSecs(_secsToAddToRestTime));

    }
        break;

    default:
        break;
    }

    _ui->totalTimeEdit->setTime(_ui->totalTimeEdit->time().addSecs(-1));
}

void MainWindow::onTimeLineFinished()
{
    switch (_state) {
    case State::Work: WorkToInitial(); break;
    case State::Rest: RestToInitial(); break;
    default: break;
    }
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::setCurrentTimeUp(bool isTimeUp)
{
    _ui->currentSessionTimeEdit->setPalette(isTimeUp ? _timeUpPalette : _timePalette);
    _secsToAddToCurrentTime = isTimeUp ? 1 : -1;
}

void MainWindow::setWorkTimeUp(bool isTimeUp)
{
    _ui->workTimeEdit->setPalette(isTimeUp ? _timeUpPalette : _timePalette);
    _secsToAddToWorkTime = isTimeUp ? 1 : -1;
}

void MainWindow::setRestTimeUp(bool isTimeUp)
{
    _ui->restTimeEdit->setPalette(isTimeUp ? _timeUpPalette : _timePalette);
    _secsToAddToRestTime = isTimeUp ? 1 : -1;
}
