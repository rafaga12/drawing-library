REM C:\Users\Garay\Documents\Visual Studio 2012\Projects\EDrawingLibrary\EDrawingLibraryVSIX
REM C:\Users\Garay\Documents\Visual Studio 2012\Projects\EDrawingLibrary\EDrawingLibraryLicensing\bin\Release\
REM C:\Users\Garay\Documents\Visual Studio 2012\Projects\EDrawingLibrary\x64\Release\EDrawingLibrary\
REM C:\Users\Garay\Documents\Visual Studio 2012\Projects\EDrawingLibrary\ARM\Release\EDrawingLibrary\
REM C:\Users\Garay\Documents\Visual Studio 2012\Projects\EDrawingLibrary\Release\EDrawingLibrary\

REM echo %1

echo Copy Headers
copy /Y %1..\EDrawingLibrary\*.h %1DesignTime\CommonConfiguration\Neutral\Include\
del %1DesignTime\CommonConfiguration\Neutral\Include\pch.h
del %1DesignTime\CommonConfiguration\Neutral\Include\NamespaceDoc.h
del %1DesignTime\CommonConfiguration\Neutral\Include\Drawing2DNamespaceDoc.h

echo Copy Libs
copy /Y %1..\Release\EDrawingLibrary\ModernComponents.System.Drawing.lib %1DesignTime\CommonConfiguration\x86\
copy /Y %1..\x64\Release\EDrawingLibrary\ModernComponents.System.Drawing.lib %1DesignTime\CommonConfiguration\x64\
copy /Y %1..\ARM\Release\EDrawingLibrary\ModernComponents.System.Drawing.lib %1DesignTime\CommonConfiguration\ARM\

echo Copy Redistributables
copy /Y %1..\Release\EDrawingLibrary\ModernComponents.System.Drawing.dll %1Redist\CommonConfiguration\x86\
copy /Y %1..\x64\Release\EDrawingLibrary\ModernComponents.System.Drawing.dll %1Redist\CommonConfiguration\x64\
copy /Y %1..\ARM\Release\EDrawingLibrary\ModernComponents.System.Drawing.dll %1Redist\CommonConfiguration\ARM\

echo Copy References
copy /Y %1..\Release\EDrawingLibrary\ModernComponents.System.Drawing.pri %1References\CommonConfiguration\Neutral\
copy /Y %1..\Release\EDrawingLibrary\ModernComponents.System.Drawing.winmd %1References\CommonConfiguration\Neutral\
