#include <string.h>
#include <sodium.h>

#include "../Utils/strchr.h"
#include "signer.h"


Signer::Signer(unsigned char* secretKey) : m_sk(secretKey){}

std::string Signer::getSignature(std::string const& message)
{
  unsigned long long const msgLength = message.length();

  unsigned char *msg = new unsigned char[msgLength + 1ULL];
  strcpy((char*)msg, message.c_str());

  unsigned char sig[crypto_sign_BYTES];

  unsigned long long signLength;
  crypto_sign_detached(sig, &signLength, msg, msgLength, m_sk);
  std::string ret = util::uCharToStr(sig, signLength);

  delete msg;
  return ret;
}
