/* gpgme_wrapper.h
 * Copyright (C) 2016  Jonathan Bennett
 * Header for gpgme_wrapper.c
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

#ifndef GPGME_WRAPPER
#define GPGME_WRAPPER
#include <gpgme.h>  //This seems strange, not sure how to handle this.  Ship with my code?
#include <wx/dynlib.h>
#include <wx/wx.h>
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/richmsgdlg.h>

class gpgme_wrapper {
    public:
        bool enabled;
        bool doInit(wxFileConfig * configFile);
        void getAllKeys(wxArrayString * keys);
        bool selectHomeDir(wxFileConfig * configFile);
        bool selectEngine(wxFileConfig * configFile);
        bool setDefaults(wxFileConfig * configFile);
        wxString gpgEngine;
        wxString gpgHomeFolder;
    private:
        gpgme_ctx_t gpgcon;
        gpgme_error_t gpgerr;
        wxString gpgEngineDefault;
        wxString gpgFolderDefault;

};
#endif
