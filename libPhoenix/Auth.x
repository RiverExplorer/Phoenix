#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
%#include "CommonDefinitions.hpp"
%#include "Auth_e.hpp"
%#include "AuthAnonymousPayload.hpp"
%#include "AuthCertPayload.hpp"
%#include "AuthCertTlsPayload.hpp"
%#include "AuthCertUserPayload.hpp"
%#include "AuthMD5Payload.hpp"
%#else
%#include <RiverExplorer/Phoenix/CommonDefinitions.hpp>
%#include <RiverExplorer/Phoenix/Auth_e.hpp>
%#include <RiverExplorer/Phoenix/AuthAnonymousPayload.hpp>
%#include <RiverExplorer/Phoenix/AuthCertPayload.hpp>
%#include <RiverExplorer/Phoenix/AuthCertTlsPayload.hpp>
%#include <RiverExplorer/Phoenix/AuthCertUserPayload.hpp>
%#include <RiverExplorer/Phoenix/AuthMD5Payload.hpp>
%#endif
#endif

union AuthPayload switch(Auth_e Auth) {

  case AUTHANONYMOUS:
    AuthAnonymousPayload AnonymousPayload;

  case AUTHMD5:
    AuthMD5Payload MD5Payload;
	
	case AUTHCERT:
    AuthCertPayload CertPayload;
		
  case AUTHCERT_TLS:
    AuthCertTlsPayload CertTlsPayload;
		
  case AUTHCERT_USER:
    AuthCertUserPayload CertUserPayload;
};

union AuthReplyPayload switch(Auth_e Auth) {

  case AUTHANONYMOUS:
    AuthAnonymousReplyPayload AnonymousPayload;

  case AUTHMD5:
    AuthMD5ReplyPayload MD5Payload;
	
	case AUTHCERT:
    AuthCertReplyPayload CertPayload;
		
  case AUTHCERT_TLS:
    AuthCertTlsReplyPayload CertTlsPayload;
		
  case AUTHCERT_USER:
    AuthCertUserReplyPayload CertUserPayload;
};
