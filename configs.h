/* configs.h
 * Copyright (C) 2016  Jonathan Bennett
 * Header file for configs.cpp
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

#ifndef FWKNOP_CONFIGS
#define FWKNOP_CONFIGS

#include <wx/app.h>
#include <wx/fileconf.h>
#include <wx/socket.h>
#include <wx/regex.h>
#include <wx/timer.h>
#include <wx/busyinfo.h>
#include <wx/wx.h>
#include <wx/protocol/http.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <sstream>
#include <fko.h>
#include "gpgme_wrapper.h"



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
        wxString DIGEST_TYPE;
        wxString HMAC_TYPE;
        bool KEEP_OPEN;
        bool USE_GPG_CRYPT;
        wxString GPG_CRYPT_ID;
        wxString GPG_SIG_ID;


        void getAllConfigs(wxArrayString * configs, wxFileConfig *configFile);
        wxString validateConfig();
        void saveConfig(wxFileConfig *configFile);
        void loadConfig(wxString Nick, wxFileConfig *configFile);
        void defaultConfig();
        wxString gen_SPA(wxString ip_resolver_url, wxString gpgEngine, wxString gpgHomeFolder, bool debug); // returns status.
        wxString send_SPA(wxIPV4address *serverAddr);

    private:
        typedef struct fwknop_options
        {
            char           *spa_server_str;
            unsigned int    spa_dst_port;
            char           *spa_data;

            //nat access options // am I using these?
            char nat_access_str[1024];
            int  nat_local;
            int  nat_port;
            int  nat_rand_port;
            char server_command[1024];
        } fwknop_options_t;
};
#endif
