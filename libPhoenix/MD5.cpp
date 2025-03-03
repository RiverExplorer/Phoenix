/**
 * Project: Phoenix
 * Time-stamp: <2025-03-02 22:46:43 doug>
 *
 * @file MD5.cpp
 * @copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * @author Douglas Mark Royer
 * @date 24-FEB-20205
 *
 * licensed under CC BY 4.0.
 *
 * RiverExplorer is a trademark of Douglas Mark Royer
 */

#include <openssl/evp.h>
#include <openssl/md5.h>
#include <string>
#include "MD5.hpp"

namespace RiverExplorer::Phoenix
{

	const std::string
	MD5(const std::string & ToHash)
	{
		std::string			Results;
	
		EVP_MD_CTX		*	Context = EVP_MD_CTX_new();
		const EVP_MD	*	Md5Engine = EVP_md5();
		unsigned char		Hash[EVP_MAX_MD_SIZE];
		unsigned int		HashLen;

		EVP_DigestInit_ex2(Context, Md5Engine, NULL);
		EVP_DigestUpdate(Context, ToHash.c_str(), ToHash.length());
		EVP_DigestFinal_ex(Context, Hash, &HashLen);
		EVP_MD_CTX_free(Context);

		Results.resize(HashLen * 2);
	
		for (unsigned int i = 0 ; i < HashLen ; ++i) {
			std::sprintf(&Results[i * 2], "%02x", Hash[i]);
		}

		return(Results);
	}
}
