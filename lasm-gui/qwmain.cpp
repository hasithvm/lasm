#include "qwmain.h"
#include "ui_qwmain.h"

QWMain::QWMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QWMain)
{
    ui->setupUi(this);
    connect(&pRunnable, SIGNAL(readyReadStandardOutput()), this, SLOT(sigStdOutReady()));
    connect(&pRunnable, SIGNAL(readyReadStandardError()), this, SLOT(sigStdErrReady()));
	connect(&pRunnable, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(sigProcessFinished(int, QProcess::ExitStatus)));
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
    sFilename = QFileDialog::getOpenFileName(this, tr("Select File"),
                                                    sDefaultPath,
                                                    tr("Assembler listing (*.asm)"));
    if (!sFilename.isNull()){
        //update MRU path only if file is selected.
        sDefaultPath= QFileInfo(sFilename).absoluteDir().absolutePath();
        
        ui->pbAssemble->setEnabled(true);
        ui->taConsole->clear();
        ui->taConsole->setFontWeight(63);
        ui->taConsole->append("Selected file ");
        ui->taConsole->insertPlainText(sFilename);
        ui->cbDebug->setEnabled(true);

    }
    else{
        ui->pbAssemble->setEnabled(false);
        ui->taConsole->clear();
        ui->taConsole->setFontWeight(63);
        ui->taConsole->append("No file selected");
		
        ui->cbDebug->setEnabled(false);


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
    args.append(sFilename);
    args.append(sOutName);
    ui->taConsole->append("===============================");
    pRunnable.start("./lasm",args);
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

void QWMain::on_bClear_clicked()
{
    ui->taConsole->clear();
    statusBar()->showMessage(tr("Ready"));

}
