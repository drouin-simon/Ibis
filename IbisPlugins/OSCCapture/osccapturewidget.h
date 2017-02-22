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

#ifndef __OSCCaptureWidget_h_
#define __OSCCaptureWidget_h_

#include <QWidget>

class OSCCapturePluginInterface;

namespace Ui
{
    class OSCCaptureWidget;
}


class OSCCaptureWidget : public QWidget
{

    Q_OBJECT

public:

    explicit OSCCaptureWidget(QWidget *parent = 0);
    ~OSCCaptureWidget();

    void SetPluginInterface( OSCCapturePluginInterface * pi );

private slots:

    void UpdateUi();

private:

    OSCCapturePluginInterface * m_pluginInterface;
    Ui::OSCCaptureWidget * ui;

};

#endif
