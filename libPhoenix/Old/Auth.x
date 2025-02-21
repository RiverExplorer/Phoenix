#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
%#include "CommonDefinitions.hpp"
%#include "Commands.hpp"
%#include "AuthAnonymous.hpp"
%#include "AuthCert.hpp"
%#include "AuthCertTls.hpp"
%#include "AuthCertUser.hpp"
%#include "AuthMD5.hpp"
%#else
%#include <RiverExplorer/Phoenix/CommonDefinitions.hpp>
%#include <RiverExplorer/Phoenix/Commands.hpp>
%#include <RiverExplorer/Phoenix/AuthAnonymous.hpp>
%#include <RiverExplorer/Phoenix/AuthCert.hpp>
%#include <RiverExplorer/Phoenix/AuthCertTls.hpp>
%#include <RiverExplorer/Phoenix/AuthCertUser.hpp>
%#include <RiverExplorer/Phoenix/AuthMD5.hpp>
%#endif
#endif

union AuthPayload switch(CMD_e Cmd) {

  case AUTHANONYMOUS:
    AuthAnonymousPayload AnonymousPayload;

  case AUTHMD5:
    AuthMD5Payload MD5Payload;
		
  case AUTHCERT_TLS:
    AuthCertTlsPayload CertTlsPayload;
		
  case AUTHCERT_USER:
    AuthCertUserPayload CertUserPayload;
};

/**
 * There is no AUTH... REPLY payload.
 * An authentication replies with:
 *
 * CAPABILITY_POST: Authentication passed.
 *
 * CAPABILITY_PRE: Authentication failed.
 *
 * NotSupported:   Authentication method not supported.
 */
