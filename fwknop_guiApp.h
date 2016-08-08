/* fwknop_guiApp.h
 * Copyright (C) 2016  Jonathan Bennett
 * Header file for fwknop_guiApp.cpp
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 */

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
