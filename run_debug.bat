@echo off
REM Configure Qt pour afficher les logs de débogage
set QT_DEBUG_PLUGINS=1
set QT_QPA_PLATFORM_PLUGIN_PATH=C:\Qt\6.7.3\mingw_64\plugins
set PATH=C:\Qt\6.7.3\mingw_64\bin;%PATH%

REM Lancer l'application et rediriger la sortie
cd /d "C:\Users\safab\OneDrive\Desktop\Smart_Reaseach_Publication_Management-main\build\Release"
ResearchManagementSystem.exe 2>&1

pause
