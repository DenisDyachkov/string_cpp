#ifndef BASE_STRING_HPP
#define BASE_STRING_HPP
#include "IStringContainer.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>

template<typename CharType>
class base_string: public IStringContainer<CharType>
{
    enum
    {
        type_size = sizeof(CharType)
    };
    using string_type = base_string<CharType>;//typedef base_string<CharType> string_type;

    CharType *_data;
    uint32_t _capacity;
    uint32_t _size;

    uint32_t customGetLength(const CharType *str)
    {
        uint32_t _return = 0;//CharType nullChar = 0;
        while (*str++ != 0) ++_return;
        return _return;
    }

    void _reserve(uint32_t length)
    {
        if (length < _size)
            length = _size;
        if (length != _capacity) {
            if (length > _capacity) {
                //if (_capacity == 0)  _capacity = 1;
                _capacity = _capacity * 2 > length ? _capacity * 2 : length;
            }
            else {
                _capacity = length;
            }
            _data = (CharType *) realloc(_data, _capacity * type_size);
        }
    }

    void _copy(const CharType *_src, uint32_t _size, ptrdiff_t destOffset = 0)
    {
        if (_size > 0) {
            if (_size + destOffset > _capacity) {
                _reserve(_size + destOffset);
            }
            memcpy(_data + destOffset, _src, _size * type_size);
        }
    }

    void _move(const CharType *_src, uint32_t _size, ptrdiff_t destOffset = 0)
    {
        if (_size > 0) {
            if (_size + destOffset > _capacity) {
                _reserve(_size + destOffset);
            }
            memmove(_data + destOffset, _src, _size * type_size);
        }
    }

public:
    base_string()
        : _data(nullptr), _capacity(0), _size(0)
    {}
    base_string(const CharType *init, uint32_t length = 0)
        : _data(nullptr), _capacity(0), _size(length)
    {
        if (_size > 0 || (_size = customGetLength(init)) > 0) {
            _copy(init, _size);
        }
    }
    base_string(const string_type &init)
        : _data(nullptr), _capacity(0), _size(init._size)
    {
        if (!init.empty()) {
            _copy(init._data, _size);
        }
    }

    ~base_string()
    { free(_data); }

    const CharType *data() const
    { return _data; };
    CharType *begin()
    { return _data; };
    CharType *end()
    { return begin() + _size; };
    bool empty() const
    { return _size == 0; };
    uint32_t length() const
    { return _size; }
    uint32_t capacity() const
    { return _capacity; }

    void push(const CharType &value)
    {
        _move(begin(), _size, 1);
        *begin() = value;
        ++_size;
    };
    void push_back(const CharType &value)
    {
        if (_size == _capacity) _reserve(_size + 1);
        *end() = value;
        ++_size;
    };

    void pop()
    { _move(begin() + 1, --_size); };
    void pop_back()
    { --_size; };

    void erase(uint32_t start, uint32_t count = UINT32_MAX)
    {
        if (start >= _size) return;
        if (count > _size - start) count = _size - start;
        _size -= count;
        _move(begin() + start + count, _size - start, start);
    }

    void insert(const CharType &val, uint32_t pos)
    {
        if (pos > _size) return;
        if (_size == _capacity) reserve(_size + 1);
        CharType *write = begin() + pos;
        _move(write, _size - pos, pos + 1);
        *write = val;
        ++_size;
    }

    void insert(const CharType *val, uint32_t pos, uint32_t length = 0)
    {
        if (pos > _size) return;
        if (length == 0) length = customGetLength(val);
        if (_size + length >= _capacity) reserve(_size + length);
        _move(begin() + pos, _size - pos, pos + length);
        _copy(val, length, pos);
        _size += length;
    }

    void insert(const string_type &val, uint32_t pos)
    {
        if (!val.empty())
            insert(val.data(), pos, val.length());
    }

    bool compare(const string_type &init) const
    {
        return (this == &init) || (_size == init.length() && memcmp(data(), init.data(), _size * type_size) == 0);
    }
    bool operator==(const string_type &init) const
    {
        return compare(init);
    }

    void append(const CharType &init)
    {
        push_back(init);
    }
    void append(const CharType *init, uint32_t len = 0)
    {
        if (len != 0 || (len = customGetLength(init)) != 0) {
            _copy(init, len, _size);
            _size += len;
        }
    }
    void append(const string_type &init)
    {
        if (!init.empty()) {
            append(init.data(), init.length());
        }
    }

    void operator+=(const string_type &init)
    {
        append(init);
    }
    void operator+=(const CharType *init)
    {
        append(init);
    }
    void operator+=(const CharType &init)
    {
        push_back(init);
    }

    string_type operator+(const string_type &init) const
    {
        string_type _return(*this);
        _return.append(init);
        return _return;
    }
    string_type operator+(const CharType *init) const
    {
        string_type _return(*this);
        _return.append(init);
        return _return;
    }
    string_type operator+(const CharType &init) const
    {
        string_type _return(*this);
        _return.push_back(init);
        return _return;
    }

    void assign(const CharType *init, uint32_t len = 0)
    {
        if (len != 0 || (len = customGetLength(init)) != 0) {
            _size = len;
            _copy(init, _size);
        }
    }
    void assign(const string_type &init)
    {
        if (!init.empty()) {
            assign(init.data(), init.length());
        }
    }

    void operator=(const CharType *init)
    {
        assign(init);
    }
    void operator=(const string_type &init)
    {
        assign(init);
    }

    CharType &operator[](uint32_t id)
    {
        return begin()[id % _size];
    }

    void clear()
    {
        _size = 0;
    }

    void shrink_to_fit()
    {
        _reserve(_size);
    }

    void resize(uint32_t size)
    {
        if (size > _size)
            memset(end(), 0, (size - _size) * type_size);
        _size = size;
    }

    void reserve(uint32_t capacity)
    {
        _reserve(capacity);
    }

    string_type substring(uint32_t start = 0, uint32_t count = UINT32_MAX) const
    {
        return string_type(data() + start, (count > _size - start ? _size - start : count));
    }

    uint32_t find(CharType ch, uint32_t start = 0)
    {
        const CharType *begin = data();
        for (; start < _size; ++start)
            if (begin[start] == ch)
                return start;
        return UINT32_MAX;
    }

    uint32_t find(const CharType *ch, uint32_t start = 0, uint32_t len = 0)
    {
        if (len == 0) len = customGetLength(ch);
        if (len > 0) {
            const CharType *begin = data();
            for (uint32_t end = _size - len, iter; start <= end; ++start) {
                for (iter = 0;
                     iter < len && begin[start] == ch[iter];
                     ++iter, ++start);
                if (iter == len)
                    return start - len;
            }
        }
        return UINT32_MAX;
    }

    uint32_t find(const string_type &check, uint32_t start = 0)
    {
        return find(check.data(), start, check.length());
    }

    template<typename Type>
    friend std::basic_ostream<Type> &operator<<(std::basic_ostream<Type> &out, const base_string<Type> &in);
    template<typename Type>
    friend std::basic_istream<Type> &operator>>(std::basic_istream<Type> &in, base_string<Type> &out);
};

template<typename CharType>
std::basic_ostream<CharType> &operator<<(std::basic_ostream<CharType> &out, const base_string<CharType> &data)
{
    out.write(data.data(), data.length());
    return out;
}
template<typename CharType>
std::basic_istream<CharType> &operator>>(std::basic_istream<CharType> &in, base_string<CharType> &data)
{
    /*
    s1.rdbuf()->in_avail() - length input
    (char)s1.rdbuf()->sbumpc() - return int, = in.get(), read and next sym
    (char)s1.rdbuf()->sgetc() - return int, only read sym
    (char)s1.rdbuf()->snextc() - return int, next and read sym
    */
    CharType read;
    const CharType newLine('\n');
    const CharType rw('\r');
    while ((read = in.get()) != in.eofbit &&
        read != newLine && read != rw)
        data.push_back(read);
    return in;
}

#endif //BASE_STRING_HPP