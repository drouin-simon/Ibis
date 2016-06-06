/*=========================================================================
Ibis Neuronav
Copyright (c) Simon Drouin, Anna Kochanowska, Louis Collins.
All rights reserved.
See Copyright.txt or http://ibisneuronav.org/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.
=========================================================================*/
#ifndef __GuiUtilities_h_
#define __GuiUtilities_h_

#include <QList>

class SceneObject;
class QComboBox;

class GuiUtilities
{

public:

    template< class ObjectType >
    static void UpdateSceneObjectComboBox( QComboBox * cb, QList<ObjectType*> allObjs, int currentObjectId )
    {
        cb->blockSignals( true );
        cb->clear();
        for( int i = 0; i < allObjs.size(); ++i )
        {
            cb->addItem( allObjs[i]->GetName(), QVariant( allObjs[i]->GetObjectID() ) );
            if( allObjs[i]->GetObjectID() == currentObjectId )
                cb->setCurrentIndex( i );
        }
        if( allObjs.size() == 0 )
            cb->addItem( "None", QVariant( SceneObject::InvalidObjectId ) );
        cb->blockSignals( false );
    }

    static int ObjectIdFromSceneObjectComboBox( QComboBox * cb, int index )
    {
        QVariant v = cb->itemData( index );
        bool ok = false;
        int objectId = v.toInt( &ok );
        if( !ok )
            objectId = SceneObject::InvalidObjectId;
        return objectId;
    }
};

#endif
