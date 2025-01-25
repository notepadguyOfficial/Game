#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>
#include <AntiDebug.h>
#include <curl/curl.h>
#include <boost/asio.hpp>
#include <XOR/xor.h>
#include <Cini/CiniFile.h>

const std::string cFile = "config.ini";

enum class State {
    Waiting,
    Running,
    Terminated
};

struct DATABASE {
    std::string host;
    int port;
    std::string schema;
    std::string username;
    std::string password;
};

DATABASE LoadDBInfo(const std::string& _path) {
    CiniFile iniFile(std::wstring(_path.begin(), _path.end()));
    DATABASE database;

    database.host = iniFile.GetValue(L"Database", L"Host");

    if (database.host.empty()) {
        LOG_ERROR("Host is missing in the configuration.");
    }

    database.port = iniFile.GetIntValue(L"Database", L"Port");

    if(database.port == 0) {
        LOG_ERROR("Port is missing in the configuration.");
    }

    database.schema = iniFile.GetValue(L"Database", L"Schema");

    if (database.schema.empty()) {
        LOG_ERROR("Schema is missing in the configuration.");
    }

    database.username = iniFile.GetValue(L"Database", L"Username");

    if (database.username.empty()) {
        LOG_ERROR("Username is missing in the configuration.");
    }

    database.password = iniFile.GetValue(L"Database", L"Password");

    if (database.password.empty()) {
        LOG_ERROR("Password is missing in the configuration.");
    }

    return database;
}


size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string GetPublicHost() {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.ipify.org?format=json");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        
        if (res != CURLE_OK) {
            LOG_ERROR("Error getting Public Host: %s", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return "";
        }
        
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return readBuffer;
}

State state = State::Waiting;
std::mutex = STATE_MUTEX;

int main(int argc, const char* argv[]) {
    Log.Initialize();

    __try {
        adbg.patchDebuggingFunctions();
        DATABASE *database = LoadDBInfo(cFile);

        {
            LOG_DATABASE("Local Host: %s", database->host.c_str());
            LOG_DATABASE("Remote Host: %s", GetPublicHost());
            LOG_DATABASE("Port: %d", database->port);
            LOG_DATABASE("Schema: %s", database->schema.c_str());
            //LOG_DATABASE("Username: %s", database->username.c_str());
            //LOG_DATABASE("Password: %s", database->password.c_str());
        }

        //ToDo: check if the database connection is successful
        // code ...
    }
    __except(EXCEPTION_EXECUTE_HANDLER) {
        DWORD code = GetExceptionCode();
        LOG_ERROR("An exception occurred. Exception code: %d", code);
    }
    __finally {
        return EXIT_SUCCESS;
    }
}