#ifndef DATATYPE99_H
#define DATATYPE99_H

#include <datatype99/unit_t.h>

#include <stddef.h>

#include <epilepsy.h>

#ifndef DATATYPE99_NO_ALIASES

#define datatype  datatype99
#define match     match99
#define matches   matches99
#define of        of99
#define otherwise otherwise99

#endif // DATATYPE99_NO_ALIASES

#define datatype99(name, ...)                                                                      \
    DATATYPE99_PRIV_GEN(name, EPILEPSY_eval(EPILEPSY_list(v(__VA_ARGS__))))                        \
    EPILEPSY_semicolon()

#define DATATYPE99_PRIV_GEN(name, variants)                                                        \
    EPILEPSY_eval(DATATYPE99_PRIV_GEN_TYPEDEFS(v(name), v(variants)))                              \
                                                                                                   \
        typedef struct name {                                                                      \
        enum { EPILEPSY_eval(DATATYPE99_PRIV_GEN_TAGS(v(name), v(variants))) } tag;                \
                                                                                                   \
        union {                                                                                    \
            EPILEPSY_eval(DATATYPE99_PRIV_GEN_UNION_FIELDS(v(name), v(variants)))                  \
        } data;                                                                                    \
    } name;                                                                                        \
                                                                                                   \
    EPILEPSY_eval(DATATYPE99_PRIV_GEN_CONSTRUCTORS(v(name), v(variants)))

#define match99(val, ...)                                                                          \
    for (const void *datatype99_priv_match_expr = (const void *)&(val);                            \
         datatype99_priv_match_expr != NULL;                                                       \
         datatype99_priv_match_expr = NULL)                                                        \
                                                                                                   \
        switch ((val).tag)

#define of99(tag, ...)                                                                             \
    break;                                                                                         \
    case tag##Tag:                                                                                 \
        EPILEPSY_listEval(EPILEPSY_listMapI(                                                       \
            EPILEPSY_appl(v(DATATYPE99_PRIV_GEN_BOUNDED_VAR), v(tag)),                             \
            EPILEPSY_list(v(__VA_ARGS__))))

#define DATATYPE99_PRIV_GEN_BOUNDED_VAR_IMPL(tag_, x, i)                                           \
    v(for (tag_##i *x = &((tag_##ChoiceT *)datatype99_priv_match_expr)->data.tag_._##i; x != NULL; \
           x = NULL))

#define otherwise99                                                                                \
    break;                                                                                         \
    default:

#define matches99(val, tag_) ((val).tag == tag_##Tag)

// Desugaring {
#define DATATYPE99_PRIV_GEN_TYPEDEFS(name, variants)                                               \
    EPILEPSY_call(DATATYPE99_PRIV_GEN_TYPEDEFS, name variants)
#define DATATYPE99_PRIV_GEN_TAGS(name, variants)                                                   \
    EPILEPSY_call(DATATYPE99_PRIV_GEN_TAGS, name variants)
#define DATATYPE99_PRIV_GEN_UNION_FIELDS(name, variants)                                           \
    EPILEPSY_call(DATATYPE99_PRIV_GEN_UNION_FIELDS, name variants)
#define DATATYPE99_PRIV_GEN_CONSTRUCTORS(name, variants)                                           \
    EPILEPSY_call(DATATYPE99_PRIV_GEN_CONSTRUCTORS, name variants)
#define DATATYPE99_PRIV_MAP_VARIANTS(f, variants)                                                  \
    EPILEPSY_call(DATATYPE99_PRIV_MAP_VARIANTS, f variants)
// }

// Implementation {

// Generate type definitions for variants {
#define DATATYPE99_PRIV_GEN_TYPEDEFS_IMPL(name, variants)                                          \
    EPILEPSY_listUnwrap(DATATYPE99_PRIV_MAP_VARIANTS(                                              \
        EPILEPSY_appl(v(DATATYPE99_PRIV_GEN_TYPEDEFS_MAP), v(name)),                               \
        EPILEPSY_listFilter(                                                                       \
            EPILEPSY_compose(v(EPILEPSY_not), v(DATATYPE99_PRIV_IS_EMPTY_VARIANT)),                \
            v(variants))))

#define DATATYPE99_PRIV_GEN_TYPEDEFS_MAP_IMPL(name, tag, ...)                                      \
    EPILEPSY_call(                                                                                 \
        DATATYPE99_PRIV_GEN_TYPEDEFS_MAP_AUX,                                                      \
        v(name, tag) EPILEPSY_listUnwrap(EPILEPSY_listMapI(                                        \
            v(DATATYPE99_PRIV_GEN_VARIANT_FIELD),                                                  \
            EPILEPSY_list(v(__VA_ARGS__))))                                                        \
            EPILEPSY_listUnwrap(EPILEPSY_listMapI(                                                 \
                EPILEPSY_appl(v(DATATYPE99_PRIV_GEN_FIELD_TYPEDEF), v(tag)),                       \
                EPILEPSY_list(v(__VA_ARGS__)))))

#define DATATYPE99_PRIV_GEN_TYPEDEFS_MAP_AUX_IMPL(name, tag, fields, field_typedefs)               \
    v(typedef struct {fields} name##tag; field_typedefs typedef struct name tag##ChoiceT;)

#define DATATYPE99_PRIV_GEN_VARIANT_FIELD_IMPL(x, i)      v(x _##i;)
#define DATATYPE99_PRIV_GEN_FIELD_TYPEDEF_IMPL(tag, x, i) v(typedef x tag##i;)
// }

// Generate tags of variants {
#define DATATYPE99_PRIV_GEN_TAGS_IMPL(name, variants)                                              \
    EPILEPSY_listUnwrapCommaSep(DATATYPE99_PRIV_MAP_VARIANTS(                                      \
        EPILEPSY_appl(v(DATATYPE99_PRIV_GEN_TAGS_MAP), v(name)),                                   \
        v(variants)))

#define DATATYPE99_PRIV_GEN_TAGS_MAP_IMPL(name, tag, ...) v(tag##Tag)
// }

// Generate a union of fields of possible data {
#define DATATYPE99_PRIV_GEN_UNION_FIELDS_IMPL(name, variants)                                      \
    EPILEPSY_listUnwrap(DATATYPE99_PRIV_MAP_VARIANTS(                                              \
        EPILEPSY_appl(v(DATATYPE99_PRIV_GEN_UNION_FIELDS_MAP), v(name)),                           \
        v(variants)))

#define DATATYPE99_PRIV_GEN_UNION_FIELDS_MAP_IMPL(name, tag, ...) v(name##tag tag;)
// }

// Generate value constructors {
#define DATATYPE99_PRIV_GEN_CONSTRUCTORS_IMPL(name, variants)                                      \
    EPILEPSY_listUnwrap(DATATYPE99_PRIV_MAP_VARIANTS(                                              \
        EPILEPSY_appl(v(DATATYPE99_PRIV_GEN_CONSTRUCTORS_MAP), v(name)),                           \
        v(variants)))

#define DATATYPE99_PRIV_GEN_CONSTRUCTORS_MAP_IMPL(name, tag, ...)                                  \
    EPILEPSY_call(                                                                                 \
        DATATYPE99_PRIV_GEN_CONSTRUCTORS_MAP_AUX,                                                  \
        v(name, tag) EPILEPSY_list(v(__VA_ARGS__)))

#define DATATYPE99_PRIV_GEN_CONSTRUCTORS_MAP_AUX_IMPL(name, tag, types)                            \
    EPILEPSY_call(                                                                                 \
        DATATYPE99_PRIV_GEN_CONSTRUCTOR,                                                           \
        v(name, tag) EPILEPSY_parenthesise(EPILEPSY_listUnwrapCommaSep(                            \
            EPILEPSY_listMapI(v(DATATYPE99_PRIV_GEN_PARAM), v(types))))                            \
            EPILEPSY_listUnwrapCommaSep(                                                           \
                EPILEPSY_listMapI(v(DATATYPE99_PRIV_GEN_PARAM_NAME), v(types))))

#define DATATYPE99_PRIV_GEN_CONSTRUCTOR_IMPL(name, tag_, params, ...)                              \
    v(inline static name tag_ params {                                                             \
        return ((name){.tag = tag_##Tag, .data.tag_ = {__VA_ARGS__}});                             \
    })

#define DATATYPE99_PRIV_GEN_PARAM_IMPL(x, i)       v(x _##i)
#define DATATYPE99_PRIV_GEN_PARAM_NAME_IMPL(_x, i) v(_##i)
// }

#define DATATYPE99_PRIV_IS_EMPTY_VARIANT_IMPL(variant)                                             \
    EPILEPSY_uintEq(EPILEPSY_variadicsCount(EPILEPSY_unparenthesise(v(variant))), v(1))

#define DATATYPE99_PRIV_MAP_VARIANTS_IMPL(f, variants)                                             \
    EPILEPSY_listMap(EPILEPSY_compose(v(f), v(EPILEPSY_unparenthesise)), v(variants))
// } (Implementation)

// Arity specifiers {
#define DATATYPE99_PRIV_GEN_TYPEDEFS_MAP_ARITY     2
#define DATATYPE99_PRIV_GEN_TAGS_MAP_ARITY         2
#define DATATYPE99_PRIV_GEN_UNION_FIELDS_MAP_ARITY 2
#define DATATYPE99_PRIV_GEN_CONSTRUCTORS_MAP_ARITY 2
#define DATATYPE99_PRIV_GEN_BOUNDED_VAR_ARITY      3
#define DATATYPE99_PRIV_GEN_VARIANT_FIELD_ARITY    2
#define DATATYPE99_PRIV_GEN_FIELD_TYPEDEF_ARITY    3
#define DATATYPE99_PRIV_GEN_PARAM_ARITY            2
#define DATATYPE99_PRIV_GEN_PARAM_NAME_ARITY       2

#define DATATYPE99_PRIV_IS_EMPTY_VARIANT_ARITY 1
// }

#endif // DATATYPE99_H
