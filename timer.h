#include <wx/wx.h>
#include "configs.h"

class timerDialog : public wxDialog
{
public:
  timerDialog(const wxString& title, Config *selectedConfig, wxIPV4address *serverAddr);

private:
  wxTextCtrl *tc;
  int time_left;
  wxStaticText *timerText;
  wxStopWatch *main_timer;
  wxTimer *second_timer;
  Config *ourConfig;
  wxIPV4address *ourAddr;
void onClose(wxCloseEvent& event);
  enum
  {
    ID_SECOND_TIMER
  };
  void tickTock(wxTimerEvent& event);

  DECLARE_EVENT_TABLE()
};

