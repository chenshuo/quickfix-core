#include <quickfix/fix42/Quote.h>

#include <algorithm>
#include <map>
#include <stdio.h>

using namespace std;

double getnow()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec / 1000000.0;
}

struct display_fix
{
  char operator()(char c)
  {
    if (c == '\001')
      return ';';
    else
      return c;
  }
};

string display(const string& msg)
{

  string result(msg);
  std::transform(result.begin(), result.end(), result.begin(), display_fix());
  return result;
}

class Message
{
  typedef std::map<int, string> IntToString;
  std::map<int, string> header_;
  std::map<int, string> fields_;
  string type_;

 public:

  explicit Message(const char* type)
    : type_(type)
  {
  }

  void setHeader(int field, const string& value)
  {
    header_[field] = value;
  }

  void setField(int field, const string& value)
  {
    fields_[field] = value;
  }


  void toString(string* out) const
  {
    out->clear();
    out->append("8=FIX.4.2;9=0;35=");
    out->append(type_);

    for (IntToString::const_iterator it = header_.begin();
        it != header_.end(); ++it)
    {
      char buf[32];
      snprintf(buf, sizeof buf, ";%d=", it->first);
      out->append(buf);
      out->append(it->second);
    }

    for (IntToString::const_iterator it = fields_.begin();
        it != fields_.end(); ++it)
    {
      char buf[32];
      snprintf(buf, sizeof buf, ";%d=", it->first);
      out->append(buf);
      out->append(it->second);
    }

    out->append(";10=000;");
  }
};

int main()
{
  Message msg("S");

  msg.setHeader(49, "BANK");
  msg.setHeader(56, "CLIENT");
  msg.setHeader(115, "Customer");
  msg.setField(131, "qr001");
  msg.setField(303, "2");
  msg.setField(55, "IBM");
  msg.setField(134, "10000");
  msg.setField(135, "10000");
  msg.setField(64, "20120320");

  /*
  FIX42::Quote quote;
  FIX42::Header& header = quote.getHeader();
  header.set(FIX::SenderCompID("BANK"));
  header.set(FIX::TargetCompID("CLIENT"));
  header.set(FIX::OnBehalfOfCompID("Customer"));

  quote.set(FIX::QuoteReqID("qr001"));
  quote.setField(FIX::QuoteRequestType(2));
  quote.set(FIX::Symbol("IBM"));
  quote.set(FIX::BidSize(10000));
  quote.set(FIX::OfferSize(10000));
  quote.setField(FIX::SettlDate("20120320"));
  */

  const int kN = 1000*1000;
  double start = getnow();
  string message;
  for (int i = 0; i < kN; ++i)
  {
    /*
    header.set(FIX::MsgSeqNum(0));
    header.set(FIX::SendingTime(true));
    quote.set(FIX::QuoteID("q1"));
    quote.set(FIX::BidPx(i*0.01));
    quote.set(FIX::OfferPx(i*0.02));
    quote.toString(message);
    */
    msg.setHeader(34, "0");
    FIX::SendingTime t(true);
    msg.setHeader(t.getField(), t.getString());
    msg.setField(117, "q1");
    char buf[32];
    snprintf(buf, sizeof buf, "%g", i*0.01);
    msg.setField(132, buf);
    snprintf(buf, sizeof buf, "%g", i*0.02);
    msg.setField(133, buf);
    msg.toString(&message);
    // quote.setField(FIX::TransactTime(true));
  }
  double end = getnow();

  double t = end - start;
  printf("%f seconds, %d messages, %f msgs/sec, %zd bytes\n",
         t, kN, kN/t, message.size());
  printf("message: %s\n", display(message).c_str());

}
