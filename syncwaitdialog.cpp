#include "syncwaitdialog.h"
#include "ui_syncwaitdialog.h"

SyncWaitDialog::SyncWaitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SyncWaitDialog)
{
    ui->setupUi(this);
}

SyncWaitDialog::~SyncWaitDialog()
{
    delete ui;
}

void SyncWaitDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
