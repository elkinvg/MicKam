#ifndef SETSETTINGSFORCAMERA_H
#define SETSETTINGSFORCAMERA_H

#include <QGroupBox>

namespace Ui {
class SetSettingsForCamera;
}

class SetSettingsForCamera : public QGroupBox
{
    Q_OBJECT
    
public:
    explicit SetSettingsForCamera(QWidget *parent = 0);
    ~SetSettingsForCamera();
    
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::SetSettingsForCamera *ui;
};

#endif // SETSETTINGSFORCAMERA_H
