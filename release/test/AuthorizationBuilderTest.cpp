/* 
 * File:   AuthorizationBuilderTest.cpp
 * Author: phoehne
 * 
 * Created on June 26, 2014, 6:05 PM
 */

#include <string>
#include <iostream>
#include <sstream>
#include <cstdint>
#include <iomanip>
#include <openssl/md5.h>

#include "AuthorizationBuilderTest.hpp"
#include "AuthorizationBuilder.hpp"
#include "MLCrypto.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(AuthorizationBuilderTest);

void AuthorizationBuilderTest::TestUsernameRealmAndPassword() {
  std::string username = "joeuser";
  std::string realm = "myrealm";
  std::string password = "password";

  std::ostringstream oss;
  uint8_t buffer[16];

  oss << username << ":" << realm << ":" << password;

  MD5_CTX _context;
  MD5_Init(&_context);
  MD5_Update(&_context, oss.str().c_str(), oss.str().size());
  MD5_Final(buffer, &_context);

  std::ostringstream hex_ss;
  hex_ss << std::hex;
  for (size_t i = 0; i < 16; i++) { 
    hex_ss << std::setfill('0') << std::setw(2) << (int)buffer[i]; 
  }

  AuthorizationBuilder builder;
  CPPUNIT_ASSERT_EQUAL(hex_ss.str(), builder.UsernameRealmAndPassword(username, realm, password));
}

void AuthorizationBuilderTest::TestUserRealmPassNonceCnonce() {
  MLCrypto crypto;
  std::string username = "joe";
  std::string realm = "realm";
  std::string password = "password";
  std::string nonce = "012345";
  std::string cnonce = "000001";
  
  std::string part_1 = crypto.Md5(username + ":" + realm + ":" + password);
  std::string hash = crypto.Md5(part_1 + ":" + nonce + ":" + cnonce);
  
  AuthorizationBuilder builder;
  
  CPPUNIT_ASSERT_EQUAL(hash, builder.UserRealmPassNonceCnonce(username, realm,
      password, nonce, cnonce));
}


void AuthorizationBuilderTest::TestMethodAndURI() {
  MLCrypto crypto;
  
  std::string method = "GET";
  std::string uri = "http://foo.bar.com/some/uri.html";
  std::string hash = "fbf2a47e19579171f6587884a7c7cd88";
  
  AuthorizationBuilder builder;
  CPPUNIT_ASSERT_EQUAL(hash, builder.MethodAndURL(method, uri));
}

void AuthorizationBuilderTest::TestMethodURIAndBodyHash() {
  std::string hash = "8e8c36970df5e7371dfda6202a6720be";
  MLCrypto crypto;
  
  std::string method = "GET";
  std::string uri = "http://foo.bar.com/some/uri.html";
  std::string bodyHash = crypto.Md5("body");
  
  AuthorizationBuilder builder;
  CPPUNIT_ASSERT_EQUAL(hash, builder.MethodURLAndBodyHash(method, uri, bodyHash));
}

/*
 * username="admin", 
 * realm="public", 
 * nonce="9ca86b1d41652cdc4c2d14d2043d2c25", 
 * uri="/", 
 * response="7af18aeaf1b3e96ade1fcde51aad422e", 
 * opaque="c8fb41173a096fd0", 
 * qop=auth, 
 * nc=00000001, 
 * cnonce="72315add23f0224a"
 */
void AuthorizationBuilderTest::TestResponse1() {
  std::string hash = "ec817aae17c45b164882ecaa9491c6e2";
  std::string username = "admin";
  std::string realm = "public";
  std::string password = "password";
  std::string uri="/";
  std::string method = "GET";
  std::string opaque = "c8fb41173a096fd0";
  std::string qop = "auth";
  std::string nonce_count = "00000001";
  std::string cnonce = "72315add23f0224a";
  std::string nonce = "9ca86b1d41652cdc4c2d14d2043d2c25";
  
  std::string hash1, hash2;
  
  AuthorizationBuilder builder;
  hash1 = builder.UsernameRealmAndPassword(username, realm, password);
  hash2 = builder.MethodAndURL(method, uri);
  std::string response = builder.Response(hash1, nonce, nonce_count, cnonce, qop,
      hash2);
  
  CPPUNIT_ASSERT_EQUAL(hash, response);
}

void AuthorizationBuilderTest::TestResponse2() {
  std::string hash = "f2aecff82351aae06344385e4a3add6e";
  std::string username = "admin";
  std::string realm = "public";
  std::string password = "password";
  std::string uri="/";
  std::string method = "GET";
  std::string nonce = "9ca86b1d41652cdc4c2d14d2043d2c25";
  
  std::string hash1, hash2;
  
  AuthorizationBuilder builder;
  hash1 = builder.UsernameRealmAndPassword(username, realm, password);
  hash2 = builder.MethodAndURL(method, uri);
   
  std::string response = builder.Response(hash1, nonce, hash2);
  
  CPPUNIT_ASSERT_EQUAL(hash, response);
}
