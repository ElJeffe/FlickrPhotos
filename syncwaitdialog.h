#ifndef SYNCWAITDIALOG_H
#define SYNCWAITDIALOG_H

#include <QDialog>

namespace Ui {
    class SyncWaitDialog;
}

class SyncWaitDialog : public QDialog {
    Q_OBJECT
public:
    SyncWaitDialog(QWidget *parent = 0);
    ~SyncWaitDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SyncWaitDialog *ui;
};

#endif // SYNCWAITDIALOG_H
