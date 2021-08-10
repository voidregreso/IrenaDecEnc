#ifndef __GETOPT_H__
#define __GETOPT_H__

#include <crtdefs.h>

#if defined( WINGETOPT_SHARED_LIB )
# if defined( BUILDING_WINGETOPT_DLL )
#  define WINGETOPT_API __declspec(dllexport)
# else
#  define WINGETOPT_API __declspec(dllimport)
# endif
#else
# define WINGETOPT_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

WINGETOPT_API extern int optind;
WINGETOPT_API extern int optopt;
WINGETOPT_API extern int opterr;
WINGETOPT_API extern char *optarg;

extern int getopt(int nargc, char *const *nargv, const char *options);

#ifdef _BSD_SOURCE
# define optreset  __mingw_optreset
extern int optreset;
#endif
#ifdef __cplusplus
}
#endif
#endif

#if !defined(__UNISTD_H_SOURCED__) && !defined(__GETOPT_LONG_H__)
#define __GETOPT_LONG_H__

#ifdef __cplusplus
extern "C" {
#endif

struct option {
    const char *name;
    int         has_arg;
    int *flag;
    int         val;
};

enum {
    no_argument = 0,
    required_argument,
    optional_argument
};

extern int getopt_long(int nargc, char *const *nargv, const char *options,
                       const struct option *long_options, int *idx);
extern int getopt_long_only(int nargc, char *const *nargv, const char *options,
                            const struct option *long_options, int *idx);
#ifndef HAVE_DECL_GETOPT
# define HAVE_DECL_GETOPT	1
#endif

#ifdef __cplusplus
}
#endif

#endif