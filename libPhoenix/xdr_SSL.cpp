/*
 * BEGIN_INET_COPYRIGHT
 *
 * Copyright (C) 2016,2018 Douglas Mark Royer, All Rights Reserved.
 *
 * NOTICE:
 *
 * THIS SOURCE CODE BELONGS TO INET-CONSULTING.COM, INCORPORATED
 * CORPORATION, A NEVADA CORPORATION.  THIS CODE IS PROPERTY
 * OF THE COPYRIGHT HOLDER AND IS NOT TO BE DIVULGED OR USED BY
 * PARTIES WHO HAVE NOT RECEIVED WRITTEN AUTHORIZATION FROM THE OWNER.
 *
 * THIS SOFTWARE AND MANUAL ARE BOTH PROTECTED BY UNITED STATES
 * COPYRIGHT LAW (TITLE 17 UNITED STATES CODE). UNAUTHORIZED REPRODUCTION
 * AND/OR SALE MAY RESULT IN IMPRISONMENT OF UP TO ONE YEAR AND FINES
 * OF UP TO $100,000 (17 USC 506). COPYRIGHT INFRINGERS MAY ALSO
 * BE SUBJECT TO CIVIL LIABILITY.
 *
 * THIS NOTICE AND THE ABOVE COPYRIGHT NOTICE MUST REMAIN IN ALL
 * COPIES AND ANY DERIVED WORK.
 *
 * ALL OF THIS CODE IS ORIGINAL WORK BY THE COPYRIGHT HOLDER.
 * THIS SOURCE CODE IS UNPUBLISHED WORK FULLY PROTECTED BY THE UNITED
 * STATES COPYRIGHT LAWS AND IS CONSIDERED A TRADE SECRET BELONGING
 * TO THE COPYRIGHT HOLDER.
 *
 * THIS  DOCUMENT  AND  THE  INFORMATION  CONTAINED  HEREIN  IS
 * PROVIDED  ON  AN "AS IS" BASIS AND INET-CONSULTING.COM, INC.
 * DISCLAIM ALL WARRANTIES, EXPRESS OR IMPLIED,  INCLUDING  BUT
 * NOT  LIMITED TO ANY WARRANTY THAT THE USE OF THE INFORMATION
 * HEREIN  WILL  NOT  INFRINGE  ANY  RIGHTS  OR   ANY   IMPLIED
 * WARRANTIES  OF  MERCHANTABILITY  OR FITNESS FOR A PARTICULAR
 * PURPOSE.  ANY SERVICE CONTRACT  WILL  BE  UNDER  A  SEPARATE
 * LICENSE.
 *
 * END_INET_COPYRIGHT
 */

#include <rpc/rpc.h>

#ifdef BUILDING_LIBPHOENIX
#include "xdr_SSL.hpp"
#else
#include <RiverExplorer/Phoenix/xdr_SSL.hpp>
#endif

//#include <SaS/Common/File.hpp>
#ifndef SunOS
#include <sys/cdefs.h>
#endif

/*
 * xdr_SSL.c, XDR implementation on SSL SSL.
 *
 * This set of routines implements a XDR on a SSL stream.
 * XDR_ENCODE serializes onto the stream, XDR_DECODE de-serializes
 * from the stream.
 */

#include <stdio.h>
#include <arpa/inet.h>

#include <rpc/rpc.h>
#include <rpc/xdr.h>

#ifdef BUILDING_LIBPHOENIX
#include "CppTypes.hpp"
#else
#incldue <RiverExplorer/Phoenix/CppTypes.hpp>
#endif

/**
 * SSL_read() can return less than request when their is insufficent
 * data in the BIO. This method does not return until all data requested
 * has been received, or error.
 *
 * @param Ssl the SSL connection to read from.
 *
 * @param PutHere The address of where to store the data.
 *
 * @param Octets How many octests to get.
 *
 * @return true when Octets octets have been placed at PutHere.
 * Returns false on error.
 */
bool
SSL_get(SSL * Ssl, void * PutHere, size_t Octets)
{
  bool			Results = false;

  char		*	Ptr = (char*)PutHere;
  int			Count = 0;
  size_t		ToGo = Octets;
  int			Err = 0;
  
  while (ToGo > 0) {
    Count = SSL_read(Ssl, Ptr, (int)ToGo);
    if (Count > 0) {
      ToGo -= Count;
      Ptr += Count;
      continue;
    }

    Err = SSL_get_error(Ssl, Count);

    switch (Err) {

    case SSL_ERROR_SSL:
      /*SSL Library (usage?) error */
      /*FALLTHRU*/
    case SSL_ERROR_SYSCALL:
      /* System error - exit method */
      /*FALLTHRU*/
    case SSL_ERROR_ZERO_RETURN:
    /*Connection closed - exit method */
    break;

    case SSL_ERROR_WANT_READ:
      /* Wait for more data */
      continue;
    }
  }
  if (ToGo == 0) {
    Results = TRUE;
  }
  
  return(Results);
}

/**
 * SSL_write() can return less than request when their is insufficent
 * data in the BIO or the BIO is blocked. This method does not return until
 * all data requested has been sent, or error.
 *
 * @param Ssl the SSL connection to write to.
 *
 * @param GetHere The address of where to get the data.
 *
 * @param Octets How many octests to put.
 *
 * @return true when Octets octets have been written.
 * Returns false on error.
 */
bool
SSL_put(SSL * Ssl, void * GetHere, size_t Octets)
{
  bool			Results = false;

  char		*	Ptr = (char*)GetHere;
  int			Count = 0;
  size_t		ToGo = Octets;
  int			Err = 0;
  
  while (ToGo > 0) {
    Count = SSL_write(Ssl, Ptr, (int)ToGo);
    if (Count > 0) {
      ToGo -= Count;
      Ptr += Count;
      continue;
    }

    Err = SSL_get_error(Ssl, Count);

    switch (Err) {

    case SSL_ERROR_SSL:
      /*SSL Library (usage?) error */
      /*FALLTHRU*/
    case SSL_ERROR_SYSCALL:
      /* System error - exit method */
      /*FALLTHRU*/
    case SSL_ERROR_ZERO_RETURN:
    /*Connection closed - exit method */
    break;

    case SSL_ERROR_WANT_WRITE:
      /* Wait for ability to write */
      continue;
    }
  }
  if (ToGo == 0) {
    Results = TRUE;
  }
  
  return(Results);
}

extern "C" {

#ifndef SunOS
  /*
   * Destroy a SSL xdr stream.
   * Cleans up the xdr stream handle xdrs previously set up by
   * xdrSSL_create.
   */
  static void
  xdrSSL_destroy(XDR * Xdrs)
  {
    if (Xdrs != nullptr) {
      memset(Xdrs, 0, sizeof(XDR));
    }
    
    return;
  }

  static bool_t
  xdrSSL_getint32(XDR * Xdrs, long * Ip)
  {
    bool_t		Results = FALSE;

    if (Xdrs != nullptr && Ip != nullptr) {
      SSL	*	Ssl = (SSL*)Xdrs->x_private;
      int32_t		Temp = 0;
      
      if (Ssl != nullptr) {
	if (SSL_get(Ssl, &Temp, sizeof(int32_t))) {
	  Results = TRUE;
	  *Ip = ntohl(Temp);
	} else {
	  *Ip = 0;
	}
      }
    }

    return(Results);
  }

  static bool_t
  xdrSSL_putint32(XDR * Xdrs, const long * Ip)
  {
    bool_t		Results = FALSE;

    if (Xdrs != nullptr && Ip != nullptr) {
      SSL	*	Ssl = (SSL*)Xdrs->x_private;

      if (Ssl != nullptr) {
	int32_t		MyCopy = htonl((int32_t)*Ip);

	if (SSL_put(Ssl, &MyCopy, sizeof(int32_t))) {
	  Results = TRUE;
	}
      }
    }

    return(Results);
  }

  static bool_t
  xdrSSL_getbytes(XDR * Xdrs, char * Addr, u_int Len)
  {
    bool_t		Results = FALSE;

    if (Xdrs != nullptr && Addr != nullptr && Len > 0) {
      SSL	*	Ssl = (SSL*)Xdrs->x_private;

      if (Ssl != nullptr) {
	if (SSL_get(Ssl, Addr, Len) == (int)Len) {
	  Results = TRUE;
	}
      }
    }

    return(Results);
  }

  static bool_t
  xdrSSL_putbytes(XDR * Xdrs, const char * Addr, u_int Len)
  {
    bool_t		Results = FALSE;

    if (Xdrs != nullptr && Addr != nullptr && Len > 0) {
      SSL	*	Ssl = (SSL*)Xdrs->x_private;

      if (Ssl != nullptr) {
	if (SSL_put(Ssl, (void*)Addr, Len) == (int)Len) {
	  Results = TRUE;
	}
      }
    }

    return(Results);
  }

  static u_int
  xdrSSL_getpos(XDR * /*Xdrs*/)
  {
    return (0);	// A SSL can not be positioned.
  }

  static bool_t
  xdrSSL_setpos(XDR * /*Xdrs*/, u_int /*Pos*/)
  {
    return(0);	// A SSL can not be positioned.
  }

  /* ARGSUSED */
  static int32_t *
  xdrSSL_inline(XDR * /*Xdrs*/, u_int/*Len*/)
  {
    /*
     * Must do some work to implement this: must insure
     * enough data in the underlying SSL buffer,
     * that the buffer is aligned so that we can indirect through a
     * long *, and stuff this pointer in Xdrs->x_buf.  Doing
     * a fread or fwrite to a scratch buffer would defeat
     * most of the gains to be had here and require storage
     * management on this buffer, so we don't do this.
     */
    return(nullptr);
  }

  static int
  xdrSSL_control(XDR *, int, void *)
  {
    return(0);
  }
}

using namespace RiverExplorer::Phoenix;

/*
 * Ops vector for File type XDR
 */
static struct XDR::xdr_ops	xdrSSL_ops = {
  xdrSSL_getint32,	/* deseraialize a 32 bit int */
  xdrSSL_putint32,	/* seraialize a 32 bit int */
  xdrSSL_getbytes,	/* deserialize counted bytes */
  xdrSSL_putbytes,	/* serialize counted bytes */
  xdrSSL_getpos,	/* get offset in the stream */
  xdrSSL_setpos,	/* set offset in the stream */
  xdrSSL_inline,	/* prime stream for inline macros */
  xdrSSL_destroy,	/* destroy stream */
  xdrSSL_control
};
#endif

extern "C" {
  /*
   * Initialize a SSL xdr stream.
   * Sets the xdr stream handle xdrs for use on the stream file.
   * Operation flag is set to op.
   */
  void
  xdrSSL_create(XDR * Xdrs, SSL * Ssl, enum xdr_op op)
  {
    if (Xdrs != nullptr) {
      memset(Xdrs, 0, sizeof(XDR));
      Xdrs->x_op = op;
      Xdrs->x_ops = &xdrSSL_ops;
      Xdrs->x_private = (caddr_t)Ssl;
    }

    return;
  }

  SSL	*
  xdrSSL_GetSSL(XDR * Xdrs)
  {
    SSL	*	Results = NULL;

    if (Xdrs != NULL) {
      Results = (SSL*)Xdrs->x_private;
    }

    return(Results);
  }

  int
  xdrSSL_GetFd(XDR * Xdrs)
  {
    int		Results = -1;

    if (Xdrs != nullptr) {
      SSL	*	Ssl = (SSL*)Xdrs->x_private;

      if (Ssl != nullptr) {
	Results = SSL_get_fd(Ssl);
      }
    }
    
    return(Results);
  }
  
}
