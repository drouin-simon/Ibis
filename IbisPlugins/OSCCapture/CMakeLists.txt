# define sources
set( PluginSrc osccaptureplugininterface.cpp osccapturewidget.cpp )
set( PluginHdrMoc osccapturewidget.h osccaptureplugininterface.h )
set( PluginUi osccapturewidget.ui )

# Create plugin
DefinePlugin( "${PluginSrc}" "${PluginHdr}" "${PluginHdrMoc}" "${PluginUi}" )
