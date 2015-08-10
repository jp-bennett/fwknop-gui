//Defines the configs class
//#include "fwknop_guiApp.h"
#include <wx/app.h>
#include <wx/fileconf.h>
#include <wx/socket.h>
#include <wx/regex.h>

class Config
{
    public:
        wxString NICK_NAME;
        wxString SERVER_IP;
        bool LEGACY;
        wxString SERVER_PORT;
        wxString PROTOCOL;
        wxString KEY;
        bool KEY_BASE64;
        wxString HMAC;
        bool HMAC_BASE64;
        wxString MESS_TYPE;
        wxString ACCESS_IP;
        wxString PORTS;
        wxString SERVER_TIMEOUT;
        wxString NAT_IP;
        wxString NAT_PORT;
        wxString SERVER_CMD;
        wxString SPA_STRING;

        void getAllConfigs(wxArrayString * configs, wxFileConfig *configFile);
        wxString validateConfig();
        void saveConfig(wxFileConfig *configFile);
        void loadConfig(wxString Nick, wxFileConfig *configFile);
};
