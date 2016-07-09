#include "timer.h"

BEGIN_EVENT_TABLE(timerDialog, wxDialog)
    EVT_CLOSE(timerDialog::onClose)
    EVT_TIMER(ID_SECOND_TIMER, timerDialog::tickTock)
END_EVENT_TABLE()

timerDialog::timerDialog(const wxString & title, Config *selectedConfig, wxIPV4address *serverAddr)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(150, 150))
{
    ourConfig = selectedConfig;
    ourAddr = serverAddr;
    time_left = wxAtoi(ourConfig->SERVER_TIMEOUT);
    wxFont* font = new wxFont();
    wxPanel *panel = new wxPanel(this, -1);
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    main_timer = new wxStopWatch();
    main_timer->Start();
    second_timer = new wxTimer(this, ID_SECOND_TIMER);
    second_timer->Start(1000);
    timerText = new wxStaticText(this,wxID_ANY,wxEmptyString);
    font->SetPointSize(50);
	timerText->SetFont(*font);
    timerText->SetLabel(ourConfig->SERVER_TIMEOUT);





    SetSizer(vbox);
    Centre();
    Show();
    //second_timer->Stop();

    //Destroy();
    //delete this;

}

void timerDialog::tickTock(wxTimerEvent &event)
{
    timerText->SetLabel(wxString::Format(wxT("%ld"), time_left - (main_timer->Time()/1000)));
    if (time_left - (main_timer->Time()/1000) < 1) {
        EndModal( wxID_OK);
    }
    if (time_left - (main_timer->Time()/1000) < 11  && ourConfig->KEEP_OPEN) {
        ourConfig->gen_SPA(_(""), nullptr);
        if (ourConfig->gen_SPA(_(""), nullptr).CmpNoCase(_("Success")) == 0) {
            if(ourConfig->send_SPA(ourAddr).CmpNoCase(_("Knock sent successfully.")) == 0) {
                main_timer->Start();
            }
        }
    }
}

void timerDialog::onClose(wxCloseEvent& event) {
    second_timer->Stop();
    delete second_timer;
    delete main_timer;
    delete timerText;
    Destroy();
}

//Start with the time specified

//when we hit 10 seconds, resend if that option is set
