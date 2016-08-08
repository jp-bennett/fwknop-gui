/* general_config.h
 * Copyright (C) 2016  Jonathan Bennett
 * Header file for general_config.cpp
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
#include <wx/fileconf.h>

class gConfigDialog : public wxDialog
{
public:
  gConfigDialog(wxFileConfig *configFile);

private:

  wxTextCtrl *url_txt;
  wxFileConfig *privateConfigFile;
  wxCheckBox *countdownCheck;
  wxCheckBox *debugCheck;

  enum
    {
    ID_OKButton = 1200,
    ID_DefButton,
    ID_CancelButton
    };

  void OnDef(wxCommandEvent& event);
  void OnOK(wxCommandEvent& event);
  void OnCancel(wxCommandEvent& event);
  DECLARE_EVENT_TABLE()
};
