/*=========================================================================
Ibis Neuronav
Copyright (c) Simon Drouin, Anna Kochanowska, Louis Collins.
All rights reserved.
See Copyright.txt or http://ibisneuronav.org/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.
=========================================================================*/
#include <iostream>
#include <QApplication>
#include <QTimer>
#include <QtPlugin>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include "application.h"
#include "mainwindow.h"
#include "ignsconfig.h"
#include "commandlinearguments.h"
#include "hardwaresettings.h"
#include "vtkObject.h"

QString GetHardwareConfigFile( CommandLineArguments & cmdArgs );
QString GetSystemTypeString();

int main( int argc, char** argv )
{
    // Disable VTK warnings unless not wanted
#ifdef VTK_NO_WARNINGS
    vtkObject::SetGlobalWarningDisplay( 0 );
#endif

    // Create Qt app
    QApplication a( argc, argv );
    Q_INIT_RESOURCE(IbisLib);

    // Warning : IBIS IS NOT APPROVED FOR CLINICAL USE.
    if( !QFile::exists( QDir::homePath() + QString("/.ibis/no-clinical-warning.txt")  ) )
        QMessageBox::warning( 0, "WARNING!", QString("The Ibis platform is not approved for clinical use.") );
	
	// On Mac, we always do Viewer-mode only without command-line params for now
	if( GetSystemTypeString() == "Mac" )
		Application::CreateInstance( true );
	else 
	{
		// Parse command-line arguments
		CommandLineArguments cmdArgs;
		QStringList args = a.arguments();
		cmdArgs.ParseArguments( args );

		// Create unique instance of application
		Application::CreateInstance( cmdArgs.GetViewerOnly() );

		// Tell the application to load files specified on the command line immediately after startup
		Application::GetInstance().SetInitialDataFiles( cmdArgs.GetDataFilesToLoad() );

		// Initialize Hardware if not in viewer-only mode
		if( !cmdArgs.GetViewerOnly() )
		{
			if( cmdArgs.GetLoadDefaultConfig() )
				Application::GetInstance().InitHardware();
			else
			{
				QString configFile = GetHardwareConfigFile( cmdArgs );
				if( configFile.isEmpty() )
					exit(0);
                Application::GetInstance().InitHardware( configFile.toUtf8().data() );
			}
		}
	}

    // Application is initialized properly. Now we can load plugins
    Application::GetInstance().LoadPlugins();

    // Create main window
    MainWindow * mw = new MainWindow( 0 );
    mw->setAttribute( Qt::WA_DeleteOnClose );
    mw->show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
	a.installEventFilter( mw );

    // Will cause OnStartMainLoop slot to be called after main loop is started
    QTimer::singleShot( 0, mw, SLOT(OnStartMainLoop()) );

    // Start main loop
    int ret = a.exec();

    // Delete application instance explicitly to make sure qsettings are written before QApplication is destroyed
    Application::DeleteInstance();
    return ret;
}


QString GetHardwareConfigFile( CommandLineArguments & cmdArgs )
{
    QString configFile;
    if( cmdArgs.GetLoadPrevConfig() )
        configFile = Application::GetInstance().GetSettings()->LastConfigFile;
    else if( cmdArgs.GetLoadConfigFile() )
        configFile = cmdArgs.GetConfigFile();
    QFileInfo info( configFile );
    if( !info.exists() || !info.isReadable() )
    {
        QString initialChoice = Application::GetInstance().GetSettings()->LastConfigFile;
        HardwareSettings * hwSettingsDlg = new HardwareSettings( initialChoice );
        if( hwSettingsDlg->exec() == QDialog::Accepted )
            configFile = hwSettingsDlg->GetSelectedHardwareSettingsPath();
        delete hwSettingsDlg;
    }
    return configFile;
}

#include <QtGlobal>

QString GetSystemTypeString()
{
#ifdef Q_OS_LINUX
    return QString("Linux");
#endif
	
#ifdef Q_OS_OSX
    return QString("Mac");
#endif
	
    return QString("Unsupported");
}

