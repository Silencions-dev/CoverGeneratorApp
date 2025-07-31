#include "BoolDimInput.hpp"

/*!
 * \brief Initialization of the class BoolDimInput
 *
 * Create the new check box and trigger the actions
 * corresponding to not checked box. Connect the signal
 * from check box with appropriate slot.
 *
 * \param[in] parent - pointer to the parent widget
 */
BoolDimInput::BoolDimInput(QWidget *parent) : DimInput(parent)
{
    box = new QCheckBox("", this);

    boxUnchecked();

    connect(box, SIGNAL(stateChanged(int)), this, SLOT(boxStateChanged(int)));
}
/*!
 * \brief Sets the horizontal layout
 *
 * Align check box, QLineEdit field and QLabel with the
 * unit horizontally.
 */
void BoolDimInput::setHorizontalLayout()
{
    h_layout->addWidget(box);
    h_layout->addWidget(input_dim);
    h_layout->addWidget(unit);
}
/*!
 * \brief Reaction when the box is checked
 *
 * Enable the QLineEdit - set visible frame and turn on
 * the possibility to write in it. Verify the data stored
 * in the QLineEdit and adjust the appearance to the status.
 */
void BoolDimInput::boxChecked()
{
    input_dim->setFrame(true);
    input_dim->setReadOnly(false);
    newData();
    setEditStatus(dim_status);
}
/*!
 * \brief Reaction when the box is unchecked
 *
 * Verify the data inside the QLineEdit. Turn off the
 * frame and set read-only mode. Change the style of the
 * QLineEdit and notify about changes the higher widgets.
 */
void BoolDimInput::boxUnchecked()
{
    QString new_data = input_dim->text();

    checkBoxData(new_data);

    input_dim->setFrame(false);
    input_dim->setReadOnly(true);
    changeStyle(input_dim, styles::EDIT_DISABLED);

    emit dataChanged();
}
/*!
 * \brief Slot that reacts to the state change of the check box
 *
 * Calls the appropriate function depending on the state
 * returned by the check box - checked or unchecked.
 *
 * \param[in] state - integer with current state of the check box
 */
void BoolDimInput::boxStateChanged(int state)
{
    if(state == Qt::Unchecked || state == Qt::PartiallyChecked)
        boxUnchecked();
    else if(state == Qt::Checked)
        boxChecked();
}
/*!
 * \brief Check the text from the input box
 *
 * If the data is empty or correct, then set the disabled
 * status. Otherwise assign wrong status.
 *
 * \param[in] new_data - constant reference to the string with the new data
 */
void BoolDimInput::checkBoxData(const QString & new_data)
{
    if(new_data.isEmpty() || checkNewData(new_data))
        dim_status = DimensionStatus::DISABLED;
    else
        dim_status = DimensionStatus::WRONG;
}
