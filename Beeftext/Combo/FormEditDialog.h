#ifndef FORMEDITDIALOG_H
#define FORMEDITDIALOG_H

#include <QDialog>


class FormResult {
public:
    FormResult() {}
    FormResult(QString name, QString type, QList<QString> choices, QString defaultText = "") :
    name(name), type(type),choices(choices), defaultText(defaultText)
    {
        
    }

    QJsonObject toJsonObject() {
        QJsonObject formEntry;
        formEntry.insert("name", this->name);
        formEntry.insert("type", this->type);
        formEntry.insert("choices", this->choices.join("\n"));
        formEntry.insert("defaultText", this->defaultText);
        return formEntry;
    }

    static FormResult fromJsonObject(QJsonValue v) {
        FormResult res;
        res.name = v.toObject().value("name").toString();
        res.type = v.toObject().value("type").toString();
        res.choices = v.toObject().value("choices").toString().split("\n");
        res.defaultText = v.toObject().contains("defaultText") ? v.toObject().value("defaultText").toString() : "";
        return res;
    }

    enum ItemType { FORM_TYPE = 1001, FORM_NAME = 1002, FORM_CHOICES = 1003, FORM_DEFAULT = 1004 };

    QString name, type, defaultText;
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
