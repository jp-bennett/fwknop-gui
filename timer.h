/* timer.h
 * Copyright (C) 2016  Jonathan Bennett
 * Header for timer.cpp
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
#include "configs.h"

class timerDialog : public wxDialog
{
public:
  timerDialog(const wxString& title, Config *selectedConfig, wxIPV4address *serverAddr, gpgme_wrapper *ourGPG, bool debug);
private:
  bool ourDebug;
  wxTextCtrl *tc;
  int time_left;
  wxStaticText *timerText;
  wxStopWatch *main_timer;
  wxTimer *second_timer;
  Config *ourConfig;
  wxIPV4address ourAddr;
  //gpgme_wrapper *ourLocalGPG;
  wxString gpgEngine;
  wxString gpgHomeFolder;

void onClose(wxCloseEvent& event);
  enum
  {
    ID_SECOND_TIMER
  };
  void tickTock(wxTimerEvent& event);

  DECLARE_EVENT_TABLE()
};

