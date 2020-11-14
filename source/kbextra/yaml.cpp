// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kbextra/yaml.h>

#include <kb/log.h>
#include <kb/table.h>
#include <kb/crt.h>

#include <kbextra/variant.h>

#include <yaml.h> // libyaml


const char* MERGE_KEY = "<<";

// TODO: Do not have hard limits
#define MAX_VARIANT_SIZE  1024
#define MAX_VARIANT_COUNT 1024

#define STACK_TOP       stack[stack_pos - 1]
#define LIST_BACK       list[list_pos - 1]
#define STR_CHECK(s)    (s[0] != '\0')
#define STR_UNDEF(s)    (s[0] = '\0')
#define STR_COPY(d, s)  if (s == NULL) { d[0] = '\0'; } else { kb_strcpy((char*) d, (char*) s); }

typedef enum ParseAction {
  None            = 0,
  AddScalarValue  = 1 << 0,
  AddMapping      = 1 << 1,
  AddSequence     = 1 << 2,
  SolveAlias      = 1 << 3,
  Pop             = 1 << 4,
} ParseAction;

bool parse_str(kb_variant* var, const char* str) {
  kb_variant_create_str(var, str);
  return true;
}

bool parse_int(kb_variant* var, const char* str) {
  char* end;
  
  long res = std::strtol(str, &end, 0);
  if (str == end) return false;
  
  kb_variant_create_num(var, res);

  return true;
}

bool parse_float(kb_variant* var, const char* str) {
  char* end;
  
  float res = std::strtof(str, &end);
  if (str == end) return false;
  
  kb_variant_create_num(var, res);

  return true;
}

bool parse_null(kb_variant* var, const char* str) {
  if (kb_strlen(str) == 0 || !kb_strcmp(str, "null")) {
    kb_variant_create_null(var);
    return true;
  }

  return false;
}

bool parse_bool(kb_variant* var, const char* str) {  
  if (!kb_strcmp(str, "true") || !kb_strcmp(str, "yes")) {
    kb_variant_create_bool(var, true);
    return true;
  }

  if (!kb_strcmp(str, "false") || !kb_strcmp(str, "no")) {
    kb_variant_create_bool(var, false);
    return true;
  }

  return false;
}

bool parse_timestamp(kb_variant* var, const char* str) {
  kb_log_warn("Timestamp parsing not implemented");
  return false;
}

void parse_scalar(kb_variant* var, const char* str, const char* tag, yaml_scalar_style_t style) {
  // Parse with tag
  if (tag) {
    if (!kb_strcmp(tag, YAML_STR_TAG))         { if (parse_str(var, str))        return; }
    if (!kb_strcmp(tag, YAML_FLOAT_TAG))       { if (parse_float(var, str))      return; }
    if (!kb_strcmp(tag, YAML_INT_TAG))         { if (parse_int(var, str))        return; }
    if (!kb_strcmp(tag, YAML_NULL_TAG))        { if (parse_null(var, str))       return; }
    if (!kb_strcmp(tag, YAML_BOOL_TAG))        { if (parse_bool(var, str))       return; }
    if (!kb_strcmp(tag, YAML_TIMESTAMP_TAG))   { if (parse_timestamp(var, str))  return; }
  }

  // Parse non-tagged, non-plain always as string
  if (style != YAML_PLAIN_SCALAR_STYLE) {
    if (parse_str(var, str)) return;
  }

  if (parse_bool(var, str))   return;
  if (parse_float(var, str))  return;
  if (parse_int(var, str))    return;

  // Fallback
  if (parse_str(var, str))    return;

  return;
}

int yaml_rwops_reader(void* data, unsigned char* buffer, size_t size, size_t* size_read) {
  kb_rwops* rwops = (kb_rwops*) data;

  *size_read = kb_rwops_read(rwops, buffer, size);
  
  if (*size_read < size) {
    return 1;
  }
  
  return 0;
}

KB_API bool kb_yaml_parse(kb_variant* variant, kb_rwops* rwops) {

  yaml_parser_t parser;
  yaml_event_t event;
    
  kb_memset(&parser, 0, sizeof(parser));
  kb_memset(&event, 0, sizeof(event));

  yaml_parser_initialize(&parser);
  yaml_parser_set_input(&parser, yaml_rwops_reader, (void*) rwops);

  // Current stack
  uint32_t stack_pos = 0;
  kb_variant* stack[MAX_VARIANT_COUNT];

  // List of all variants
  uint32_t list_pos = 0;
  kb_variant* list[MAX_VARIANT_COUNT]; // TODO: Make resizable

  kb_variant root;

  // Init root and push to stack  
  // Stream root is always array (list of documents that can be arr or obj)
  kb_variant_create_arr(&root, MAX_VARIANT_SIZE); // TODO: Real size
  stack[stack_pos++] = &root;

  kb_table aliases {};
  kb_table_create(&aliases, MAX_VARIANT_SIZE);
  
  uint32_t action = None;

  char key    [1024] = { 0 };
  char anchor [1024] = { 0 };

  yaml_scalar_style_t scalar_style;
  yaml_event_type_t event_type;

  do {
    char scalar [1024] = { 0 };
    char tag    [1024] = { 0 };

    if (!yaml_parser_parse(&parser, &event)) {
      kb_log_trace("Failed to load yaml event");
      break;
    }

    event_type = event.type;

    // Parse event
    switch (event_type) {
      case YAML_DOCUMENT_START_EVENT: {
      } break;

      case YAML_ALIAS_EVENT: {
        STR_COPY(anchor, event.data.alias.anchor);
        action = SolveAlias;
      } break;
      
      case YAML_MAPPING_START_EVENT: {
        STR_COPY(tag, event.data.scalar.tag);
        action = AddMapping;
      } break;

      case YAML_SEQUENCE_START_EVENT: {
        STR_COPY(tag, event.data.scalar.tag);
        action = AddSequence;
      } break;

      case YAML_SEQUENCE_END_EVENT: {
        action = Pop;
      } break;
      
      case YAML_MAPPING_END_EVENT: {
        action = Pop;
      } break;

      case YAML_SCALAR_EVENT: {
        STR_COPY(scalar, event.data.scalar.value);
        STR_COPY(tag, event.data.scalar.tag);

        scalar_style  = event.data.scalar.style;
      
        if (kb_variant_is_obj(STACK_TOP)) { // Top is object
          if (STR_CHECK(key)) {
            action = AddScalarValue; // Key is set, this is a value
          }
          else {
            kb_strcpy(key, scalar); // Key is not set, this is a key
          }
        } else if (kb_variant_is_arr(STACK_TOP)) { // Top is array
          action = AddScalarValue; // Array do not have keys, always a value
        } else {
          KB_ASSERT(false, "Invalid YAML. Scalar can only be in either object or array");
        }

      } break;

      case YAML_DOCUMENT_END_EVENT: {
      } break;

      case YAML_NO_EVENT: {
      } break;

      case YAML_STREAM_START_EVENT: {
      } break;

      case YAML_STREAM_END_EVENT: {
      } break;
    }

    // Execute action 

    // Alias
    if (action & SolveAlias) {
      KB_ASSERT(STR_CHECK(key), "Invalid YAML. Can't solve an alias without an anchor!");

      if (kb_variant_is_obj(STACK_TOP)) { // Top is object
        KB_ASSERT(STR_CHECK(key), "Invalid YAML. Can't solve an alias to map without an key!");

        if (kb_strcmp(key, MERGE_KEY) == 0) { // Merge
          kb_hash anchor_hash = kb_hash_string(anchor);

          uint32_t anchor_idx = kb_table_get(&aliases, anchor_hash);

          if (anchor_idx < MAX_VARIANT_COUNT) {
            kb_variant_merge(STACK_TOP, list[anchor_idx]);
          } else {
            kb_log_warn("Could not find node with anchor '{}'", anchor);
          }

        } else { // Normal solve          
          kb_variant* obj = kb_variant_obj_put(STACK_TOP, key);
          stack[stack_pos++]  = obj;
          list[list_pos++]    = obj;
        
          // Copy anchor
          uint32_t anchor_idx = kb_table_get(&aliases, kb_hash_string(anchor));
          if (anchor_idx < MAX_VARIANT_COUNT) {
            kb_variant_copy(STACK_TOP, list[anchor_idx]);
          } else {
            kb_log_warn("Could not find node with anchor '{}'", anchor);
          }

          action |= Pop;
        }
        
        STR_UNDEF(key); // Key used

      } else if (kb_variant_is_arr(STACK_TOP)) { // Top is array
        kb_variant* obj = kb_variant_arr_put(STACK_TOP);
        stack[stack_pos++]  = obj;
        list[list_pos++]    = obj;
        
        // Copy anchor
        uint32_t anchor_idx = kb_table_get(&aliases, kb_hash_string(anchor));
        if (anchor_idx < MAX_VARIANT_COUNT) {
          kb_variant_copy(STACK_TOP, list[anchor_idx]);
        } else {
          kb_log_warn("Could not find node with anchor '{}'", anchor);
        }
        
        action |= Pop;
      }

      STR_UNDEF(anchor); // Anchor used
    }
    
    // Add kb_variant
    if (action & (AddScalarValue | AddMapping | AddSequence)) {
      
      // Add new variant and push is on top of stack
      if (kb_variant_is_obj(STACK_TOP)) { // Top is object
        KB_ASSERT(STR_CHECK(key), "Invalid YAML. Can't add to an map without an key");

        kb_variant* obj = kb_variant_obj_put(STACK_TOP, key);
        stack[stack_pos++]  = obj;
        list[list_pos++]    = obj;
        
        STR_UNDEF(key); // Key used
      } else if (kb_variant_is_arr(STACK_TOP)) { // Top is array
        kb_variant* obj = kb_variant_arr_put(STACK_TOP);
        stack[stack_pos++]  = obj;
        list[list_pos++]    = obj;
      } else {
        KB_ASSERT(STR_CHECK(key), "Invalid YAML. Add is valid only when in object or array");
      }
      
      // Initialize the new variant

      // New object is object
      if (action & AddMapping) {
        kb_variant_create_obj(STACK_TOP, MAX_VARIANT_SIZE);
      }

      // New object is array
      if (action & AddSequence) {
        kb_variant_create_arr(STACK_TOP, MAX_VARIANT_SIZE);
      }

      // New object is scalar.
      if (action & AddScalarValue) {
        parse_scalar(STACK_TOP, scalar, tag, scalar_style);
        action |= Pop;
      }

      // New object has an alias
      if (event.data.alias.anchor) {
        kb_table_insert(&aliases, kb_hash_string((char*) event.data.alias.anchor), list_pos - 1);
      }
    }
    
    // Pop stack
    if (action & Pop) {
      stack_pos--;
    }

    action = None;
  
    yaml_event_delete(&event);
  } while (event_type != YAML_STREAM_END_EVENT);

  yaml_parser_delete(&parser);
  kb_table_destroy(&aliases);

  bool result = false;

  // Could (should?) support multiple documents
  if (kb_variant_arr_count(&root) > 0) {
    kb_variant_copy(variant, kb_variant_arr_at(&root, 0));
    result = true;
  }

  kb_variant_destroy(&root);
  
  return result;
}
