#ifndef FORMEDITDIALOG_H
#define FORMEDITDIALOG_H

#include <QDialog>


class FormResult {
public:
    FormResult() {}
    FormResult(QString name, QString type, QList<QString> choices) : 
    name(name), type(type),choices(choices)
    {
        
    }

    enum ItemType { FORM_TYPE = 1001, FORM_NAME = 1002, FORM_CHOICES = 1003 };

    QString name, type;
    QList<QString> choices;
};

namespace Ui {
class FormEditDialog;
}

class FormEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FormEditDialog(QWidget *parent = nullptr);
    FormEditDialog(FormResult* res, QWidget* parent = nullptr);
    ~FormEditDialog();

    FormResult* getResult();

private:
    Ui::FormEditDialog *ui;
    FormResult *result;

private slots:
    void onNameEditTextChanged(QString text);
    void onTypeSelectionChanged(QString type);
    void onChoiceEditTextChanged();
    void onButtonBoxAccepted();
};

#endif // FORMEDITDIALOG_H
