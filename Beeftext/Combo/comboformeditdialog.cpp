#include "stdafx.h"
#include "comboformeditdialog.h"
#include "ui_comboformeditdialog.h"

ComboFormEditDialog::ComboFormEditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ComboFormEditDialog), result(new ComboFormResult("","text",QStringList()))
{
    ui->setupUi(this);

    connect(this->ui->nameEdit, &QLineEdit::textChanged, this, &ComboFormEditDialog::onNameEditTextChanged);
    connect(this->ui->typeCombo, &QComboBox::textActivated, this, &ComboFormEditDialog::onTypeSelectionChanged);
    connect(this->ui->choiceEdit, &QTextEdit::textChanged, this, &ComboFormEditDialog::onChoiceEditTextChanged);
    connect(this->ui->buttonBox, &QDialogButtonBox::accepted, this, &ComboFormEditDialog::onButtonBoxAccepted);
}

ComboFormEditDialog::ComboFormEditDialog(ComboFormResult* res, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::ComboFormEditDialog), result(res)
{
    ui->setupUi(this);

    ui->nameEdit->setText(this->result->name);
    ui->typeCombo->setCurrentText(this->result->type);
    if (this->result->type == "choice") {
        ui->choiceEdit->setReadOnly(false);
    }
    ui->choiceEdit->setPlainText(this->result->choices.join("\n"));
    connect(this->ui->nameEdit, &QLineEdit::textChanged, this, &ComboFormEditDialog::onNameEditTextChanged);
    connect(this->ui->typeCombo, &QComboBox::textActivated, this, &ComboFormEditDialog::onTypeSelectionChanged);
    connect(this->ui->choiceEdit, &QTextEdit::textChanged, this, &ComboFormEditDialog::onChoiceEditTextChanged);
    connect(this->ui->buttonBox, &QDialogButtonBox::accepted, this, &ComboFormEditDialog::onButtonBoxAccepted);
}

ComboFormEditDialog::~ComboFormEditDialog()
{
    delete ui;
}

ComboFormResult* ComboFormEditDialog::getResult()
{
    return this->result;
}

void ComboFormEditDialog::onButtonBoxAccepted()
{

}

void ComboFormEditDialog::onNameEditTextChanged(QString text) {
    this->result->name = text;
}

void ComboFormEditDialog::onTypeSelectionChanged(QString type) {
    qDebug() << "onTypeSelectionChanged: Selection changed to type: " << type;
    if (type == "choice") {
        ui->choiceEdit->setReadOnly(false);
    }
    else ui->choiceEdit->setReadOnly(true);
    this->result->type = type;
}

void ComboFormEditDialog::onChoiceEditTextChanged()
{
    this->result->choices = ui->choiceEdit->toPlainText().split("\n");
}
