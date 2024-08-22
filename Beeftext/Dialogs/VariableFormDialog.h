#pragma once

#include <QDialog>
#include "ui_VariableFormDialog.h"
#include "combo/FormEditDialog.h"

class VariableFormDialog : public QDialog
{
	Q_OBJECT

public:
	explicit VariableFormDialog(QList<FormResult> formList);
	~VariableFormDialog();

private:
	Ui::VariableFormDialogClass ui;
	QMap<QString, QLineEdit*> formWidgets;
	QMap<QString, QString> formVariables;

protected: // member function
	void showEvent(QShowEvent* event) override; ///< Callback for the show event

public:
	QMap<QString, QString> getFormVariables();
	// static member functions
	static bool run(QString const& description, QList<FormResult> formList, QMap<QString, QString>& outFormVariables); ///< Run the dialog

protected: // member functions
	void changeEvent(QEvent*) override; ///< Change event handler
};
