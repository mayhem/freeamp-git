/*____________________________________________________________________________

   FreeAmp - The Free MP3 Player

   Copyright (C) 1999 EMusic

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   $Id$
____________________________________________________________________________*/ 

// The debugger can't handle symbols more than 255 characters long.
// STL often creates symbols longer than that.
// When symbols are longer than 255 characters, the warning is disabled.
#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#define _S_IFDIR S_IFDIR
#define _stat stat
#endif
#include <map>
#include <algorithm>
#include <assert.h>
#include "Theme.h"
#include "ButtonControl.h"
#include "DialControl.h"
#include "SliderControl.h"
#include "VSliderControl.h"
#include "TextControl.h"
#include "MultiStateControl.h"
#include "ThemeZip.h"
#include "MessageDialog.h"
#include "debug.h"
#include "config.h"

#define DB Debug_v("%s:%d\n", __FILE__, __LINE__);

#ifdef WIN32
#include <direct.h>
#define stat(a,b) _stat(a,b)
#define rmdir(a) _rmdir(a)
#define MKDIR(z) mkdir(z)
#else
#define MKDIR(z) mkdir(z, 0755)
#endif

#ifdef WIN32
#include "Win32Window.h"
#include "Win32Bitmap.h"
#include "Win32Font.h"
#elif defined(HAVE_GTK)
#include "GTKWindow.h"
#include "GTKBitmap.h"
#include "GTKFont.h"
#include "GTKUtility.h"
#elif defined(__BEOS__)
#include "BeOSWindow.h"
#include "BeOSBitmap.h"
#include "BeOSFont.h"
#endif

const int iThemeVersionMajor = 1;
const int iThemeVersionMinor = 0;
const char *szThemeNotFoundError = "Cannot open the theme specified in the preferences. Using default theme instead.";
const char *szThemeUnzipError = "Cannot unzip the specified theme. Using default theme instead.";
const char *szCannotCreateTempDirError = "Cannot create a temporary directory to unzip theme.";

Theme::Theme(FAContext *context)
{
    m_pContext = context;
    m_pCurrentWindow = NULL;
    m_pCurrentControl = NULL;
    m_pParsedWindows = m_pWindows = NULL;
    m_pParsedBitmaps = m_pBitmaps = NULL;
    m_pParsedFonts = m_pFonts = NULL;
    m_bReloadTheme = false;
    m_bReloadWindow = false;
    m_oReloadWindow = string("");
    m_eCurrentControl = eUndefinedControl;
    m_pThemeMan = new ThemeManager(m_pContext);
    m_bThemeLoaded = false;
    m_bCreditsShown = false;
    
    string funkyName = "Frunobulax"; 
#ifdef WIN32
    m_pWindow = new Win32Window(this, funkyName);
#elif defined(HAVE_GTK)
    InitializeGTK(context);    
    m_pWindow = new GTKWindow(this, funkyName);
#elif defined(__BEOS__)
    m_pWindow = new BeOSWindow(this, funkyName);
#endif
    m_pWindow->VulcanMindMeldHost(true);
}

Theme::~Theme(void)
{
    delete m_pCurrentWindow;
    delete m_pCurrentControl;
    delete m_pThemeMan;
    delete m_pWindow;
    
    ClearWindows();
    ClearBitmaps();
    ClearFonts();

#ifdef HAVE_GTK
    ShutdownGTK();
#endif
}

void Theme::ClearWindows(void)
{
	if (m_pWindows)
    {
       while(m_pWindows->size() > 0)
       {
          delete (*m_pWindows)[0];
          m_pWindows->erase(m_pWindows->begin());
       }
       delete m_pWindows;
       m_pWindows = NULL;
    }   
}

void Theme::ClearBitmaps(void)
{  
  	if (m_pBitmaps)
    {
       while(m_pBitmaps->size() > 0)
       {
          delete (*m_pBitmaps)[0];
          m_pBitmaps->erase(m_pBitmaps->begin());
       }
       delete m_pBitmaps;
       m_pBitmaps = NULL;
    }
}

void Theme::ClearFonts(void)
{  
  	if (m_pFonts)
    {
       while(m_pFonts->size() > 0)
       {
          delete (*m_pFonts)[0];
          m_pFonts->erase(m_pFonts->begin());
       }
       delete m_pFonts;
       m_pFonts = NULL;
    }
}

void Theme::SetThemePath(string &oThemePath)
{
	m_oThemePath = oThemePath + string(DIR_MARKER_STR);
}

Error Theme::LoadTheme(string &oFile, string &oWindowName)
{
    char    *pTemp;
    ThemeZip oZip;
    string   oCompleteFile, oTempPath, oDefaultPath;
    Error    eRet;
    struct   _stat buf;

    if (_stat(oFile.c_str(), &buf) == 0 && (buf.st_mode & _S_IFDIR))
    {
        SetThemePath(oFile);
        oCompleteFile = oFile + string(DIR_MARKER_STR) + string("theme.xml");
        eRet = Parse::ParseFile(oCompleteFile);
    }
    else
    {
        pTemp = tempnam(NULL, "fat");
        oTempPath = pTemp;
        free(pTemp);
        if (MKDIR(oTempPath.c_str()))
        {
            MessageDialog oBox(m_pContext);
            string        oErr, oMessage(szCannotCreateTempDirError);

            oBox.Show(oMessage.c_str(), string(BRANDING), kMessageOk);
            return kError_InvalidParam;
        }    
        SetThemePath(oTempPath);
	 
        eRet = oZip.DecompressThemeZip(oFile, oTempPath);
        if (eRet == kError_FileNotFound)
        {
            string oDefaultPath;
        
            if (oFile.length() > 0)
            {
                MessageDialog oBox(m_pContext);
       
                string        oErr, oMessage(szThemeNotFoundError);

                oBox.Show(oMessage.c_str(), string(BRANDING), kMessageOk);
            }
       
            m_pThemeMan->GetDefaultTheme(oFile);
            eRet = oZip.DecompressThemeZip(oFile, oTempPath);
            if (IsError(eRet))
            {
                m_oLastError = "Cannot find default theme";
                rmdir(oTempPath.c_str());
                return eRet;
            }    
        }
        if (IsError(eRet))
        {
            MessageDialog oBox(m_pContext);
            string        oErr, oMessage(szThemeUnzipError);

            oBox.Show(oMessage.c_str(), string(BRANDING), kMessageOk);
            return kError_InvalidParam;
        }    

        oCompleteFile = oTempPath + string(DIR_MARKER_STR) 
	                + string("theme.xml");
        eRet = Parse::ParseFile(oCompleteFile);
        oZip.CleanupThemeZip();
        rmdir(oTempPath.c_str());
    }    

    if (!IsError(eRet))
    {
       string                      oTemp;
       vector<Window *>::iterator  i;
       Window                     *pMainWindow, *pNewWindow = NULL; 
      
       // Is this a reload, as opposed to a new load?
       if (m_pWindows)
       {
          // Clear the current window list
          ClearWindows();

          // Adopt the new window vector
          m_pWindows = m_pParsedWindows;

          // Select the new window, based on the passed in name
          for(i = m_pWindows->begin(); i != m_pWindows->end(); i++)
          {
              (*i)->GetName(oTemp);
              if (oTemp == oWindowName)
              {
              	  pNewWindow = *i;
                  break;
              }    
              if (oTemp == string("MainWindow"))
              	  pMainWindow = *i;
          }

	      // If we can't find the proper mode, switch to mainwindow
          if (!pNewWindow)
             pNewWindow = pMainWindow;
          
          // Clear out all the old bitmaps and fonts
          ClearBitmaps();
          ClearFonts();

          // Accept the new bitmaps and font.
          m_pBitmaps = m_pParsedBitmaps;
          m_pFonts = m_pParsedFonts;
          m_pParsedWindows = NULL;
          m_pParsedBitmaps = NULL;
          m_pParsedFonts = NULL;
         
          // Now the window lists have been properly adjusted, so
          // adopt all the info from the new window into the existing
          // window via the VulcanMindLink 
          m_pWindow->VulcanMindMeld(pNewWindow);
          // And if god doesn't stike me down right now,
          // everything *should* be fine.
       }
       else
       {
          // Its a new load -- just take things verbatim and run
          m_pWindows = m_pParsedWindows;
          m_pBitmaps = m_pParsedBitmaps;
          m_pFonts = m_pParsedFonts;
          m_pParsedWindows = NULL;
          m_pParsedBitmaps = NULL;
          m_pParsedFonts = NULL;

          for(i = m_pWindows->begin(); i != m_pWindows->end(); i++)
          {
              (*i)->GetName(oTemp);
              if (oTemp == oWindowName)
              {
              	  pNewWindow = *i;
                  break;
              }    
              if (oTemp == string("MainWindow"))
              	  pMainWindow = *i;
          }
          if (!pNewWindow)
             pNewWindow = pMainWindow;
          m_pWindow->VulcanMindMeld(pNewWindow);
       }   
       m_bThemeLoaded = true;
    }   
       
    return eRet;
}

Error Theme::SwitchWindow(const string &oWindowName)
{
	string                      oTemp;
    vector<Window *>::iterator  i;
    Window                     *pMainWindow, *pNewWindow = NULL; 
    
    for(i = m_pWindows->begin(); i != m_pWindows->end(); i++)
    {
        (*i)->GetName(oTemp);
        if (oTemp == oWindowName)
        {
            pNewWindow = *i;
            break;
        }    
        if (oTemp == string("MainWindow"))
        	  pMainWindow = *i;
    }
    if (!pNewWindow)
       pNewWindow = pMainWindow;

    return m_pWindow->VulcanMindMeld(pNewWindow);
}

Error Theme::Run(Pos &oWindowPos)
{
    if (!m_bThemeLoaded)
       return kError_YouScrewedUp;
       
    InitWindow();
    return m_pWindow->Run(oWindowPos);
}

Error Theme::Close(void)
{
    if (m_pWindow)
       return m_pWindow->Close();
    else
       return kError_NoErr;
}

Error Theme::BeginElement(string &oElement, AttrMap &oAttrMap)
{
    Error  eRet;
    string oCompleteFile;

    if (oElement == string("Version"))
    {
       int iRet, iVersionMajor, iVersionMinor;

       iRet = sscanf(oAttrMap["Version"].c_str(), " %d.%d", &iVersionMajor,
                                                    &iVersionMinor);
       if (iRet != 2)
       {
           m_oLastError = string("Improperly formatted version number "
                                 "in the <Version> tag");
           return kError_ParseError;
       }

       if (iVersionMajor != iThemeVersionMajor ||
           iVersionMinor != iThemeVersionMinor)
       {
           char szError[512];

           
           sprintf(szError, "This ThemeManager cannot use version %d.%d "
                            "themes", iVersionMajor, iVersionMinor);
           m_oLastError = string(szError);

           return kError_ParseError;
       }

       return kError_NoErr;
    }

    if (oElement == string("Bitmap"))
    {
       Bitmap *pBitmap;
       Color   oColor;

#ifdef WIN32
       pBitmap = new Win32Bitmap(oAttrMap["Name"]);
#elif defined(HAVE_GTK)
       pBitmap = new GTKBitmap(oAttrMap["Name"]);
#elif defined(__BEOS__)
       pBitmap = new BeOSBitmap(oAttrMap["Name"]);
#endif

       if (oAttrMap.find("TransColor") != oAttrMap.end())
       {
           eRet = ParseColor(oAttrMap["TransColor"], oColor);
           if (eRet == kError_NoErr)
               pBitmap->SetTransColor(oColor);
       }

           
       if (oAttrMap.find("File") == oAttrMap.end())
       {
           m_oLastError = string("the <Bitmap> tag needs a File attribute");
           return kError_ParseError;
       }        

       oCompleteFile = m_oThemePath + oAttrMap["File"];
       eRet = pBitmap->LoadBitmapFromDisk(oCompleteFile);
       if (eRet != kError_NoErr)
       {
           string oBitmapErr;

           pBitmap->GetErrorString(oBitmapErr);
           m_oLastError = string("Cannot load bitmap ") +
                          oAttrMap["File"] + string(": ") + oBitmapErr;
           return eRet;
       }

       if (!m_pParsedBitmaps)
           m_pParsedBitmaps = new vector<Bitmap *>;
           
       m_pParsedBitmaps->push_back(pBitmap);

       return kError_NoErr;
    }
    if (oElement == string("Font"))
    {
       Font *pFont;

	   if (oAttrMap.find("Name") == oAttrMap.end())
       {
           m_oLastError = string("the <Font> tag needs a Name attribute");
           return kError_ParseError;
       }        
	   if (oAttrMap.find("Face") == oAttrMap.end())
       {
           m_oLastError = string("the <Font> tag needs a Face attribute");
           return kError_ParseError;
       }        

#ifdef WIN32
       pFont = new Win32Font(oAttrMap["Name"], oAttrMap["Face"], m_oDefaultFont);
#elif defined (HAVE_GTK)
       pFont = new GTKFont(m_pContext, oAttrMap["Name"], oAttrMap["Face"], 
                           m_oDefaultFont);
#elif defined (__BEOS__)
       pFont = new BeOSFont(oAttrMap["Name"], oAttrMap["Face"], m_oDefaultFont);
#endif
       if (!m_pParsedFonts)
           m_pParsedFonts = new vector<Font *>;
           
       m_pParsedFonts->push_back(pFont);

       return kError_NoErr;
    }
    if (oElement == string("Window"))
    {
       if (m_pCurrentWindow)
       {
           m_oLastError = string("<Window> tags cannot be nested");
           return kError_InvalidParam;
       }
	   if (oAttrMap.find("Name") == oAttrMap.end())
       {
           m_oLastError = string("the <Window> tag needs a Name attribute");
           return kError_ParseError;
       }        

#ifdef WIN32
       m_pCurrentWindow = new Win32Window(this, oAttrMap["Name"]);
#elif defined(HAVE_GTK)
       m_pCurrentWindow = new GTKWindow(this, oAttrMap["Name"]);
#elif defined(__BEOS__)
       m_pCurrentWindow = new BeOSWindow(this, oAttrMap["Name"]);
#endif

       return kError_NoErr;
    }
    if (oElement == string("BackgroundBitmap"))
    {
       Canvas *pCanvas;
       Bitmap *pBitmap;
       Rect    oRect;

       if (m_pCurrentWindow == NULL)
       {
           m_oLastError = string("<BackgroundBitmap> must occur inside "
                                 "of a <Window> tag");
           return kError_InvalidParam;
       }

	   if (oAttrMap.find("Name") == oAttrMap.end())
       {
           m_oLastError = string("the <BackgroundBitmap> tag needs a Name attribute");
           return kError_ParseError;
       }        

       pBitmap = FindBitmap(oAttrMap["Name"]);
       if (pBitmap == NULL)
       {
           m_oLastError = string("Undefined bitmap ") + 
                          oAttrMap["Name"] + 
                          string("in tag <BackgroundBitmap>");
           return kError_InvalidParam;
       }

       if (oAttrMap.find("Rect") == oAttrMap.end())
       {
           m_oLastError = string("the <BackgroundBitmap> tag needs a Rect attribute");
           return kError_ParseError;
       }        
       
       eRet = ParseRect(oAttrMap["Rect"], oRect);
       if (eRet != kError_NoErr)
       {
           m_oLastError = string("Improperly formatted rect coordinates: ") +
                          oAttrMap["Rect"];
           return kError_InvalidParam;
       }
       pCanvas = m_pCurrentWindow->GetCanvas();
       pCanvas->SetBackgroundBitmap(pBitmap);
       pCanvas->SetBackgroundRect(oRect);

       return kError_NoErr;
    }

    if (oElement == string("Controls"))
    {
       if (m_pCurrentWindow == NULL)
       {
           m_oLastError = string("<Controls> must occur inside of a "
                          "<Window> tag");
           return kError_InvalidParam;
       }

       return kError_NoErr;
    }

    if (oElement == string("ButtonControl"))
    {
       if (m_pCurrentControl)
       {
           m_oLastError = string("Controls cannot be nested");
           return kError_InvalidParam;
       }

	   if (oAttrMap.find("Name") == oAttrMap.end())
       {
           m_oLastError = string("the <ButtonControl> tag needs a Name attribute");
           return kError_ParseError;
       }        
	   if (oAttrMap.find("URL") != oAttrMap.end() &&
           oAttrMap["Name"] != string("Logo"))
       {
           m_oLastError = string("only the Logo button can have a URL attribute");
           return kError_ParseError;
       }        

       m_eCurrentControl = eButtonControl;
	   if (oAttrMap.find("URL") != oAttrMap.end())
           m_pCurrentControl = new ButtonControl(m_pCurrentWindow, 
                                                 oAttrMap["Name"],
                                                 oAttrMap["URL"]);
       else                                          
           m_pCurrentControl = new ButtonControl(m_pCurrentWindow, 
                                                 oAttrMap["Name"]);
       return kError_NoErr;
    }

    if (oElement == string("DialControl"))
    {
       if (m_pCurrentControl)
       {
           m_oLastError = string("Controls cannot be nested");
           return kError_InvalidParam;
       }

	   if (oAttrMap.find("Name") == oAttrMap.end())
       {
           m_oLastError = string("the <DialControl> tag needs a Name attribute");
           return kError_ParseError;
       }        

       m_eCurrentControl = eDialControl;
       m_pCurrentControl = new DialControl(m_pCurrentWindow, 
                                           oAttrMap["Name"]);
       return kError_NoErr;
    }

    if (oElement == string("MultiStateControl"))
    {
       if (m_pCurrentControl)
       {
           m_oLastError = string("Controls cannot be nested");
           return kError_InvalidParam;
       }

	   if (oAttrMap.find("Name") == oAttrMap.end())
       {
           m_oLastError = string("the <MultiStateControl> tag needs a Name attribute");
           return kError_ParseError;
       }        
	   if (oAttrMap.find("NumStates") == oAttrMap.end())
       {
           m_oLastError = string("the <MultiStateControl> tag needs a NumStates attribute");
           return kError_ParseError;
       }        

       m_eCurrentControl = eMultiStateControl;
       m_pCurrentControl = new MultiStateControl(m_pCurrentWindow, 
                                                 oAttrMap["Name"],
                                                 atoi(oAttrMap["NumStates"].
                                                 c_str()));
       return kError_NoErr;
    }

    if (oElement == string("SliderControl"))
    {
       if (m_pCurrentControl)
       {
           m_oLastError = string("Controls cannot be nested");
           return kError_InvalidParam;
       }
	   if (oAttrMap.find("Name") == oAttrMap.end())
       {
           m_oLastError = string("the <SliderControl> tag needs a Name attribute");
           return kError_ParseError;
       }        

       m_eCurrentControl = eSliderControl;
       m_pCurrentControl = new SliderControl(m_pCurrentWindow,
                                             oAttrMap["Name"]);
       return kError_NoErr;
    }

    if (oElement == string("VSliderControl"))
    {
       if (m_pCurrentControl)
       {
           m_oLastError = string("Controls cannot be nested");
           return kError_InvalidParam;
       }
	   if (oAttrMap.find("Name") == oAttrMap.end())
       {
           m_oLastError = string("the <VSliderControl> tag needs a Name attribute");
           return kError_ParseError;
       }        

       m_eCurrentControl = eVSliderControl;
       m_pCurrentControl = new VSliderControl(m_pCurrentWindow,
                                              oAttrMap["Name"]);
       return kError_NoErr;
    }

    if (oElement == string("TextControl"))
    {
       if (m_pCurrentControl)
       {
           m_oLastError = string("Controls cannot be nested");
           return kError_InvalidParam;
       }
	   if (oAttrMap.find("Name") == oAttrMap.end())
       {
           m_oLastError = string("the <TextControl> tag needs a Name attribute");
           return kError_ParseError;
       }        

       m_eCurrentControl = eTextControl;
       m_pCurrentControl = new TextControl(m_pCurrentWindow,
                                           oAttrMap["Name"]);
                                               
       return kError_NoErr;
    }

    if (oElement == string("Style"))
    {
       Color  oColor(0, 0, 0);
       Font  *pFont;
       bool   bBold = false, bItalic = false, bUnderline = false;
       string oAlign("");
       
       if (m_pCurrentControl == NULL || m_eCurrentControl != eTextControl)
       {
           m_oLastError = string("The <Style> tag must be inside of a "
                                 "<TextControl> tag");
           return kError_InvalidParam;
       }

	   if (oAttrMap.find("Font") == oAttrMap.end())
       {
           m_oLastError = string("the <Style> tag needs a Font attribute");
           return kError_ParseError;
       }        

       pFont = FindFont(oAttrMap["Font"]);
       if (pFont == NULL)
       {
           m_oLastError = string("Undefined font ") + 
                          oAttrMap["Name"] + 
                          string("in tag <Style>");
           return kError_InvalidParam;
       }
       
	   if (oAttrMap.find("Color") != oAttrMap.end() && 
           oAttrMap["Color"] != string(""))
       {
           eRet = ParseColor(oAttrMap["Color"], oColor);
           if (eRet != kError_NoErr)
    	   {
    	       m_oLastError = string("Improperly formatted color info: ") +
    	                      oAttrMap["Color"];
    	       return kError_InvalidParam;
    	   }
       }

       if (oAttrMap.find("Bold") != oAttrMap.end())
       	  bBold = strcasecmp(oAttrMap["Bold"].c_str(), "yes") == 0;
       if (oAttrMap.find("Italic") != oAttrMap.end())
       	  bItalic = strcasecmp(oAttrMap["Italic"].c_str(), "yes") == 0;
       if (oAttrMap.find("Underline") != oAttrMap.end())
       	  bUnderline = strcasecmp(oAttrMap["Underline"].c_str(), "yes") == 0;

       if (oAttrMap.find("Align") != oAttrMap.end()) 
          oAlign = oAttrMap["Align"]; 

       ((TextControl *)m_pCurrentControl)->SetStyle(pFont, oAlign,
               oColor, bBold, bItalic, bUnderline);
       
       return kError_NoErr;
	}

    if (oElement == string("ChangeWindow"))
    {
       if (m_pCurrentControl == NULL || m_eCurrentControl != eButtonControl)
       {
           m_oLastError = string("The <Style> tag must be inside of a "
                                 "<ButtonControl> tag");
           return kError_InvalidParam;
       }

       if (oAttrMap.find("Window") == oAttrMap.end())
       {
           m_oLastError = string("the <ChangeWindow> tag needs a Window attribute");
           return kError_ParseError;
       }        

       ((ButtonControl *)m_pCurrentControl)->SetTargetWindow(oAttrMap["Window"]);
       
       return kError_NoErr;
	}


    if (oElement == string("Position"))
    {
       Rect oRect;
       Pos  oPos;

       if (m_pCurrentControl == NULL)
       {
          m_oLastError = string("The <Position> tag must be inside of a "
                                "<XXXXControl> tag");
          return kError_InvalidParam;
       }

       if (oAttrMap.find("Rect") == oAttrMap.end() &&
           oAttrMap.find("Pos") == oAttrMap.end())
       {
           m_oLastError = string("the <Position> tag needs a Rect or a Pos attribute");
           return kError_ParseError;
       }        

       if (oAttrMap.find("Pos") != oAttrMap.end() &&
           m_eCurrentControl != eButtonControl &&
           m_eCurrentControl != eMultiStateControl)
       {
           m_oLastError = string("the Pos attribute in the <Position> tag can only be used "
                                 "for a MultiStateControl or a ButtonControl");
           return kError_ParseError;
       }        
       
       if (oAttrMap.find("Rect") != oAttrMap.end())
       {
           eRet = ParseRect(oAttrMap["Rect"], oRect);
           if (eRet != kError_NoErr)
           {
               m_oLastError = string("Improperly formatted Rect coordinates: ") +
                              oAttrMap["Rect"];
               return kError_InvalidParam;
           }
           m_pCurrentControl->SetRect(oRect);
       }    
       if (oAttrMap.find("Pos") != oAttrMap.end())
       {
           eRet = ParsePos(oAttrMap["Pos"], oPos);
           if (eRet != kError_NoErr)
           {
               m_oLastError = string("Improperly formatted Rect coordinates: ") +
                              oAttrMap["Rect"];
               return kError_InvalidParam;
           }
           m_pCurrentControl->SetPos(oPos);
       }    

       return kError_NoErr;
    }

    if (oElement == string("ControlBitmap"))
    {
       Bitmap *pBitmap = NULL;
       Rect    oRect;
       bool    bHoriz = true;

       if (m_pCurrentControl == NULL)
       {
          m_oLastError = string("The <ControlBitmap> tag must be inside of a "
                                "<XXXXControl> tag");
          return kError_InvalidParam;
       }

       if (oAttrMap.find("Rect") == oAttrMap.end())
       {
           m_oLastError = string("the <ControlBitmap> tag needs a Rect attribute");
           return kError_ParseError;
       }        
       if (oAttrMap.find("Name") == oAttrMap.end())
       {
           m_oLastError = string("the <ControlBitmap> tag needs a Name attribute");
           return kError_ParseError;
       }        
       
       if (oAttrMap.find("Style") != oAttrMap.end())
       {
           if (oAttrMap["Style"] == string("Vert"))
               bHoriz = false;
       }        

       eRet = ParseRect(oAttrMap["Rect"], oRect);
       if (eRet != kError_NoErr)
       {
           m_oLastError = string("Improperly formatted Rect coordinates: ") +
                                 oAttrMap["Rect"];
           return kError_InvalidParam;
       }

       pBitmap = FindBitmap(oAttrMap["Name"]);
       if (pBitmap == NULL)
       {
           m_oLastError = string("Undefined bitmap ") +
                          oAttrMap["Name"] +
                          string(" in tag <BackgroundBitmap>");
           return kError_InvalidParam;
       }

       m_pCurrentControl->SetBitmap(pBitmap, oRect, bHoriz);
       return kError_NoErr;
    }

    if (oElement == string("Info"))
    {
       if (m_pCurrentControl == NULL)
       {
          m_oLastError = string("The <Info> tag must be inside of a "
                                "<XXXXControl> tag");
          return kError_InvalidParam;
       }

	   if (oAttrMap.find("Desc") != oAttrMap.end())
           m_pCurrentControl->SetDesc(oAttrMap["Desc"]);
	   if (oAttrMap.find("Tip") != oAttrMap.end())
           m_pCurrentControl->SetTip(oAttrMap["Tip"]);
       
       return kError_NoErr;
    }

    if (oElement == string("ThemeInfo"))
    {
       if (m_pCurrentControl != NULL)
       {
          m_oLastError = string("The <ThemeInfo> tag must be top level tag");
          return kError_InvalidParam;
       }

	   if (oAttrMap.find("Name") != oAttrMap.end())
           m_oThemeName = oAttrMap["Name"];
	   if (oAttrMap.find("Author") != oAttrMap.end())
           m_oThemeAuthor = oAttrMap["Author"];
	   if (oAttrMap.find("EMail") != oAttrMap.end())
           m_oAuthorEMail = oAttrMap["EMail"];
	   if (oAttrMap.find("WebPage") != oAttrMap.end())
           m_oAuthorWebPage = oAttrMap["WebPage"];
       
       return kError_NoErr;
    }

    m_oLastError = string("Invalid tag: ") + oElement;

    return kError_InvalidParam;
}

Error Theme::EndElement(string &oElement)
{
    if (oElement == string("Bitmap") ||
        oElement == string("BackgroundBitmap") ||
        oElement == string("Font") ||
        oElement == string("ChangeWindow") ||
        oElement == string("ThemeInfo") ||
        oElement == string("MaskBitmap"))
       return kError_NoErr;

    if (oElement == string("Controls"))
    {
       if (m_pCurrentWindow == NULL)
       {
           m_oLastError = string("The <Controls> tag must be inside a "
                                 "<Window> tag");
           return kError_InvalidParam;
       }
       return kError_NoErr;
    }

    if (oElement == string("Action") ||
        oElement == string("Position") ||
        oElement == string("Style") ||
        oElement == string("Info") ||
        oElement == string("ControlBitmap"))
    {
       if (m_pCurrentControl == NULL)
       {
           m_oLastError = string("The <") + 
                          oElement.c_str()+ 
                          string("> tag must be inside a <XXXXControl>");
           return kError_InvalidParam;
       }
       return kError_NoErr;
    }

    if (oElement == string("ButtonControl") ||
        oElement == string("DialControl") ||
        oElement == string("SliderControl") ||
        oElement == string("VSliderControl") ||
        oElement == string("MultiStateControl"))
    {
       m_pCurrentWindow->AddControl(m_pCurrentControl);
       m_pCurrentControl = NULL;
       m_eCurrentControl = eUndefinedControl;

       return kError_NoErr;
    }
    if (oElement == string("TextControl")) 
    {
       if (m_eCurrentControl == eTextControl)
       {
       	   if (!((TextControl *)m_pCurrentControl)->StyleHasBeenSet())
           {
               m_oLastError = "A <TextControl> needs to define a <Style> tag";
               return kError_InvalidParam;
           }    
       }
       m_pCurrentWindow->AddControl(m_pCurrentControl);
       m_pCurrentControl = NULL;
       m_eCurrentControl = eUndefinedControl;

       return kError_NoErr;
    }

    if (oElement == string("Window"))
    {
       if (!m_pParsedWindows)
           m_pParsedWindows = new vector<Window *>;
           
       m_pParsedWindows->push_back(m_pCurrentWindow);
           
       m_pCurrentWindow = NULL;

       return kError_NoErr;
    }

    m_oLastError = string("Invalid close tag: ") + oElement;

    return kError_InvalidParam;
}    

Error Theme::PCData(string &oData)
{
    m_oLastError = string("Invalid character data: ") + oData;
    
    return kError_InvalidParam;
}

Bitmap *Theme::FindBitmap(string &oName)
{
    vector<Bitmap *>::iterator i;
    string                     oTemp;

	if (!m_pParsedBitmaps)
		return NULL;
        
    for(i = m_pParsedBitmaps->begin(); i != m_pParsedBitmaps->end(); i++)
    {
       (*i)->GetName(oTemp);
       if (oTemp == oName)
          return *i;
    }

    return NULL;
}

Font *Theme::FindFont(string &oName)
{
    vector<Font *>::iterator i;
    string                   oTemp;

	if (!m_pParsedFonts)
		return NULL;
        
    for(i = m_pParsedFonts->begin(); i != m_pParsedFonts->end(); i++)
    {
       (*i)->GetName(oTemp);
       if (oTemp == oName)
          return *i;
    }

    return NULL;
}

Error Theme::ParseRect(string &oRectstring, Rect &oRect)
{
    int iRet;

    iRet = sscanf(oRectstring.c_str(), " %d , %d , %d , %d",
             &oRect.x1, &oRect.y1, &oRect.x2, &oRect.y2);

    return (iRet == 4) ? kError_NoErr : kError_InvalidParam;
}

Error Theme::ParseColor(string &oColorstring, Color &oColor)
{
    int iRet;
    int iRed, iGreen, iBlue;

    iRet = sscanf(oColorstring.c_str(), "#%02X%02X%02X",
                  &iRed, &iGreen, &iBlue);

    if (iRet == 3)
    {
    	oColor.red = iRed;
        oColor.green = iGreen;
        oColor.blue = iBlue;
        return kError_NoErr;
    }     

    return kError_InvalidParam;
}

Error Theme::ParsePos(string &oPosstring, Pos &oPos)
{
    int iRet;

    iRet = sscanf(oPosstring.c_str(), " %d , %d",
    		&oPos.x, &oPos.y);

    return (iRet == 2) ? kError_NoErr : kError_InvalidParam;
}

void Theme::SetDefaultFont(const string &oFont)
{ 
	m_oDefaultFont = oFont;
}

void Theme::PostWindowCreate(void)
{
}

void Theme::ShowThemeCredits(void)
{
    string oText;

    if (m_bCreditsShown)
    {
       m_bCreditsShown = false;
       m_pWindow->ControlStringValue("Title", true, m_oSavedText);
       return;
    }
    
    m_pWindow->ControlStringValue("Title", false, m_oSavedText);
    if (m_oThemeName.size() > 0)
    {
        oText = m_oThemeName;
       
        if (m_oThemeAuthor.size() > 0)
           oText += string(" written by ") + m_oThemeAuthor;     

        if (m_oAuthorEMail.size() > 0)
           oText += string(", ") + m_oAuthorEMail;     

        if (m_oAuthorWebPage.size() > 0)
           oText += string(" (") + m_oAuthorWebPage + string(")");     
    }   
    else
       oText = "<No theme credit info available>";
       
    m_pWindow->ControlStringValue("Title", true, oText);
    m_bCreditsShown = true;
}
