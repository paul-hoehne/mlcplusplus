/* 
 * File:   MLCrypto.cpp
 * Author: phoehne
 * 
 * Created on July 7, 2014, 10:27 AM
 */

#include "MLCrypto.hpp"

#include <string>
#include <iomanip>
#include <sstream>
#include <openssl/md5.h>


MLCrypto::MLCrypto() {
}

MLCrypto::MLCrypto(const MLCrypto& orig) {
}

MLCrypto::~MLCrypto() {
}

std::string MLCrypto::Md5(const std::string& raw) const {
  uint8_t buffer[16];
  
  MD5_CTX _context;
  MD5_Init(&_context);
  MD5_Update(&_context, raw.c_str(), raw.size());
  MD5_Final(buffer, &_context);
  
  return ToHex(buffer, 16);
}

std::string MLCrypto::ToHex(const uint8_t* bytes, const size_t& length) const {
  std::ostringstream hex_ss;
  hex_ss << std::hex;
  for (size_t i = 0; i < length; i++) { 
    hex_ss << std::setfill('0') << std::setw(2) << (int)bytes[i]; 
  }
  return hex_ss.str();
}

