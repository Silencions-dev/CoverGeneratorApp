#ifndef STYLESPATHS_H
#define STYLESPATHS_H

/*!
 * \file
 * \brief Paths to the style sheets used in the application
 */

namespace styles
{
    /*!
     * \brief CSS-like style for the locked button (disabled)
     */
    constexpr const char *BUTTON_LOCKED = ":/button_stylesheet/styles/ButtonLocked.qss";
/*!
     * \brief CSS-like style for the unlocked button (enabled)
     */
    constexpr const char *BUTTON_UNLOCKED = ":/button_stylesheet/styles/ButtonUnlocked.qss";
    /*!
     * \brief CSS-like style for the correct data in QLineEdit
     */
    constexpr const char *EDIT_CORRECT = ":/lineedit_stylesheet/styles/LineEditCorrect.qss";
    /*!
     * \brief CSS-like style for the currently editing QLineEdit
     */
    constexpr const char *EDIT_NEUTRAL = ":/lineedit_stylesheet/styles/LineEditNeutral.qss";
    /*!
     * \brief CSS-like style for the disabled QLineEdit
     */
    constexpr const char *EDIT_DISABLED = ":/lineedit_stylesheet/styles/LineEditDisabled.qss";
    /*!
     * \brief CSS-like style for the wrong data in QLineEdit
     */
    constexpr const char *EDIT_WRONG = ":/lineedit_stylesheet/styles/LineEditWrong.qss";
    /*!
     * \brief Default CSS-like style for the QTextEdit
     */
    constexpr const char *EDIT_WIN_BASIC = ":/textedit_stylesheet/styles/TextEditStyle.qss";
}

#endif // STYLESPATHS_H
