#include "configs.h"

void Config::getAllConfigs(wxArrayString * configs, wxFileConfig *configFile)
{
    configFile->SetPath("/");
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
    wxRegEx b64Validate( "^[A-Za-z0-9+/]+={0,3}$");
    wxRegEx portStringValidate( "(tcp|udp)/+[0-9]");

    if (this->NICK_NAME.CmpNoCase("") == 0 ) { // this breaks editing.
        return wxT("You must choose a Nickname.");
    } else if (ipValidate->Hostname(this->SERVER_IP) == false) { //check for valid ip or hostname
        return wxT("You must supply a valid server address.");
    } else if (wxAtoi(this->SERVER_PORT) < 1 || wxAtoi(this->SERVER_PORT) > 65535) {
        return wxT("Invalid Server Port"); //check server port is valid port or random
    } else if (this->LEGACY == true && this->HMAC.CmpNoCase("") != 0) {//check no hmac with legacy
        return wxT("You cannot use an HMAC in legacy mode.");
    } else if (this->KEY_BASE64 && wxStrlen(this->KEY) % 4 != 0) { //check // base64 must have a multiple of 4 length
        return wxT("Invalid Base64 Key Length.");
    } else if (this->KEY_BASE64 && !b64Validate.Matches(this->KEY)) { // looks for disallowed b64 characters
        return wxT("Invalid Base64 Key.");
    } else if (this->HMAC_BASE64 && wxStrlen(this->HMAC) % 4 != 0) { //check // base64 must have a multiple of 4 length
        return wxT("Invalid Base64 HMAC Length.");
    } else if (this->HMAC_BASE64 && !b64Validate.Matches(this->HMAC)) { // looks for disallowed b64 characters
        return wxT("Invalid Base64 HMAC.");
    } else if (!(this->MESS_TYPE.CmpNoCase("Server Command") == 0 || portStringValidate.Matches(this->PORTS))) {
        return wxT("Invalid Port string. Must look like tcp/22.");
    } else if (!(this->ACCESS_IP.CmpNoCase("Resolve IP") ||  this->ACCESS_IP.CmpNoCase("Source IP") || ipValidate->Hostname(this->ACCESS_IP) )) {
        return wxT("Invalid IP to allow."); // Have to have a valid ip to allow, if using allow ip
    } else {
        return wxT("valid");
    }
}

void Config::saveConfig(wxFileConfig *configFile)
{
configFile->SetPath("/" + this->NICK_NAME);
    configFile->Write("SERVER_IP", this->SERVER_IP);
    configFile->Write("LEGACY", this->LEGACY);
    configFile->Write("SERVER_PORT", this->SERVER_PORT);
    configFile->Write("PROTOCOL", this->PROTOCOL);
    configFile->Write("KEY", this->KEY);
    configFile->Write("KEY_BASE64", this->KEY_BASE64);
    configFile->Write("HMAC", this->HMAC);
    configFile->Write("HMAC_BASE64", this->HMAC_BASE64);
    configFile->Write("MESS_TYPE", this->MESS_TYPE);
    configFile->Write("ACCESS_IP", this->ACCESS_IP);
    configFile->Write("PORTS", this->PORTS);
    configFile->Write("SERVER_TIMEOUT", this->SERVER_TIMEOUT);
    configFile->Write("NAT_IP", this->NAT_IP);
    configFile->Write("NAT_PORT", this->NAT_PORT);
    configFile->Write("SERVER_CMD", this->SERVER_CMD);
    configFile->Flush();

}

void Config::loadConfig(wxString Nick, wxFileConfig *configFile)
{
    configFile->SetPath("/" + Nick);

    this->NICK_NAME = Nick;
    this->SERVER_IP = configFile->Read("SERVER_IP");
    this->LEGACY = configFile->ReadBool("LEGACY", false);
    this->SERVER_PORT = configFile->Read("SERVER_PORT");
    this->PROTOCOL = configFile->Read("PROTOCOL");
    this->KEY = configFile->Read("KEY");
    this->KEY_BASE64 = configFile->ReadBool("KEY_BASE64", false);
    this->HMAC = configFile->Read("HMAC");
    this->HMAC_BASE64 = configFile->ReadBool("HMAC_BASE64", false);
    this->MESS_TYPE = configFile->Read("MESS_TYPE");
    this->ACCESS_IP = configFile->Read("ACCESS_IP");
    this->PORTS = configFile->Read("PORTS");
    this->SERVER_TIMEOUT = configFile->Read("SERVER_TIMEOUT");
    this->NAT_IP = configFile->Read("NAT_IP");
    this->NAT_PORT = configFile->Read("NAT_PORT");
    this->SERVER_CMD = configFile->Read("SERVER_CMD");


}
