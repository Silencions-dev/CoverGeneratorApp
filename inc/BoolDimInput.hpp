#ifndef BOOLDIMINPUT_HPP
#define BOOLDIMINPUT_HPP

/*!
 * \file
 * \brief Definition of the class BoolDimInput
 *
 * This file contains definition of the class BoolDimInput,
 * which provides user a tool to type OPTIONAL required
 * dimension.
 */

#include <QCheckBox>
#include "DimInput.hpp"

/*!
 * \brief Class that implements input field for optional info
 *
 * BoolDimInput class inheriths features of the class DimInput and
 * adds QCheckBox object in order to let the user choice whether to
 * type additional information / dimension or not.
 */
class BoolDimInput : public DimInput
{
    Q_OBJECT
    private:

        // Functional members =======================================

        /*!
         * \brief Yes/no check box
         */
        QCheckBox *box;

        // Private methods ==========================================

        /*!
         * \brief Sets the horizontal layout
         */
        void setHorizontalLayout() override;
        /*!
         * \brief Reaction when the box is checked
         */
        void boxChecked();
        /*!
         * \brief Reaction when the box is unchecked
         */
        void boxUnchecked();
        /*!
         * \brief Check the text from the input box
         */
        void checkBoxData(const QString & new_data);

    public:
        /*!
         * \brief Initialization of the class BoolDimInput
         */
        BoolDimInput(QWidget *parent = nullptr);

    public slots:
        /*!
         * \brief Slot that reacts to the state change of the check box
         */
        void boxStateChanged(int state);
};

#endif // BOOLDIMINPUT_HPP
