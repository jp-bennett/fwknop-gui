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

