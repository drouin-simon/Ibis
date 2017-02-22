/*=========================================================================
Ibis Neuronav
Copyright (c) Simon Drouin, Anna Kochanowska, Louis Collins.
All rights reserved.
See Copyright.txt or http://ibisneuronav.org/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.
=========================================================================*/
// Thanks to Simon Drouin for writing this class

#include "osccapturewidget.h"
#include "ui_osccapturewidget.h"
#include "osccaptureplugininterface.h"

OSCCaptureWidget::OSCCaptureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OSCCaptureWidget)
{
    ui->setupUi(this);
}

OSCCaptureWidget::~OSCCaptureWidget()
{
    delete ui;
}

void OSCCaptureWidget::SetPluginInterface( OSCCapturePluginInterface * pi )
{
    m_pluginInterface = pi;
    connect( m_pluginInterface, SIGNAL(Modified()), this, SLOT(UpdateUi()) );
    UpdateUi();
}

void OSCCaptureWidget::UpdateUi()
{
    Q_ASSERT( m_pluginInterface );
    double * p = m_pluginInterface->GetTipPosition();
    QString pos = QString("Pointer Pos: ( %1, %2, %3 )").arg( p[0] ).arg( p[1] ).arg( p[2] );
    ui->pointerPosLabel->setText( pos );
}
