/*--------------------------------------------------------------------------*/
/**
   @file    splash_begin_code.h
   @author  P. Batty
   @brief   Preprocessor dll exports
   This module implements Preprocessor dll exports
   and global preprocssor settings
*/
/*--------------------------------------------------------------------------*/

#ifndef _BEGIN_CODE_H_
#define _BEGIN_CODE_H_

//#define DEBUG
#define BUILD 1

#ifndef DLL_EXPORT
#	if defined(__BEOS__) || defined(__HAIKU__)
#		if defined(__GNUC__)
#   		define DLL_EXPORT __declspec(dllexport)
#  		else
#   		define DLL_EXPORT __declspec(export)
#  		endif
# 	elif defined(__WIN32__)
#  		ifdef __BORLANDC__
#   		ifdef BUILD
#    			define DLL_EXPORT
#   		else
#    			define DLL_EXPORT    __declspec(dllimport)
#   		endif
#  		else
#   		define DLL_EXPORT __declspec(dllexport)
#  		endif
# 	else
#  		if defined(__GNUC__) && __GNUC__ >= 4
#   		define DLL_EXPORT __attribute__ ((visibility("default")))
#  		else
#   		define DLL_EXPORT
#  		endif
# 	endif
#endif


#ifndef CALL
#	if defined(__WIN32__) && !defined(__GNUC__)
#		define CALL __cdecl
#	else
#		define CALL
#	endif
#endif


#ifdef __SYMBIAN32__
#	undef DLL_EXPORT
#	define DLL_EXPORT
#endif
#endif
