#ifndef GPGME_WRAPPER
#define GPGME_WRAPPER
#include <gpgme.h>  //This seems strange, not sure how to handle this.  Ship with my code?
#include <wx/dynlib.h>
#include <wx/wx.h>
#include <wx/base64.h>
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
