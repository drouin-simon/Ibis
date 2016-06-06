/*=========================================================================
Ibis Neuronav
Copyright (c) Simon Drouin, Anna Kochanowska, Louis Collins.
All rights reserved.
See Copyright.txt or http://ibisneuronav.org/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.
=========================================================================*/
#include "volumerenderingsinglevolumesettingswidget.h"
#include "ui_volumerenderingsinglevolumesettingswidget.h"
#include "scenemanager.h"
#include "volumerenderingobject.h"
#include "vtkVolumeProperty.h"
#include "imageobject.h"
#include "volumeshadereditorwidget.h"

VolumeRenderingSingleVolumeSettingsWidget::VolumeRenderingSingleVolumeSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VolumeRenderingSingleVolumeSettingsWidget)
{
    m_sceneManager = 0;
    m_volumeIndex = -1;
    ui->setupUi(this);
    m_shaderWidget = 0;
}

VolumeRenderingSingleVolumeSettingsWidget::~VolumeRenderingSingleVolumeSettingsWidget()
{
    if( m_shaderWidget )
        m_shaderWidget->close();
    delete ui;
}

void VolumeRenderingSingleVolumeSettingsWidget::SetSceneManager( SceneManager * man, int volumeIndex )
{
    m_sceneManager = man ;
    m_volumeIndex = volumeIndex;
    vtkVolumeProperty * vp = m_sceneManager->GetMainVolumeRenderer()->GetVolumeProperty( m_volumeIndex );
    ui->colorFunctionWidget->SetColorTransferFunction( vp->GetRGBTransferFunction() );
    ui->colorFunctionWidget->SetXRange( 0.0, 255.0 );
    ui->opacityFunctionWidget->SetFunction( vp->GetScalarOpacity() );
    ui->opacityFunctionWidget->SetYRange( 0.0, 1.0 );
    UpdateUi();
}

void VolumeRenderingSingleVolumeSettingsWidget::UpdateUi()
{
    Q_ASSERT( m_sceneManager );

    ui->enableVolumeCheckBox->blockSignals( true );
    ui->volumeComboBox->blockSignals( true );

    // Update enable checkbox
    ui->enableVolumeCheckBox->setChecked( m_sceneManager->GetMainVolumeRenderer()->IsVolumeEnabled( m_volumeIndex ) );

    // Update use 16 bits volume
    ui->use16BitsVolumeCheckBox->blockSignals( true );
    ui->use16BitsVolumeCheckBox->setChecked( m_sceneManager->GetMainVolumeRenderer()->GetUse16BitsVolume( m_volumeIndex ) );
    ui->use16BitsVolumeCheckBox->blockSignals( false );

    // Update use linear sampling checkbox
    ui->useLinearSamplingCheckBox->blockSignals( true );
    ui->useLinearSamplingCheckBox->setChecked( m_sceneManager->GetMainVolumeRenderer()->GetUseLinearSampling( m_volumeIndex ) );
    ui->useLinearSamplingCheckBox->blockSignals( false );

    // Update image object combo box
    ui->volumeComboBox->clear();
    QList< ImageObject* > allObjects;
    m_sceneManager->GetAllImageObjects( allObjects );
    ImageObject * currentImage = m_sceneManager->GetMainVolumeRenderer()->GetImage( m_volumeIndex );
    bool none = true;
    for( int i = 0; i < allObjects.size(); ++i )
    {
        ImageObject * im = allObjects[i];
        ui->volumeComboBox->addItem( im->GetName(), QVariant( im->GetObjectID() ) );
        if( im == currentImage )
        {
            ui->volumeComboBox->setCurrentIndex( i );
            ui->enableVolumeCheckBox->setEnabled( true );
            none = false;
        }
    }
    ui->volumeComboBox->addItem( "None", QVariant((int)-1) );
    if( none )
    {
        ui->volumeComboBox->setCurrentIndex( allObjects.size() );
        ui->enableVolumeCheckBox->setEnabled( false );
    }

    ui->enableVolumeCheckBox->blockSignals( false );
    ui->volumeComboBox->blockSignals( false );

    // Update shader contribution combo box
    ui->contributionTypeComboBox->blockSignals( true );
    ui->contributionTypeComboBox->clear();
    VolumeRenderingObject * vr = m_sceneManager->GetMainVolumeRenderer();
    for( int i = 0; i < vr->GetNumberOfShaderContributionTypes(); ++i )
    {
        ui->contributionTypeComboBox->addItem( QString( vr->GetShaderContributionTypeName( i ) ) );
    }
    ui->contributionTypeComboBox->setCurrentIndex( vr->GetShaderContributionType( m_volumeIndex ) );
    ui->contributionTypeComboBox->blockSignals( false );

    // make sure we repaint transfer func widgets
    ui->colorFunctionWidget->update();
    ui->opacityFunctionWidget->update();
}

void VolumeRenderingSingleVolumeSettingsWidget::on_enableVolumeCheckBox_toggled(bool checked)
{
    Q_ASSERT( m_sceneManager );
    m_sceneManager->GetMainVolumeRenderer()->EnableVolume( m_volumeIndex, checked );
}

void VolumeRenderingSingleVolumeSettingsWidget::on_volumeComboBox_currentIndexChanged( int index )
{
    int objectId = ui->volumeComboBox->itemData( index ).toInt();
    ImageObject * newObject = 0;
    if( objectId != SceneObject::InvalidObjectId )
        newObject =  ImageObject::SafeDownCast( m_sceneManager->GetObjectByID( objectId ) );
    m_sceneManager->GetMainVolumeRenderer()->SetImage( m_volumeIndex, newObject );
    UpdateUi();
}

void VolumeRenderingSingleVolumeSettingsWidget::on_contributionTypeComboBox_currentIndexChanged(int index)
{
    Q_ASSERT( m_sceneManager );
    m_sceneManager->GetMainVolumeRenderer()->SetShaderContributionType( m_volumeIndex, index );
}

void VolumeRenderingSingleVolumeSettingsWidget::OnVolumeShaderEditorWidgetClosed()
{
    m_shaderWidget = 0;
    ui->shaderPushButton->blockSignals( true );
    ui->shaderPushButton->setChecked( false );
    ui->shaderPushButton->blockSignals( false );
}

void VolumeRenderingSingleVolumeSettingsWidget::on_shaderPushButton_toggled( bool checked )
{
    if( checked )
    {
        Q_ASSERT( m_shaderWidget == 0 );
        m_shaderWidget = new VolumeShaderEditorWidget( 0 );
        m_shaderWidget->setAttribute( Qt::WA_DeleteOnClose );
        m_shaderWidget->SetVolumeRenderer( m_sceneManager->GetMainVolumeRenderer(), m_volumeIndex );
        connect( m_shaderWidget, SIGNAL(destroyed()), this, SLOT(OnVolumeShaderEditorWidgetClosed()) );
        m_shaderWidget->show();
    }
    else
    {
        Q_ASSERT( m_shaderWidget );
        m_shaderWidget->close();
    }
}

void VolumeRenderingSingleVolumeSettingsWidget::on_use16BitsVolumeCheckBox_toggled( bool checked )
{
    Q_ASSERT( m_sceneManager );
    m_sceneManager->GetMainVolumeRenderer()->SetUse16BitsVolume( m_volumeIndex, checked );
}

void VolumeRenderingSingleVolumeSettingsWidget::on_useLinearSamplingCheckBox_toggled(bool checked)
{
    Q_ASSERT( m_sceneManager );
    m_sceneManager->GetMainVolumeRenderer()->SetUseLinearSampling( m_volumeIndex, checked );
}

void VolumeRenderingSingleVolumeSettingsWidget::on_addShaderContribTypeButton_clicked()
{
    Q_ASSERT( m_sceneManager );
    VolumeRenderingObject * volRen = m_sceneManager->GetMainVolumeRenderer();
    int contribType = volRen->GetShaderContributionType( m_volumeIndex );
    volRen->DuplicateShaderContribType( contribType );
    volRen->SetShaderContributionType( m_volumeIndex, volRen->GetNumberOfShaderContributionTypes() - 1 );
    UpdateUi();
}
