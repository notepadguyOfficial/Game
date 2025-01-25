#include <Logs/Logs.h>
#include <mutex>
#include <cstdarg>
#include <cstdio>
#include <XOR/xor.h>

Logs& Logs::GetInstance()
{
	static Logs instance;
	return instance;
}

void Logs::SetLogFile(const std::string& filename)
{
	std::lock_guard<std::mutex> guard(mutex_);
	filestream_.open(filename, std::ios_base::app | std::ios_base::out);

	if (!filestream_)
	{
		std::cerr << XorStr("Error opening the file!") << std::endl;
	}
}

void Logs::SetLogLevel(LOG_LEVEL level)
{
	LogLevel_ = level;
}

std::string Logs::GetCurrentTime()
{
	std::time_t now = std::time(nullptr);
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), XorStr("%Y-%m-%d %H:%M:%S"), std::localtime(&now));
	return std::string(buffer);
}

std::string Logs::FormatFileName()
{
	std::time_t now = std::time(nullptr);
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), XorStr("%Y-%m-%d_%H-%M-%S"), std::localtime(&now));
	return XorStr("Secret_") + std::string(buffer) + XorStr(".log");
}

std::string Logs::FormatMessage(const std::string& message, LOG_LEVEL level)
{
	std::ostringstream ss;
    std::string color;
	std::string levelStr;

    switch(level)
    {
    case LOG_LEVEL::DEBUG:
        color = XorStr("\033[32m");
		levelStr = XorStr("DEBUG");
        break;
	case LOG_LEVEL::INFO:
        color = XorStr("\033[37m");
		levelStr = XorStr("INFO");
        break;
	case LOG_LEVEL::WARNING:
        color = XorStr("\033[33m");
		levelStr = XorStr("WARNING");
        break;
	case LOG_LEVEL::ERROR:
        color = XorStr("\033[31m");
		levelStr = XorStr("ERROR");
        break;
    case LOG_LEVEL::HTTP:
        color = XorStr("\033[36m");
		levelStr = XorStr("HTTP");
        break;
    case LOG_LEVEL::DATABASE:
        color = XorStr("\033[34m");
		levelStr = XorStr("DATABASE");
        break;
    default:
        color = XorStr("\033[0m");
		levelStr = XorStr("UKNOWN");
        break;
    }

	ss << "[" << GetCurrentTime() << "] [" << color << levelStr << color << "] : " << color << message << color;
	return ss.str();
}


void Logs::Print(const char* fmt, LOG_LEVEL level, ...)
{
	if (level >= LogLevel_)
	{
		std::lock_guard<std::mutex> guard(mutex_);

		va_list args;
		va_start(args, level);

		char buffer[4096];
		vsnprintf(buffer, sizeof(buffer), fmt, args);
		va_end(args);

		std::string output = FormatMessage(buffer, level);

		std::cout << output << std::endl;

		if (filestream_.is_open())
		{
			filestream_ << output << std::endl;
		}
	}
}

void Logs::Input(std::string& input, const std::string& prompt)
{
	std::cout << prompt;
	std::cin >> input;

	std::string temp = prompt + " " + input;

	Info(temp);
}

void Logs::Initialized()
{
	GetInstance();
#if _DEBUG
	SetLogFile(XorStr("Test.log"));
#else
	SetLogFile(FormatFileName());
#endif
	SetLogLevel(LOG_LEVEL::INFO);
}

Logs& Log = Logs::GetInstance();