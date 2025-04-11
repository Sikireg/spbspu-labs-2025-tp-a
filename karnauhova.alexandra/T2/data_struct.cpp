#include "data_struct.hpp"
#include <sstream>
#include <string>
#include <exception>
#include <iomanip>
#include "scope_guard.hpp"

std::istream& karnauhova::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (std::tolower(c) != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& karnauhova::operator>>(std::istream& in, DoubleIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  in >> dest.ref >> DelimiterIO{'d'};
  return in;
}

std::istream& karnauhova::operator>>(std::istream& in, UllIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  in >> dest.ref >> DelimiterIO{'u'} >> DelimiterIO{'l'} >> DelimiterIO{'l'};
  return in;
}

std::istream& karnauhova::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
  return in;
}

std::istream& karnauhova::operator>>(std::istream& in, KeyIO& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string input = "";
  in >> input;
  if (input.substr(0, 3) != "key")
  {
    in.setstate(std::ios::failbit);
  }
  dest.num = input.back();
  return in;
}

std::istream& karnauhova::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct input;
  {
    using key = KeyIO;
    using symb = DelimiterIO;
    using dbl = DoubleIO;
    using ull = UllIO;
    using str = StringIO;
    in >> symb{'('} >> symb{':'};
    for (size_t i = 0; i < 3; i++)
    {
      key key_num{ 0 };
      in >> key_num;
      if (key_num.num == '1')
      {
        in >> dbl{input.key1} >> symb{':'};
      }
      else if (key_num.num == '2')
      {
        in >> ull{input.key2} >> symb{':'};
      }
      else if (key_num.num == '3')
      {
        in >> str{input.key3} >> symb{':'};
      }
    }
    in >> symb{')'};
  }
  if (in)
  {
    dest = input;
  }
  return in;
}

std::ostream& karnauhova::operator<<(std::ostream& out, const DoubleIO&& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  iofmtguard fmtguard(out);
  out << std::fixed << std::setprecision(1) << dest.ref << "d";
  return out;
}

std::ostream& karnauhova::operator<<(std::ostream& out, const UllIO&& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << dest.ref << "ull";
  return out;
}

std::ostream& karnauhova::operator<<(std::ostream& out, const DataStruct& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  double dbl = src.key1;
  unsigned long long ull = src.key2;
  iofmtguard fmtguard(out);
  out << "(:key1 " << DoubleIO{dbl};
  out << ":key2 " << UllIO{ull};
  out << ":key3 \"" << src.key3;
  out << "\":)";
  return out;
}