#ifndef QWMAIN_H
#define QWMAIN_H

#include <QMainWindow>
#include <QString>
#include <QSettings>
#include <QDir>
#include <QFileDialog>
#include <QProcess>
#include <QFileSystemWatcher>
namespace Ui {
class QWMain;
}

class QWMain : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit QWMain(QWidget *parent = 0);
    ~QWMain();
    
private slots:
    void on_pbLoad_clicked();
    void on_pbAssemble_clicked();
    void on_bClear_clicked();

public slots:
    void sigStdErrReady();
    void sigStdOutReady();
	void sigProcessFinished(int exitcode, QProcess::ExitStatus e);
    void sigWatchedFileChanged(const QString& path);

private:
    Ui::QWMain *ui;
    void writeSettings();
    void loadSettings();
    QString sFilename;
    QString sDefaultPath;
    bool bGenDebug;
    QProcess pRunnable;
    QFileSystemWatcher watcher;
    unsigned int lastUpdatedTimeStamp;
};

#endif // QWMAIN_H
