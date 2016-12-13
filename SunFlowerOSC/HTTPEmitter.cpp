//
// HTTPEmitter.cpp
//
// This file is part of the SunflowerDataService application.
//
// SunflowerDataService is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SunflowerDataService is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SunflowerDataService.  If not, see <http://www.gnu.org/licenses/>.
//
// Created by Makina on behalf of Southern Cross University.
// Copyright (c) 2014 Makina. All rights reserved.
//

#include "HTTPEmitter.h"
#include "curl/curl.h"
#include "Log.h"

#include <boost/foreach.hpp>
#include <cstring>
#include <string>

using namespace std;

void HTTPEmitter::setURL(string url)
{
    myURL = url;
}

void HTTPEmitter::addField(string name, string value)
{
    pair<string, string> kv;
    kv.first = name;
    kv.second = value;

    myFields.insert(kv);
}

void HTTPEmitter::emit()
{
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        throw ERR_EMITFAILURE;
    }

    CURL * handle;
    if ((handle = curl_easy_init()) == NULL) {
        curl_global_cleanup();
        throw ERR_EMITFAILURE;
    }

    // Set the CURL options
//#if DEBUG
    curl_easy_setopt(handle, CURLOPT_VERBOSE, 0);
//#endif

    // Set the HTTP Content-Type header
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);

    // Set the URL
    curl_easy_setopt(handle, CURLOPT_URL, myURL.c_str());

    // Set the post fields
    string field = myFields["json"];

    // Build the data points array
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, field.c_str());
    curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)strlen(field.c_str()));


    /* Perform the request, res will get the return code */
    CURLcode res = curl_easy_perform(handle);
    /* Check for errors */
    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        Log::error("HTTP post failed with err=%s", curl_easy_strerror(res));
        curl_easy_cleanup(handle);
        curl_global_cleanup();
        throw ERR_EMITFAILURE;
    }

    /* always cleanup */
    curl_easy_cleanup(handle);
    curl_global_cleanup();
}
