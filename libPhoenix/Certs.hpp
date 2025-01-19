/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0.
 * RiverExplorer is a trademark of RiverExplorer Games LLC.
 */

#ifndef _RIVEREXPLORE_PHOENIX_CERTS_HPP_
#define _RIVEREXPLORE_PHOENIX_CERTS_HPP_

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

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
		 */
		void				Generate();

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
		 */
		void				_GeneratePrivateKey();

		/**
		 * Generate the public part of the cert.
		 */
		void				_GenerateCertificate();
		
		/**
		 *  The private cert.
		 */
		EVP_PKEY	*	_Private;

		/**
		 *  The public cert.
		 */
		X509			* _Public;

		/*
		 * The context used to tie them together.
		 */
		SSL_CTX		*	_Ctx;

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
