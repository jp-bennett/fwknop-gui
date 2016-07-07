/***************************************************************
 * Name:      fwknop_guiApp.h
 * Purpose:   Defines Application Class
 * Author:    Jonathan Bennett (JBennett@incomsystems.biz)
 * Created:   2015-08-05
 * Copyright: Jonathan Bennett (incomsystems.biz)
 * License:
 **************************************************************/

#ifndef FWKNOP_GUIAPP_H
#define FWKNOP_GUIAPP_H

#include <wx/app.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/spinctrl.h>
#include <wx/event.h>
#include <wx/fileconf.h>
#include <wx/arrstr.h>
#include <wx/aboutdlg.h>
#include <wx/wxhtml.h>
#include <wx/stdpaths.h>
#include <fko.h>
#include <wx/sstream.h>
#include <wx/protocol/http.h>
#include <wx/stream.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <sstream>
#include <wx/choicdlg.h>
#include <wx/dynlib.h>


#include <gpgme.h>




class fwknop_guiApp : public wxApp
{
    public:
        virtual bool OnInit();

      //  wxDECLARE_EVENT_TABLE();
};

#endif // FWKNOP_GUIAPP_H
