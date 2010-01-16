; languages.nsi
;
; This is an example of a multilingual installer
; The user can select the language on startup

;--------------------------------

OutFile FlickrPhotos.exe
Name FlickrPhotos
InstallDir $PROGRAMFILES\FlickrPhotos

RequestExecutionLevel admin

;--------------------------------

Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; First is default
LoadLanguageFile "${NSISDIR}\Contrib\Language files\English.nlf"
LoadLanguageFile "${NSISDIR}\Contrib\Language files\Dutch.nlf"
LoadLanguageFile "${NSISDIR}\Contrib\Language files\German.nlf"

; Set name using the normal interface (Name command)
LangString Name ${LANG_ENGLISH} "English"
LangString Name ${LANG_DUTCH} "Dutch"
LangString Name ${LANG_GERMAN} "German"

; Directly change the inner lang strings (Same as ComponentText)
LangString LangFile ${LANG_DUTCH} "flickrphotos_nl.qm"
LangString LangFile ${LANG_GERMAN} "flickrphotos_de.qm"
;--------------------------------


Function .onInit

	;Language selection dialog

	Push ""
	Push ${LANG_ENGLISH}
	Push English
	Push ${LANG_DUTCH}
	Push Nederlands
	Push ${LANG_GERMAN}
	Push Deutsch
	Push A ; A means auto count languages
	       ; for the auto count to work the first empty push (Push "") must remain
	LangDLL::LangDialog "Installer Language" "Please select the language of the installer"

	Pop $LANGUAGE
	StrCmp $LANGUAGE "cancel" 0 +2
		Abort
FunctionEnd

; The stuff to install
Section "FlickrPhotos" 

  ; user cannot change this section
  SectionIn RO

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File ..\release\FlickrPhotos.exe
  File files\QtCore4.dll
  File files\QtGui4.dll
  File files\QtXml4.dll
  File files\QtNetwork4.dll
  File files\mingwm10.dll
  File files\libgcc_s_dw2-1.dll
  StrCmp $LANGUAGE ${LANG_DUTCH} 0 +2
    File /oname=flickrphotos.qm ..\flickrphotos_nl.qm
  StrCmp $LANGUAGE ${LANG_GERMAN} 0 +2
    File /oname=flickrphotos.qm ..\flickrphotos_de.qm
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\FlickrPhotos "Install_Dir" "$INSTDIR"

  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FlickrPhotos" "DisplayName" "FlickrPhotos"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FlickrPhotos" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FlickrPhotos" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FlickrPhotos" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd ; end the section

Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\FlickrPhotos"
  CreateShortCut "$SMPROGRAMS\FlickrPhotos\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\FlickrPhotos\FlickrPhotos.lnk" "$INSTDIR\FlickrPhotos.exe" "" "$INSTDIR\FlickrPhotos.exe" 0
  
SectionEnd

Section "Desktop Shortcuts"
  CreateShortCut "$DESKTOP\FlickrPhotos.lnk" "$INSTDIR\FlickrPhotos.exe" "" "$INSTDIR\FlickrPhotos.exe" 0
SectionEnd

; Uninstaller

Section "Uninstall"
  
  ReadRegStr $INSTDIR HKLM Software\FlickrPhotos "Install_Dir"

  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FlickrPhotos"

  ; Delete files
  Delete "$INSTDIR\*.*"
  
  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\FlickrPhotos\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\FlickrPhotos"
  RMDir "$INSTDIR"

SectionEnd

