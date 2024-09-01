#include "stdafx.h"
#include "FormEditDialog.h"
#include "ui_formeditdialog.h"

FormEditDialog::FormEditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FormEditDialog), result(new FormResult("","text",QStringList(), ""))
{
    ui->setupUi(this);

    ui->choiceLabel->setText("Default");

    connect(this->ui->nameEdit, &QLineEdit::textChanged, this, &FormEditDialog::onNameEditTextChanged);
    connect(this->ui->typeCombo, &QComboBox::textActivated, this, &FormEditDialog::onTypeSelectionChanged);
    connect(this->ui->choiceEdit, &QPlainTextEdit::textChanged, this, &FormEditDialog::onChoiceEditTextChanged);
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
        ui->choiceLabel->setText("Choice");
        ui->choiceEdit->setPlainText(this->result->choices.join("\n"));
    }
    else if (this->result->type == "text") {
        ui->choiceLabel->setText("Default");
        ui->choiceEdit->setPlainText(this->result->defaultText);
    }
    
    connect(this->ui->nameEdit, &QLineEdit::textChanged, this, &FormEditDialog::onNameEditTextChanged);
    connect(this->ui->typeCombo, &QComboBox::textActivated, this, &FormEditDialog::onTypeSelectionChanged);
    connect(this->ui->choiceEdit, &QPlainTextEdit::textChanged, this, &FormEditDialog::onChoiceEditTextChanged);
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
    this->result->type = type;
    if (type == "choice") {
        ui->choiceLabel->setText("Choice");
    }
    else
        ui->choiceLabel->setText("Default");
}

void FormEditDialog::onChoiceEditTextChanged()
{
    if (result->type == "choice")
        result->choices = ui->choiceEdit->toPlainText().split("\n");
    else if (result->type == "text")
        result->defaultText = ui->choiceEdit->toPlainText();
}
