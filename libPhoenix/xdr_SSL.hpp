/**
 * Project: Phoenix
 *
 * @file xdr_SSL.hpp
 * @copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * @author Douglas Mark Royer
 * @date 24-FEB-20205
 *
 * licensed under CC BY 4.0.
 *
 * RiverExplorer is a trademark of RiverExplorer Games LLC.
 */

#ifndef SOFTWARE_AND_SERVICES_LIBRARY_COMMON_XDR_SSL_HPP_
#define SOFTWARE_AND_SERVICES_LIBRARY_COMMON_XDR_SSL_HPP_

#include <openssl/ssl.h>
#include <openssl/err.h>

#ifdef __cplusplus
extern "C" {
#endif
  
  /**
   * xdrFile_create() - Create an XDR SSL stream.
   *
   * @param Xdrs The XDR object to be initialized.
   *
   * @param Ssl An initialized SSL object.
   *
   * @param Xop XDR_ENCODE, XDR_DECODE, or XDR_FREE
   */
  void xdrSSL_create(XDR * Xdrs, SSL * Fp, enum xdr_op Xop);

  /**
   * Get the SSL from the XDR object.
   * Returns junk if Xdrs was not created with xdrSSL_create().
   *
   * @param Xdrs A pointer the XDR object to fetch the SSL from.
   *
   * @return The SSL object or junk when Xdrs was not created
   * with xdrSSL_create().
   */
  SSL	*	xdrSSL_GetSSL(XDR * Xdrs);

  /**
   * Get the file descriptor from the XDR object.
   * Returns junk if Xdrs was not created with xdrSSL_create(),
   * and may crash as the internal data would point to unknown things.
   *
   * @param Xdrs A pointer the XDR object to fetch the SSL from.
   *
   * @return The file descriptor, or -1 if not open. Or junk
   * when Xdrs was not created with xdrSSL_create().
   */
  int		xdrSSL_GetFd(XDR * Xdrs);
  
#ifdef __cplusplus
}
#endif

#endif //SOFTWARE_AND_SERVICES_LIBRARY_COMMON_XDR_SSL_HPP_
