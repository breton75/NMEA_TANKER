#ifndef TAGGSTATELABEL_H
#define TAGGSTATELABEL_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QString>

class TAggStateLabel
{
public:
    explicit TAggStateLabel(QString& caption, QWidget* parentWidget);
    ~TAggStateLabel();

    QHBoxLayout* hlay = nullptr;
    QLabel* lblCaption = nullptr;
    QLabel* lblState = nullptr;
};

#endif // TAGGSTATELABEL_H
