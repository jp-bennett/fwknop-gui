#include "timer.h"

BEGIN_EVENT_TABLE(timerDialog, wxDialog)
    EVT_CLOSE(timerDialog::onClose)
    EVT_TIMER(ID_SECOND_TIMER, timerDialog::tickTock)
END_EVENT_TABLE()

timerDialog::timerDialog(const wxString & title, Config *selectedConfig, wxIPV4address *serverAddr, gpgme_wrapper *ourGPG, bool debug)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 200))
{
    ourDebug = debug;
    gpgEngine = ourGPG->gpgEngine;
    gpgHomeFolder = ourGPG->gpgHomeFolder;
    ourConfig = new Config (*selectedConfig);  //Need to copy the whole object for the timer
    ourAddr = *serverAddr;
    time_left = wxAtoi(ourConfig->SERVER_TIMEOUT);
    wxFont* font = new wxFont();
    //wxPanel *panel = new wxPanel(this, -1);
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    main_timer = new wxStopWatch();
    main_timer->Start();
    second_timer = new wxTimer(this, ID_SECOND_TIMER);
    second_timer->Start(1000);
    timerText = new wxStaticText(this,wxID_ANY,wxEmptyString, wxPoint(10,10), wxSize(100, 100), wxALIGN_CENTRE_HORIZONTAL);
    font->SetPointSize(50);
	timerText->SetFont(*font);
    timerText->SetLabel(ourConfig->SERVER_TIMEOUT);
    wxStaticText * descText = new wxStaticText(this,wxID_ANY, _("The SPA request will timeout in:"), wxPoint(10,10), wxSize(200, 40), wxALIGN_CENTER_HORIZONTAL);
    vbox->Add(descText, 0, wxALIGN_CENTER_HORIZONTAL);
    vbox->Add(timerText, 0, wxALIGN_CENTER_HORIZONTAL);



    SetSizer(vbox);
    Centre();
    Show();

}

void timerDialog::tickTock(wxTimerEvent &event)
{
    timerText->SetLabel(wxString::Format(wxT("%ld"), time_left - (main_timer->Time()/1000)));
    if (time_left - (main_timer->Time()/1000) < 1) {
    second_timer->Stop();
        this->EndDialog(wxID_OK);
    }
    if (time_left - (main_timer->Time()/1000) < 11  && ourConfig->KEEP_OPEN) {
        //ourConfig->gen_SPA(_(""), ourLocalGPG);
        if (ourConfig->gen_SPA(_(""), gpgEngine, gpgHomeFolder, ourDebug).CmpNoCase(_("Success")) == 0) {
            if(ourConfig->send_SPA(&ourAddr).CmpNoCase(_("Knock sent successfully.")) == 0) {
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
