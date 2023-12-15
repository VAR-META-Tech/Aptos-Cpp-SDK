/*!
 * toolbox.
 * toolbox_config.h
 *
 * \date 11/30/2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef TOOLBOX_CONFIG_H
#define TOOLBOX_CONFIG_H

#define HAVE_IOSFWD_H 1
#define HAVE_REGEX_H 1
#define HAVE_STD_REGEX 1
/* #undef HAVE_BOOST_REGEX */
#define HAVE_EXCEPTION_H 1
#define HAVE_TERMIOS_H 1
#define HAVE_UNISTD_H 1
#define TOOLBOX_BUILT_AS_STATIC

#ifdef TOOLBOX_BUILT_AS_STATIC
#define TOOLBOX_API
#define TOOLBOX_NO_EXPORT
#else
#ifndef TOOLBOX_API
#if _MSC_VER
#ifdef TOOLBOX_EXPORTS
#define TOOLBOX_API __declspec(dllexport)
#else
#define TOOLBOX_API __declspec(dllimport)
#endif
#else
#ifdef TOOLBOX_EXPORTS
#define TOOLBOX_API __attribute__((visibility("default")))
#else
#define TOOLBOX_API
#endif
#endif
#endif

#ifndef TOOLBOX_NO_EXPORT
#define TOOLBOX_NO_EXPORT
#endif
#endif

#endif // TOOLBOX_CONFIG_H
