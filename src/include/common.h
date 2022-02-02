//
// Created by Frityet on 2022-01-11.
//

#ifndef HANGMANC_COMMON_
#define HANGMANC_COMMON_

#define var     __auto_type

#ifdef bool
#undef bool
#endif
#ifdef true
#undef true
#endif
#ifdef false
#undef false
#endif
#ifdef TRUE
#undef TRUE
#endif
#ifdef FALSE
#undef FALSE
#endif

typedef enum {
    TRUE    = 1,
    FALSE   = 0,
    true    = 1,
    false   = 0
} bool;

static const void *nullptr = NULL;

#endif //HANGMANC_COMMON_
