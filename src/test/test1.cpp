#include <quickfix/fix42/Quote.h>
#include <quickfix/fix42/QuoteRequest.h>

#include <iostream>

using namespace std;

int main()
{
  FIX42::QuoteRequest qr;
  FIX42::QuoteRequest::NoRelatedSym nosym;
  qr.getGroup(1, nosym);
  nosym.hasGroup(1, nosym);
  FIX42::Quote msg;
  msg.setField(FIX::BidPx(0.01));
  msg.setField(FIX::TransactTime(true));
  cout << msg.toString() << endl;
}
