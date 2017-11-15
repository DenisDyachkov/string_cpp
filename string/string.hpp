#ifndef STRING_HPP
#define STRING_HPP

#include "base_string.hpp"
#include <deque>
#include <algorithm>

class string: public base_string<char>
{
    typedef base_string<char> base;
    typedef std::deque<string> container;
public:
    using base::operator=;
    enum class EntryType
    {
        All,
        First
    };
    string()
        : base()
    {};
    string(const char *input)
        : base(input)
    {};
    string(const base &input)
        : base(input)
    {};

    void toLower()
    {
        std::transform(begin(), end(), begin(), tolower);
    }

    void toUpper()
    {
        std::transform(begin(), end(), begin(), toupper);
    }

    int toInt(int base = 10)
    {
        return std::stoi(begin(), 0, base);
    }
    unsigned int toUInt(int base = 10)
    {
        return unsigned(toULong());
    }
    long toLong(int base = 10)
    {
        return std::stol(begin(), 0, base);
    }
    unsigned long toULong(int base = 10)
    {
        return std::stoul(begin(), 0, base);
    }
    long long toLongLong(int base = 10)
    {
        return std::stoll(begin(), 0, base);
    }
    unsigned long long toULongLong(int base = 10)
    {
        return std::stoull(begin(), 0, base);
    }
    float toFloat()
    {
        return std::stof(begin());
    }
    double toDouble()
    {
        return std::stod(begin());
    }
    long double toLongDouble()
    {
        return std::stold(begin());
    }

    bool indexOf(char ch, uint32_t start = 0)
    {
        return find(ch, start) != UINT32_MAX;
    }

    container split(char delimiter)
    {
        container _ret;
        uint32_t start = 0, end;
        do {
            end = find(delimiter, start);
            _ret.emplace_back(this->substring(start, end - start));
            start = end + 1;
        }
        while (end != UINT32_MAX);
        return _ret;
    }

    static string concatenation(const container &containe, char delimiter)
    {
        if (containe.empty())
            return string();
        string concatString(containe.front());
        for (auto iter = containe.begin() + 1, end = containe.end(); iter != end; ++iter) {
            concatString.push_back(delimiter);
            concatString.append(*iter);
        }
        return concatString;
    }

    void deleteFirst(uint32_t count)
    {
        if (count > 0)
            this->erase(0, count);
    }

    void deleteLast(uint32_t count)
    {
        this->erase(length() - count);
    }

    void deleteEntry(char ch, const EntryType type = EntryType::First)
    {
        uint32_t end = 0;
        do {
            end = find(ch, end);
            this->erase(end, 1);
        }
        while (end != UINT32_MAX && type == EntryType::All);
    }

    void deleteEntry(const char *str, const EntryType type = EntryType::First, uint32_t length = 0)
    {
        if (length == 0) length = strlen(str);
        if (length > 0) {
            if (type == EntryType::First)
                this->erase(find(str, 0, length), length);
            else {
                uint32_t start = 0;
                while ((start = find(str, start, length)) != UINT32_MAX) {
                    this->erase(start, length);
                }
            }
        }
    }

    void deleteEntry(const string &str, const EntryType type = EntryType::First)
    {
        deleteEntry(str.data(), type, str.length());
    }

    string operator-(const char *str)
    {
        string temp(*this);
        temp.deleteEntry(str);
        return temp;
    }

    string operator-(const string &str)
    {
        string temp(*this);
        temp.deleteEntry(str);
        return temp;
    }

    string &operator-=(const char *str)
    {
        deleteEntry(str);
        return *this;
    }

    string &operator-=(const string &str)
    {
        deleteEntry(str);
        return *this;
    }

    void replace(char _old, char _new, const EntryType type = EntryType::First)
    {
        uint32_t start = 0;
        if (type == EntryType::First) {
            if ((start = this->find(_old, 0)) != UINT32_MAX)
                this->operator[](start) = _new;
            return;
        }
        while ((start = this->find(_old, start)) != UINT32_MAX) {
            this->operator[](start) = _new;
            ++start;
        };
    }

    void replace(const char *_old, const char *_new, const EntryType type = EntryType::First, uint32_t count = 0)
    {
        uint32_t oldLength = strlen(_old);
        if (oldLength == 0) return;
        if (count == 0) count = strlen(_new);
        uint32_t start = this->find(_old, 0, oldLength);
        do {
            this->erase(start, oldLength);
            this->insert(_new, start, count);
            start += count;
        }
        while (type == EntryType::All &&
            (start = this->find(_old, start, oldLength)) != UINT32_MAX);
    }

    void replace(const string &_old, const string &_new, const EntryType type = EntryType::First)
    {
        if (_old.empty())
            return;
        uint32_t start = this->find(_old, 0);
        do {
            this->erase(start, _old.length());
            this->insert(_new, start);
            start += _new.length();
        }
        while (type == EntryType::All &&
            (start = this->find(_old, start)) != UINT32_MAX);
    }
};

#endif //STRING_HPP
