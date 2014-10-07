#include "qwmain.h"
#include "ui_qwmain.h"
#include <QDateTime>

QWMain::QWMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QWMain)
{
    ui->setupUi(this);
    sFilename.clear();
    lastUpdatedTimeStamp = 0;
    connect(&pRunnable, SIGNAL(readyReadStandardOutput()), this, SLOT(sigStdOutReady()));
    connect(&pRunnable, SIGNAL(readyReadStandardError()), this, SLOT(sigStdErrReady()));
	connect(&pRunnable, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(sigProcessFinished(int, QProcess::ExitStatus)));
    connect(&watcher, SIGNAL(fileChanged(QString)),this, SLOT(sigWatchedFileChanged(QString)));
    loadSettings();
	statusBar()->showMessage(tr("Ready"));
}

QWMain::~QWMain()
{
    writeSettings();
    delete ui;
}

void QWMain::on_pbLoad_clicked()
{
    QString loadedFilename = QFileDialog::getOpenFileName(this, tr("Select File"),
                                                                       sDefaultPath,
                                                    tr("Assembler listing (*.asm)"));
    if (!loadedFilename.isNull())
    {
        //update MRU path only if file is selected.
        sDefaultPath= QFileInfo(loadedFilename).absoluteDir().absolutePath();
        ui->pbAssemble->setEnabled(true);
        ui->taConsole->clear();
        ui->taConsole->setFontWeight(63);
        ui->taConsole->append("Selected file ");
        ui->taConsole->insertPlainText(loadedFilename);
        ui->cbDebug->setEnabled(true);
        if (!sFilename.isEmpty())
        {
            watcher.removePath(sFilename);
        }
        watcher.addPath(loadedFilename);
        sFilename = loadedFilename;
    }

}

void QWMain::loadSettings(){
    QSettings appSettings("Libra Project","lasm-GUI");
    appSettings.beginGroup("UI/Window");
    resize(appSettings.value("size", QSize(528, 315)).toSize());
    move(appSettings.value("pos", QPoint(200, 200)).toPoint());
    appSettings.endGroup();
    appSettings.beginGroup("Persistent");
    sDefaultPath = appSettings.value("MRUPath", QDir::homePath()).toString();
    appSettings.endGroup();
}


void QWMain::writeSettings(){
    QSettings appSettings("Libra Project","lasm-GUI");
    appSettings.beginGroup("UI/Window");
    appSettings.setValue("size", size());
    appSettings.setValue("pos", pos());
    appSettings.endGroup();

    appSettings.beginGroup("Persistent");
    appSettings.setValue("MRUPath", sDefaultPath);
    appSettings.endGroup();



}


void QWMain::on_pbAssemble_clicked()
{
    if (!sFilename.isNull())
    {QStringList args;
    QString sOutName = sFilename;
    sOutName.replace(sOutName.lastIndexOf("."),sOutName.size(),".obj");
	if (ui->cbDebug->isChecked())
	{
		args.append("-d");
	}
    args.append("-o");
    args.append(sOutName);
    args.append(sFilename);
    ui->taConsole->append("===============================");
#ifdef __APPLE__
    pRunnable.start(QCoreApplication::applicationDirPath() + "/lasm", args);
#elif _WIN32
    pRunnable.start("./lasm",args);
#else
    pRunnable.start("lasm", args);
#endif
	statusBar()->showMessage(tr("Running"));
    }
    else
        ui->taConsole->append("No file selected!");
    }



void QWMain::sigStdErrReady(){
    ui->taConsole->setTextColor(QColor("Red"));
    ui->taConsole->append(QString(pRunnable.readAllStandardError()));

}

void QWMain::sigStdOutReady(){
    QString text = QString(pRunnable.readAllStandardOutput());
    if (text.contains("^WARNING"))
            ui->taConsole->setTextColor(QColor("Orange"));
    else
    ui->taConsole->setTextColor(QColor("Black"));
    ui->taConsole->append(text);


}

void QWMain::sigProcessFinished(int exitcode, QProcess::ExitStatus e){
	if (e ==QProcess::NormalExit){
		statusBar()->showMessage("Finished");
		ui->taConsole->setTextColor(QColor("Black"));
	}
	else{
		ui->taConsole->setTextColor(QColor("Red"));
        ui->taConsole->append("The assembler encountered an unknown error");
		ui->taConsole->setTextColor(QColor("Black"));
		statusBar()->showMessage("Finished");
	}
}

void QWMain::sigWatchedFileChanged(const QString &path)
{
    // debounce the file notification. Some editors delete and then recreates the file.
    QDateTime currentTime = QDateTime::currentDateTimeUtc();
    unsigned int currentTimeStamp = currentTime.toTime_t();
    if (currentTimeStamp - lastUpdatedTimeStamp > 2)
    {
        ui->taConsole->setFontItalic(true);
        ui->taConsole->setTextColor(QColor("Black"));
        ui->taConsole->append(QFileInfo(path).fileName() + " has changed. Click Assemble to update the object file.");
        lastUpdatedTimeStamp = currentTimeStamp;
        ui->taConsole->setFontItalic(false);
    }

}

void QWMain::on_bClear_clicked()
{
    ui->taConsole->clear();
    statusBar()->showMessage(tr("Ready"));

}
