#pragma once

/// Macro based binary blob loader, for use with ld linking trick
///
/// Expands to binary_resources_NAME_start, _size and _end and prepends
/// underscore on non-Windows systems.  Dots replaced with underscores in name.
///
/// To link:
///   ld -r -b binary -o $object resources/$file_name.$file_ext
/// or
///   xxd -i resources/$file_name.$file_ext | sed 's/\[\] = {/\[\] __attribute__((__aligned__(16))) = {/;s/\(resources[a-zA-Z0-9_]*\)/_binary_\1_x/' | $compiler -m32 -c -o $object -x c++ -
///
///   ld is assumed on windows, xxd everywhere else.  To override the assumption, set
///   BLOB_LOADER_LINK_MODE=1 for ld or BLOB_LOADER_LINK_MODE=2 for xxd
///
/// To load (in every translation unit it's referred to):
///   BLOB_LOAD(chintzy_ttf);
///
/// To access:
///   font_score = font_load3d(BLOB(chintzy_ttf), BLOB_SIZE(chintzy_ttf));
///   sound.load(BLOB(Explosion_7_Discharge__Debt__raw), BLOB_SIZE(Explosion_7_Discharge__Debt__raw));
///

#include "platform_defines.h"

#ifndef BLOB_LOADER_LINK_MODE
  #ifdef PLATFORM_WINDOWS
    // default mode to ld
    #define BLOB_LOADER_LINK_MODE 1
  #else
    // default mode to xxd
    #define BLOB_LOADER_LINK_MODE 2
  #endif
#endif // BLOB_LOADER_LINK_MODE

#if BLOB_LOADER_LINK_MODE == 1
  // ld mode
  /// Loader
  #define BLOB_LOAD(name) extern unsigned char const binary_resources_##name##_start[]; \
                          extern unsigned char const binary_resources_##name##_end[]; \
                          extern unsigned char const binary_resources_##name##_size[]

  /// Full symbol expansion
  #define BLOB(name) binary_resources_##name##_start

  /// Size pointer int converter
  #define BLOB_SIZE(name) reinterpret_cast<uintptr_t>(&binary_resources_##name##_size)

#elif BLOB_LOADER_LINK_MODE == 2
  // xxd mode
  /// Loader
  #define BLOB_LOAD(name) extern unsigned char _binary_resources_##name##_x[]; \
                          extern unsigned int  _binary_resources_##name##_len_x

  /// Full symbol expansion
  #define BLOB(name) _binary_resources_##name##_x

  /// Size pointer int converter
  #define BLOB_SIZE(name) _binary_resources_##name##_len_x

#else // BLOB_LOADER_LINK_MODE
  #error "Invalid BLOB_LOADER_LINK_MODE"
#endif // BLOB_LOADER_LINK_MODE

/// Convenience function to load as a string or string view
#define STRING_BLOB(name) std::string(reinterpret_cast<char const*>(BLOB(name)), BLOB_SIZE(name))
#define STRING_VIEW_BLOB(name) std::string_view(reinterpret_cast<char const*>(BLOB(name)), BLOB_SIZE(name))
