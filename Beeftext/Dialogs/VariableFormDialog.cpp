#include "stdafx.h"
#include "VariableFormDialog.h"

VariableFormDialog::VariableFormDialog(QList<FormResult> formList)
	: QDialog(nullptr)
{
	ui.setupUi(this);

	for (FormResult result : formList) {
		QLineEdit* edit = new QLineEdit(this);
		QLabel* label = new QLabel(result.name,this);
		QHBoxLayout* layout = new QHBoxLayout(this);
		layout->addWidget(label);
		layout->addWidget(edit);
		ui.formInputsLayout->addLayout(layout);
		formWidgets.insert(result.name,edit);
	}

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
		QLineEdit* edit = formWidgets.value(variable);
		formVariables.insert(variable, edit->text());
	}

	return formVariables;
}

bool VariableFormDialog::run(QString const& description, QList<FormResult> formList, QMap<QString, QString>& outFormVariables)
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
