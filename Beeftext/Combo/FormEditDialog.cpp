#include "stdafx.h"
#include "FormEditDialog.h"
#include "ui_comboformeditdialog.h"

FormEditDialog::FormEditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FormEditDialog), result(new FormResult("","text",QStringList()))
{
    ui->setupUi(this);

    connect(this->ui->nameEdit, &QLineEdit::textChanged, this, &FormEditDialog::onNameEditTextChanged);
    connect(this->ui->typeCombo, &QComboBox::textActivated, this, &FormEditDialog::onTypeSelectionChanged);
    connect(this->ui->choiceEdit, &QTextEdit::textChanged, this, &FormEditDialog::onChoiceEditTextChanged);
    connect(this->ui->buttonBox, &QDialogButtonBox::accepted, this, &FormEditDialog::onButtonBoxAccepted);
}

FormEditDialog::FormEditDialog(FormResult* res, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::FormEditDialog), result(res)
{
    ui->setupUi(this);

    ui->nameEdit->setText(this->result->name);
    ui->typeCombo->setCurrentText(this->result->type);
    if (this->result->type == "choice") {
        ui->choiceEdit->setReadOnly(false);
    }
    ui->choiceEdit->setPlainText(this->result->choices.join("\n"));
    connect(this->ui->nameEdit, &QLineEdit::textChanged, this, &FormEditDialog::onNameEditTextChanged);
    connect(this->ui->typeCombo, &QComboBox::textActivated, this, &FormEditDialog::onTypeSelectionChanged);
    connect(this->ui->choiceEdit, &QTextEdit::textChanged, this, &FormEditDialog::onChoiceEditTextChanged);
    connect(this->ui->buttonBox, &QDialogButtonBox::accepted, this, &FormEditDialog::onButtonBoxAccepted);
}

FormEditDialog::~FormEditDialog()
{
    delete ui;
}

FormResult* FormEditDialog::getResult()
{
    return this->result;
}

void FormEditDialog::onButtonBoxAccepted()
{

}

void FormEditDialog::onNameEditTextChanged(QString text) {
    this->result->name = text;
}

void FormEditDialog::onTypeSelectionChanged(QString type) {
    qDebug() << "onTypeSelectionChanged: Selection changed to type: " << type;
    if (type == "choice") {
        ui->choiceEdit->setReadOnly(false);
    }
    else ui->choiceEdit->setReadOnly(true);
    this->result->type = type;
}

void FormEditDialog::onChoiceEditTextChanged()
{
    this->result->choices = ui->choiceEdit->toPlainText().split("\n");
}
