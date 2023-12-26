#pragma once

/*
 * This a is a macro for creating field reflections in from a json object
 * This json object MUST BE j ,too bad so sad wish I cared
 */
#define FROM_JSON_FIELD(obj, field)                                            \
  if (json_obj.contains(#field)) {                                             \
    if (!json_obj[#field].is_null()) {                                         \
      obj.field = json_obj[#field];                                            \
    }                                                                          \
  } else {                                                                     \
    Utils::warning << "Missing field: " << #obj << "." << #field << std::endl; \
  }
/*
 * This is a macro for taking a json object and reflecting it to a struct
 * specified as ob json that is being reflected to MUST HAVE THE VARIABLE j if
 * you want i know you're too lazy to type out the json object name
 *
 */
#define TO_JSON_FIELD(obj, field) json_obj[#field] = obj.field;
