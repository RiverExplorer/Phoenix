/**
 * Project: Phoenix
 * Time-stamp: <2025-03-06 15:44:39 doug>
 * 
 * @file Certs.hpp
 * @author Douglas Mark Royer
 * @date 24-FEB-20205
 * 
 * @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * 
 * Licensed under the MIT License. See LICENSE file
 * or https://opensource.org/licenses/MIT for details.
 * 
 * RiverExplorer is a trademark of Douglas Mark Royer
 */

#ifndef _RIVEREXPLORE_PHOENIX_CERTS_HPP_
#define _RIVEREXPLORE_PHOENIX_CERTS_HPP_

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#include <map>
#include <string>

namespace RiverExplorer::Phoenix
{
	
	/**
	 * @class Certs "Certs.hpp <RiverExplorer/Phoenix/Certs.hpp>
	 * An object that helps manage certs.
	 */
	class Cert
	{
	public:

		/**
		 * Cert - Default Constructor.
		 */
		Cert();

		/**
		 * Cert - Destructor.
		 */
		~Cert();

		/**
		 * Generate a new key pair.
		 *
		 * @param TargetHost The target hostname for the TLS connection.
		 *
		 * @return true on no error.
		 */
		bool				Generate(const char * TargetHost);

		/**
		 * Don't generate, use provided certs.
		 *
		 * @param TargetHost The host to associate with
		 * the key and certificate.
		 *
		 * @param PrivateFile The private PEM file (key).
		 *
		 * @param PublicFile The public PEM file (cert).
		 *
		 * @param ChainPath The path to the chain.
		 * When nullptr, use the system default.
		 *
		 * @return true on no error.
		 */
		bool				UseCerts(const char * TargetHost,
												 const char * PrivateFile,
												 const char * PublicFile,
												 const char * ChainPath = nullptr);
		

		/**
		 * Get the public certificate.
		 */
		X509			*	Public();

		/**
		 * Get the private certificate.
		 *
		 * @return The private certificate, or nullptr
		 * when not yet set.
		 */
		EVP_PKEY	*	Private();

		/**
		 * Accept a socket.
		 *
		 * @param FD The socket file descriptor.
		 *
		 * @param ErrorsTo The (FILE*) to log errors to.
		 *
		 * @return The value from SSL_accept().
		 */
		int				Accept(int Fd, FILE * ErrorsTo);

		/**
		 * Get the number of octets we can read on the SSL connection.
		 *
		 * @return The results of SSL_pending()
		 */
		int				CanRead();

		/**
		 * Get the result of SSL_get_error() for our SSL connection.
		 *
		 * @param ErrorResult The error result we got.
		 *
		 * @return The SSL erro code.
		 */
		int				GetError(int ErrorResult);
		
		/**
		 * Read from the SSL/TLS connection.
		 *
		 * @param Buf Read into this buffer.
		 *
		 * @param Number Read up to this many octets.
		 *
		 * @return the value from SSL_Read().
		 */
		int	Read(void * Buf, size_t Number);
		
	private:

		/**
		 * When an incomming TLS connection happens,
		 * this callback will be called by openssl
		 * to indicated the target host name the client
		 * attempted to connect to.
		 *
		 * @param Ssl The SSL connection.
		 *
		 * @param Ad
		 *
		 * @param Arg
		 */
		int _ClientWantsHostCallback(SSL * Ssl, int * Ad, void * Arg);
		
		/**
		 * Print an SSL error. openssl seems to
		 * keep state, odd and I am not sure what
		 * would happen in multi-threaded server
		 * with more than one thread getting an TLS error
		 *
		 * And as there is no stack trace, methods detecting
		 * an error should log their specific error first
		 * then call this to get and log the TLS error.
		 * Then it will be easier to track errors by looking
		 * at the logs.
		 */
		static void _LogSslError();
		
		/**
		 * Password callback needed by openssl.
		 *
		 * @param Buf Where to place the password to use.
		 *
		 * @param Size The total size of Buf.
		 *
		 * @param RwFlag
		 *
		 * @param Password The raw text that came in.
		 *
		 * @return The number of octets in Buf that are the password
		 * to use.
		 */
		static int					_PasswordCb(char * Buf,
																		int Size,
																		int RwFlag,
																		void * Password);

		/**
		 * Generate the private part of the cert.
		 *
		 * @param Target The target TLS host name.
		 *
		 * @return The iterator of the new private key
		 * in _PrivateKeys;
		 */
		std::map<std::string,EVP_PKEY*>::iterator
		_GeneratePrivateKey(const char * TargetHost);

		/**
		 * Generate the public part of the cert.
		 *
		 * @param PIt the itererator to the private key entry
		 * in _PrivateKeys;
		 *
		 * @return
		 * The iterator of the new certifiate in _PublicKeys;
		 */
		std::map<std::string,X509*>::iterator
		_GenerateCertificate(std::map<std::string,EVP_PKEY*>::iterator PIt);

		/**
		 * The path to the cert chain file to use.
		 * If nullptr, then use the system default.
		 */
		const char * _FullRootCertPath;

		/**
		 *  The private certs.
		 *
		 * <Hostname,EVP_PKEY*>
		 */
		static std::map<std::string,EVP_PKEY*> _PrivateKeys;
		
		/**
		 *  The public certs.
		 *
		 * <Hostname,X509*>
		 */
		static std::map<std::string,X509*> _PublicKeys;

		/**
		 * The context to use for this connection.
		 */
		SSL_CTX		*	_CtxToUse;

		/*
		 * The context used to tie them together in the inital
		 * connection (post SNI).
		 *
		 * @note
		 * During startup, the map may be filled with a list
		 * of supported host names. And the CTX portion will
		 * be nullptr. After the first client connects
		 * requesting the host, will that target host CTX
		 * be created, and only if an empty entry already
		 * exsists for that hostname.
		 *
		 * Target Host Entries can be added at any time.
		 *
		 * <hostname, CTX>
		 */
		static std::map<std::string,SSL_CTX*> _SniCtx;

		/**
		 *  The SSL/TLS connection.
		 */
		SSL				*	_Ssl;

		/**
		 * The accept BIO.
		 */
		BIO				*	_Accept;

		/**
		 * The I/O BIO.
		 */
		BIO				*	_Bio;

	};
}

#endif // _RIVEREXPLORE_PHOENIX_CERTS_HPP_
