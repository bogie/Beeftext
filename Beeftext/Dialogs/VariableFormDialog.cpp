#include "stdafx.h"
#include "VariableFormDialog.h"

VariableFormDialog::VariableFormDialog(QMap<QString, FormResult> formList)
	: QDialog(nullptr), formList(formList)
{
	ui.setupUi(this);

	for (FormResult result : formList.values()) {
		QLabel* label = new QLabel(result.name, this);
		QHBoxLayout* layout = new QHBoxLayout(this);
		layout->addWidget(label);

		if (result.type == "text") {
			QLineEdit* edit = new QLineEdit(this);
			edit->setText(result.defaultText);
			layout->addWidget(edit);
			formWidgets.insert(result.name, edit);
		} else if(result.type == "choice") {
			QComboBox* combo = new QComboBox(this);
			combo->addItems(result.choices);
			layout->addWidget(combo);
			formWidgets.insert(result.name, combo);
		} else {
			QLineEdit* edit = new QLineEdit(this);
			layout->addWidget(edit);
			formWidgets.insert(result.name, edit);
		}
		
		ui.formInputsLayout->addLayout(layout);
		
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
		FormResult res = formList.value(variable);

		qDebug() << "type of widget is: " << formWidgets.value(variable)->objectName();

		if (QLineEdit* edit = qobject_cast<QLineEdit*>(formWidgets.value(variable))) {
			formVariables.insert(variable, edit->text());
		}
		else if (QComboBox* box = qobject_cast<QComboBox*>(formWidgets.value(variable))) {
			formVariables.insert(variable, box->currentText());
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
