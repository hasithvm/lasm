#ifndef QWMAIN_H
#define QWMAIN_H

#include <QMainWindow>
#include <QString>
#include <QSettings>
#include <QDir>
#include <QFileDialog>
#include <QProcess>
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

public slots:
    void sigStdErrReady();
    void sigStdOutReady();
	void sigProcessFinished(int exitcode, QProcess::ExitStatus e);

private:
    Ui::QWMain *ui;
    void writeSettings();
    void loadSettings();
    QString sFilename;
    QString sDefaultPath;
    bool bGenDebug;
    QProcess pRunnable;
};

#endif // QWMAIN_H
