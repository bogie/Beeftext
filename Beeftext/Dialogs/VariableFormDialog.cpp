#include "stdafx.h"
#include "VariableFormDialog.h"

VariableFormDialog::VariableFormDialog(QMap<QString, FormResult> formList)
	: QDialog(nullptr), formList(formList)
{
	ui.setupUi(this);

	for (FormResult result : formList.values()) {
		if (result.type == "text") {
			QLineEdit *edit = new QLineEdit(result.defaultText, this);
			formWidgets.insert(result.name, edit);
			ui.formInputsLayout->addRow(result.name, edit);
		} else if(result.type == "choice") {
			QComboBox* combo = new QComboBox(this);
			combo->addItems(result.choices);
			formWidgets.insert(result.name, combo);
			ui.formInputsLayout->addRow(result.name, combo);
		} else if (result.type == "multiline") {
			QPlainTextEdit* edit = new QPlainTextEdit(result.defaultText, this);
			formWidgets.insert(result.name, edit);
			ui.formInputsLayout->addRow(result.name, edit);
		} else {
			QLineEdit* edit = new QLineEdit(result.defaultText,this);
			formWidgets.insert(result.name, edit);
			ui.formInputsLayout->addRow(result.name, edit);
		}		
	}
	
	formWidgets.values().at(0)->setFocus();
	this->show();
}

VariableFormDialog::~VariableFormDialog()
{}

void VariableFormDialog::showEvent(QShowEvent * event)
{
	this->raise();
	this->activateWindow();
	QDialog::showEvent(event);
}

QMap<QString, QString> VariableFormDialog::getFormVariables()
{

	for (QString variable : formWidgets.keys()) {
		FormResult res = formList.value(variable);

		if (QLineEdit* edit = qobject_cast<QLineEdit*>(formWidgets.value(variable))) {
			formVariables.insert(variable, edit->text());
		} else if (QComboBox* box = qobject_cast<QComboBox*>(formWidgets.value(variable))) {
			formVariables.insert(variable, box->currentText());
		} else if (QPlainTextEdit* edit= qobject_cast<QPlainTextEdit*>(formWidgets.value(variable))) {
			formVariables.insert(variable, edit->toPlainText());
		}
	}

	return formVariables;
}

bool VariableFormDialog::run(QString const& description, QMap<QString, FormResult> formList, QMap<QString, QString>& outFormVariables)
{
	VariableFormDialog dlg(formList);
	if (Accepted != dlg.exec())
		return false;

	outFormVariables = dlg.getFormVariables();
	return true;
}

void VariableFormDialog::changeEvent(QEvent* event)
{
	/*if ((event->type() == QEvent::ActivationChange) && !this->isActiveWindow())
		this->reject(); // when the dialog looses the focus, we dismiss is because we don't know where the input
	// focus can be now.
	QWidget::changeEvent(event);*/
}
