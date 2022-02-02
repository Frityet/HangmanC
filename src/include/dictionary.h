//
// Created by Frityet on 2022-01-25.
//

#ifndef HANGMANC_DICTIONARY_
#define HANGMANC_DICTIONARY_

#include <stddef.h>

#include "generic_list.h"

#define keyvaluepair_t(_key, _value) struct { _key key; _value value; }
#define dictionary_t(_key, _value) keyvaluepair_t(_key, _value) *
#define DICTIONARY(_key, _value) LIST(keyvaluepair_t(_key, _value))

#define KEY_VALUE_PAIR(_key, _value) (keyvaluepair_t(typeof(_key), typeof(_value))){ _key, _value }

#endif //HANGMANC_DICTIONARY_
