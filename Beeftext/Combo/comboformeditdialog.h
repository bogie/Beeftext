#ifndef COMBOFORMEDITDIALOG_H
#define COMBOFORMEDITDIALOG_H

#include <QDialog>


class ComboFormResult {
public:
    ComboFormResult() {}
    ComboFormResult(QString name, QString type, QList<QString> choices) : 
    name(name), type(type),choices(choices)
    {
        
    }

    enum ItemType { FORM_TYPE = 1001, FORM_NAME = 1002, FORM_CHOICES = 1003 };

    QString name, type;
    QList<QString> choices;
};

namespace Ui {
class ComboFormEditDialog;
}

class ComboFormEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ComboFormEditDialog(QWidget *parent = nullptr);
    ComboFormEditDialog(ComboFormResult* res, QWidget* parent = nullptr);
    ~ComboFormEditDialog();

    ComboFormResult* getResult();

private:
    Ui::ComboFormEditDialog *ui;
    ComboFormResult *result;

private slots:
    void onNameEditTextChanged(QString text);
    void onTypeSelectionChanged(QString type);
    void onChoiceEditTextChanged();
    void onButtonBoxAccepted();
};

#endif // COMBOFORMEDITDIALOG_H
