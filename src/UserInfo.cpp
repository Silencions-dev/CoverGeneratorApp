#include "UserInfo.hpp"

UserInfo::UserInfo(const char *widget_title, QWidget *parent) : FramedWidget(widget_title, parent)
{
    for(uint8_t i = 0; i < INFO_SIZE; ++i)
    {
        this->info_name[i] = new QLabel(this);
        this->input[i] = new QLineEdit(this);
    }

    this->columns_layout = new QHBoxLayout();
    this->label_layout = new QVBoxLayout();
    this->input_layout = new QVBoxLayout();

    this->setMinimumHeight(MIN_HEIGHT);

    this->setWidgets();
    this->setLayout();
}

void UserInfo::setWidgets()
{
    this->info_name[0]->setText("Adres email:");
    this->info_name[1]->setText("Numer telefonu:");
    this->info_name[0]->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    this->info_name[1]->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    this->setLineEdit(this->input[0]);
    this->setLineEdit(this->input[1]);
}

void UserInfo::setLayout()
{
    for(uint8_t i = 0; i < INFO_SIZE; ++i)
    {
        this->info_name[i]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        this->input[i]->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        this->label_layout->addWidget(this->info_name[i]);
        this->input_layout->addWidget(this->input[i]);
    }
    this->label_layout->setAlignment(Qt::AlignRight);
    this->input_layout->setAlignment(Qt::AlignLeft);
    this->columns_layout->addLayout(this->label_layout);
    this->columns_layout->addLayout(this->input_layout);
    this->columns_layout->setAlignment(Qt::AlignCenter);

    this->title->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->v_layout->addWidget(this->title);
    this->v_layout->addItem(new QSpacerItem(1, 10, QSizePolicy::Minimum, QSizePolicy::Preferred));
    this->v_layout->addLayout(this->columns_layout);
}

void UserInfo::setLineEdit(QLineEdit *input)
{
    input->setReadOnly(false);
    input->setFrame(true);
    input->setTextMargins(1, 1, 1, 1);
    input->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    input->setMaxLength(TEXT_LIMIT);
    input->setPlaceholderText("");

    changeStyle(input, ":/lineedit_stylesheet/LineEditNeutral.qss");
}
