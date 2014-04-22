#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <string>

class StringUtil
{
public:
    static bool endsWith(std::string const &fullString, std::string const &ending)
    {
        if (fullString.length() >= ending.length()) {
            return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
        } else {
            return false;
        }
    }

	static std::list<std::string> split(std::string &toSplit, const std::string &separator)
	{
		size_t pos = 0;
		std::string token;
		std::list<std::string> result;
		while((pos = toSplit.find(separator)) != std::string::npos)
		{
			token = toSplit.substr(0, pos);
			result.push_back(token);
			toSplit.erase(0, pos + separator.length());
		}

		result.push_back(toSplit);
		return result;
	}

    static std::string separator()
    {
#ifdef __APPLE__
        return std::string("/");
#elif _WIN32
        return std::string("\\");
#endif
    }


private:
    StringUtil();
    StringUtil(StringUtil&);
};

#endif // STRINGUTIL_H
