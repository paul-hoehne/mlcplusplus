//
//  Credentials.h
//  Scratch
//
//  Created by Paul Hoehne on 5/29/14.
//  Copyright (c) 2014 Paul Hoehne. All rights reserved.
//

#ifndef __Scratch__Credentials__
#define __Scratch__Credentials__

#include <string>
#include <map>
#include <cstdint>
#include <cpprest/http_client.h>

#include "Types.hpp"

using namespace web;

///
/// Used to manage the credentials for digest authentication.  Normally the
/// username and password are initially known.  As part of the Digest challenge,
/// the server will return a qop value, nonce, and relam.  The client is 
/// responsible for generating a client side nonce and for managing the 
/// nonce count.
///
class Credentials {
    std::wstring _user;
    std::wstring _pass;
    
    std::string _nonce;
    std::string _qop;
    std::string _opaque;
    std::string _realm;
    std::string _uri;
    std::string _cnonce;
    uint32_t _nonce_count;
    
protected:
    ///
    /// Generate the authentication header contents.  This is what goes into 
    /// the Authorize header.  Parses the incoming authenticate header 
    /// to produce the response header.
    ///
    /// \param method The HTTP method used.
    /// \param uri The path portion of the URI
    /// \param auth_header The contents of the WWW Authenticate header
    /// \return The contents of the Authorization header
    ///
    std::string Authenticate(const std::string& method, const std::string& uri,
            const std::string& auth_header);
    
    ///
    /// Generate the authentication header contents.  This is what goes into 
    /// the Authorize header.  Requires that the credentials are set up to
    /// perform authentication and have a qop and nonce.
    ///
    /// \param method The HTTP method used.
    /// \param uri The path portion of the URI
    /// \return The contents of the Authorization header
    ///
    std::string Authenticate(const std::string& method, const std::string& uri);
    

    ///
    /// Generate a random client nonce.
    ///
    /// \return A random nonce value
    ///
    std::string RandomCnonce(void) const;
public:
    
    ///
    /// Constructor
    ///
    Credentials();
    
    /// 
    /// Constructor
    ///
    /// \param username The username
    /// \param password The user's (plaintext) password
    ///
    Credentials(const std::string& username, const std::string& password);
    
    ///
    /// Constructor
    ///
    /// \param username The username
    /// \param password The user's (plaintext) password
    /// \param cnonce The client side nonce
    /// \param nc The nonce count
    ///
    Credentials(const std::string& username, const std::string& password,
        const std::string& cnonce, const uint32_t& nc);
    
    ///
    /// Constructor
    /// 
    /// \param username The username
    /// \param password The user's (plaintext) password
    ///
    Credentials(const std::wstring& username, const std::wstring& password);
    
    ~Credentials(void);
    
    ///
    /// Returns the if the credentials are capable of generating a response
    /// to the digest challenge.  
    ///
    /// \return Weather or not it can generate a challenge response
    ///
    bool Authenticating(void) const;
    
    ///
    /// Parses the Authenticate header to extract the nonce, the qop and the
    /// realm.  Once the credentials have been provided the authenticate 
    /// header, the credentials will be capable of formulating a response
    /// to the digest challenge.
    /// 
    /// \param _raw The raw WWW Authenticate header
    ///
    void ParseWWWAthenticateHeader(const std::string& _raw);
    
    ///
    /// Returns the server provided nonce
    /// 
    /// \return The nonce
    ///
    std::string Nonce(void) const;
    
    ///
    /// Returns the server provided qop
    ///
    /// \return The qop
    ///
    std::string Qop(void) const;
    
    ///
    /// Returns the server provided opaque value
    ///
    /// \return The opaque
    ///
    std::string Opaque(void) const;
    
    ///
    /// Returns the server provided realm
    ///
    /// \return The realm
    ///
    std::string Realm(void) const;
        
    friend class AuthenticatingProxy;
    friend class TestCredentials;
};

#endif /* defined(__Scratch__Credentials__) */
