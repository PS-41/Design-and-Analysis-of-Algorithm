#include <cstdio>
#include "io.hpp"

/**
 * Gets next character from the buffer. Reads STDIN into buffer
 * if buffer is empty.
 * @return the character read.
 */
int Input::_get_char()
{
    static char buf[_BUF_SIZE]; ///< Input buffer.
    static int len, pos;
    if(pos == len)
        pos = 0, len = (int) fread(buf, 1, _BUF_SIZE, stdin);
    if(pos == len)
        return -1;
    return buf[pos++];
}

/**
 * Reads the next non-whitespace character from the input buffer.
 * @return the character read.
 */
int Input::_read_char()
{
    int c;
    do
        c = _get_char();
    while(c <= 32 and c >= 0);
    if(c == -1)
        _eof_found = 1;
    return c;
}

/**
 * Reads the next (whitespace-separated) integer from the input buffer.
 * @return the integer read.
 */
template <class T = int>
T Input::_read_int()
{
    int s = 1, c = _read_char();
    T x = 0;
    if(c == '-')
        s = 0, c = _get_char();
    while('0' <= c and c <= '9')
        x = x * 10 + c - '0', c = _get_char();
    return s ? x : -x;
}

/**
 * Chained istream operator for reading an integer from the input buffer.
 * @param[in] in instance of Input class.
 * @param[out] x int variable in which the integer is to be read.
 * @return reference to the Input instance for chaining.
 */
Input& operator>>(Input& in, int& x)
{
    x = Input::_read_int();
    return in;
}

/**
 * Chained istream operator for reading a double (but it's an integer) from the input buffer.
 * @param[in] in instance of Input class.
 * @param[out] x int variable in which the integer is to be read.
 * @return reference to the Input instance for chaining.
 */
Input& operator>>(Input& in, double& x)
{
    x = Input::_read_int();
    return in;
}

/**
 * Chained istream operator for reading a string from the input buffer.
 * @param[in] in instance of Input class.
 * @param[out] s std::string variable in which the integer is to be read.
 * @return reference to the Input instance for chaining.
 */
Input& operator>>(Input& in, std::string& s)
{
    s.clear();
    char c = Input::_read_char();
    while(c > 32)
        s += c, c = Input::_get_char();
    return in;
}

/**
 * bool() overloaded to indicate whether EOF has been encountered.
 */
Input::operator bool()
{
    return not _eof_found;
}

/**
 * Writes a character to the output buffer.
 * @param[in] x Character to be written.
 */
void Output::_write_char(int x)
{
    if(_write_pos == _BUF_SIZE)
        fwrite(_write_buf, 1, _BUF_SIZE, stdout), _write_pos = 0;
    _write_buf[_write_pos++] = (char) x;
}

/**
 * Writes an integer to the output buffer.
 * param[in] x Integer to be written.
 */
template <class T>
void Output::_write_int(T x)
{
    if(x < 0)
        _write_char('-'), x = -x;
    char s[24];
    int n = 0;
    while(x or !n)
        s[n++] = char('0' + x % 10), x /= 10;
    while(n--)
        _write_char(s[n]);
}

/**
 * Chained ostream operator for writing an integer to the output buffer.
 * @param[in] out Instance of Output class.
 * @param[in] x Integer to be written.
 * @return reference to the Output instance for chaining.
 */
Output& operator<<(Output& out, int x)
{
    Output::_write_int(x);
    return out;
}

/**
 * Chained ostream operator for writing an integer to the output buffer.
 * @param[in] out Instance of Output class.
 * @param[in] x Integer to be written.
 * @return reference to the Output instance for chaining.
 */
Output& operator<<(Output& out, double x)
{
    Output::_write_int(int(x));
    return out;
}

/**
 * Chained ostream operator for writing a character to the output buffer.
 * @param[in] out Instance of Output class.
 * @param[in] c Character to be written.
 * @return reference to the Output instance for chaining.
 */
Output& operator<<(Output& out, char c)
{
    Output::_write_char(c);
    return out;
}

/**
 * Chained ostream operator for writing an std::string to the output buffer.
 * @param[in] out Instance of Output class.
 * @param[in] s String to be written.
 * @return reference to the Output instance for chaining.
 */
Output& operator<<(Output& out, std::string s)
{
    for(char c : s)
        Output::_write_char(c);
    return out;
}

/**
 * Chained ostream operator for writing an C-style string to the output buffer.
 * @param[in] out Instance of Output class.
 * @param[in] s String to be written.
 * @return reference to the Output instance for chaining.
 */
Output& operator<<(Output& out, const char* s)
{
    while(*s)
        Output::_write_char(*s++);
    return out;
}

/**
 * Destructor. Flushes the output buffer.
 */
Output::~Output()
{
    if(_write_pos)
        fwrite(_write_buf, 1, _write_pos, stdout), _write_pos = 0;
}
