/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of dialog for creating/editing a combo
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboDialog.h"
#include "ComboManager.h"
#include "MatchingMode.h"
#include "CaseSensitivity.h"
#include "BeeftextUtils.h"
#include "BeeftextConstants.h"
#include "Group/GroupDialog.h"
#include "Preferences/PreferencesManager.h"
#include <XMiLib/Exception.h>
#include <XMiLib/XMiLibConstants.h>
#include <Dialogs/ShortcutDialog.h>


namespace {

//****************************************************************************************************************************************************
/// \return The text for the variable label
//****************************************************************************************************************************************************
QString variablesLabel() {
    return QObject::tr(R"(
      <html><head/><body><p><a href="https://github.com/xmichelo/Beeftext/wiki/Variables">
      <span style=" text-decoration: underline; color:#%1;">
      About variables</span></a></p></body></html>)").arg(colorToHex(constants::blueBeeftextColor, false));
}


//****************************************************************************************************************************************************
/// \param[in] keyword The keyword.
/// \param[in] parent The parent widget of the dialog.
/// \return true if and only if the user decided to proceed with the short keyword.
//****************************************************************************************************************************************************
bool showShortKeywordConfirmationDialog(QString const &keyword, QWidget *parent = nullptr) {
    QMessageBox msgBox(parent);
    msgBox.setText(QObject::tr("The keyword is very short. Are you sure you want to use the keyword '%1'?")
        .arg(keyword));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setEscapeButton(QMessageBox::No);
    QCheckBox *check = new QCheckBox(QObject::tr("Do not show this warning again."), &msgBox);
    msgBox.setCheckBox(check);
    qint32 const button = msgBox.exec();
    PreferencesManager::instance().setWarnAboutShortComboKeywords(!check->isChecked());
    return (QMessageBox::Yes == button);
}


//****************************************************************************************************************************************************
/// \param[in] parent The parent widget of the dialog.
/// \return true if and only if the user decided to proceed with the short keyword.
//****************************************************************************************************************************************************
bool showEmptyKeywordConfirmationDialog(QWidget *parent = nullptr) {
    QMessageBox msgBox(parent);
    msgBox.setText(QObject::tr("You have not defined a keyword. Are you sure you want to continue?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setEscapeButton(QMessageBox::No);
    QCheckBox *check = new QCheckBox(QObject::tr("Do not show this warning again."), &msgBox);
    msgBox.setCheckBox(check);
    qint32 const button = msgBox.exec();
    PreferencesManager::instance().setWarnAboutEmptyComboKeywords(!check->isChecked());
    return (QMessageBox::Yes == button);
}


} // Anonymous namespace


//****************************************************************************************************************************************************
/// \param[in] combo The combo
/// \param[in] title The title to display in the dialog's title bar
/// \param[in] parent The parent widget of the dialog
/// \return true if and only if the user validated the dialog
//****************************************************************************************************************************************************
bool ComboDialog::run(SpCombo const &combo, QString const &title, QWidget *parent) {
    return Accepted == ComboDialog(combo, title, parent).exec();
}


//****************************************************************************************************************************************************
/// \param[in] combo The combo
/// \param[in] title The title to display in the dialog's title bar
/// \param[in] parent The parent widget of the dialog
//****************************************************************************************************************************************************
ComboDialog::ComboDialog(SpCombo const &combo, QString const &title, QWidget *parent)
    : QDialog(parent, xmilib::constants::kDefaultDialogFlags | Qt::WindowMaximizeButtonHint)
    , combo_(combo) {
    ComboManager::instance().groupListRef().ensureNotEmpty();
    if (!combo)
        throw xmilib::Exception("%1(): combo is null.");
    ui_.setupUi(this);
    this->setWindowTitle(title);
    ui_.editName->setText(combo->name());
    ui_.comboGroup->setContent(ComboManager::instance().groupListRef());
    ui_.comboGroup->setCurrentGroup(combo_->group());
    fillMatchingModeCombo(*ui_.comboMatching, true);
    selectMatchingModeInCombo(*ui_.comboMatching, combo->matchingMode(false), true);
    fillCaseSensitivityCombo(*ui_.comboCaseSensitivity, true);
    selectCaseSensitivityInCombo(*ui_.comboCaseSensitivity, combo->caseSensitivity(false), true);
    ui_.editKeyword->setText(combo->keyword());
    ui_.editKeyword->setValidator(&validator_);
    ui_.comboEditor->setPlainText(combo->snippet());
    ui_.editDescription->setPlainText(combo->description());
    ui_.labelVariables->setText(variablesLabel());
    fillFormList(combo->getFormList());

    if (combo->getShortcut()) {
        ui_.shortcutLabel->setText(combo->getShortcut()->toString());
        ui_.shortcutReset->setVisible(true);
        ui_.shortcutReset->setEnabled(true);
    }
    else {
        ui_.shortcutReset->setVisible(false);
    }

    ui_.secretCheckbox->setChecked(combo_->isSecret());

    this->updateGui();
    connect(ui_.editKeyword, &QLineEdit::textChanged, this, &ComboDialog::updateGui);
    connect(ui_.comboEditor, &ComboEditor::textChanged, this, &ComboDialog::updateGui);
    connect(ui_.comboGroup, &GroupComboBox::currentIndexChanged, this, &ComboDialog::updateGui);
    connect(ui_.buttonOk, &QPushButton::clicked, this, &ComboDialog::onActionOk);
    connect(ui_.buttonCancel, &QPushButton::clicked, this, &ComboDialog::reject);
    connect(ui_.buttonNewGroup, &QPushButton::clicked, this, &ComboDialog::onActionNewGroup);
    connect(new QShortcut(QKeySequence("Ctrl+Return"), this), &QShortcut::activated, this, &ComboDialog::onActionOk);
    connect(ui_.formInputWidget, &QListWidget::customContextMenuRequested, this, &ComboDialog::onFormInputCustomContextMenuRequested);
    connect(ui_.formInputWidget, &QListWidget::itemDoubleClicked, this, &ComboDialog::onFormInputItemDoubleClicked);
    connect(ui_.shortcutButton, &QPushButton::clicked, this, &ComboDialog::onChangeShortcut);
    connect(ui_.shortcutReset, &QPushButton::clicked, this, &ComboDialog::onResetShortcut);
    connect(ui_.secretCheckbox, &QCheckBox::checkStateChanged, this, &ComboDialog::onSecretStateChanged);
}


//****************************************************************************************************************************************************
/// \return true if and only if the combo is valid
//****************************************************************************************************************************************************
bool ComboDialog::checkAndReportInvalidCombo() {
    if (ui_.comboEditor->plainText().isEmpty()) {
        QMessageBox::critical(this, tr("Error"), tr("The snippet text is empty."));
        return false;
    }
    QString text = ui_.editKeyword->text();
    if (QValidator::Acceptable != validator_.validate(text)) {
        QMessageBox::critical(this, tr("Error"), tr("The keyword is invalid."));
        return false;
    }

    SpGroup const group = ui_.comboGroup->currentGroup();
    if (!group) {
        QMessageBox::critical(this, tr("Error"), tr("The group is invalid."));
        return false;
    }

    // we check that the keyword is not already in use
    QString const newKeyword = ui_.editKeyword->text();
    ComboList const &comboList = ComboManager::instance().comboListRef();
    if ((!newKeyword.isEmpty()) && (comboList.end() != std::find_if(comboList.begin(), comboList.end(), [&](SpCombo const &existing) -> bool { return (existing != combo_) && (existing->keyword() == newKeyword); })))
        return questionDialog(this, tr("Duplicate keyword"), tr("This keyword is already in use. \n\n"
                                                                "You can have multiple combos with the same keyword, Beeftext will pick one of the matching combos "
                                                                "randomly."), tr("&Continue"), tr("C&ancel"));

    // we check for conflicts that would make some combo 'unreachable'
    if (newKeyword.isEmpty()) // We do not check for conflicts if the user validated an empty keyword.
        return true;
    qint32 const conflictCount = qint32(std::count_if(comboList.begin(), comboList.end(), [&](SpCombo const &existing)
        -> bool {
        return (existing != combo_) && ((!existing->keyword().isEmpty()) &&
                                        (existing->keyword().startsWith(newKeyword) || newKeyword.startsWith(existing->keyword())));
    }));
    if (!conflictCount)
        return true;
    QString const singleConflictStr = tr("An existing combo is creating a conflict with this combo.");
    QString const multipleConflictStr = tr("%1 existing combos are creating conflicts with this combo.")
        .arg(conflictCount);
    return QMessageBox::Yes == QMessageBox::question(this, tr("Conflict"), tr("%1 If you use automatic substitution, "
                                                                              "conflicts make some combos impossible to trigger.\n\nDo you want to continue anyway?")
        .arg(conflictCount > 1 ? multipleConflictStr : singleConflictStr), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
}

void ComboDialog::fillFormList(QList<FormResult> formList_)
{
    for (FormResult res : formList_) {
        QListWidgetItem* item = new QListWidgetItem(res.name);
        item->setData(FormResult::ItemType::FORM_TYPE, res.type);
        item->setData(FormResult::ItemType::FORM_NAME, res.name);
        item->setData(FormResult::ItemType::FORM_CHOICES, res.choices);
        item->setData(FormResult::ItemType::FORM_DEFAULT, res.defaultText);
        ui_.formInputWidget->addItem(item);
    }
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void ComboDialog::onActionOk() {
    Q_ASSERT(combo_);
    if (!checkAndReportInvalidCombo())
        return;
    QString const keyword = ui_.editKeyword->text().trimmed();
    PreferencesManager const &prefs = PreferencesManager::instance();
    if (keyword.isEmpty()) {
        if (prefs.warnAboutEmptyComboKeywords() && (!showEmptyKeywordConfirmationDialog(this)))
            return;
    } else if ((keyword.size() < 3) && prefs.warnAboutShortComboKeywords()
               && (!showShortKeywordConfirmationDialog(keyword, this)))
        return;
    combo_->setName(ui_.editName->text().trimmed());
    combo_->setGroup(ui_.comboGroup->currentGroup());
    combo_->setMatchingMode(selectedMatchingModeInCombo(*ui_.comboMatching));
    combo_->setCaseSensitivity(selectedCaseSensitivityInCombo(*ui_.comboCaseSensitivity));
    combo_->setKeyword(keyword);
    combo_->setSnippet(ui_.comboEditor->plainText());
    combo_->setDescription(ui_.editDescription->toPlainText());
    QList<FormResult> formList_;
    for (int i = 0; i < ui_.formInputWidget->count(); i++) {
        QListWidgetItem* item = ui_.formInputWidget->item(i);
        FormResult res;
        res.name = item->data(FormResult::ItemType::FORM_NAME).toString();
        res.type = item->data(FormResult::ItemType::FORM_TYPE).toString();
        res.choices = item->data(FormResult::ItemType::FORM_CHOICES).toStringList();
        res.defaultText = item->data(FormResult::ItemType::FORM_DEFAULT).toString();
        formList_.append(res);
    }
    combo_->setFormList(formList_);
    this->accept();
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void ComboDialog::onActionNewGroup() {
    SpGroup const group = std::make_shared<Group>(QString());
    if ((!GroupDialog::run(group, tr("New Group"), this)) || !group)
        return;
    ComboManager::instance().comboListRef().groupListRef().append(group);
    ui_.comboGroup->setContent(ComboManager::instance().groupListRef());
    ui_.comboGroup->setCurrentGroup(group);
}


//****************************************************************************************************************************************************
// 
//****************************************************************************************************************************************************
void ComboDialog::updateGui() const {
    QString keyword = ui_.editKeyword->text();
    bool const canAccept = (QValidator::Acceptable == validator_.validate(keyword)) &&
                           (!ui_.comboEditor->plainText().isEmpty()) && ui_.comboGroup->currentGroup();
    ui_.buttonOk->setEnabled(canAccept);
    ui_.editName->setPlaceholderText(Combo::placeholderName(ui_.editKeyword->text(), ui_.comboEditor->plainText()));
}

void ComboDialog::onFormInputCustomContextMenuRequested(const QPoint& pos) {
    qDebug() << "Custom Context menu";

    QMenu contextMenu(tr("Context menu"), this);

    // Todo: check if we have clicked on an item and add modify/delete functions
    QListWidgetItem* item = ui_.formInputWidget->itemAt(pos);

    QAction* addAction = new QAction("Add new form input", this);
    connect(addAction, &QAction::triggered, this, &ComboDialog::onAddFormInput);
    contextMenu.addAction(addAction);

    if (item == nullptr) {
        qDebug() << "no item";
    }
    else {
        QAction *modifyAction = new QAction("Modify", this);
        QAction* deleteAction = new QAction("Delete", this);

        connect(modifyAction, &QAction::triggered, this, [=]() {
            onModifyFormInput(item);
            });

        connect(deleteAction, &QAction::triggered, this, [=]() {
            onDeleteFormInput(item);
            });
        contextMenu.addAction(modifyAction);
        contextMenu.addAction(deleteAction);
    }

    contextMenu.exec(ui_.formInputWidget->viewport()->mapToGlobal(pos));
}

void ComboDialog::onFormInputItemDoubleClicked(QListWidgetItem* item)
{
    FormResult* cr = new FormResult(
        item->data(FormResult::ItemType::FORM_NAME).toString(),
        item->data(FormResult::ItemType::FORM_TYPE).toString(),
        item->data(FormResult::ItemType::FORM_CHOICES).toStringList(),
        item->data(FormResult::ItemType::FORM_DEFAULT).toString());
    FormEditDialog* dg = new FormEditDialog(cr, this);
    if (dg->exec() == QDialog::Accepted) {
        item->setText(cr->name);
        item->setData(FormResult::ItemType::FORM_TYPE, cr->type);
        item->setData(FormResult::ItemType::FORM_NAME, cr->name);
        item->setData(FormResult::ItemType::FORM_CHOICES, cr->choices);
        item->setData(FormResult::ItemType::FORM_DEFAULT, cr->defaultText);
    }
    delete cr;
}

void ComboDialog::onAddFormInput() {
    qDebug() << "onAddFormInput";
    // Open FormEditDialog
    FormEditDialog dg = FormEditDialog(this);

    if (dg.exec() == QDialog::Accepted) {
        FormResult* res = dg.getResult();
        qDebug() << "new form input: " << res->name << res->type << res->choices << res->defaultText;
        QListWidgetItem* item = new QListWidgetItem(res->name);
        item->setData(FormResult::ItemType::FORM_TYPE, res->type);
        item->setData(FormResult::ItemType::FORM_NAME, res->name);
        item->setData(FormResult::ItemType::FORM_CHOICES, res->choices);
        item->setData(FormResult::ItemType::FORM_DEFAULT, res->defaultText);
        ui_.formInputWidget->addItem(item);
    }
}

void ComboDialog::onModifyFormInput(QListWidgetItem *item)
{
    qDebug() << "modify form input for item: " << item->data(FormResult::ItemType::FORM_NAME).toString();
    FormResult* cr = new FormResult(
        item->data(FormResult::ItemType::FORM_NAME).toString(),
        item->data(FormResult::ItemType::FORM_TYPE).toString(),
        item->data(FormResult::ItemType::FORM_CHOICES).toStringList(),
        item->data(FormResult::ItemType::FORM_DEFAULT).toString());
    FormEditDialog *dg = new FormEditDialog(cr, this);
    if (dg->exec() == QDialog::Accepted) {
        item->setText(cr->name);
        item->setData(FormResult::ItemType::FORM_TYPE, cr->type);
        item->setData(FormResult::ItemType::FORM_NAME, cr->name);
        item->setData(FormResult::ItemType::FORM_CHOICES, cr->choices);
        item->setData(FormResult::ItemType::FORM_DEFAULT, cr->defaultText);
    }
    delete cr;
}

void ComboDialog::onDeleteFormInput(QListWidgetItem* item)
{
    qDebug() << "deleting item: " << item->data(FormResult::ItemType::FORM_NAME);
    delete item;
}

void ComboDialog::onChangeShortcut()
{
    SpShortcut shortcut = combo_->getShortcut();
    if (!ShortcutDialog::run(this, shortcut, false))
        return;
    combo_->setShortcut(shortcut);
    ui_.shortcutLabel->setText(shortcut ? shortcut->toString() : ui_.shortcutLabel->text());
    if (shortcut) {
        ui_.shortcutReset->setVisible(true);
        ui_.shortcutReset->setEnabled(true);
    }
        
}

void ComboDialog::onResetShortcut()
{
    ui_.shortcutReset->setVisible(false);
    ui_.shortcutReset->setEnabled(false);
    combo_->resetShortcut();
}

void ComboDialog::onSecretStateChanged(bool secret)
{
    combo_->setSecrecy(secret);
}
