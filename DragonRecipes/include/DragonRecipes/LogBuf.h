#ifndef LOG_BUF_H
#define LOG_BUF_H

#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>

#define DEFAULT_BUF_SIZE 10000
#define DEFAULT_PUT_BACK 8
#define MIN_PUT_BACK 1
#define SUCCESS 0
#define ERROR 1

// References:
// http://www.mr-edd.co.uk/blog/beginners_guide_streambuf
// http://stackoverflow.com/questions/21420526/implementing-stdbasic-streambuf-subclass-for-manipulating-input

namespace dragon
{

template <class P, class S>
class AssignBuf
{
public:
	static P *create_buf(S *s)
	{
		return s;
	}

	static const bool own;
};

template <class P, class S>
const bool AssignBuf<P,S>::own = false;

template<class P, class S, class A>
class LogBuf : public std::streambuf
{
public:
    explicit LogBuf(S *logWriter_, size_t buff_sz = DEFAULT_BUF_SIZE, std::size_t put_back = DEFAULT_PUT_BACK) :
		_putBack(std::max(put_back, size_t(MIN_PUT_BACK))),
		_inBuffer(std::max(_putBack, buff_sz)),
		_outBuffer(buff_sz),
		_error(SUCCESS)
	{
        logWriter = A::create_buf(logWriter_);
		char *base = &_outBuffer.front();
		setp(base,base + _outBuffer.size() - 1);
		char *end = &_inBuffer.front() + _inBuffer.size();
		setg(end, end, end);
	}

    virtual ~LogBuf()
	{
		sync();
	}

	std::streambuf::int_type overflow(std::streambuf::int_type ch)
	{
		if (ch != traits_type::eof()) {
			*pptr() = ch;
			pbump(1);
			if (doFlush())
				return ch;
		}

		return traits_type::eof();
	}

	bool doFlush()
	{
		std::ptrdiff_t n = pptr() - pbase();

		if (n <= 0)
			return true;

		char *buf = pbase();
        logWriter->write(buf, static_cast<int>(n));
        pbump(-static_cast<int>(n));
		return true;
	}

	int sync()
	{
		return doFlush() ? SUCCESS : ERROR;
	}

	int error()
	{
		return _error;
	}

	void clearError()
	{
		_error = SUCCESS;
	}

private:
	// Dont't allow copy or assignment
    LogBuf(const LogBuf &);
    LogBuf &operator= (const LogBuf &);

private:
    P					*logWriter;
	const size_t		_putBack;
	std::vector<char>	_inBuffer;
	std::vector<char>	_outBuffer;
	int					_error;
};

}

#endif
