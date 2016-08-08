/* rc_import.h
 * Copyright (C) 2016  Jonathan Bennett
 * Header for rc_import.cpp
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

#include <wx/wx.h>
#include <wx/stdpaths.h>
#include "configs.h"
#include <wx/tokenzr.h>

class rc_import : public wxDialog
{
public:
  rc_import(const wxString& title, Config *ourGivenConfig, bool *is_changed);

private:
    wxListBox      *listbox;
    wxArrayString  *configs;
    wxArrayString  rc_Pair;
    wxString        filename;
    wxTextFile      rcFile;
    Config         *ourConfig;
    wxTextCtrl     *tc;
    wxString        preview;
    bool           *InternalIsChanged;

enum
    {
    ID_OKButton = 1200,
    ID_ListBox,
    ID_CancelButton
    };

    void OnOK(wxCommandEvent& event);
    void OnList(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    DECLARE_EVENT_TABLE()
};
