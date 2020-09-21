#pragma once

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

#define ENUM_OF(name, def) enum name { def(GENERATE_ENUM) }; static const char* name##keys[] = { def(GENERATE_STRING) };
