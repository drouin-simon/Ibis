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

#ifndef __LabelVolumeToSurfacesPluginInterface_h_
#define __LabelVolumeToSurfacesPluginInterface_h_

#include <QObject>
#include "objectplugininterface.h"
#include "serializer.h"

class LabelVolumeToSurfacesPluginInterface : public QObject, public ObjectPluginInterface
{

    Q_OBJECT
    Q_INTERFACES(ObjectPluginInterface)
    Q_PLUGIN_METADATA(IID "Ibis.LabelVolumeToSurfacesPluginInterface" )

public:

    LabelVolumeToSurfacesPluginInterface();
    ~LabelVolumeToSurfacesPluginInterface();
    QString GetMenuEntryString() { return QString("Label volume to surfaces"); }
    void CreateObject();
    QString GetObjectClassName() { return "PolyDataObject"; }
};

#endif
