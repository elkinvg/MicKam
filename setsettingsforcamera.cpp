#include "setsettingsforcamera.h"
#include "ui_setsettingsforcamera.h"

SetSettingsForCamera::SetSettingsForCamera(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::SetSettingsForCamera)
{
    ui->setupUi(this);
}

SetSettingsForCamera::~SetSettingsForCamera()
{
    delete ui;
}

void SetSettingsForCamera::on_buttonBox_accepted()
{
    this->close();
}

void SetSettingsForCamera::on_buttonBox_rejected()
{
    this->close();
}
