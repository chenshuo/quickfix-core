#include <quickfix/fix42/Quote.h>

#include <algorithm>
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

int main()
{
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
  const int kN = 1000*1000;
  double start = getnow();
  string message;
  for (int i = 0; i < kN; ++i)
  {
    header.set(FIX::MsgSeqNum(0));
    header.set(FIX::SendingTime(true));
    quote.set(FIX::QuoteID("q1"));
    quote.set(FIX::BidPx(i*0.01));
    quote.set(FIX::OfferPx(i*0.02));
    quote.toString(message);
    // quote.setField(FIX::TransactTime(true));
  }
  double end = getnow();

  double t = end - start;
  printf("%f seconds, %d messages, %f msgs/sec, %zd bytes\n",
         t, kN, kN/t, message.size());
  printf("message: %s\n", display(message).c_str());

}
