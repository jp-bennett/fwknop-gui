#include "configs.h"

void Config::getAllConfigs(wxArrayString * configs, wxFileConfig *configFile)
{
    configFile->SetPath(wxT("/"));
    int numGroups = configFile->GetNumberOfGroups();
    wxString tmpGroup;
    long tmpindex;
    configs->Empty();

    if (numGroups > 0 )
    {
        configFile->GetFirstGroup(tmpGroup, tmpindex);
        configs->Insert(tmpGroup,0);
        numGroups--;
        while(numGroups > 0)
        {
            numGroups--;
            configFile->GetNextGroup(tmpGroup,tmpindex );
            configs->Insert(tmpGroup,0);
        }
    }
configs->Sort();
}

wxString Config::validateConfig()
{
    wxIPV4address *ipValidate = new wxIPV4address;
    wxRegEx b64Validate( wxT("^[A-Za-z0-9+/]+={0,3}$"));
    wxRegEx portStringValidate( wxT("(tcp|udp)/+[0-9]"));

    if (this->NICK_NAME.CmpNoCase(wxEmptyString) == 0 ) { // this breaks editing.
        return wxT("You must choose a Nickname.");
    } else if (ipValidate->Hostname(this->SERVER_IP) == false) { //check for valid ip or hostname
        return wxT("You must supply a valid server address.");
    } else if (wxAtoi(this->SERVER_PORT) < 1 || wxAtoi(this->SERVER_PORT) > 65535) {
        return wxT("Invalid Server Port"); //check server port is valid port or random
    } else if (this->LEGACY == true && this->HMAC.CmpNoCase(wxEmptyString) != 0) {//check no hmac with legacy
        return wxT("You cannot use an HMAC in legacy mode.");
    } else if (this->KEY_BASE64 && wxStrlen(this->KEY) % 4 != 0) { //check // base64 must have a multiple of 4 length
        return wxT("Invalid Base64 Key Length.");
    } else if (this->KEY_BASE64 && !b64Validate.Matches(this->KEY)) { // looks for disallowed b64 characters
        return wxT("Invalid Base64 Key.");
    } else if (this->HMAC_BASE64 && wxStrlen(this->HMAC) % 4 != 0) { //check // base64 must have a multiple of 4 length
        return wxT("Invalid Base64 HMAC Length.");
    } else if (this->HMAC_BASE64 && !b64Validate.Matches(this->HMAC)) { // looks for disallowed b64 characters
        return wxT("Invalid Base64 HMAC.");
    } else if (!(this->MESS_TYPE.CmpNoCase(wxT("Server Command")) == 0 || portStringValidate.Matches(this->PORTS))) {
        return wxT("Invalid Port string. Must look like tcp/22.");
    } else if (!(this->ACCESS_IP.CmpNoCase(wxT("Resolve IP")) ||  this->ACCESS_IP.CmpNoCase(wxT("Source IP")) || ipValidate->Hostname(this->ACCESS_IP) )) {
        return wxT("Invalid IP to allow."); // Have to have a valid ip to allow, if using allow ip
    } else {
        return wxT("valid");
    }
}

void Config::saveConfig(wxFileConfig *configFile)
{
configFile->SetPath(wxT("/") + this->NICK_NAME);
    configFile->Write(wxT("SERVER_IP"), this->SERVER_IP);
    configFile->Write(wxT("LEGACY"), this->LEGACY);
    configFile->Write(wxT("SERVER_PORT"), this->SERVER_PORT);
    configFile->Write(wxT("PROTOCOL"), this->PROTOCOL);
    configFile->Write(wxT("KEY"), this->KEY);
    configFile->Write(wxT("KEY_BASE64"), this->KEY_BASE64);
    configFile->Write(wxT("HMAC"), this->HMAC);
    configFile->Write(wxT("HMAC_BASE64"), this->HMAC_BASE64);
    configFile->Write(wxT("MESS_TYPE"), this->MESS_TYPE);
    configFile->Write(wxT("ACCESS_IP"), this->ACCESS_IP);
    configFile->Write(wxT("PORTS"), this->PORTS);
    configFile->Write(wxT("SERVER_TIMEOUT"), this->SERVER_TIMEOUT);
    configFile->Write(wxT("NAT_IP"), this->NAT_IP);
    configFile->Write(wxT("NAT_PORT"), this->NAT_PORT);
    configFile->Write(wxT("SERVER_CMD"), this->SERVER_CMD);
    configFile->Flush();

}

void Config::loadConfig(wxString Nick, wxFileConfig *configFile)
{
    configFile->SetPath(wxT("/") + Nick);

    this->NICK_NAME = Nick;
    this->SERVER_IP = configFile->Read(wxT("SERVER_IP"));
    configFile->Read(wxT("LEGACY"), &this->LEGACY, false);
    this->SERVER_PORT = configFile->Read(wxT("SERVER_PORT"));
    this->PROTOCOL = configFile->Read(wxT("PROTOCOL"));
    this->KEY = configFile->Read(wxT("KEY"));
    configFile->Read(wxT("KEY_BASE64"), &this->KEY_BASE64, false);
    this->HMAC = configFile->Read(wxT("HMAC"));
    configFile->Read(wxT("HMAC_BASE64"), &this->HMAC_BASE64, false);
    this->MESS_TYPE = configFile->Read(wxT("MESS_TYPE"));
    this->ACCESS_IP = configFile->Read(wxT("ACCESS_IP"));
    this->PORTS = configFile->Read(wxT("PORTS"));
    this->SERVER_TIMEOUT = configFile->Read(wxT("SERVER_TIMEOUT"));
    this->NAT_IP = configFile->Read(wxT("NAT_IP"));
    this->NAT_PORT = configFile->Read(wxT("NAT_PORT"));
    this->SERVER_CMD = configFile->Read(wxT("SERVER_CMD"));


}

void Config::defaultConfig()
{

    this->NICK_NAME = wxEmptyString;
    this->SERVER_IP = wxEmptyString;
    this->LEGACY = false;
    this->SERVER_PORT = wxT("62201");
    this->PROTOCOL = wxT("UDP");
    this->KEY = wxEmptyString;
    this->KEY_BASE64 = false;
    this->HMAC = wxEmptyString;
    this->HMAC_BASE64 = false;
    this->MESS_TYPE = wxT("Open Port");
    this->ACCESS_IP = wxT("Resolve IP");
    this->PORTS = wxT("tcp/22");
    this->SERVER_TIMEOUT = wxT("60");
    this->NAT_IP = wxEmptyString;
    this->NAT_PORT = wxEmptyString;
    this->SERVER_CMD = wxEmptyString;

}
