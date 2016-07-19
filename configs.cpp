#include "configs.h"
CURLcode curl_read(const std::string& url, std::ostream& os, long timeout = 30);

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
    wxRegEx findIP( wxT("^(([0-9]{1}|[0-9]{2}|[0-1][0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]{1}|[0-9]{2}|[0-1][0-9]{2}|2[0-4][0-9]|25[0-5])$"));
    wxRegEx b64Validate( wxT("^[A-Za-z0-9+/]+={0,3}$"));
    wxRegEx portStringValidate( wxT("(tcp|udp)/+[0-9]"));

    if (this->NICK_NAME.CmpNoCase(wxEmptyString) == 0 ) {
        return wxT("You must choose a Nickname.");
    } else if (this->SERVER_IP.Find(_(".")) == wxNOT_FOUND) { //check for valid ip or hostname -- Relex this check greatly and throw the error when trying to send.
        return wxT("You must supply a valid server address.");
    } else if((wxAtoi(this->SERVER_PORT) < 1 || wxAtoi(this->SERVER_PORT) > 65535) && this->SERVER_PORT.CmpNoCase(wxT("Random")) != 0){
        return wxT("Invalid Server Port"); //check server port is valid port or random
    } else if (this->LEGACY == true && this->HMAC.CmpNoCase(wxEmptyString) != 0) {//check no hmac with legacy
        return wxT("You cannot use an HMAC in legacy mode.");
    } else if (this->KEY_BASE64 && wxStrlen(this->KEY) % 4 != 0) { //check base64 must have a multiple of 4 length
        return wxT("Invalid Base64 Key Length.");
    } else if (this->KEY_BASE64 && !b64Validate.Matches(this->KEY)) { // looks for disallowed b64 characters
        return wxT("Invalid Base64 Key.");
    } else if (this->HMAC_BASE64 && wxStrlen(this->HMAC) % 4 != 0) { //check base64 must have a multiple of 4 length
        return wxT("Invalid Base64 HMAC Length.");
    } else if (this->HMAC_BASE64 && !b64Validate.Matches(this->HMAC)) { // looks for disallowed b64 characters
        return wxT("Invalid Base64 HMAC.");
    } else if (!(this->MESS_TYPE.CmpNoCase(wxT("Server Command")) == 0 || portStringValidate.Matches(this->PORTS))) { //If not a server command, make sure the port string is valid
        return wxT("Invalid Port string. Must look like tcp/22.");
    } else if (!(this->ACCESS_IP.CmpNoCase(wxT("Resolve IP")) == 0 ||  this->ACCESS_IP.CmpNoCase(wxT("Source IP")) == 0 || this->ACCESS_IP.CmpNoCase(wxT("Prompt IP")) == 0 || findIP.Matches(this->ACCESS_IP) )) { //if specifying ip, make sure is valid
        return wxT("Invalid IP to allow."); // Have to have a valid ip to allow, if using allow ip
    } else if (this->MESS_TYPE.CmpNoCase(wxT("Nat Access")) == 0 && !(findIP.Matches(this->NAT_IP) && (0 < wxAtoi(NAT_PORT) && wxAtoi(NAT_PORT) < 65536))) { //NAT_IP must be a valid ip, and NAT_PORT must be a valid port
        return wxT("Invalid NAT ip/port.");
    } else if (!(this->DIGEST_TYPE.CmpNoCase(wxT("MD5")) == 0
                || this->DIGEST_TYPE.CmpNoCase(wxT("SHA1")) == 0
                || this->DIGEST_TYPE.CmpNoCase(wxT("SHA256")) == 0
                || this->DIGEST_TYPE.CmpNoCase(wxT("SHA384")) == 0
                || this->DIGEST_TYPE.CmpNoCase(wxT("SHA512")) == 0)) {
        return wxT("Invalid SPA digest type.");
    } else if (!(this->HMAC_TYPE.CmpNoCase(wxT("MD5")) == 0
                || this->HMAC_TYPE.CmpNoCase(wxT("SHA1")) == 0
                || this->HMAC_TYPE.CmpNoCase(wxT("SHA256")) == 0
                || this->HMAC_TYPE.CmpNoCase(wxT("SHA384")) == 0
                || this->HMAC_TYPE.CmpNoCase(wxT("SHA512")) == 0)) {
        return wxT("Invalid HMAC digest type.");
    } else { //Could check for valid looking gpg keys if enabled
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
    configFile->Write(wxT("DIGEST_TYPE"), this->DIGEST_TYPE);
    configFile->Write(wxT("HMAC_TYPE"), this->HMAC_TYPE);
    configFile->Write(wxT("KEEP_OPEN"), this->KEEP_OPEN);
    configFile->Write(wxT("USE_GPG_CRYPT"), this->USE_GPG_CRYPT);
    configFile->Write(wxT("GPG_CRYPT_ID"), this->GPG_CRYPT_ID);
    configFile->Write(wxT("GPG_SIG_ID"), this->GPG_SIG_ID);
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
    this->DIGEST_TYPE = configFile->Read(wxT("DIGEST_TYPE"), wxT("SHA256"));
    this->HMAC_TYPE = configFile->Read(wxT("HMAC_TYPE"), wxT("SHA256"));
    configFile->Read(wxT("KEEP_OPEN"), &this->KEEP_OPEN, false);

    configFile->Read(wxT("USE_GPG_CRYPT"), &this->USE_GPG_CRYPT, false);
    this->GPG_CRYPT_ID = configFile->Read(wxT("GPG_CRYPT_ID"), wxEmptyString);
    this->GPG_SIG_ID = configFile->Read(wxT("GPG_SIG_ID"), wxEmptyString);
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
    this->DIGEST_TYPE = wxT("SHA256");
    this->HMAC_TYPE = wxT("SHA256");
    this->KEEP_OPEN = false;
    this->USE_GPG_CRYPT = false;
    this->GPG_CRYPT_ID = wxEmptyString;
    this->GPG_SIG_ID = wxEmptyString;
}

wxString Config::gen_SPA(wxString ip_resolver_url, wxString gpgEngine, wxString gpgHomeFolder, bool debug)
{
    CURLcode curl_Res;
    fko_ctx_t ctx;
    fwknop_options_t opts;
    int key_len = 0;
    int res;
    int hmac_str_len = 0;
    short message_type = FKO_CLIENT_TIMEOUT_NAT_ACCESS_MSG;
    short digest_type = FKO_DIGEST_SHA256;
    short hmac_type = FKO_HMAC_SHA256;
    char key_str[129] = {0}, hmac_str[129] = {0};
    char spa_msg[256] = {0};
//    char spa_buf[4096] = {0};
//    char * spa_buf_ptr;
//    char crypt_buf[4096] = {0};
    char nat_access_str[25] = {0};
//    char * hmac_buf;
//    char * spa_digest_ptr;


    memset(&opts, 0, sizeof(fwknop_options_t));

    if (this->KEY.IsEmpty() && !this->USE_GPG_CRYPT)
        return _("Key cannot be blank!");

    wxBusyInfo wait(_("Please wait, working..."));
    if (this->SERVER_PORT.CmpNoCase(wxT("random")) == 0)
    {
        srand((int)wxGetLocalTime());
        this->SERVER_PORT.Empty();
        this->SERVER_PORT << (rand()%55535 + 10000); // do this better, this isn't a horribly good random function
    }
    if (this->ACCESS_IP.CmpNoCase(wxT("Source IP")) == 0)
        this->ACCESS_IP = wxT("0.0.0.0");
    else if (this->ACCESS_IP.CmpNoCase(wxT("Resolve IP")) == 0)
    {
        std::ostringstream oss;
        curl_Res = curl_read(std::string(ip_resolver_url.mb_str()), oss);
        if (curl_Res == CURLE_OK)
        {
            wxString result_tmp = wxString::FromUTF8(oss.str().c_str());
            wxRegEx findIP( wxT("(([0-9]{1}|[0-9]{2}|[0-1][0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]{1}|[0-9]{2}|[0-1][0-9]{2}|2[0-4][0-9]|25[0-5])"));
            if (!findIP.Matches(result_tmp))
                return _("Unable to resolve our IP!");

            this->ACCESS_IP = findIP.GetMatch(result_tmp);
        } else
            return _("Libcurl returned the error: ") + wxString::FromUTF8(curl_easy_strerror(curl_Res));

    } //end resolve ip
    if (fko_new(&ctx) != FKO_SUCCESS)
        return _("Could not get new FKO context");

    if (USE_GPG_CRYPT) {
        fko_set_spa_encryption_type(ctx, FKO_ENCRYPTION_GPG);
        fko_set_gpg_exe(ctx, gpgEngine.mb_str());
        fko_set_gpg_home_dir(ctx, gpgHomeFolder.mb_str());


        fko_set_gpg_recipient(ctx, GPG_CRYPT_ID.mb_str());
        if (GPG_SIG_ID.CmpNoCase(_("None")) != 0)
        fko_set_gpg_signer(ctx, GPG_SIG_ID.mb_str());



        fko_set_spa_encryption_mode(ctx, FKO_ENC_MODE_ASYMMETRIC);

    } else {
        if (this->KEY_BASE64)
        {
            key_len = fko_base64_decode(this->KEY.mb_str(), (unsigned char *)key_str);
        } else {
            strncpy(key_str, (const char*)this->KEY.mb_str(wxConvUTF8), 128);
            key_len = (int)strlen(key_str);
        }
    }

    if (this->HMAC_BASE64)
    {
        hmac_str_len = fko_base64_decode(this->HMAC.mb_str(), (unsigned char *)hmac_str);
    } else {
        strncpy(hmac_str, (const char*)this->HMAC.mb_str(wxConvUTF8), 128);
        hmac_str_len = (int)strlen(hmac_str);
    }



    if (MESS_TYPE.CmpNoCase(wxT("Server Command")) == 0)
    {
        message_type = FKO_COMMAND_MSG;
        if (fko_set_spa_message_type(ctx, message_type) != FKO_SUCCESS)
            return _("Could not set message type");

        snprintf(spa_msg, 256, "%s,%s", (const char*)this->ACCESS_IP.mb_str(wxConvUTF8), (const char*)this->SERVER_CMD.mb_str(wxConvUTF8));
        res = fko_set_spa_message(ctx, spa_msg);
        if (res != FKO_SUCCESS)
            return _("Could not set command message");

    } else {
        if (fko_set_spa_client_timeout(ctx, wxAtoi(this->SERVER_TIMEOUT)) != FKO_SUCCESS)
            return _("Could not set SPA timeout");

        snprintf(spa_msg, 256, "%s,%s", (const char*)this->ACCESS_IP.mb_str(wxConvUTF8), (const char*)this->PORTS.mb_str(wxConvUTF8));
        if (fko_set_spa_message(ctx, spa_msg) != FKO_SUCCESS)
            return _("Could not set SPA Message");

    }
    if (this->LEGACY) { // technically should trim hmac keys
        if (fko_set_spa_encryption_mode(ctx, FKO_ENC_MODE_CBC_LEGACY_IV) != FKO_SUCCESS)
            return _("Could not set Legacy mode.");

    }
    if (!this->HMAC.IsEmpty()){
        if (this->HMAC_TYPE.CmpNoCase(wxT("MD5"))==0)
            hmac_type = FKO_HMAC_MD5;
        else if (this->HMAC_TYPE.CmpNoCase(wxT("SHA1"))==0)
            hmac_type = FKO_HMAC_SHA1;
        else if (this->HMAC_TYPE.CmpNoCase(wxT("SHA256"))==0)
            hmac_type = FKO_HMAC_SHA256;
        else if (this->HMAC_TYPE.CmpNoCase(wxT("SHA384"))==0)
            hmac_type = FKO_HMAC_SHA384;
        else if (this->HMAC_TYPE.CmpNoCase(wxT("SHA512"))==0)
            hmac_type = FKO_HMAC_SHA512;
        if (fko_set_spa_hmac_type(ctx, hmac_type) != FKO_SUCCESS)
            return _("Could not set HMAC type.");

    }
    if (this->MESS_TYPE.CmpNoCase(wxT("Nat Access")) == 0)
    {
        sprintf(nat_access_str, "%s,%s", (const char*)this->NAT_IP.mb_str(wxConvUTF8), (const char*)this->NAT_PORT.mb_str(wxConvUTF8));
        if (fko_set_spa_nat_access(ctx, nat_access_str) != FKO_SUCCESS)
            return _("Could not set nat access string.");

    } else if (this->MESS_TYPE.CmpNoCase(wxT("Local Nat Access")) == 0) {
        message_type = FKO_CLIENT_TIMEOUT_LOCAL_NAT_ACCESS_MSG;
        if (fko_set_spa_message_type(ctx, message_type) != FKO_SUCCESS)
        return _("Chould not set message type");
        sprintf(nat_access_str, "%s,%s", (const char*)this->SERVER_IP.mb_str(wxConvUTF8), (const char*)this->NAT_PORT.mb_str(wxConvUTF8));
        if (fko_set_spa_nat_access(ctx, nat_access_str) != FKO_SUCCESS)
            return _("Could not set nat access string.");
    }
    if (this->DIGEST_TYPE.CmpNoCase(wxT("MD5"))==0)
        digest_type = FKO_DIGEST_MD5;
    else if (this->DIGEST_TYPE.CmpNoCase(wxT("SHA1"))==0)
        digest_type = FKO_DIGEST_SHA1;
    else if (this->DIGEST_TYPE.CmpNoCase(wxT("SHA256"))==0)
        digest_type = FKO_DIGEST_SHA256;
    else if (this->DIGEST_TYPE.CmpNoCase(wxT("SHA384"))==0)
        digest_type = FKO_DIGEST_SHA384;
    else if (this->DIGEST_TYPE.CmpNoCase(wxT("SHA512"))==0)
        digest_type = FKO_DIGEST_SHA512;
    if (fko_set_spa_digest_type(ctx, digest_type) != FKO_SUCCESS)
        return _("Could not set SPA digest type.");
    if (fko_spa_data_final(ctx, key_str, key_len, hmac_str, hmac_str_len) != FKO_SUCCESS)
        return _("Could not generate SPA data.");

    if (fko_get_spa_data(ctx, &opts.spa_data) != FKO_SUCCESS)
        return _("Could not retrieve SPA data.");
   // if (!USE_GPG_CRYPT) {
        this->SPA_STRING = wxString::FromUTF8(opts.spa_data);
    /*} else {  //could retain this for libfko without gpg support
        fko_get_encoded_data(ctx, &spa_buf_ptr);
        fko_get_spa_digest(ctx, &spa_digest_ptr);
        sprintf(spa_buf,"%s:%s", spa_buf_ptr, spa_digest_ptr);
        ourGPG->encryptAndSign(GPG_CRYPT_ID, GPG_SIG_ID, spa_buf, crypt_buf);
        fko_set_spa_data(ctx, crypt_buf);
        fko_set_spa_hmac(ctx, hmac_str, hmac_str_len);
        fko_get_spa_hmac(ctx, &hmac_buf);
        strcat(crypt_buf, hmac_buf);
        this->SPA_STRING = wxString::FromUTF8(crypt_buf + 2);

    }*/

    if (debug) {
        wxTextEntryDialog *debugMessage = new wxTextEntryDialog(NULL, _("Debug info"), _("Debug info"),  "Source IP: " + this->ACCESS_IP +"\n" + "SPA String: " + this->SPA_STRING, wxOK | wxTE_MULTILINE );
        debugMessage->SetSize(620, 320);
        debugMessage->ShowModal();
        debugMessage->Destroy();
    }
    return _("Success");
}

wxString Config::send_SPA(wxIPV4address *serverAddr)
{
//wxGetTextFromUser(wxEmptyString, wxEmptyString, this->SPA_STRING);//Make this part of debug

    if (this->PROTOCOL.CmpNoCase(wxT("UDP")) == 0) {

        wxIPV4address ourAddr;
        ourAddr.AnyAddress();
        ourAddr.Service(0);

        if (serverAddr->Service(this->SERVER_PORT))
        {
            wxDatagramSocket *m_socket;
            m_socket = new wxDatagramSocket(ourAddr, wxSOCKET_NOWAIT);
            m_socket->SendTo(*serverAddr, this->SPA_STRING.mb_str(), this->SPA_STRING.Len());
            m_socket->WaitForWrite();
            if (m_socket->Error()) {
                m_socket->Destroy();
                return(_("Could not send knock: Error sending."));
            } else {
                m_socket->Destroy();
                return(_("Knock sent successfully."));
            }


        } else
            return(_("Could not send knock: could not set server port."));

    } else if (this->PROTOCOL.CmpNoCase(wxT("TCP")) == 0) {
        wxIPV4address ourAddr;
        ourAddr.AnyAddress();
        ourAddr.Service(0);

        if (serverAddr->Service(this->SERVER_PORT))
        {
            wxSocketClient *tcp_socket = new wxSocketClient;
            tcp_socket->Connect(*serverAddr);
            tcp_socket->WaitForWrite();
            tcp_socket->Write(this->SPA_STRING.mb_str(), this->SPA_STRING.Len());
            tcp_socket->WaitForWrite();
            if (tcp_socket->Error()) {
                tcp_socket->Destroy();
                return(_("Could not send knock: Error sending."));
            } else {
                tcp_socket->Destroy();
                return(_("Knock sent successfully."));
            }


        } else
        return(_("Could not send knock: could not set server address."));

    } else if (this->PROTOCOL.CmpNoCase(wxT("HTTP")) == 0) {
        wxHTTP *http_serv = new wxHTTP;
        http_serv->Connect(this->SERVER_IP, wxAtoi(this->SERVER_PORT));
        wxInputStream *tmp_stream;
        tmp_stream = http_serv->GetInputStream(this->SPA_STRING);
        delete tmp_stream;
        http_serv->Destroy();
        return(_("Knock sent successfully."));

    } else
        return(_("Not implemented yet"));
    return(_("Unknown error"));

}

static size_t data_write(void* buf, size_t size, size_t nmemb, void* userp)
{
	if(userp)
	{
		std::ostream& os = *static_cast<std::ostream*>(userp);
		std::streamsize len = size * nmemb;
		if(os.write(static_cast<char*>(buf), len))
			return len;
	}

	return 0;
}

/**
 * timeout is in seconds
 **/
CURLcode curl_read(const std::string& url, std::ostream& os, long timeout)
{
	CURLcode code(CURLE_FAILED_INIT);
	CURL* curl = curl_easy_init();

	if(curl)
	{
		if(CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &data_write))
		&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L))
		&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
		&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FILE, &os))
		&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout))
		#ifdef __WIN32__
		&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_CAINFO, "./ca-bundle.crt"))
		#endif
		&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str())))

		{
			code = curl_easy_perform(curl);
		}
		curl_easy_cleanup(curl);
	}
	return code;
}
