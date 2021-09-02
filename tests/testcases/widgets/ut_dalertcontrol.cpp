/*
 * Copyright (C) 2021 ~ 2021 Deepin Technology Co., Ltd.
 *
 * Author:     Wang Peng <wangpenga@uniontech.com>
 *
 * Maintainer: Wang Peng <wangpenga@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtest/gtest.h>
#include "dalertcontrol.h"
#include "private/dalertcontrol_p.h"
#include "DLineEdit"
#include "DWidget"
#include <QApplication>

DWIDGET_USE_NAMESPACE

class ut_DAlertcontrol : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;

    DWidget *widget = nullptr;
    DLineEdit *lineEdit = nullptr;
    DAlertControl *control = nullptr;
};

void ut_DAlertcontrol::SetUp()
{
    widget = new DWidget(nullptr);
    lineEdit = new DLineEdit(widget);
    control = new DAlertControl(lineEdit, widget);

    widget->resize(320, 280);
    lineEdit->resize(180, 80);
    lineEdit->move(40, 10);
}

void ut_DAlertcontrol::TearDown()
{
    control->deleteLater();
    lineEdit->deleteLater();
    widget->deleteLater();
}

TEST_F(ut_DAlertcontrol, showALertMessage)
{
    // 测试 LineEdit 的文字内容是否设置成功
    QString testStr = QStringLiteral("xxxxxxxxxxxxxxxxxxxxxxxxxx");

    lineEdit->setText(testStr);
    ASSERT_EQ(lineEdit->text(), testStr);

    // 测试 showAlertMessage 是否生效
    control->showAlertMessage(testStr);
    ASSERT_EQ(control->d_func()->tooltip->text(), testStr);

    control->hideAlertMessage();
    ASSERT_EQ(control->d_func()->frame->isVisible(), false);

    // 测试LineEdit里面设置的警告色是否正常，setAlert 是否生效
    control->setAlert(false);
    ASSERT_EQ(control->isAlert(), false);

    control->setAlertColor(Qt::red);
    control->setAlert(true);
    ASSERT_EQ(control->isAlert(), true);
    ASSERT_EQ(QColor(Qt::red) == control->alertColor(), true);

    // 默认左对齐，测试左右对齐
    control->setMessageAlignment(Qt::AlignLeft);
    ASSERT_EQ(control->messageAlignment(), Qt::AlignLeft);
    control->setMessageAlignment(Qt::AlignRight);
    ASSERT_EQ(control->messageAlignment(), Qt::AlignRight);

    // 默认左对齐，测试垂直居中、居中对齐
    control->setMessageAlignment(Qt::AlignHCenter);
    ASSERT_EQ(control->messageAlignment(), Qt::AlignHCenter);
    control->setMessageAlignment(Qt::AlignCenter);
    ASSERT_EQ(control->messageAlignment(), Qt::AlignCenter);

    // 测试输入文本后通过信号槽获取内容
    QObject::connect(lineEdit, &DLineEdit::textChanged, widget, [&](const QString &text){
        control->showAlertMessage(lineEdit->text());
        ASSERT_EQ(text, testStr);
    });

    emit lineEdit->textChanged(testStr);
}
