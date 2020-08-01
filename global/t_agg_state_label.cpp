#include "t_agg_state_label.h"

TAggStateLabel::TAggStateLabel(QString& caption, QWidget* parentWidget)
{
    hlay = new QHBoxLayout();
    hlay->setSpacing(6);
    hlay->setObjectName(QStringLiteral("hlay"));

    lblCaption = new QLabel(parentWidget);
    lblCaption->setObjectName(QStringLiteral("lblCaption"));
    lblCaption->setText(caption);

    lblState = new QLabel(parentWidget);
    lblState->setObjectName(QStringLiteral("lblState"));
    lblState->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    hlay->addWidget(lblCaption);
    hlay->addWidget(lblState);

}

TAggStateLabel::~TAggStateLabel()
{
    delete lblCaption;
    delete lblState;
    delete hlay;
}

