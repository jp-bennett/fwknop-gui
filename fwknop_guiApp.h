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
#include <fko.h>
#include "configs.h"
#include <wx/sstream.h>
#include <wx/protocol/http.h>
#include <wx/stream.h>

class fwknop_guiApp : public wxApp
{
    public:
        virtual bool OnInit();


      //  wxDECLARE_EVENT_TABLE();
};

#endif // FWKNOP_GUIAPP_H
