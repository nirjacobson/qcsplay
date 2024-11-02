#include "player.h"
#include "ui_player.h"

Player::Player(QWidget *parent, Application* app)
    : QMainWindow(parent)
    , ui(new Ui::Player)
    , _app(app)
    , _playlistTableModel(this, _playlist)
    , _currentTrack(-1)
    , _isPlaying(false)
{
    ui->setupUi(this);

    setAcceptDrops(true);

    ui->openButton->setIcon(ui->playButton->style()->standardIcon(QStyle::SP_DirOpenIcon));
    ui->prevButton->setIcon(ui->playButton->style()->standardIcon(QStyle::SP_MediaSkipBackward));
    ui->playButton->setIcon(ui->playButton->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->stopButton->setIcon(ui->playButton->style()->standardIcon(QStyle::SP_MediaStop));
    ui->nextButton->setIcon(ui->playButton->style()->standardIcon(QStyle::SP_MediaSkipForward));

    ui->playlistTableView->setModel(&_playlistTableModel);
    ui->playlistTableView->setColumnWidth(3, 64);
    ui->playlistTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->playlistTableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->playlistTableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->playlistTableView->verticalHeader()->setDefaultSectionSize(16);
    ui->playlistTableView->setDragEnabled(true);

    connect(ui->actionOpenFiles, &QAction::triggered, this, &Player::openFiles);
    connect(ui->actionOpenFolder, &QAction::triggered, this, &Player::openFolder);
    connect(ui->actionAddFiles, &QAction::triggered, this, &Player::addFiles);
    connect(ui->actionAddFolder, &QAction::triggered, this, &Player::addFolder);

    connect(ui->actionSettings, &QAction::triggered, this, &Player::settingsTriggered);

    connect(ui->actionClose, &QAction::triggered, this, &Player::close);

    connect(ui->openButton, &QToolButton::clicked, this, &Player::openFiles);
    connect(ui->addButton, &QToolButton::clicked, this, &Player::addFiles);

    connect(ui->prevButton, &QToolButton::clicked, this, &Player::prev);
    connect(ui->playButton, &QToolButton::clicked, this, &Player::playPause);
    connect(ui->stopButton, &QToolButton::clicked, this, &Player::stop);
    connect(ui->nextButton, &QToolButton::clicked, this, &Player::next);

    connect(ui->playlistTableView, &QTableView::doubleClicked, this, &Player::itemDoubleClicked);

    connect(&_app->chromasound(), &Chromasound::stopped, this, &Player::chromasoundStopped);

    _timer.setInterval(1000 / 30);
    connect(&_timer, &QTimer::timeout, this, &Player::frame);

    ui->menubar->setNativeMenuBar(false);
}

Player::~Player()
{
    delete ui;
}

PlaylistItem Player::getVGMPlaylistItem(const QString& path)
{
    GD3 gd3 = GD3::parseGd3(path);

    QFile file(path);
    file.open(QIODevice::ReadOnly);

    QDataStream in(&file);
    in.setByteOrder(QDataStream::LittleEndian);
    in.skipRawData(0x18);
    quint32 totalSamples;
    in >> totalSamples;
    file.close();

    return PlaylistItem(path, gd3.title(), gd3.author(), gd3.game(), totalSamples / 44100);
}

PlaylistItem Player::getPCMPlaylistItem(const QString& path)
{
    QFile file(path);
    QFileInfo fileInfo(file);

    return PlaylistItem(path, fileInfo.fileName(), "", "", fileInfo.size() / 44100);
}

void Player::openFiles_(const QStringList& files, const bool clear)
{
    if (clear) {
        _playlistTableModel.clear();
    }

    for (const QString& file : files) {
        QString suffix = QFileInfo(file).suffix().toLower();

        if (suffix == "vgm") {
            _playlistTableModel.insertRow(getVGMPlaylistItem(file));
        } else if (suffix == "pcm") {
            _playlistTableModel.insertRow(getPCMPlaylistItem(file));
        }
    }
}

void Player::play(const int index)
{
    QString path = _playlist[index].path();
    QFile file(path);
    QFileInfo fileInfo(file);

    if (fileInfo.suffix().toLower() == "vgm") {
        file.open(QIODevice::ReadOnly);
        QByteArray vgm = file.readAll();
        file.close();

        _isPlaying = true;
        _currentTrack = index;
        ui->playlistTableView->selectionModel()->setCurrentIndex(_playlistTableModel.index(index, 0), QItemSelectionModel::ClearAndSelect);
        _app->chromasound().play(vgm, 0, 0);
    } else if (fileInfo.suffix().toLower() == "pcm") {
        QByteArray vgm = pcmToVgm(path);

        _isPlaying = true;
        _currentTrack = index;
        ui->playlistTableView->selectionModel()->setCurrentIndex(_playlistTableModel.index(index, 0), QItemSelectionModel::ClearAndSelect);
        _app->chromasound().play(vgm, 0, 0);
    }
    ui->playButton->setIcon(ui->playButton->style()->standardIcon(QStyle::SP_MediaPause));
    _timer.start();

}

void Player::playPause()
{
    if (_isPlaying) {
        _isPaused = true;
        _isPlaying = false;
        _app->chromasound().pause();
        _timer.stop();
    } else {
        if (_isPaused) {
            _isPaused = false;
            _isPlaying = true;
            _app->chromasound().play();
            _timer.start();
        } else {
            _isPlaying = true;
            play(0);
        }
    }

    ui->playButton->setIcon(ui->playButton->style()->standardIcon(_isPlaying ? QStyle::SP_MediaPause : QStyle::SP_MediaPlay));
}

void Player::stop()
{
    _isPlaying = false;
    _isPaused = false;
    _currentTrack = 0;
    _app->chromasound().stop();
    ui->playButton->setIcon(ui->playButton->style()->standardIcon(QStyle::SP_MediaPlay));
    _timer.stop();
    frame();
}

void Player::prev()
{
    if (_currentTrack > 0) {
        play(--_currentTrack);
    }
}

void Player::next()
{
    if (_currentTrack < _playlist.size() - 1) {
        play(++_currentTrack);
    }
}

void Player::openFiles()
{
    QStringList paths = QFileDialog::getOpenFileNames(this, tr("Open files"), "", "VGM files (*.vgm);;PCM files (*.pcm)", nullptr, QFileDialog::DontUseNativeDialog);

    if (!paths.isEmpty()) {
        openFiles_(paths);
    }
}

void Player::openFolder()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Open folder"), "", QFileDialog::DontUseNativeDialog | QFileDialog::ShowDirsOnly);

    if (!path.isNull()) {
        QFileInfoList infoList = QDir(path).entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
        QStringList paths;
        for (const QFileInfo& fileInfo : infoList) {
            paths.append(fileInfo.absoluteFilePath());
        }
        openFiles_(paths);
    }
}

void Player::addFiles()
{
    QStringList paths = QFileDialog::getOpenFileNames(this, tr("Add files"), "", "VGM files (*.vgm);;PCM files (*.pcm)", nullptr, QFileDialog::DontUseNativeDialog);

    if (!paths.isEmpty()) {
        openFiles_(paths, false);
    }
}

void Player::addFolder()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Add folder"), "", QFileDialog::DontUseNativeDialog | QFileDialog::ShowDirsOnly);

    if (!path.isNull()) {
        QFileInfoList infoList = QDir(path).entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
        QStringList paths;
        for (const QFileInfo& fileInfo : infoList) {
            paths.append(fileInfo.absoluteFilePath());
        }
        openFiles_(paths, false);
    }
}

void Player::itemDoubleClicked(const QModelIndex& index)
{
    play(index.row());
    ui->playlistTableView->selectRow(index.row());
}

void Player::frame()
{
    if (!_playlist.isEmpty()) {
        quint32 totalLength = _playlist[_currentTrack].length() * 44100;
        quint32 pos = _app->chromasound().position();
        float percentage = (float)pos / (float)totalLength;

        QString posString = QString("%1:%2/%3:%4")
                            .arg((pos / 44100) / 60)
                            .arg((pos / 44100) % 60, 2, 10, QLatin1Char('0'))
                            .arg((totalLength / 44100) / 60)
                            .arg((totalLength / 44100) % 60, 2, 10, QLatin1Char('0'));

        ui->seekSlider->setSliderPosition(percentage * ui->seekSlider->maximum());
        ui->positionLabel->setText(posString);
    }
}

void Player::chromasoundStopped()
{
    _isPlaying = false;
    ui->playButton->setIcon(ui->playButton->style()->standardIcon(QStyle::SP_MediaPlay));
    _timer.stop();
    frame();
}

void Player::settingsTriggered()
{
    _settingsDialog = new SettingsDialog(this);

    connect(_settingsDialog, &SettingsDialog::done, this, [&]() {
        _app->setupChromasound();
    });

    _settingsDialog->show();
}

QByteArray Player::pcmToVgm(const QString& path)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);

    QByteArray pcm = file.readAll();

    for (int i = 1; i < pcm.size(); i++) {
        pcm.removeAt(i);
    }

    QByteArray vgm;

    QByteArray enableDac;
    enableDac.append(0x52);
    enableDac.append(0x2B);
    enableDac.append(0x80);

    vgm.append(enableDac);

    quint32 pcmSize = pcm.size();

    vgm.append(0x97);
    vgm.append(0xFE);
    vgm.append((char*)&pcmSize, sizeof(pcmSize));
    vgm.append(pcm);

    vgm.append(0x66);

    QByteArray headerData = VGM::generateHeader(vgm, pcmSize * 2, 0, 0, false);
    vgm.prepend(headerData);

    file.close();

    return vgm;
}

void Player::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("text/uri-list")) {
        event->acceptProposedAction();
    }
}

void Player::dropEvent(QDropEvent* event)
{
    QByteArray data = event->mimeData()->data("text/uri-list");
    QString pathsString(data);
    QStringList paths = pathsString.split("\r\n");
    paths.removeDuplicates();
    paths.removeAll("");

    for (QString& str : paths) {
        str = str.mid(QString("file://").length());
        str = str.replace("%20", " ");
    }

    for (auto it = paths.begin(); it != paths.end();) {
        QString path = *it;
        if (QFileInfo(QFile(path)).isDir()) {
            QFileInfoList infoList = QDir(path).entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
            auto before = it;
            for (const QFileInfo& fileInfo : infoList) {
                QString newPath = fileInfo.absoluteFilePath();
                newPath = newPath.mid(QString("file://").length());
                newPath = newPath.replace("%20", " ");
                it = paths.insert(++it, fileInfo.absoluteFilePath());
            }
            paths.removeAll(path);
            it = paths.begin();
        } else {
            ++it;
        }
    }

    openFiles_(paths, false);
}
