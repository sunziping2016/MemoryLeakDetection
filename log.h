#ifndef LOG_H_INLCUDE
#define LOG_H_INLCUDE

#include <iostream>
#include <fstream>
#include <vector>
#include <initializer_list>
#include <algorithm>

template <typename char_type,
	typename traits = std::char_traits<char_type> >
class basic_teebuf :
	public std::basic_streambuf<char_type, traits>
{
public:
	typedef typename traits::int_type int_type;
	basic_teebuf() {}
	basic_teebuf(std::initializer_list<std::basic_streambuf<char_type, traits> *> sbs)
		: sbs(sbs.begin(), sbs.end())
	{}
	void push_back(std::basic_streambuf<char_type, traits> *sb) {
		sbs.push_back(sb);
	}
private:
	virtual int sync()
	{
		return std::all_of(sbs.begin(), sbs.end(), [](std::basic_streambuf<char_type, traits> * p) {
			return p->pubsync() == 0 ? true : false;
		}) ? 0 : -1;
	}
	virtual int_type overflow(int_type c)
	{
		const int_type eof = traits::eof();
		if (traits::eq_int_type(c, eof))
			return traits::not_eof(c);
		else {
			char_type const ch = traits::to_char_type(c);
			return std::any_of(sbs.begin(), sbs.end(), [ch, eof](std::basic_streambuf<char_type, traits> * p) {
				return traits::eq_int_type(p->sputc(ch), eof) ? true : false;
			}) ? eof : c;
		}
	}

private:
	std::vector<std::basic_streambuf<char_type, traits> *> sbs;
};

template <typename char_type,
	typename traits = std::char_traits<char_type> >
class basic_logbuf :
	public std::basic_streambuf<char_type, traits>
{
public:
	typedef typename traits::int_type int_type;
	basic_logbuf() {}
	basic_logbuf(std::basic_streambuf<char_type, traits> *infosb,
				 std::basic_streambuf<char_type, traits> *warningsb,
				 std::basic_streambuf<char_type, traits> *errorsb)
		: m_infosb(infosb), m_warningsb(warningsb), m_errorsb(errorsb), m_loglevel(INFO)
	{}
	enum loglevel { INFO, WARNING, ERROR, NONE};
	void set_loglevel(loglevel new_loglevel)
	{
		m_loglevel = new_loglevel;
	}
	loglevel get_loglevel()
	{
		return m_loglevel;
	}
private:
	virtual int sync()
	{
		return m_infosb->pubsync() == 0 && m_warningsb->pubsync()  == 0 && m_errorsb->pubsync() == 0 ? 0 : -1;
	}
	virtual int_type overflow(int_type c)
	{
		const int_type eof = traits::eof();
		if (traits::eq_int_type(c, eof))
			return traits::not_eof(c);
		else {
			char_type const ch = traits::to_char_type(c);
			switch (m_loglevel) {
			case INFO:
				return traits::eq_int_type(m_infosb->sputc(ch), eof) ? eof : c;
			case WARNING:
				return traits::eq_int_type(m_warningsb->sputc(ch), eof) ? eof : c;
			case ERROR:
				return traits::eq_int_type(m_errorsb->sputc(ch), eof) ? eof : c;
			case NONE:
				return c;
			}
		}
		return eof;
	}
private:
	std::basic_streambuf<char_type, traits> *m_infosb;
	std::basic_streambuf<char_type, traits> *m_warningsb;
	std::basic_streambuf<char_type, traits> *m_errorsb;
	loglevel m_loglevel;
};

inline std::ostream& nl( std::ostream& out)
{
	out << "\n";
	return out;
}

template <typename char_type,
	typename traits = std::char_traits<char_type> >
	std::basic_ostream<char_type, traits> &info(std::basic_ostream<char_type, traits> &out)
{
	basic_logbuf<char_type, traits> *log = dynamic_cast<basic_logbuf<char_type, traits> *>(out.rdbuf());
	if (log != nullptr)
		log->set_loglevel(log->INFO);
	return out;
}

template <typename char_type,
	typename traits = std::char_traits<char_type> >
	std::basic_ostream<char_type, traits> &warning(std::basic_ostream<char_type, traits> &out)
{
	basic_logbuf<char_type, traits> *log = dynamic_cast<basic_logbuf<char_type, traits> *>(out.rdbuf());
	if (log != nullptr)
		log->set_loglevel(log->WARNING);
	return out;
}

template <typename char_type,
	typename traits = std::char_traits<char_type> >
	std::basic_ostream<char_type, traits> &error(std::basic_ostream<char_type, traits> &out)
{
	basic_logbuf<char_type, traits> *log = dynamic_cast<basic_logbuf<char_type, traits> *>(out.rdbuf());
	if (log != nullptr)
		log->set_loglevel(log->ERROR);
	return out;
}

typedef basic_teebuf<char> teebuf;
typedef basic_logbuf<char> logbuf;

#endif
