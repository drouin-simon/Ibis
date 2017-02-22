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

#ifndef __OSCCapturePluginInterface_h_
#define __OSCCapturePluginInterface_h_

#include <QObject>
#include "toolplugininterface.h"

class OSCCapturePluginInterface : public QObject, public ToolPluginInterface
{

    Q_OBJECT
    Q_INTERFACES(IbisPlugin)
    Q_PLUGIN_METADATA(IID "Ibis.OSCCapturePluginInterface" )

public:

    OSCCapturePluginInterface();
    virtual ~OSCCapturePluginInterface();
    virtual QString GetPluginName() { return QString("OSCCapture"); }
    virtual bool CanRun();
    virtual QString GetMenuEntryString() { return QString("OSC Capture"); }

    virtual QWidget * CreateTab();
    virtual bool WidgetAboutToClose();

    double * GetTipPosition() { return m_tipPosition; }

private slots:

    void OnUpdate();

signals:

    void Modified();

protected:

    double m_tipPosition[3];
};

#endif
