#include <cstdio>
#include <string>

/**
 * Custom input class designed for better performance than conventional methods,
 * such as std::cin or scanf.
 */
class Input
{
    static const int _BUF_SIZE = 4096;  ///< Input buffer size (for STDIN).
    inline static int _eof_found;   ///< Indicates if EOF has been encountered.
    inline static int _get_char();
    inline static int _read_char();
    template <class T = int> inline static T _read_int();
    friend Input& operator>>(Input&, int&);
    friend Input& operator>>(Input&, double&);
    friend Input& operator>>(Input&, std::string&);
public:
    explicit operator bool();
};

/**
 * Custom output class designed for better performance than conventional methods,
 * such as std::cout or printf.
 */
class Output
{
    static const int _BUF_SIZE = 65536; ///< Output buffer size (for STDOUT).
    inline static int _write_pos;   ///< Current writing position in the output buffer.
    inline static char _write_buf[_BUF_SIZE];   ///< Output buffer.
    inline static void _write_char(int);
    template <class T> inline static void _write_int(T);
    friend Output& operator<<(Output&, int);
    friend Output& operator<<(Output&, double);
    friend Output& operator<<(Output&, char);
    friend Output& operator<<(Output&, std::string);
    friend Output& operator<<(Output&, const char*);
public:
    ~Output();
};
