

#ifdef __cplusplus
/**
 * @file addons/cpp/flecs.hpp
 * @brief Flecs C++17 API.
 */

#pragma once
#include <type_traits>

// STL includes
//#include <type_traits>

/**
 * @defgroup cpp C++ API
 * @{
 */

export namespace flecs
{

struct world;
struct world_async_stage;
struct iter;
struct entity_view;
struct entity;
struct type;
struct table;
struct table_range;
struct untyped_component;

template <typename T>
struct component;

struct untyped_ref;
template <typename T>
struct ref;

namespace _
{
template <typename T, typename U = int>
struct type;

template <typename Func, typename ... Components>
struct each_delegate;

} // namespace _
} // export namespace flecs

// Types imported from C API
/**
 * @file addons/cpp/c_types.hpp
 * @brief Aliases for types/constants from C API
 */

#pragma once

export namespace flecs {

/**
 * @defgroup cpp_globals API Types & Globals
 * @ingroup cpp_core
 * Types & constants bridged from C API.
 *
 * @{
 */

using world_t = ecs_world_t;
using world_info_t = ecs_world_info_t;
using id_t = ecs_id_t;
using entity_t = ecs_entity_t;
using type_t = ecs_type_t;
using table_t = ecs_table_t;
using term_t = ecs_term_t;
using query_t = ecs_query_t;
using query_group_info_t = ecs_query_group_info_t;
using observer_t = ecs_observer_t;
using iter_t = ecs_iter_t;
using ref_t = ecs_ref_t;
using table_record_t = ecs_table_record_t;
using table_records_t = ecs_table_records_t;
using component_record_t = ecs_component_record_t;
using type_info_t = ecs_type_info_t;
using type_hooks_t = ecs_type_hooks_t;
using flags32_t = ecs_flags32_t;
using flags64_t = ecs_flags64_t;

enum inout_kind_t {
    InOutDefault = EcsInOutDefault,
    InOutNone = EcsInOutNone,
    InOutFilter = EcsInOutFilter,
    InOut = EcsInOut,
    In = EcsIn,
    Out = EcsOut
};

enum oper_kind_t {
    And = EcsAnd,
    Or = EcsOr,
    Not = EcsNot,
    Optional = EcsOptional,
    AndFrom = EcsAndFrom,
    OrFrom = EcsOrFrom,
    NotFrom = EcsNotFrom
};

enum query_cache_kind_t {
    QueryCacheDefault = EcsQueryCacheDefault,
    QueryCacheAuto = EcsQueryCacheAuto,
    QueryCacheAll = EcsQueryCacheAll,
    QueryCacheNone = EcsQueryCacheNone
};

/** Id bit flags */
const flecs::entity_t PAIR = ECS_PAIR;
const flecs::entity_t AUTO_OVERRIDE = ECS_AUTO_OVERRIDE;
const flecs::entity_t TOGGLE = ECS_TOGGLE;

////////////////////////////////////////////////////////////////////////////////
//// Builtin components and tags
////////////////////////////////////////////////////////////////////////////////

/* Builtin components */
using Component = EcsComponent;
using Identifier = EcsIdentifier;
using Poly = EcsPoly;
using DefaultChildComponent = EcsDefaultChildComponent;

/* Builtin tags */
const flecs::entity_t Query = EcsQuery;
const flecs::entity_t Observer = EcsObserver;
const flecs::entity_t Private = EcsPrivate;
const flecs::entity_t Module = EcsModule;
const flecs::entity_t Prefab = EcsPrefab;
const flecs::entity_t Disabled = EcsDisabled;
const flecs::entity_t Empty = EcsEmpty;
const flecs::entity_t Monitor = EcsMonitor;
const flecs::entity_t System = EcsSystem;
const flecs::entity_t Pipeline = ecs_id(EcsPipeline);
const flecs::entity_t Phase = EcsPhase;
const flecs::entity_t Constant = EcsConstant;

/* Builtin event tags */
const flecs::entity_t OnAdd = EcsOnAdd;
const flecs::entity_t OnRemove = EcsOnRemove;
const flecs::entity_t OnSet = EcsOnSet;
const flecs::entity_t OnTableCreate = EcsOnTableCreate;
const flecs::entity_t OnTableDelete = EcsOnTableDelete;

/* Builtin term flags */
const uint64_t Self = EcsSelf;
const uint64_t Up = EcsUp;
const uint64_t Trav = EcsTrav;
const uint64_t Cascade = EcsCascade;
const uint64_t Desc = EcsDesc;
const uint64_t IsVariable = EcsIsVariable;
const uint64_t IsEntity = EcsIsEntity;
const uint64_t IsName = EcsIsName;
const uint64_t TraverseFlags = EcsTraverseFlags;
const uint64_t TermRefFlags = EcsTermRefFlags;

/* Builtin entity ids */
const flecs::entity_t Flecs = EcsFlecs;
const flecs::entity_t FlecsCore = EcsFlecsCore;
const flecs::entity_t World = EcsWorld;

/* Component traits */
const flecs::entity_t Wildcard = EcsWildcard;
const flecs::entity_t Any = EcsAny;
const flecs::entity_t This = EcsThis;
const flecs::entity_t Transitive = EcsTransitive;
const flecs::entity_t Reflexive = EcsReflexive;
const flecs::entity_t Final = EcsFinal;
const flecs::entity_t Inheritable = EcsInheritable;
const flecs::entity_t PairIsTag = EcsPairIsTag;
const flecs::entity_t Exclusive = EcsExclusive;
const flecs::entity_t Acyclic = EcsAcyclic;
const flecs::entity_t Traversable = EcsTraversable;
const flecs::entity_t Symmetric = EcsSymmetric;
const flecs::entity_t With = EcsWith;
const flecs::entity_t OneOf = EcsOneOf;
const flecs::entity_t Trait = EcsTrait;
const flecs::entity_t Relationship = EcsRelationship;
const flecs::entity_t Target = EcsTarget;
const flecs::entity_t CanToggle = EcsCanToggle;

/* OnInstantiate trait */
const flecs::entity_t OnInstantiate = EcsOnInstantiate;
const flecs::entity_t Override = EcsOverride;
const flecs::entity_t Inherit = EcsInherit;
const flecs::entity_t DontInherit = EcsDontInherit;

/* OnDelete/OnDeleteTarget traits */
const flecs::entity_t OnDelete = EcsOnDelete;
const flecs::entity_t OnDeleteTarget = EcsOnDeleteTarget;
const flecs::entity_t Remove = EcsRemove;
const flecs::entity_t Delete = EcsDelete;
const flecs::entity_t Panic = EcsPanic;

/* Builtin relationships */
const flecs::entity_t IsA = EcsIsA;
const flecs::entity_t ChildOf = EcsChildOf;
const flecs::entity_t DependsOn = EcsDependsOn;
const flecs::entity_t SlotOf = EcsSlotOf;

/* Misc */
const flecs::entity_t OrderedChildren = EcsOrderedChildren;
const flecs::entity_t Singleton = EcsSingleton;

/* Builtin identifiers */
const flecs::entity_t Name = EcsName;
const flecs::entity_t Symbol = EcsSymbol;

/* Storage */
const flecs::entity_t Sparse = EcsSparse;
const flecs::entity_t DontFragment = EcsDontFragment;

/* Builtin predicates for comparing entity ids in queries. */
const flecs::entity_t PredEq = EcsPredEq;
const flecs::entity_t PredMatch = EcsPredMatch;
const flecs::entity_t PredLookup = EcsPredLookup;

/* Builtin marker entities for query scopes */
const flecs::entity_t ScopeOpen = EcsScopeOpen;
const flecs::entity_t ScopeClose = EcsScopeClose;

/** @} */

}


// C++ utilities
/**
 * @file addons/cpp/utils/utils.hpp
 * @brief Flecs STL (FTL?)
 *
 * Flecs STL (FTL?)
 * Minimalistic utilities that allow for STL like functionality without having
 * to depend on the actual STL.
 */

// Macros so that C++ new calls can allocate using ecs_os_api memory allocation functions
// Rationale:
//  - Using macros here instead of a templated function bc clients might override ecs_os_malloc
//    to contain extra debug info like source tracking location. Using a template function
//    in that scenario would collapse all source location into said function vs. the
//    actual call site
//  - FLECS_PLACEMENT_NEW(): exists to remove any naked new calls/make it easy to identify any regressions
//    by grepping for new/delete

#define FLECS_PLACEMENT_NEW(_ptr, _type)  ::new(flecs::_::placement_new_tag, _ptr) _type
#define FLECS_NEW(_type)                  FLECS_PLACEMENT_NEW(ecs_os_malloc(sizeof(_type)), _type)
#define FLECS_DELETE(_ptr)          \
  do {                              \
    if (_ptr) {                     \
      flecs::_::destruct_obj(_ptr); \
      ecs_os_free(_ptr);            \
    }                               \
  } while (false)

/* Faster (compile time) alternatives to std::move / std::forward. From:
 *   https://www.foonathan.net/2020/09/move-forward/
 */

#define FLECS_MOV(...) \
  static_cast<flecs::remove_reference_t<decltype(__VA_ARGS__)>&&>(__VA_ARGS__)

#define FLECS_FWD(...) \
  static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)

export namespace flecs
{

namespace _
{

// Dummy Placement new tag to disambiguate from any other operator new overrides
struct placement_new_tag_t{};
constexpr placement_new_tag_t placement_new_tag{};
template<class Ty> inline void destruct_obj(Ty* _ptr) { _ptr->~Ty(); }
template<class Ty> inline void free_obj(void* _ptr) {
    if (_ptr) {
        destruct_obj(static_cast<Ty*>(_ptr));
        ecs_os_free(_ptr);
    }
}

} // namespace _

} // export namespace flecs

// Allows overriding flecs_static_assert, which is useful when testing
#ifndef flecs_static_assert
#define flecs_static_assert(cond, str) static_assert(cond, str)
#endif

inline void* operator new(size_t,   flecs::_::placement_new_tag_t, void* _ptr) noexcept { return _ptr; }
inline void  operator delete(void*, flecs::_::placement_new_tag_t, void*)      noexcept {              }

export namespace flecs
{

// faster (compile time) alternative to std::conditional
template <bool> struct condition;

template <> struct condition<false> {
    template <typename T, typename F> using type = F;
};

template <> struct condition<true> {
    template <typename T, typename F> using type = T;
};

// C++11/C++14 convenience template replacements

template <bool C, typename T, typename F>
using conditional_t = typename condition<C>::template type<T, F>;

template <typename T>
using decay_t = typename std::decay<T>::type;

template <bool V, typename T = void>
using enable_if_t = typename std::enable_if<V, T>::type;

template <typename T>
using remove_pointer_t = typename std::remove_pointer<T>::type;

template <typename T>
using remove_reference_t = typename std::remove_reference<T>::type;

template <typename T>
using underlying_type_t = typename std::underlying_type<T>::type;

using std::is_base_of;
using std::is_empty;
using std::is_const;
using std::is_pointer;
using std::is_reference;
using std::is_volatile;
using std::is_same;
using std::is_enum;

// GCC 4.9.2 compatibility: missing C++11 type traits
#if defined(__GNUC__) && (__GNUC__ == 4) && (__GNUC_MINOR__ == 9)
// Direct implementations for missing type traits in GCC 4.9.2
} // export namespace flecs

namespace std {
    // Only implement the ones that are actually missing in GCC 4.9.2
    template<typename T>
    struct is_trivially_constructible {
        const bool value = __is_pod(T);
    };

    template<typename T>
    struct is_trivially_move_assignable {
        const bool value = __is_pod(T);
    };

    template<typename T>
    struct is_trivially_copy_assignable {
        const bool value = __is_pod(T);
    };

    template<typename T>
    struct is_trivially_copy_constructible {
        const bool value = __is_pod(T);
    };

    template<typename T>
    struct is_trivially_move_constructible {
        const bool value = __is_pod(T);
    };

    template<typename T>
    struct is_trivially_copyable {
        const bool value = __is_pod(T);
    };
}

export namespace flecs {
#else
using std::is_trivially_constructible;
using std::is_trivially_move_assignable;
using std::is_trivially_copy_assignable;
using std::is_trivially_copy_constructible;
using std::is_trivially_move_constructible;
using std::is_trivially_copyable;
#endif

// These exist in GCC 4.9.2, so we can always use them
using std::is_move_assignable;
using std::is_move_constructible;
using std::is_copy_constructible;
using std::is_trivially_destructible;

// Determine constness even if T is a pointer type
template <typename T>
using is_const_p = is_const< remove_pointer_t<T> >;

// Apply cv modifiers from source type to destination type
// (from: https://stackoverflow.com/questions/52559336/add-const-to-type-if-template-arg-is-const)
template<class Src, class Dst>
using transcribe_const_t = conditional_t<is_const<Src>::value, Dst const, Dst>;

template<class Src, class Dst>
using transcribe_volatile_t = conditional_t<is_volatile<Src>::value, Dst volatile, Dst>;

template<class Src, class Dst>
using transcribe_cv_t = transcribe_const_t< Src, transcribe_volatile_t< Src, Dst> >;

template<class Src, class Dst>
using transcribe_pointer_t = conditional_t<is_pointer<Src>::value, Dst*, Dst>;

template<class Src, class Dst>
using transcribe_cvp_t = transcribe_cv_t< Src, transcribe_pointer_t< Src, Dst> >;


// More convenience templates. The if_*_t templates use int as default type
// instead of void. This enables writing code that's a bit less cluttered when
// the templates are used in a template declaration:
//
//     enable_if_t<true>* = nullptr
// vs:
//     if_t<true> = 0

template <bool V>
using if_t = enable_if_t<V, int>;

template <bool V>
using if_not_t = enable_if_t<false == V, int>;

namespace _
{

// Utility to prevent static assert from immediately triggering
template <class... T>
struct always_false {
    const bool value = false;
};

} // namespace _

} // export namespace flecs

/**
 * @file addons/cpp/utils/array.hpp
 * @brief Array class.
 *
 * Array class. Simple std::array like utility that is mostly there to aid
 * template code where template expansion would lead to an array with size 0.
 */

export namespace flecs {

template <typename T>
struct array_iterator
{
    explicit array_iterator(T* value, int index) {
        value_ = value;
        index_ = index;
    }

    bool operator!=(array_iterator const& other) const
    {
        return index_ != other.index_;
    }

    T & operator*() const
    {
        return value_[index_];
    }

    array_iterator& operator++()
    {
        ++index_;
        return *this;
    }

private:
    T* value_;
    int index_;
};

template <typename T, size_t Size, class Enable = void>
struct array final { };

template <typename T, size_t Size>
struct array<T, Size, enable_if_t<Size != 0> > final {
    array() {};

    array(const T (&elems)[Size]) {
        int i = 0;
        for (auto it = this->begin(); it != this->end(); ++ it) {
            *it = elems[i ++];
        }
    }

    T& operator[](int index) {
        return array_[index];
    }

    T& operator[](size_t index) {
        return array_[index];
    }

    array_iterator<T> begin() {
        return array_iterator<T>(array_, 0);
    }

    array_iterator<T> end() {
        return array_iterator<T>(array_, Size);
    }

    size_t size() {
        return Size;
    }

    T* ptr() {
        return array_;
    }

    template <typename Func>
    void each(const Func& func) {
        for (auto& elem : *this) {
            func(elem);
        }
    }

private:
    T array_[Size];
};

template<typename T, size_t Size>
array<T, Size> to_array(const T (&elems)[Size]) {
    return array<T, Size>(elems);
}

// Specialized class for zero-sized array
template <typename T, size_t Size>
struct array<T, Size, enable_if_t<Size == 0>> final {
    array() {};
    array(const T* (&elems)) { (void)elems; }
    T operator[](size_t index) { ecs_os_abort(); (void)index; return T(); }
    array_iterator<T> begin() { return array_iterator<T>(nullptr, 0); }
    array_iterator<T> end() { return array_iterator<T>(nullptr, 0); }

    size_t size() {
        return 0;
    }

    T* ptr() {
        return NULL;
    }
};

}

/**
 * @file addons/cpp/utils/string.hpp
 * @brief String utility that doesn't implicitly allocate memory.
 */

export namespace flecs {

struct string_view;

// This removes dependencies on std::string (and therefore STL) and allows the
// API to return allocated strings without incurring additional allocations when
// wrapping in an std::string.
struct string {
    explicit string()
        : str_(nullptr)
        , const_str_("")
        , length_(0) { }

    explicit string(char *str)
        : str_(str)
        , const_str_(str ? str : "")
        , length_(str ? ecs_os_strlen(str) : 0) { }

    ~string() {
        // If flecs is included in a binary but is not used, it is possible that
        // the OS API is not initialized. Calling ecs_os_free in that case could
        // crash the application during exit. However, if a string has been set
        // flecs has been used, and OS API should have been initialized.
        if (str_) {
            ecs_os_free(str_);
        }
    }

    string(string&& str) noexcept {
        ecs_os_free(str_);
        str_ = str.str_;
        const_str_ = str.const_str_;
        length_ = str.length_;
        str.str_ = nullptr;
    }

    operator const char*() const {
        return const_str_;
    }

    string& operator=(string&& str) noexcept {
        ecs_os_free(str_);
        str_ = str.str_;
        const_str_ = str.const_str_;
        length_ = str.length_;
        str.str_ = nullptr;
        return *this;
    }

    // Ban implicit copies/allocations
    string& operator=(const string& str) = delete;
    string(const string& str) = delete;

    bool operator==(const flecs::string& str) const {
        if (str.const_str_ == const_str_) {
            return true;
        }

        if (!const_str_ || !str.const_str_) {
            return false;
        }

        if (str.length_ != length_) {
            return false;
        }

        return ecs_os_strcmp(str, const_str_) == 0;
    }

    bool operator!=(const flecs::string& str) const {
        return !(*this == str);
    }

    bool operator==(const char *str) const {
        if (const_str_ == str) {
            return true;
        }

        if (!const_str_ || !str) {
            return false;
        }

        return ecs_os_strcmp(str, const_str_) == 0;
    }

    bool operator!=(const char *str) const {
        return !(*this == str);
    }

    const char* c_str() const {
        return const_str_;
    }

    std::size_t length() const {
        return static_cast<std::size_t>(length_);
    }

    template <size_t N>
    static constexpr size_t length( char const (&)[N] ) {
        return N - 1;
    }

    std::size_t size() const {
        return length();
    }

    void clear() {
        ecs_os_free(str_);
        str_ = nullptr;
        const_str_ = nullptr;
    }

    bool contains(const char *substr) {
        if (const_str_) {
            return strstr(const_str_, substr) != nullptr;
        } else {
            return false;
        }
    }

protected:
    // Must be constructed through string_view. This allows for using the string
    // class for both owned and non-owned strings, which can reduce allocations
    // when code conditionally should store a literal or an owned string.
    // Making this constructor private forces the code to explicitly create a
    // string_view which emphasizes that the string won't be freed by the class.
    string(const char *str)
        : str_(nullptr)
        , const_str_(str ? str : "")
        , length_(str ? ecs_os_strlen(str) : 0) { }

    char *str_ = nullptr;
    const char *const_str_;
    ecs_size_t length_;
};

// For consistency, the API returns a string_view where it could have returned
// a const char*, so an application won't have to think about whether to call
// c_str() or not. The string_view is a thin wrapper around a string that forces
// the API to indicate explicitly when a string is owned or not.
struct string_view : string {
    explicit string_view(const char *str)
        : string(str) { }
};

}

/**
 * @file addons/cpp/utils/enum.hpp
 * @brief Compile time enum reflection utilities.
 *
 * Discover at compile time valid enumeration constants for an enumeration type
 * and their names. This is used to automatically register enum constants.
 */

#include <limits>

// 126, so that FLECS_ENUM_MAX_COUNT is 127 which is the largest value
// representable by an int8_t.
#define FLECS_ENUM_MAX(T) _::to_constant<T, 126>::value
#define FLECS_ENUM_MAX_COUNT (FLECS_ENUM_MAX(int) + 1)

// Flag to turn off enum reflection
#ifdef FLECS_CPP_NO_ENUM_REFLECTION
#define FLECS_CPP_ENUM_REFLECTION_SUPPORT 0
#endif

// Test if we're using a compiler that supports the required features
#ifndef FLECS_CPP_ENUM_REFLECTION_SUPPORT
#if !defined(__clang__) && defined(__GNUC__)
#if __GNUC__ > 7 || (__GNUC__ == 7 && __GNUC_MINOR__ >= 5)
#define FLECS_CPP_ENUM_REFLECTION_SUPPORT 1
#else
#define FLECS_CPP_ENUM_REFLECTION_SUPPORT 0
#endif
#else
#define FLECS_CPP_ENUM_REFLECTION_SUPPORT 1
#endif
#endif

#if defined(__clang__) && __clang_major__ >= 16
// https://reviews.llvm.org/D130058, https://reviews.llvm.org/D131307
#define flecs_enum_cast(T, v) __builtin_bit_cast(T, v)
#elif defined(__GNUC__) && __GNUC__ > 10
#define flecs_enum_cast(T, v) __builtin_bit_cast(T, v)
#else
#define flecs_enum_cast(T, v) static_cast<T>(v)
#endif

export namespace flecs {

/** Int to enum */
namespace _ {
template <typename E, underlying_type_t<E> Value>
struct to_constant {
    static constexpr E value = flecs_enum_cast(E, Value);
};

template <typename E, underlying_type_t<E> Value>
constexpr E to_constant<E, Value>::value;
}

/** Convenience type with enum reflection data */
template <typename E>
struct enum_data;

template <typename E>
enum_data<E> enum_type(flecs::world_t *world);

template <typename E>
struct enum_last {
    static constexpr E value = FLECS_ENUM_MAX(E);
};

/* Utility macro to override enum_last trait */
#define FLECS_ENUM_LAST(T, Last)\
    export namespace flecs {\
    template<>\
    struct enum_last<T> {\
        static constexpr T value = Last;\
    };\
    }

namespace _ {

#if INTPTR_MAX == INT64_MAX
    #ifdef ECS_TARGET_MSVC
        #if _MSC_VER >= 1929
            #define ECS_SIZE_T_STR "unsigned __int64"
        #else
            #define ECS_SIZE_T_STR "unsigned int"
        #endif
    #elif defined(__clang__)
        #define ECS_SIZE_T_STR "size_t"
    #else
        #ifdef ECS_TARGET_WINDOWS
            #define ECS_SIZE_T_STR "constexpr size_t; size_t = long long unsigned int"
        #else
            #define ECS_SIZE_T_STR "constexpr size_t; size_t = long unsigned int"
        #endif
    #endif
#else
    #ifdef ECS_TARGET_MSVC
        #if _MSC_VER >= 1929
            #define ECS_SIZE_T_STR "unsigned __int32"
        #else
            #define ECS_SIZE_T_STR "unsigned int"
        #endif
    #elif defined(__clang__)
        #define ECS_SIZE_T_STR "size_t"
    #else
        #ifdef ECS_TARGET_WINDOWS
            #define ECS_SIZE_T_STR "constexpr size_t; size_t = unsigned int"
        #else
            #define ECS_SIZE_T_STR "constexpr size_t; size_t = unsigned int"
        #endif
    #endif
#endif

template <typename E>
constexpr size_t enum_type_len() {
    return ECS_FUNC_TYPE_LEN(, enum_type_len, ECS_FUNC_NAME)
        - (sizeof(ECS_SIZE_T_STR) - 1u);
}

/** Test if value is valid for enumeration.
 * This function leverages that when a valid value is provided,
 * __PRETTY_FUNCTION__ contains the enumeration name, whereas if a value is
 * invalid, the string contains a number or a negative (-) symbol. */
#if defined(ECS_TARGET_CLANG)
#if ECS_CLANG_VERSION < 13
template <typename E, E C>
constexpr bool enum_constant_is_valid() {
    return !((
        (ECS_FUNC_NAME[ECS_FUNC_NAME_FRONT(bool, enum_constant_is_valid) +
            enum_type_len<E>() + 6 /* ', C = ' */] >= '0') &&
        (ECS_FUNC_NAME[ECS_FUNC_NAME_FRONT(bool, enum_constant_is_valid) +
            enum_type_len<E>() + 6 /* ', C = ' */] <= '9')) ||
        (ECS_FUNC_NAME[ECS_FUNC_NAME_FRONT(bool, enum_constant_is_valid) +
            enum_type_len<E>() + 6 /* ', C = ' */] == '-'));
}
#else
template <typename E, E C>
constexpr bool enum_constant_is_valid() {
    return (ECS_FUNC_NAME[ECS_FUNC_NAME_FRONT(bool, enum_constant_is_valid) +
        enum_type_len<E>() + 6 /* ', E C = ' */] != '(');
}
#endif
#elif defined(ECS_TARGET_GNU)
template <typename E, E C>
constexpr bool enum_constant_is_valid() {
    return (ECS_FUNC_NAME[ECS_FUNC_NAME_FRONT(constexpr bool, enum_constant_is_valid) +
        enum_type_len<E>() + 8 /* ', E C = ' */] != '(');
}
#else
/* Use different trick on MSVC, since it uses hexadecimal representation for
 * invalid enum constants. We can leverage that msvc inserts a C-style cast
 * into the name, and the location of its first character ('(') is known. */
template <typename E, E C>
constexpr bool enum_constant_is_valid() {
    return ECS_FUNC_NAME[ECS_FUNC_NAME_FRONT(bool, enum_constant_is_valid) +
        enum_type_len<E>() + 1] != '(';
}
#endif

/* Without this wrapper __builtin_bit_cast doesn't work */
template <typename E, underlying_type_t<E> C>
constexpr bool enum_constant_is_valid_wrap() {
    return enum_constant_is_valid<E, flecs_enum_cast(E, C)>();
}

template <typename E, E C>
struct enum_is_valid {
    static constexpr bool value = enum_constant_is_valid<E, C>();
};

/** Extract name of constant from string */
template <typename E, E C>
const char* enum_constant_to_name() {
    const size_t len = ECS_FUNC_TYPE_LEN(
        const char*, enum_constant_to_name, ECS_FUNC_NAME);
    static char result[len + 1] = {};
    return ecs_cpp_get_constant_name(
        result, ECS_FUNC_NAME, string::length(ECS_FUNC_NAME),
            ECS_FUNC_NAME_BACK);
}

/**
 * @brief Provides utilities for enum reflection.
 *
 * This struct provides static functions for enum reflection, including
 * conversion between enum values and their underlying integral types, and
 * iteration over enum values.
 *
 * @tparam E The enum type.
 * @tparam Handler The handler for enum reflection operations.
 */
template <typename E, typename Handler>
struct enum_reflection {
    using U = underlying_type_t<E>;

    /**
     * @brief Iterates over the range [Low, High] of enum values between Low and
     * High.
     *
     * Recursively divide and conquers the search space to reduce the
     * template-depth. Once recursive division is complete, calls
     * Handle<E>::handle_constant in ascending order, passing the values
     * computed up the chain.
     *
     * @tparam Low The lower bound of the search range, inclusive.
     * @tparam High The upper bound of the search range, inclusive.
     * @tparam Args Additional arguments to be passed through to Handler::handle_constant
     * @param last_value The last value processed in the iteration.
     * @param args Additional arguments to be passed through to Handler::handle_constant
     * @return constexpr U The result of the iteration.
     */
    template <U Low, U High, typename... Args>
    static constexpr U each_enum_range(U last_value, Args&... args) {
        return High - Low <= 1
            ? High == Low
                ? Handler::template handle_constant<Low>(last_value, args...)
                : Handler::template handle_constant<High>(
                    Handler::template handle_constant<Low>(last_value, args...),
                    args...)
            : each_enum_range<(Low + High) / 2 + 1, High>(
                    each_enum_range<Low, (Low + High) / 2>(last_value, args...),
                    args...
              );
    }

    /**
     * @brief Iterates over the mask range (Low, High] of enum values between
     * Low and High.
     *
     * Recursively iterates the search space, looking for enums defined as
     * multiple-of-2 bitmasks. Each iteration, shifts bit to the right until it
     * hits Low, then calls Handler::handle_constant for each bitmask in
     * ascending order.
     *
     * @tparam Low The lower bound of the search range, not inclusive
     * @tparam High The upper bound of the search range, inclusive.
     * @tparam Args Additional arguments to be passed through to Handler::handle_constant
     * @param last_value The last value processed in the iteration.
     * @param args Additional arguments to be passed through to Handler::handle_constant
     * @return constexpr U The result of the iteration.
     */
    template <U Low, U High, typename... Args>
    static constexpr U each_mask_range(U last_value, Args&... args) {
        // If Low shares any bits with Current Flag, or if High is less
        // than/equal to Low (and High isn't negative because max-flag signed)
        return (Low & High) || (High <= Low && High != high_bit)
            ? last_value
            : Handler::template handle_constant<High>(
                each_mask_range<Low, ((High >> 1) & ~high_bit)>(last_value, args...),
                args...
              );
    }

    /**
     * @brief Handles enum iteration for gathering reflection data.
     *
     * Iterates over all enum values up to a specified maximum value
     * (each_enum_range<0, Value>), then iterates the rest of the possible bitmasks
     * (each_mask_range<Value, high_bit>).
     *
     * @tparam Value The maximum enum value to iterate up to.
     * @tparam Args Additional arguments to be passed through to Handler::handle_constant
     * @param args Additional arguments to be passed through to Handler::handle_constant
     * @return constexpr U The result of the iteration.
     */
    // template <U Value = static_cast<U>(FLECS_ENUM_MAX(E)), typename... Args>
    // static constexpr U each_enum(Args&... args) {
    //     return each_mask_range<Value, high_bit>(
    //         each_enum_range<0, Value>(0, args...), args...);
    // }
    /* to avoid warnings with bit manipulation, calculate the high bit with an
       unsigned type of the same size: */
    using UU = typename std::make_unsigned<U>::type;
    const U high_bit =
        static_cast<U>(static_cast<UU>(1) << (sizeof(UU) * 8 - 1));
};

/** Enumeration constant data */
template<typename T>
struct enum_constant {
    int32_t index; // Global index used to obtain world local entity id
    T value;
    T offset;
    const char *name;
};

/** Class that scans an enum for constants, extracts names & creates entities */
template <typename E>
struct enum_type {
private:
    using This = enum_type<E>;
    using U = underlying_type_t<E>;

    /**
     * @brief Handler struct for generating compile-time count of enum constants.
     */
    struct reflection_count {
        template <U Value,
            flecs::if_not_t< enum_constant_is_valid_wrap<E, Value>() > = 0>
        static constexpr U handle_constant(U last_value) {
            return last_value;
        }

        template <U Value,
            flecs::if_t< enum_constant_is_valid_wrap<E, Value>() > = 0>
        static constexpr U handle_constant(U last_value) {
            return 1 + last_value;
        }
    };

    /**
     * @brief Helper struct for filling enum_type's static `enum_data_impl<E>`
     * member with reflection data.
     *
     * Because reflection occurs in-order, we can use current value/last value
     * to determine continuity, and use that as a lookup heuristic later on.
     */
    struct reflection_init {
        template <U Value,
            flecs::if_not_t< enum_constant_is_valid_wrap<E, Value>() > = 0>
        static U handle_constant(U last_value, This&) {
            // Search for constant failed. Pass last valid value through.
            return last_value;
        }

        template <U Value,
            flecs::if_t< enum_constant_is_valid_wrap<E, Value>() > = 0>
        static U handle_constant(U last_value, This& me) {
            // Constant is valid, so fill reflection data.
            auto v = Value;
            const char *name = enum_constant_to_name<E, flecs_enum_cast(E, Value)>();

            ++me.max; // Increment cursor as we build constants array.

            // If the enum was previously contiguous, and continues to be
            // through the current value...
            if (me.has_contiguous && static_cast<U>(me.max) == v && me.contiguous_until == v) {
                ++me.contiguous_until;
            }

            // else, if the enum was never contiguous and hasn't been set as not
            // contiguous...
            else if (!me.contiguous_until && me.has_contiguous) {
                me.has_contiguous = false;
            }

            ecs_assert(!(last_value > 0 &&
                v < std::numeric_limits<U>::min() + last_value),
                ECS_UNSUPPORTED,
                "Signed integer enums causes integer overflow when recording "
                "offset from high positive to low negative. Consider using "
                "unsigned integers as underlying type.");

            me.constants[me.max].value = v;
            me.constants[me.max].offset = v - last_value;
            me.constants[me.max].name = name;
            if (!me.constants[me.max].index) {
                me.constants[me.max].index =
                    flecs_component_ids_index_get();
            }

            return v;
        }
    };
public:

    enum_type() {
        // Initialize/reset reflection data values to default state.
        min = 0;
        max = -1;
        has_contiguous = true;
        contiguous_until = 0;

#if FLECS_CPP_ENUM_REFLECTION_SUPPORT
        // enum_reflection<E, reflection_init>::
        //     template each_enum< static_cast<U>(enum_last<E>::value) >(*this);
#endif
    }

    static enum_type<E>& get() {
        static _::enum_type<E> instance;
        return instance;
    }

    flecs::entity_t entity(E value) const {
        int index = index_by_value(value);
        if (index >= 0) {
            return constants[index].id;
        }
        return 0;
    }

    void register_for_world(flecs::world_t *world, flecs::entity_t id) {
#if !FLECS_CPP_ENUM_REFLECTION_SUPPORT
        ecs_abort(ECS_UNSUPPORTED, "enum reflection requires gcc 7.5 or higher")
#endif

        ecs_log_push();
        ecs_cpp_enum_init(world, id, type<U>::id(world));

        for (U v = 0; v < static_cast<U>(max + 1); v ++) {
            if (constants[v].index) {
                flecs::entity_t constant = ecs_cpp_enum_constant_register(world,
                    type<E>::id(world), 0, constants[v].name, &constants[v].value,
                    type<U>::id(world), sizeof(U));

                flecs_component_ids_set(world, constants[v].index, constant);
            }
        }

        ecs_log_pop();
    }

    int min;
    int max;

    // If enum constants start not-sparse, contiguous_until will be the index of
    // the first sparse value, or end of the constants array
    U contiguous_until;

    // Compile-time generated count of enum constants.
    // static constexpr unsigned int constants_size =
    //     enum_reflection<E, reflection_count>::
    //         template each_enum< static_cast<U>(enum_last<E>::value) >();

    // Constants array is sized to the number of found-constants, or 1
    // to avoid 0-sized array.
    #ifdef FLECS_CPP_ENUM_REFLECTION
    enum_constant<U> constants[constants_size? constants_size: 1] = {};
    bool has_contiguous;
    #else
    // If we're not using enum reflection, we cannot statically determine the
    // upper bound of the enum, so use 128.
    enum_constant<U> constants[128] = {};
    bool has_contiguous = true; // Assume contiguous ids
    #endif
};

template <typename E, if_t< is_enum<E>::value > = 0>
inline void init_enum(flecs::world_t *world, flecs::entity_t id) {
    _::enum_type<E>::get().register_for_world(world, id);
}

template <typename E, if_not_t< is_enum<E>::value > = 0>
inline void init_enum(flecs::world_t*, flecs::entity_t) { }

} // namespace _

/** Enumeration type data wrapper with world pointer */
template <typename E>
struct enum_data {
    using U = underlying_type_t<E>;

    enum_data(flecs::world_t *world, _::enum_type<E>& impl)
        : world_(world)
        , impl_(impl) { }

	/**
     * @brief Checks if a given integral value is a valid enum value.
     *
     * @param value The integral value.
     * @return true If the value is a valid enum value.
     * @return false If the value is not a valid enum value.
     */
    bool is_valid(U value) {
        int index = index_by_value(value);
        if (index < 0) {
            return false;
        }
        return impl_.constants[index].index != 0;
    }

    /**
     * @brief Checks if a given enum value is valid.
     *
     * @param value The enum value.
     * @return true If the value is valid.
     * @return false If the value is not valid.
     */
    bool is_valid(E value) {
        return is_valid(static_cast<U>(value));
    }

    /**
     * @brief Finds the index into the constants array for a value, if one exists
     *
     * @param value The enum value.
     * @return int The index of the enum value.
     */
    int index_by_value(U value) const {
        if (impl_.max < 0) {
            return -1;
        }

        // Check if value is in contiguous lookup section
        if (impl_.has_contiguous && value < impl_.contiguous_until && value >= 0) {
            return static_cast<int>(value);
        }
        U accumulator = impl_.contiguous_until? impl_.contiguous_until - 1: 0;
        for (int i = static_cast<int>(impl_.contiguous_until); i <= impl_.max; ++i) {
            accumulator += impl_.constants[i].offset;
            if (accumulator == value) {
                return i;
            }
        }
        return -1;
    }

    /**
     * @brief Finds the index into the constants array for an enum value, if one exists
     *
     * @param value The enum value.
     * @return int The index of the enum value.
     */
    int index_by_value(E value) const {
        return index_by_value(static_cast<U>(value));
    }

    int first() const {
        return impl_.min;
    }

    int last() const {
        return impl_.max;
    }

    int next(int cur) const {
        return cur + 1;
    }

    flecs::entity entity() const;
    flecs::entity entity(U value) const;
    flecs::entity entity(E value) const;

    /**
     * @brief Manually register constant for enum.
     *
     * If automatic enum reflection is not supported, provide method for
     * manually registering constant.
     */
    #ifdef FLECS_CPP_NO_ENUM_REFLECTION
    void register_constant(flecs::world_t *world, U v, flecs::entity_t e) {
        if (v < 128) {
            if (!impl_.constants[v].index) {
                impl_.constants[v].index = flecs_component_ids_index_get();
            }

            flecs_component_ids_set(world, impl_.constants[v].index, e);

            impl_.max ++;

            if (impl_.contiguous_until <= v) {
                impl_.contiguous_until = v + 1;
            }
        }
    }
    #endif

    flecs::world_t *world_;
    _::enum_type<E>& impl_;
};

/** Convenience function for getting enum reflection data */
template <typename E>
enum_data<E> enum_type(flecs::world_t *world) {
    _::type<E>::id(world); // Ensure enum is registered
    auto& ref = _::enum_type<E>::get();
    return enum_data<E>(world, ref);
}

} // export namespace flecs

/**
 * @file addons/cpp/utils/stringstream.hpp
 * @brief Wrapper around ecs_strbuf_t that provides a simple stringstream like API.
 */

export namespace flecs {

struct stringstream {
    explicit stringstream()
        : buf_({}) { }

    ~stringstream() {
        ecs_strbuf_reset(&buf_);
    }

    stringstream(stringstream&& str) noexcept {
        ecs_strbuf_reset(&buf_);
        buf_ = str.buf_;
        str.buf_ = {};
    }

    stringstream& operator=(stringstream&& str) noexcept {
        ecs_strbuf_reset(&buf_);
        buf_ = str.buf_;
        str.buf_ = {};
        return *this;
    }

    // Ban implicit copies/allocations
    stringstream& operator=(const stringstream& str) = delete;
    stringstream(const stringstream& str) = delete;

    stringstream& operator<<(const char* str) {
        ecs_strbuf_appendstr(&buf_, str);
        return *this;
    }

    flecs::string str() {
        return flecs::string(ecs_strbuf_get(&buf_));
    }

private:
    ecs_strbuf_t buf_;
};

}

/**
 * @file addons/cpp/utils/function_traits.hpp
 * @brief Compile time utilities to inspect properties of functions.
 *
 * Code from: https://stackoverflow.com/questions/27024238/c-template-mechanism-to-get-the-number-of-function-arguments-which-would-work
 */

export namespace flecs {
namespace _ {

template <typename ... Args>
struct arg_list { };

// Base type that contains the traits
template <typename ReturnType, typename... Args>
struct function_traits_defs
{
    static constexpr bool is_callable = true;
    static constexpr size_t arity = sizeof...(Args);
    using return_type = ReturnType;
    using args = arg_list<Args ...>;
};

// Primary template for function_traits_impl
template <typename T>
struct function_traits_impl {
    static constexpr bool is_callable = false;
};

// Template specializations for the different kinds of function types (whew)
template <typename ReturnType, typename... Args>
struct function_traits_impl<ReturnType(Args...)>
    : function_traits_defs<ReturnType, Args...> {};

template <typename ReturnType, typename... Args>
struct function_traits_impl<ReturnType(*)(Args...)>
    : function_traits_defs<ReturnType, Args...> {};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits_impl<ReturnType(ClassType::*)(Args...)>
    : function_traits_defs<ReturnType, Args...> {};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits_impl<ReturnType(ClassType::*)(Args...) const>
    : function_traits_defs<ReturnType, Args...> {};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits_impl<ReturnType(ClassType::*)(Args...) const&>
    : function_traits_defs<ReturnType, Args...> {};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits_impl<ReturnType(ClassType::*)(Args...) const&&>
    : function_traits_defs<ReturnType, Args...> {};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits_impl<ReturnType(ClassType::*)(Args...) volatile>
    : function_traits_defs<ReturnType, Args...> {};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits_impl<ReturnType(ClassType::*)(Args...) volatile&>
    : function_traits_defs<ReturnType, Args...> {};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits_impl<ReturnType(ClassType::*)(Args...) volatile&&>
    : function_traits_defs<ReturnType, Args...> {};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits_impl<ReturnType(ClassType::*)(Args...) const volatile>
    : function_traits_defs<ReturnType, Args...> {};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits_impl<ReturnType(ClassType::*)(Args...) const volatile&>
    : function_traits_defs<ReturnType, Args...> {};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits_impl<ReturnType(ClassType::*)(Args...) const volatile&&>
    : function_traits_defs<ReturnType, Args...> {};

// Primary template for function_traits_no_cv. If T is not a function, the
// compiler will attempt to instantiate this template and fail, because its base
// is undefined.
template <typename T, typename V = void>
struct function_traits_no_cv
    : function_traits_impl<T> {};

// Specialized template for function types
template <typename T>
struct function_traits_no_cv<T, decltype((void)&T::operator())>
    : function_traits_impl<decltype(&T::operator())> {};

// Front facing template that decays T before ripping it apart.
template <typename T>
struct function_traits
    : function_traits_no_cv< decay_t<T> > {};

} // _


template <typename T>
struct is_callable {
    static constexpr bool value = _::function_traits<T>::is_callable;
};

template <typename T>
struct arity {
    static constexpr int value = _::function_traits<T>::arity;
};

template <typename T>
using return_type_t = typename _::function_traits<T>::return_type;

template <typename T>
using arg_list_t = typename _::function_traits<T>::args;

// First arg
template<typename Func, typename ... Args>
struct first_arg_impl;

template<typename Func, typename T, typename ... Args>
struct first_arg_impl<Func, _::arg_list<T, Args ...> > {
    using type = T;
};

template<typename Func>
struct first_arg {
    using type = typename first_arg_impl<Func, arg_list_t<Func>>::type;
};

template <typename Func>
using first_arg_t = typename first_arg<Func>::type;

// Last arg
template<typename Func, typename ... Args>
struct second_arg_impl;

template<typename Func, typename First, typename T, typename ... Args>
struct second_arg_impl<Func, _::arg_list<First, T, Args ...> > {
    using type = T;
};

template<typename Func>
struct second_arg {
    using type = typename second_arg_impl<Func, arg_list_t<Func>>::type;
};

template <typename Func>
using second_arg_t = typename second_arg<Func>::type;

} // flecs



// Mixin forward declarations
/**
 * @file addons/cpp/mixins/id/decl.hpp
 * @brief Id class.
 */

#pragma once

export namespace flecs {

struct id;
struct entity;

/**
 * @defgroup cpp_ids Ids
 * @ingroup cpp_core
 * Class for working with entity, component, tag and pair ids.
 *
 * @{
 */

/** Class that wraps around a flecs::id_t.
 * A flecs id is an identifier that can be added to entities. Ids can be:
 * - entities (including components, tags)
 * - pair ids
 * - entities with id flags set (like flecs::AUTO_OVERRIDE, flecs::TOGGLE)
 */
struct id {
    id()
        : world_(nullptr)
        , id_(0) { }

    explicit id(flecs::id_t value)
        : world_(nullptr)
        , id_(value) { }

    explicit id(flecs::world_t *world, flecs::id_t value = 0)
        : world_(world)
        , id_(value) { }

    explicit id(flecs::world_t *world, flecs::id_t first, flecs::id_t second)
        : world_(world)
        , id_(ecs_pair(first, second)) { }

    explicit id(flecs::world_t *world, const char *expr)
        : world_(world)
        , id_(ecs_id_from_str(world, expr)) { }

    explicit id(flecs::id_t first, flecs::id_t second)
        : world_(nullptr)
        , id_(ecs_pair(first, second)) { }

    explicit id(const flecs::id& first, const flecs::id& second)
        : world_(first.world_)
        , id_(ecs_pair(first.id_, second.id_)) { }

    /** Test if id is pair (has first, second) */
    bool is_pair() const {
        return (id_ & ECS_ID_FLAGS_MASK) == flecs::PAIR;
    }

    /** Test if id is a wildcard */
    bool is_wildcard() const {
        return ecs_id_is_wildcard(id_);
    }

    /** Test if id is entity */
    bool is_entity() const {
        return !(id_ & ECS_ID_FLAGS_MASK);
    }

    /** Return id as entity (only allowed when id is valid entity) */
    flecs::entity entity() const;

    /** Return id with role added */
    flecs::entity add_flags(flecs::id_t flags) const;

    /** Return id with role removed */
    flecs::entity remove_flags(flecs::id_t flags) const;

    /** Return id without role */
    flecs::entity remove_flags() const;

    /** Return id without role */
    flecs::entity remove_generation() const;

    /** Return component type of id */
    flecs::entity type_id() const;

    /** Test if id has specified role */
    bool has_flags(flecs::id_t flags) const {
        return ((id_ & flags) == flags);
    }

    /** Test if id has any role */
    bool has_flags() const {
        return (id_ & ECS_ID_FLAGS_MASK) != 0;
    }

    /** Return id flags set on id */
    flecs::entity flags() const;

    /** Test if id has specified first */
    bool has_relation(flecs::id_t first) const {
        if (!is_pair()) {
            return false;
        }
        return ECS_PAIR_FIRST(id_) == first;
    }

    /** Get first element from a pair.
     * If the id is not a pair, this operation will fail. When the id has a
     * world, the operation will ensure that the returned id has the correct
     * generation count. */
    flecs::entity first() const;

    /** Get second element from a pair.
     * If the id is not a pair, this operation will fail. When the id has a
     * world, the operation will ensure that the returned id has the correct
     * generation count. */
    flecs::entity second() const;

    /* Convert id to string */
    flecs::string str() const {
        return flecs::string(ecs_id_str(world_, id_));
    }

    /** Convert role of id to string. */
    flecs::string flags_str() const {
        return flecs::string_view( ecs_id_flag_str(id_ & ECS_ID_FLAGS_MASK));
    }

    /** Return flecs::id_t value */
    flecs::id_t raw_id() const {
        return id_;
    }

    operator flecs::id_t() const {
        return id_;
    }

    flecs::world world() const;

protected:
    /* World is optional, but guarantees that entity identifiers extracted from
     * the id are valid */
    flecs::world_t *world_;
    flecs::id_t id_;
};

/** @} */

}

/**
 * @file addons/cpp/mixins/term/decl.hpp
 * @brief Term declarations.
 */

#pragma once

export namespace flecs {

/**
 * @ingroup cpp_core_queries
 *
 * @{
 */

struct term;
struct term_builder;

/** @} */

}

/**
 * @file addons/cpp/mixins/query/decl.hpp
 * @brief Query declarations.
 */

#pragma once

export namespace flecs {

/**
 * @defgroup cpp_core_queries Queries
 * @ingroup cpp_core
 *
 * @{
 */

struct query_base;

template<typename ... Components>
struct query;

template<typename ... Components>
struct query_builder;

/** @} */

}

/**
 * @file addons/cpp/mixins/event/decl.hpp
 * @brief Event declarations.
 */

#pragma once

/**
 * @file addons/cpp/mixins/event/builder.hpp
 * @brief Event builder.
 */

#pragma once

#define ECS_EVENT_DESC_ID_COUNT_MAX (8)

export namespace flecs {

/**
 * @ingroup cpp_addons_event
 * @{
 */

/** Event builder interface */
template <typename Base, typename E>
struct event_builder_base {
    event_builder_base(flecs::world_t *world, flecs::entity_t event)
        : world_(world)
        , desc_{}
        , ids_{}
        , ids_array_{}
    {
        desc_.event = event;
    }

    /** Add component to emit for */
    template <typename T>
    Base& id() {
        ids_.array = ids_array_;
        ids_.array[ids_.count] = _::type<T>().id(world_);
        ids_.count ++;
        return *this;
    }

    /**
     * Add pair to emit for
     * @tparam First The first element of the pair.
     * @tparam Second the second element of a pair.
     */
    template <typename First, typename Second>
    Base& id() {
        return id(
            ecs_pair(_::type<First>::id(this->world_),
                _::type<Second>::id(this->world_)));
    }

    /**
     * Add pair to emit for
     * @tparam First The first element of the pair.
     * @param second The second element of the pair id.
     */
    template <typename First>
    Base& id(entity_t second) {
        return id(ecs_pair(_::type<First>::id(this->world_), second));
    }

    /**
     * Add pair to emit for
     * @param first The first element of the pair type.
     * @param second The second element of the pair id.
     */
    Base& id(entity_t first, entity_t second) {
        return id(ecs_pair(first, second));
    }

    template <typename Enum, if_t<is_enum<Enum>::value> = 0>
    Base& id(Enum value) {
        const auto& et = enum_type<Enum>(this->world_);
        flecs::entity_t target = et.entity(value);
        return id(et.entity(), target);
    }

    /** Add (component) id to emit for */
    Base& id(flecs::id_t id) {
        ids_.array = ids_array_;
        ids_.array[ids_.count] = id;
        ids_.count ++;
        return *this;
    }

    /** Set entity for which to emit event */
    Base& entity(flecs::entity_t e) {
        desc_.entity = e;
        return *this;
    }

    /* Set table for which to emit event */
    Base& table(flecs::table_t *t, int32_t offset = 0, int32_t count = 0) {
        desc_.table = t;
        desc_.offset = offset;
        desc_.count = count;
        return *this;
    }

    /* Set event data */
    Base& ctx(const E* ptr) {
        desc_.const_param = ptr;
        return *this;
    }

    /* Set event data */
    Base& ctx(E* ptr) {
        desc_.param = ptr;
        return *this;
    }

    void emit() {
        ids_.array = ids_array_;
        desc_.ids = &ids_;
        desc_.observable = const_cast<flecs::world_t*>(ecs_get_world(world_));
        ecs_emit(world_, &desc_);
    }

    void enqueue() {
        ids_.array = ids_array_;
        desc_.ids = &ids_;
        desc_.observable = const_cast<flecs::world_t*>(ecs_get_world(world_));
        ecs_enqueue(world_, &desc_);
    }

protected:
    flecs::world_t *world_;
    ecs_event_desc_t desc_;
    flecs::type_t ids_;
    flecs::id_t ids_array_[ECS_EVENT_DESC_ID_COUNT_MAX];

private:
    operator Base&() {
        return *static_cast<Base*>(this);
    }
};

struct event_builder : event_builder_base<event_builder, void> {
    using event_builder_base::event_builder_base;
};

template <typename E>
struct event_builder_typed : event_builder_base<event_builder_typed<E>, E> {
private:
    using Class = event_builder_typed<E>;

public:
    using event_builder_base<Class, E>::event_builder_base;

    /* Set event data */
    Class& ctx(const E& ptr) {
        this->desc_.const_param = &ptr;
        return *this;
    }

    /* Set event data */
    Class& ctx(E&& ptr) {
        this->desc_.param = &ptr;
        return *this;
    }
};

/** @} */

}


export namespace flecs {
namespace _ {

// Utility to derive event type from function
template  <typename Func, typename U = int>
struct event_from_func;

// Specialization for observer callbacks with a single argument
template  <typename Func>
struct event_from_func<Func, if_t< arity<Func>::value == 1>> {
    using type = decay_t<first_arg_t<Func>>;
};

// Specialization for observer callbacks with an initial entity src argument
template  <typename Func>
struct event_from_func<Func, if_t< arity<Func>::value == 2>> {
    using type = decay_t<second_arg_t<Func>>;
};

template <typename Func>
using event_from_func_t = typename event_from_func<Func>::type;

}
}

/**
 * @file addons/cpp/mixins/observer/decl.hpp
 * @brief Observer declarations.
 */

#pragma once

export namespace flecs {

/**
 * @defgroup cpp_observers Observers
 * @ingroup cpp_core
 * Observers let applications register callbacks for ECS events.
 *
 * @{
 */

struct observer;

template<typename ... Components>
struct observer_builder;

/** @} */

}

#ifdef FLECS_SYSTEM
/**
 * @file addons/cpp/mixins/system/decl.hpp
 * @brief System module declarations.
 */

#pragma once

export namespace flecs {

/**
 * @defgroup cpp_addons_systems Systems
 * @ingroup cpp_addons
 * Systems are a query + function that can be ran manually or by a pipeline.
 *
 * @{
 */

using TickSource = EcsTickSource;

struct system;

template<typename ... Components>
struct system_builder;

namespace _ {

void system_init(flecs::world& world);

/** @} */

} // namespace _
} // export namespace flecs

#endif
#ifdef FLECS_PIPELINE
/**
 * @file addons/cpp/mixins/pipeline/decl.hpp
 * @brief Pipeline module declarations.
 */

#pragma once

export namespace flecs {

/**
 * @defgroup cpp_pipelines Pipelines
 * @ingroup cpp_addons
 * Pipelines order and schedule systems for execution.
 *
 * @{
 */

template <typename ... Components>
struct pipeline;

template <typename ... Components>
struct pipeline_builder;

/* Builtin pipeline tags */
const flecs::entity_t OnStart = EcsOnStart;
const flecs::entity_t PreFrame = EcsPreFrame;
const flecs::entity_t OnLoad = EcsOnLoad;
const flecs::entity_t PostLoad = EcsPostLoad;
const flecs::entity_t PreUpdate = EcsPreUpdate;
const flecs::entity_t OnUpdate = EcsOnUpdate;
const flecs::entity_t OnValidate = EcsOnValidate;
const flecs::entity_t PostUpdate = EcsPostUpdate;
const flecs::entity_t PreStore = EcsPreStore;
const flecs::entity_t OnStore = EcsOnStore;
const flecs::entity_t PostFrame = EcsPostFrame;

/** @} */

}

#endif
#ifdef FLECS_TIMER
/**
 * @file addons/cpp/mixins/timer/decl.hpp
 * @brief Timer module declarations.
 */

#pragma once

export namespace flecs {

/**
 * @defgroup cpp_addons_timer Timer
 * @ingroup cpp_addons
 * Run systems at a time interval.
 *
 * @{
 */

using Timer = EcsTimer;
using RateFilter = EcsRateFilter;

struct timer;

/** @} */

namespace _ {

void timer_init(flecs::world& world);

} // namespace _
} // export namespace flecs

#endif
#ifdef FLECS_DOC
/**
 * @file addons/cpp/mixins/doc/decl.hpp
 * @brief Doc mixin declarations.
 */

#pragma once

export namespace flecs {
namespace doc {

/**
 * @defgroup cpp_addons_doc Doc
 * @ingroup cpp_addons
 * Utilities for documenting entities, components and systems.
 *
 * @{
 */

/** flecs.doc.Description component */
using Description = EcsDocDescription;

/** flecs.doc.Uuid component */
const flecs::entity_t Uuid = EcsDocUuid;

/** flecs.doc.Brief component */
const flecs::entity_t Brief = EcsDocBrief;

/** flecs.doc.Detail component */
const flecs::entity_t Detail = EcsDocDetail;

/** flecs.doc.Link component */
const flecs::entity_t Link = EcsDocLink;

/** flecs.doc.Color component */
const flecs::entity_t Color = EcsDocColor;

/** @private */
namespace _ {
/** @private */
void init(flecs::world& world);
}

/** @} */

}
}

#endif
#ifdef FLECS_REST
/**
 * @file addons/cpp/mixins/rest/decl.hpp
 * @brief Rest module declarations.
 */

#pragma once

export namespace flecs {

/**
 * @defgroup cpp_addons_rest Rest
 * @ingroup cpp_addons
 * REST API for querying and mutating entities.
 *
 * @{
 */

using Rest = EcsRest;

namespace rest {

namespace _ {

void init(flecs::world& world);

}
}

/** @} */

}

#endif
#ifdef FLECS_META
/**
 * @file addons/cpp/mixins/meta/decl.hpp
 * @brief Meta declarations.
 */

#pragma once

export namespace flecs {

/**
 * @defgroup cpp_addons_meta Meta
 * @ingroup cpp_addons
 * Flecs reflection framework.
 *
 * @{
 */

/* Primitive type aliases */
using bool_t = ecs_bool_t;
using char_t = ecs_char_t;
using u8_t = ecs_u8_t;
using u16_t = ecs_u16_t;
using u32_t = ecs_u32_t;
using u64_t = ecs_u64_t;
using uptr_t = ecs_uptr_t;
using i8_t = ecs_i8_t;
using i16_t = ecs_i16_t;
using i32_t = ecs_i32_t;
using i64_t = ecs_i64_t;
using iptr_t = ecs_iptr_t;
using f32_t = ecs_f32_t;
using f64_t = ecs_f64_t;

/* Embedded type aliases */
using member_t = ecs_member_t;
using enum_constant_t = ecs_enum_constant_t;
using bitmask_constant_t = ecs_bitmask_constant_t;

/* Components */
using Type = EcsType;
using TypeSerializer = EcsTypeSerializer;
using Primitive = EcsPrimitive;
using Enum = EcsEnum;
using Bitmask = EcsBitmask;
using Member = EcsMember;
using MemberRanges = EcsMemberRanges;
using Struct = EcsStruct;
using Array = EcsArray;
using Vector = EcsVector;
using Unit = EcsUnit;

/** Base type for bitmasks */
struct bitmask {
    uint32_t value;
};

/* Handles to builtin reflection types */
const flecs::entity_t Bool = ecs_id(ecs_bool_t);
const flecs::entity_t Char = ecs_id(ecs_char_t);
const flecs::entity_t Byte = ecs_id(ecs_byte_t);
const flecs::entity_t U8 = ecs_id(ecs_u8_t);
const flecs::entity_t U16 = ecs_id(ecs_u16_t);
const flecs::entity_t U32 = ecs_id(ecs_u32_t);
const flecs::entity_t U64 = ecs_id(ecs_u64_t);
const flecs::entity_t Uptr = ecs_id(ecs_uptr_t);
const flecs::entity_t I8 = ecs_id(ecs_i8_t);
const flecs::entity_t I16 = ecs_id(ecs_i16_t);
const flecs::entity_t I32 = ecs_id(ecs_i32_t);
const flecs::entity_t I64 = ecs_id(ecs_i64_t);
const flecs::entity_t Iptr = ecs_id(ecs_iptr_t);
const flecs::entity_t F32 = ecs_id(ecs_f32_t);
const flecs::entity_t F64 = ecs_id(ecs_f64_t);
const flecs::entity_t String = ecs_id(ecs_string_t);
const flecs::entity_t Entity = ecs_id(ecs_entity_t);
const flecs::entity_t Quantity = EcsQuantity;

namespace meta {

/* Type kinds supported by reflection system */
using type_kind_t = ecs_type_kind_t;
const type_kind_t PrimitiveType = EcsPrimitiveType;
const type_kind_t BitmaskType = EcsBitmaskType;
const type_kind_t EnumType = EcsEnumType;
const type_kind_t StructType = EcsStructType;
const type_kind_t ArrayType = EcsArrayType;
const type_kind_t VectorType = EcsVectorType;
const type_kind_t CustomType = EcsOpaqueType;
const type_kind_t TypeKindLast = EcsTypeKindLast;

/* Primitive type kinds supported by reflection system */
using primitive_kind_t = ecs_primitive_kind_t;
const primitive_kind_t Bool = EcsBool;
const primitive_kind_t Char = EcsChar;
const primitive_kind_t Byte = EcsByte;
const primitive_kind_t U8 = EcsU8;
const primitive_kind_t U16 = EcsU16;
const primitive_kind_t U32 = EcsU32;
const primitive_kind_t U64 = EcsU64;
const primitive_kind_t I8 = EcsI8;
const primitive_kind_t I16 = EcsI16;
const primitive_kind_t I32 = EcsI32;
const primitive_kind_t I64 = EcsI64;
const primitive_kind_t F32 = EcsF32;
const primitive_kind_t F64 = EcsF64;
const primitive_kind_t UPtr = EcsUPtr;
const primitive_kind_t IPtr = EcsIPtr;
const primitive_kind_t String = EcsString;
const primitive_kind_t Entity = EcsEntity;
const primitive_kind_t PrimitiveKindLast = EcsPrimitiveKindLast;

using op_t = ecs_meta_op_t;

/** @} */

namespace _ {

void init(flecs::world& world);

} // namespace _
} // namespace meta
} // export namespace flecs

/**
 * @file addons/cpp/mixins/meta/opaque.hpp
 * @brief Helpers for opaque type registration.
 */

#pragma once

export namespace flecs {

/**
 * @defgroup cpp_addons_meta Meta
 * @ingroup cpp_addons
 * Flecs reflection framework.
 *
 * @{
 */

/** Class for reading/writing dynamic values.
 *
 * @ingroup cpp_addons_meta
 */
struct cursor {
    cursor(flecs::world_t *world, flecs::entity_t type_id, void *ptr) {
        cursor_ = ecs_meta_cursor(world, type_id, ptr);
    }

    /** Push value scope (such as a nested struct) */
    int push() {
        return ecs_meta_push(&cursor_);
    }

    /** Pop value scope */
    int pop() {
        return ecs_meta_pop(&cursor_);
    }

    /** Move to next member/element */
    int next() {
        return ecs_meta_next(&cursor_);
    }

    /** Move to member by name */
    int member(const char *name) {
        return ecs_meta_member(&cursor_, name);
    }

    /** Move to element by index */
    int elem(int32_t elem) {
        return ecs_meta_elem(&cursor_, elem);
    }

    /** Test if current scope is a collection type */
    bool is_collection() {
        return ecs_meta_is_collection(&cursor_);
    }

    /** Get member name */
    flecs::string_view get_member() const {
        return flecs::string_view(ecs_meta_get_member(&cursor_));
    }

    /** Get type of value */
    flecs::entity get_type() const;

    /** Get unit of value */
    flecs::entity get_unit() const;

    /** Get untyped pointer to value */
    void* get_ptr() {
        return ecs_meta_get_ptr(&cursor_);
    }

    /** Set boolean value */
    int set_bool(bool value) {
        return ecs_meta_set_bool(&cursor_, value);
    }

    /** Set char value */
    int set_char(char value) {
        return ecs_meta_set_char(&cursor_, value);
    }

    /** Set signed int value */
    int set_int(int64_t value) {
        return ecs_meta_set_int(&cursor_, value);
    }

    /** Set unsigned int value */
    int set_uint(uint64_t value) {
        return ecs_meta_set_uint(&cursor_, value);
    }

    /** Set float value */
    int set_float(double value) {
        return ecs_meta_set_float(&cursor_, value);
    }

    /** Set string value */
    int set_string(const char *value) {
        return ecs_meta_set_string(&cursor_, value);
    }

    /** Set string literal value */
    int set_string_literal(const char *value) {
        return ecs_meta_set_string_literal(&cursor_, value);
    }

    /** Set entity value */
    int set_entity(flecs::entity_t value) {
        return ecs_meta_set_entity(&cursor_, value);
    }

    /** Set (component) id value */
    int set_id(flecs::id_t value) {
        return ecs_meta_set_id(&cursor_, value);
    }

    /** Set null value */
    int set_null() {
        return ecs_meta_set_null(&cursor_);
    }

    /** Get boolean value */
    bool get_bool() const {
        return ecs_meta_get_bool(&cursor_);
    }

    /** Get char value */
    char get_char() const {
        return ecs_meta_get_char(&cursor_);
    }

    /** Get signed int value */
    int64_t get_int() const {
        return ecs_meta_get_int(&cursor_);
    }

    /** Get unsigned int value */
    uint64_t get_uint() const {
        return ecs_meta_get_uint(&cursor_);
    }

    /** Get float value */
    double get_float() const {
        return ecs_meta_get_float(&cursor_);
    }

    /** Get string value */
    const char *get_string() const {
        return ecs_meta_get_string(&cursor_);
    }

    /** Get entity value */
    flecs::entity get_entity() const;

    /** Cursor object */
    ecs_meta_cursor_t cursor_;
};

/** @} */

}

/**
 * @file addons/cpp/mixins/meta/opaque.hpp
 * @brief Helpers for opaque type registration.
 */

#pragma once

export namespace flecs {

/**
 * @defgroup cpp_addons_meta Meta
 * @ingroup cpp_addons
 * Flecs reflection framework.
 *
 * @{
 */

/** Serializer object, used for serializing opaque types */
using serializer = ecs_serializer_t;

/** Serializer function, used to serialize opaque types */
using serialize_t = ecs_meta_serialize_t;

/** Type safe variant of serializer function */
template <typename T>
using serialize = int(*)(const serializer *, const T*);

/** Type safe variant of serialize_member function */
template <typename T>
using serialize_member = int(*)(const serializer *, const T*, const char* name);

/** Type safe variant of serialize_element function */
template <typename T>
using serialize_element = int(*)(const serializer *, const T*, size_t element);


/** Type safe interface for opaque types */
template <typename T, typename ElemType = void>
struct opaque {
    opaque(flecs::world_t *w = nullptr) : world(w) {
        if (world) {
            desc.entity = _::type<T>::id(world);
        }
    }

    /** Type that describes the type kind/structure of the opaque type */
    opaque& as_type(flecs::id_t func) {
        this->desc.type.as_type = func;
        return *this;
    }

    /** Serialize function */
    opaque& serialize(flecs::serialize<T> func) {
        this->desc.type.serialize =
            reinterpret_cast<decltype(
                this->desc.type.serialize)>(func);
        return *this;
    }

    /** Serialize member function */
    opaque& serialize_member(flecs::serialize_member<T> func) {
        this->desc.type.serialize_member =
            reinterpret_cast<decltype(
                this->desc.type.serialize_member)>(func);
        return *this;
    }

    /** Serialize element function */
    opaque& serialize_element(flecs::serialize_element<T> func) {
        this->desc.type.serialize_element =
            reinterpret_cast<decltype(
                this->desc.type.serialize_element)>(func);
        return *this;
    }

    /** Assign bool value */
    opaque& assign_bool(void (*func)(T *dst, bool value)) {
        this->desc.type.assign_bool =
            reinterpret_cast<decltype(
                this->desc.type.assign_bool)>(func);
        return *this;
    }

    /** Assign char value */
    opaque& assign_char(void (*func)(T *dst, char value)) {
        this->desc.type.assign_char =
            reinterpret_cast<decltype(
                this->desc.type.assign_char)>(func);
        return *this;
    }

    /** Assign int value */
    opaque& assign_int(void (*func)(T *dst, int64_t value)) {
        this->desc.type.assign_int =
            reinterpret_cast<decltype(
                this->desc.type.assign_int)>(func);
        return *this;
    }

    /** Assign unsigned int value */
    opaque& assign_uint(void (*func)(T *dst, uint64_t value)) {
        this->desc.type.assign_uint =
            reinterpret_cast<decltype(
                this->desc.type.assign_uint)>(func);
        return *this;
    }

    /** Assign float value */
    opaque& assign_float(void (*func)(T *dst, double value)) {
        this->desc.type.assign_float =
            reinterpret_cast<decltype(
                this->desc.type.assign_float)>(func);
        return *this;
    }

    /** Assign string value */
    opaque& assign_string(void (*func)(T *dst, const char *value)) {
        this->desc.type.assign_string =
            reinterpret_cast<decltype(
                this->desc.type.assign_string)>(func);
        return *this;
    }

    /** Assign entity value */
    opaque& assign_entity(
        void (*func)(T *dst, ecs_world_t *world, ecs_entity_t entity))
    {
        this->desc.type.assign_entity =
            reinterpret_cast<decltype(
                this->desc.type.assign_entity)>(func);
        return *this;
    }

    /** Assign (component) id value */
    opaque& assign_id(
        void (*func)(T *dst, ecs_world_t *world, ecs_id_t id))
    {
        this->desc.type.assign_id =
            reinterpret_cast<decltype(
                this->desc.type.assign_id)>(func);
        return *this;
    }

    /** Assign null value */
    opaque& assign_null(void (*func)(T *dst)) {
        this->desc.type.assign_null =
            reinterpret_cast<decltype(
                this->desc.type.assign_null)>(func);
        return *this;
    }

    /** Clear collection elements */
    opaque& clear(void (*func)(T *dst)) {
        this->desc.type.clear =
            reinterpret_cast<decltype(
                this->desc.type.clear)>(func);
        return *this;
    }

    /** Ensure & get collection element */
    opaque& ensure_element(ElemType* (*func)(T *dst, size_t elem)) {
        this->desc.type.ensure_element =
            reinterpret_cast<decltype(
                this->desc.type.ensure_element)>(func);
        return *this;
    }

    /** Ensure & get element */
    opaque& ensure_member(void* (*func)(T *dst, const char *member)) {
        this->desc.type.ensure_member =
            reinterpret_cast<decltype(
                this->desc.type.ensure_member)>(func);
        return *this;
    }

    /** Return number of elements */
    opaque& count(size_t (*func)(const T *dst)) {
        this->desc.type.count =
            reinterpret_cast<decltype(
                this->desc.type.count)>(func);
        return *this;
    }

    /** Resize to number of elements */
    opaque& resize(void (*func)(T *dst, size_t count)) {
        this->desc.type.resize =
            reinterpret_cast<decltype(
                this->desc.type.resize)>(func);
        return *this;
    }

    ~opaque() {
        if (world) {
            ecs_opaque_init(world, &desc);
        }
    }

    /** Opaque type descriptor */
    flecs::world_t *world = nullptr;
    ecs_opaque_desc_t desc = {};
};

/** @} */

}


#endif
#ifdef FLECS_UNITS
/**
 * @file addons/cpp/mixins/units/decl.hpp
 * @brief Units module declarations.
 */

#pragma once

export namespace flecs {
struct units {

/**
 * @defgroup cpp_addons_units Units
 * @ingroup cpp_addons
 * Common unit annotations for reflection framework.
 *
 * @{
 */

struct Prefixes { };

/**
 * @defgroup cpp_addons_units_prefixes Prefixes
 * @ingroup cpp_addons_units
 * Prefixes to indicate unit count (e.g. Kilo, Mega)
 *
 * @{
 */

struct Yocto { };
struct Zepto { };
struct Atto { };
struct Femto { };
struct Pico { };
struct Nano { };
struct Micro { };
struct Milli { };
struct Centi { };
struct Deci { };
struct Deca { };
struct Hecto { };
struct Kilo { };
struct Mega { };
struct Giga { };
struct Tera { };
struct Peta { };
struct Exa { };
struct Zetta { };
struct Yotta { };
struct Kibi { };
struct Mebi { };
struct Gibi { };
struct Tebi { };
struct Pebi { };
struct Exbi { };
struct Zebi { };
struct Yobi { };

/** @} */

/**
 * @defgroup cpp_addons_units_quantities Quantities
 * @ingroup cpp_addons_units
 * Quantities that group units (e.g. Length)
 *
 * @{
 */

struct Duration { };
struct Time { };
struct Mass { };
struct ElectricCurrent { };
struct LuminousIntensity { };
struct Force { };
struct Amount { };
struct Length { };
struct Pressure { };
struct Speed { };
struct Temperature { };
struct Data { };
struct DataRate { };
struct Angle { };
struct Frequency { };
struct Uri { };
struct Color { };

/** @} */

struct duration {
/**
 * @defgroup cpp_addons_units_duration Duration
 * @ingroup cpp_addons_units
 * @{
 */

struct PicoSeconds { };
struct NanoSeconds { };
struct MicroSeconds { };
struct MilliSeconds { };
struct Seconds { };
struct Minutes { };
struct Hours { };
struct Days { };

/** @} */
};

struct angle {
/**
 * @defgroup cpp_addons_units_angle Angle
 * @ingroup cpp_addons_units
 * @{
 */

struct Radians { };
struct Degrees { };

/** @} */
};


struct time {
/**
 * @defgroup cpp_addons_units_time Time
 * @ingroup cpp_addons_units
 * @{
 */

struct Date { };

/** @} */
};


struct mass {
/**
 * @defgroup cpp_addons_units_mass Mass
 * @ingroup cpp_addons_units
 * @{
 */

struct Grams { };
struct KiloGrams { };

/** @} */
};


struct electric_current {
/**
 * @defgroup cpp_addons_units_electric_current Electric Current
 * @ingroup cpp_addons_units
 * @{
 */

struct Ampere { };

/** @} */
};


struct amount {
/**
 * @defgroup cpp_addons_units_amount Amount
 * @ingroup cpp_addons_units
 * @{
 */

struct Mole { };

/** @} */
};


struct luminous_intensity {
/**
 * @defgroup cpp_addons_units_luminous_intensity Luminous Intensity
 * @ingroup cpp_addons_units
 * @{
 */

struct Candela { };

/** @} */
};


struct force {
/**
 * @defgroup cpp_addons_units_force Force
 * @ingroup cpp_addons_units
 * @{
 */

struct Newton { };

/** @} */
};


struct length {
/**
 * @defgroup cpp_addons_units_length Length
 * @ingroup cpp_addons_units
 * @{
 */

struct Meters { };
struct PicoMeters { };
struct NanoMeters { };
struct MicroMeters { };
struct MilliMeters { };
struct CentiMeters { };
struct KiloMeters { };
struct Miles { };
struct Pixels { };

/** @} */
};


struct pressure {
/**
 * @defgroup cpp_addons_units_pressure Pressure
 * @ingroup cpp_addons_units
 * @{
 */

struct Pascal { };
struct Bar { };

/** @} */
};


struct speed {
/**
 * @defgroup cpp_addons_units_speed Speed
 * @ingroup cpp_addons_units
 * @{
 */

struct MetersPerSecond { };
struct KiloMetersPerSecond { };
struct KiloMetersPerHour { };
struct MilesPerHour { };

/** @} */
};


struct temperature {
/**
 * @defgroup cpp_addons_units_temperature Temperature
 * @ingroup cpp_addons_units
 * @{
 */

struct Kelvin { };
struct Celsius { };
struct Fahrenheit { };

/** @} */
};


struct data {
/**
 * @defgroup cpp_addons_units_data Data
 * @ingroup cpp_addons_units
 * @{
 */

struct Bits { };
struct KiloBits { };
struct MegaBits { };
struct GigaBits { };
struct Bytes { };
struct KiloBytes { };
struct MegaBytes { };
struct GigaBytes { };
struct KibiBytes { };
struct MebiBytes { };
struct GibiBytes { };

/** @} */
};

struct datarate {
/**
 * @defgroup cpp_addons_units_datarate Data Rate
 * @ingroup cpp_addons_units
 * @{
 */

struct BitsPerSecond { };
struct KiloBitsPerSecond { };
struct MegaBitsPerSecond { };
struct GigaBitsPerSecond { };
struct BytesPerSecond { };
struct KiloBytesPerSecond { };
struct MegaBytesPerSecond { };
struct GigaBytesPerSecond { };

/** @} */
};


struct frequency {
/**
 * @defgroup cpp_addons_units_frequency Frequency
 * @ingroup cpp_addons_units
 * @{
 */

struct Hertz { };
struct KiloHertz { };
struct MegaHertz { };
struct GigaHertz { };

/** @} */
};


struct uri {
/**
 * @defgroup cpp_addons_units_uri Uri
 * @ingroup cpp_addons_units
 * @{
 */

struct Hyperlink { };
struct Image { };
struct File { };

/** @} */
};


struct color {
/**
 * @defgroup cpp_addons_units_color Color
 * @ingroup cpp_addons_units
 * @{
 */

struct Rgb { };
struct Hsl { };
struct Css { };

/** @} */
};

struct Percentage { };
struct Bel { };
struct DeciBel { };

units(flecs::world& world);

/** @} */

};
}

#endif
#ifdef FLECS_STATS
/**
 * @file addons/cpp/mixins/stats/decl.hpp
 * @brief Stats module declarations.
 */

#pragma once

export namespace flecs {

/**
 * @defgroup cpp_addons_stats Stats
 * @ingroup cpp_addons
 * The stats addon tracks statistics for the world and systems.
 *
 * @{
 */

/** Component that stores world statistics */
using WorldStats = EcsWorldStats;

/** Component that stores system/pipeline statistics */
using PipelineStats = EcsPipelineStats;

/** Component with world summary stats */
using WorldSummary = EcsWorldSummary;

struct stats {
    stats(flecs::world& world);
};

/** @} */

}

#endif
#ifdef FLECS_METRICS
/**
 * @file addons/cpp/mixins/metrics/decl.hpp
 * @brief Metrics declarations.
 */

#pragma once

/**
 * @file addons/cpp/mixins/metrics/builder.hpp
 * @brief Metric builder.
 */

#pragma once

#define ECS_EVENT_DESC_ID_COUNT_MAX (8)

export namespace flecs {

/**
 * @ingroup cpp_addons_metrics
 * @{
 */

/** Event builder interface */
struct metric_builder {
    metric_builder(flecs::world_t *world, flecs::entity_t entity)
        : world_(world)
    {
        desc_.entity = entity;
    }

    ~metric_builder();

    metric_builder& member(flecs::entity_t e) {
        desc_.member = e;
        return *this;
    }

    metric_builder& member(const char *name);

    template <typename T>
    metric_builder& member(const char *name);

    metric_builder& dotmember(const char *name);

    template <typename T>
    metric_builder& dotmember(const char *name);

    metric_builder& id(flecs::id_t the_id) {
        desc_.id = the_id;
        return *this;
    }

    metric_builder& id(flecs::entity_t first, flecs::entity_t second) {
        desc_.id = ecs_pair(first, second);
        return *this;
    }

    template <typename T>
    metric_builder& id() {
        return id(_::type<T>::id(world_));
    }

    template <typename First>
    metric_builder& id(flecs::entity_t second) {
        return id(_::type<First>::id(world_), second);
    }

    template <typename Second>
    metric_builder& id_second(flecs::entity_t first) {
        return id(first, _::type<Second>::id(world_));
    }

    template <typename First, typename Second>
    metric_builder& id() {
        return id<First>(_::type<Second>::id(world_));
    }

    metric_builder& targets(bool value = true) {
        desc_.targets = value;
        return *this;
    }

    metric_builder& kind(flecs::entity_t the_kind) {
        desc_.kind = the_kind;
        return *this;
    }

    template <typename Kind>
    metric_builder& kind() {
        return kind(_::type<Kind>::id(world_));
    }

    metric_builder& brief(const char *b) {
        desc_.brief = b;
        return *this;
    }

    operator flecs::entity();

protected:
    flecs::world_t *world_;
    ecs_metric_desc_t desc_ = {};
    bool created_ = false;
};

/**
 * @}
 */

}


export namespace flecs {

/**
 * @defgroup cpp_addons_metrics Metrics
 * @ingroup cpp_addons
 * The metrics module extracts metrics from components and makes them available
 * through a unified component interface.
 *
 * @{
 */

struct metrics {
    using Value = EcsMetricValue;
    using Source = EcsMetricSource;

    struct Instance { };
    struct Metric { };
    struct Counter { };
    struct CounterIncrement { };
    struct CounterId { };
    struct Gauge { };

    metrics(flecs::world& world);
};

/** @} */

}

#endif
#ifdef FLECS_ALERTS
/**
 * @file addons/cpp/mixins/alerts/decl.hpp
 * @brief Alert declarations.
 */

#pragma once

export namespace flecs {

/**
 * @defgroup cpp_addons_alerts Alerts
 * @ingroup cpp_addons
 * Alert implementation.
 *
 * @{
 */

/** Module */
struct alerts {
    using AlertsActive = EcsAlertsActive;
    using Instance = EcsAlertInstance;

    struct Alert { };
    struct Info { };
    struct Warning { };
    struct Error { };

    alerts(flecs::world& world);
};

template <typename ... Components>
struct alert;

template <typename ... Components>
struct alert_builder;

/** @} */

}

#endif
#ifdef FLECS_JSON
/**
 * @file addons/cpp/mixins/json/decl.hpp
 * @brief JSON addon declarations.
 */

#pragma once

export namespace flecs {

/**
 * @defgroup cpp_addons_json Json
 * @ingroup cpp_addons
 * Functions for serializing to/from JSON.
 *
 * @{
 */

using from_json_desc_t = ecs_from_json_desc_t;
using entity_to_json_desc_t = ecs_entity_to_json_desc_t;
using iter_to_json_desc_t = ecs_iter_to_json_desc_t;

/** @} */

}

#endif
#ifdef FLECS_APP
/**
 * @file addons/cpp/mixins/app/decl.hpp
 * @brief App addon declarations.
 */

#pragma once

/**
 * @file addons/cpp/mixins/app/builder.hpp
 * @brief App builder.
 */

#pragma once

export namespace flecs {

/**
 * @defgroup cpp_addons_app App
 * @ingroup cpp_addons
 * Optional addon for running the main application loop.
 *
 * @{
 */

/** App builder interface */
struct app_builder {
    app_builder(flecs::world_t *world)
        : world_(world)
        , desc_{}
    {
        const ecs_world_info_t *stats = ecs_get_world_info(world);
        desc_.target_fps = stats->target_fps;
        ecs_ftime_t t_zero = 0.0;
        if (ECS_EQ(desc_.target_fps, t_zero)) {
            desc_.target_fps = 60;
        }
    }

    app_builder& target_fps(ecs_ftime_t value) {
        desc_.target_fps = value;
        return *this;
    }

    app_builder& delta_time(ecs_ftime_t value) {
        desc_.delta_time = value;
        return *this;
    }

    app_builder& threads(int32_t value) {
        desc_.threads = value;
        return *this;
    }

    app_builder& frames(int32_t value) {
        desc_.frames = value;
        return *this;
    }

    app_builder& enable_rest(uint16_t port = 0) {
        desc_.enable_rest = true;
        desc_.port = port;
        return *this;
    }

    app_builder& enable_stats(bool value = true) {
        desc_.enable_stats = value;
        return *this;
    }

    app_builder& init(ecs_app_init_action_t value) {
        desc_.init = value;
        return *this;
    }

    app_builder& ctx(void *value) {
        desc_.ctx = value;
        return *this;
    }

    int run() {
        int result = ecs_app_run(world_, &desc_);
        if (ecs_should_quit(world_)) {
            // Only free world if quit flag is set. This ensures that we won't
            // try to cleanup the world if the app is used in an environment
            // that takes over the main loop, like with emscripten.
            if (!flecs_poly_release(world_)) {
                ecs_fini(world_);
            }
        }
        return result;
    }

private:
    flecs::world_t *world_;
    ecs_app_desc_t desc_;
};

/** @} */

}


#endif
#ifdef FLECS_SCRIPT
/**
 * @file addons/cpp/mixins/script/decl.hpp
 * @brief Script declarations.
 */

#pragma once

/**
 * @file addons/cpp/mixins/script/builder.hpp
 * @brief Script builder.
 */

#pragma once

export namespace flecs {

/**
 * @ingroup cpp_addons_script
 * @{
 */

/** Script builder interface */
struct script_builder {
    script_builder(flecs::world_t *world, const char *name = nullptr)
        : world_(world)
        , desc_{}
    {
        if (name != nullptr) {
            ecs_entity_desc_t entity_desc = {};
            entity_desc.name = name;
            entity_desc.sep = "::";
            entity_desc.root_sep = "::";
            this->desc_.entity = ecs_entity_init(world, &entity_desc);
        }
    }

    script_builder& code(const char *str) {
        desc_.code = str;
        return *this;
    }

    script_builder& filename(const char *str) {
        desc_.filename = str;
        return *this;
    }

    flecs::entity run() const;

protected:
    flecs::world_t *world_;
    ecs_script_desc_t desc_;
};

}


export namespace flecs {

/**
 * @defgroup cpp_addons_script Script
 * @ingroup cpp_addons
 *
 * @{
 */

struct script_builder;

using Script = EcsScript;

namespace script {
namespace _ {

void init(flecs::world& world);

} // namespace _
}

/** @} */

}

#endif

/**
 * @file addons/cpp/log.hpp
 * @brief Logging functions.
 */

#pragma once

export namespace flecs {
namespace log {

/**
 * @defgroup cpp_log Logging
 * @ingroup cpp_addons
 * Logging functions.
 *
 * @{
 */

/** Set log level */
inline void set_level(int level) {
    ecs_log_set_level(level);
}

inline int get_level() {
    return ecs_log_get_level();
}

/** Enable colors in logging */
inline void enable_colors(bool enabled = true) {
    ecs_log_enable_colors(enabled);
}

/** Enable timestamps in logging */
inline void enable_timestamp(bool enabled = true) {
    ecs_log_enable_timestamp(enabled);
}

/** Enable time delta in logging */
inline void enable_timedelta(bool enabled = true) {
    ecs_log_enable_timedelta(enabled);
}

/** Debug trace (level 1) */
inline void dbg(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ecs_logv(1, fmt, args);
    va_end(args);
}

/** Trace (level 0) */
inline void trace(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ecs_logv(0, fmt, args);
    va_end(args);
}

/** Trace (level -2) */
inline void warn(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ecs_logv(-2, fmt, args);
    va_end(args);
}

/** Trace (level -3) */
inline void err(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ecs_logv(-3, fmt, args);
    va_end(args);
}

/** Increase log indentation */
inline void push(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ecs_logv(0, fmt, args);
    va_end(args);
    ecs_log_push();
}

/** Increase log indentation */
inline void push() {
    ecs_log_push();
}

/** Increase log indentation */
inline void pop() {
    ecs_log_pop();
}

/** @} */

}
}

/**
 * @file addons/cpp/pair.hpp
 * @brief Utilities for working with compile time pairs.
 */

#pragma once

export namespace flecs {

namespace _ {
    struct pair_base { };
} // _


/**
 * @defgroup cpp_pair_type Pair type
 * @ingroup cpp_core
 * Compile time utilities for working with relationship pairs.
 *
 * @{
 */

/** Type that represents a pair.
 * The pair type can be used to represent a pair at compile time, and is able
 * to automatically derive the storage type associated with the pair, accessible
 * through pair::type.
 *
 * The storage type is derived using the following rules:
 * - if pair::first is non-empty, the storage type is pair::first
 * - if pair::first is empty and pair::second is non-empty, the storage type is pair::second
 *
 * The pair type can hold a temporary value so that it can be used in the
 * signatures of queries
 */
template <typename First, typename Second>
struct pair : _::pair_base {
    using type = conditional_t<!is_empty<First>::value || is_empty<Second>::value, First, Second>;
    using first = First;
    using second = Second;

    pair(type& v) : ref_(v) { }

    // This allows the class to be used as a temporary object
    pair(const type& v) : ref_(const_cast<type&>(v)) { }

    operator type&() {
        return ref_;
    }

    operator const type&() const {
        return ref_;
    }

    type* operator->() {
        return &ref_;
    }

    const type* operator->() const {
        return &ref_;
    }

    type& operator*() {
        return ref_;
    }

    const type& operator*() const {
        return ref_;
    }

private:
    type& ref_;
};

template <typename First, typename Second, if_t<is_empty<First>::value> = 0>
using pair_object = pair<First, Second>;

template <typename T>
using raw_type_t = remove_pointer_t<remove_reference_t<T>>;

/** Test if type is a pair. */
template <typename T>
struct is_pair {
    static constexpr bool value = is_base_of<_::pair_base, raw_type_t<T> >::value;
};

/** Get pair::first from pair while preserving cv qualifiers. */
template <typename P>
using pair_first_t = transcribe_cv_t<remove_reference_t<P>, typename raw_type_t<P>::first>;

/** Get pair::second from pair while preserving cv qualifiers. */
template <typename P>
using pair_second_t = transcribe_cv_t<remove_reference_t<P>, typename raw_type_t<P>::second>;

/** Get pair::type type from pair while preserving cv qualifiers and pointer type. */
template <typename P>
using pair_type_t = transcribe_cvp_t<remove_reference_t<P>, typename raw_type_t<P>::type>;

/** Get actual type from a regular type or pair. */
template <typename T, typename U = int>
struct actual_type;

template <typename T>
struct actual_type<T, if_not_t< is_pair<T>::value >> {
    using type = T;
};

template <typename T>
struct actual_type<T, if_t< is_pair<T>::value >> {
    using type = pair_type_t<T>;
};

template <typename T>
using actual_type_t = typename actual_type<T>::type;


// Get type without const, *, &
template<typename T>
struct base_type {
    using type = decay_t< actual_type_t<T> >;
};

template <typename T>
using base_type_t = typename base_type<T>::type;


// Get type without *, & (retains const which is useful for function args)
template<typename T>
struct base_arg_type {
    using type = remove_pointer_t< remove_reference_t< actual_type_t<T> > >;
};

template <typename T>
using base_arg_type_t = typename base_arg_type<T>::type;


// Test if type is the same as its actual type
template <typename T>
struct is_actual {
    static constexpr bool value =
        std::is_same<T, actual_type_t<T> >::value;
};

} // flecs

/**
 * @file addons/cpp/lifecycle_traits.hpp
 * @brief Utilities for discovering and registering component lifecycle hooks.
 */

#pragma once

export namespace flecs
{

namespace _
{

// T()
// Can't coexist with T(flecs::entity) or T(flecs::world, flecs::entity)
template <typename T>
void ctor_impl(void *ptr, int32_t count, const ecs_type_info_t *info) {
    (void)info; ecs_assert(info->size == ECS_SIZEOF(T),
        ECS_INTERNAL_ERROR, NULL);
    T *arr = static_cast<T*>(ptr);
    for (int i = 0; i < count; i ++) {
        FLECS_PLACEMENT_NEW(&arr[i], T);
    }
}

// ~T()
template <typename T>
void dtor_impl(void *ptr, int32_t count, const ecs_type_info_t *info) {
    (void)info; ecs_assert(info->size == ECS_SIZEOF(T),
        ECS_INTERNAL_ERROR, NULL);
    T *arr = static_cast<T*>(ptr);
    for (int i = 0; i < count; i ++) {
        arr[i].~T();
    }
}

// T& operator=(const T&)
template <typename T>
void copy_impl(void *dst_ptr, const void *src_ptr, int32_t count,
    const ecs_type_info_t *info)
{
    (void)info; ecs_assert(info->size == ECS_SIZEOF(T),
        ECS_INTERNAL_ERROR, NULL);
    T *dst_arr = static_cast<T*>(dst_ptr);
    const T *src_arr = static_cast<const T*>(src_ptr);
    for (int i = 0; i < count; i ++) {
        dst_arr[i] = src_arr[i];
    }
}

// T& operator=(T&&)
template <typename T>
void move_impl(void *dst_ptr, void *src_ptr, int32_t count,
    const ecs_type_info_t *info)
{
    (void)info; ecs_assert(info->size == ECS_SIZEOF(T),
        ECS_INTERNAL_ERROR, NULL);
    T *dst_arr = static_cast<T*>(dst_ptr);
    T *src_arr = static_cast<T*>(src_ptr);
    for (int i = 0; i < count; i ++) {
        dst_arr[i] = FLECS_MOV(src_arr[i]);
    }
}

// T(T&)
template <typename T>
void copy_ctor_impl(void *dst_ptr, const void *src_ptr, int32_t count,
    const ecs_type_info_t *info)
{
    (void)info; ecs_assert(info->size == ECS_SIZEOF(T),
        ECS_INTERNAL_ERROR, NULL);
    T *dst_arr = static_cast<T*>(dst_ptr);
    const T *src_arr = static_cast<const T*>(src_ptr);
    for (int i = 0; i < count; i ++) {
        FLECS_PLACEMENT_NEW(&dst_arr[i], T(src_arr[i]));
    }
}

// T(T&&)
template <typename T>
void move_ctor_impl(void *dst_ptr, void *src_ptr, int32_t count,
    const ecs_type_info_t *info)
{
    (void)info; ecs_assert(info->size == ECS_SIZEOF(T),
        ECS_INTERNAL_ERROR, NULL);
    T *dst_arr = static_cast<T*>(dst_ptr);
    T *src_arr = static_cast<T*>(src_ptr);
    for (int i = 0; i < count; i ++) {
        FLECS_PLACEMENT_NEW(&dst_arr[i], T(FLECS_MOV(src_arr[i])));
    }
}

// T(T&&), ~T()
// Typically used when moving to a new table, and removing from the old table
template <typename T>
void ctor_move_dtor_impl(void *dst_ptr, void *src_ptr, int32_t count,
    const ecs_type_info_t *info)
{
    (void)info; ecs_assert(info->size == ECS_SIZEOF(T),
        ECS_INTERNAL_ERROR, NULL);
    T *dst_arr = static_cast<T*>(dst_ptr);
    T *src_arr = static_cast<T*>(src_ptr);
    for (int i = 0; i < count; i ++) {
        FLECS_PLACEMENT_NEW(&dst_arr[i], T(FLECS_MOV(src_arr[i])));
        src_arr[i].~T();
    }
}

// Move assign + dtor (non-trivial move assignment)
// Typically used when moving a component to a deleted component
template <typename T, if_not_t<
    std::is_trivially_move_assignable<T>::value > = 0>
void move_dtor_impl(void *dst_ptr, void *src_ptr, int32_t count,
    const ecs_type_info_t *info)
{
    (void)info; ecs_assert(info->size == ECS_SIZEOF(T),
        ECS_INTERNAL_ERROR, NULL);
    T *dst_arr = static_cast<T*>(dst_ptr);
    T *src_arr = static_cast<T*>(src_ptr);
    for (int i = 0; i < count; i ++) {
        // Move assignment should free dst & assign dst to src
        dst_arr[i] = FLECS_MOV(src_arr[i]);
        // Destruct src. Move should have left object in a state where it no
        // longer holds resources, but it still needs to be destructed.
        src_arr[i].~T();
    }
}

// Move assign + dtor (trivial move assignment)
// Typically used when moving a component to a deleted component
template <typename T, if_t<
    std::is_trivially_move_assignable<T>::value > = 0>
void move_dtor_impl(void *dst_ptr, void *src_ptr, int32_t count,
    const ecs_type_info_t *info)
{
    (void)info; ecs_assert(info->size == ECS_SIZEOF(T),
        ECS_INTERNAL_ERROR, NULL);
    T *dst_arr = static_cast<T*>(dst_ptr);
    T *src_arr = static_cast<T*>(src_ptr);
    for (int i = 0; i < count; i ++) {
        // Cleanup resources of dst
        dst_arr[i].~T();
        // Copy src to dst
        dst_arr[i] = FLECS_MOV(src_arr[i]);
        // No need to destruct src. Since this is a trivial move the code
        // should be agnostic to the address of the component which means we
        // can pretend nothing got destructed.
    }
}

} // _

// Trait to test if type is constructible by flecs
template <typename T>
struct is_flecs_constructible {
    static constexpr bool value =
        std::is_default_constructible<actual_type_t<T>>::value;
};

namespace _
{

// Trivially constructible
template <typename T, if_t< std::is_trivially_constructible<T>::value > = 0>
ecs_xtor_t ctor(ecs_flags32_t &) {
    return nullptr;
}

// Not constructible by flecs
template <typename T, if_t<
    ! std::is_default_constructible<T>::value > = 0>
ecs_xtor_t ctor(ecs_flags32_t &flags) {
    flags |= ECS_TYPE_HOOK_CTOR_ILLEGAL;
    return nullptr;
}

// Default constructible
template <typename T, if_t<
    ! std::is_trivially_constructible<T>::value &&
    std::is_default_constructible<T>::value > = 0>
ecs_xtor_t ctor(ecs_flags32_t &) {
    return ctor_impl<T>;
}

// No dtor
template <typename T, if_t< std::is_trivially_destructible<T>::value > = 0>
ecs_xtor_t dtor(ecs_flags32_t &) {
    return nullptr;
}

// Dtor
template <typename T, if_t<
    std::is_destructible<T>::value &&
    ! std::is_trivially_destructible<T>::value > = 0>
ecs_xtor_t dtor(ecs_flags32_t &) {
    return dtor_impl<T>;
}

// Assert when the type cannot be destructed
template <typename T, if_not_t< std::is_destructible<T>::value > = 0>
ecs_xtor_t dtor(ecs_flags32_t &flags) {
    flecs_static_assert(always_false<T>::value,
        "component type must be destructible");
    flags |= ECS_TYPE_HOOK_DTOR_ILLEGAL;
    return nullptr;
}

// Trivially copyable
template <typename T, if_t< std::is_trivially_copyable<T>::value > = 0>
ecs_copy_t copy(ecs_flags32_t &) {
    return nullptr;
}

// Not copyable
template <typename T, if_t<
    ! std::is_trivially_copyable<T>::value &&
    ! std::is_copy_assignable<T>::value > = 0>
ecs_copy_t copy(ecs_flags32_t &flags) {
    flags |= ECS_TYPE_HOOK_COPY_ILLEGAL;
    return nullptr;
}

// Copy assignment
template <typename T, if_t<
    std::is_copy_assignable<T>::value &&
    ! std::is_trivially_copyable<T>::value > = 0>
ecs_copy_t copy(ecs_flags32_t &) {
    return copy_impl<T>;
}

// Trivially move assignable
template <typename T, if_t< std::is_trivially_move_assignable<T>::value > = 0>
ecs_move_t move(ecs_flags32_t &) {
    return nullptr;
}

// Component types must be move assignable
template <typename T, if_not_t< std::is_move_assignable<T>::value > = 0>
ecs_move_t move(ecs_flags32_t &flags) {
    flags |= ECS_TYPE_HOOK_MOVE_ILLEGAL;
    return nullptr;
}

// Move assignment
template <typename T, if_t<
    std::is_move_assignable<T>::value &&
    ! std::is_trivially_move_assignable<T>::value > = 0>
ecs_move_t move(ecs_flags32_t &) {
    return move_impl<T>;
}

// Trivially copy constructible
template <typename T, if_t<
    std::is_trivially_copy_constructible<T>::value > = 0>
ecs_copy_t copy_ctor(ecs_flags32_t &) {
    return nullptr;
}

// No copy ctor
template <typename T, if_t< ! std::is_copy_constructible<T>::value > = 0>
ecs_copy_t copy_ctor(ecs_flags32_t &flags) {
       flags |= ECS_TYPE_HOOK_COPY_CTOR_ILLEGAL;
    return nullptr;

}

// Copy ctor
template <typename T, if_t<
    std::is_copy_constructible<T>::value &&
    ! std::is_trivially_copy_constructible<T>::value > = 0>
ecs_copy_t copy_ctor(ecs_flags32_t &) {
    return copy_ctor_impl<T>;
}

// Trivially move constructible
template <typename T, if_t<
    std::is_trivially_move_constructible<T>::value > = 0>
ecs_move_t move_ctor(ecs_flags32_t &) {
    return nullptr;
}

// Component types must be move constructible
template <typename T, if_not_t< std::is_move_constructible<T>::value > = 0>
ecs_move_t move_ctor(ecs_flags32_t &flags) {
    flags |= ECS_TYPE_HOOK_MOVE_CTOR_ILLEGAL;
    return nullptr;
}

// Move ctor
template <typename T, if_t<
    std::is_move_constructible<T>::value &&
    ! std::is_trivially_move_constructible<T>::value > = 0>
ecs_move_t move_ctor(ecs_flags32_t &) {
    return move_ctor_impl<T>;
}

// Trivial merge (move assign + dtor)
template <typename T, if_t<
    std::is_trivially_move_constructible<T>::value  &&
    std::is_trivially_destructible<T>::value > = 0>
ecs_move_t ctor_move_dtor(ecs_flags32_t &) {
    return nullptr;
}

// Component types must be move constructible and destructible
template <typename T, if_t<
    ! std::is_move_constructible<T>::value ||
    ! std::is_destructible<T>::value > = 0>
ecs_move_t ctor_move_dtor(ecs_flags32_t &flags) {
    flags |= ECS_TYPE_HOOK_CTOR_MOVE_DTOR_ILLEGAL;
    return nullptr;
}

// Merge ctor + dtor
template <typename T, if_t<
    !(std::is_trivially_move_constructible<T>::value &&
      std::is_trivially_destructible<T>::value) &&
    std::is_move_constructible<T>::value &&
    std::is_destructible<T>::value > = 0>
ecs_move_t ctor_move_dtor(ecs_flags32_t &) {
    return ctor_move_dtor_impl<T>;
}

// Trivial merge (move assign + dtor)
template <typename T, if_t<
    std::is_trivially_move_assignable<T>::value  &&
    std::is_trivially_destructible<T>::value > = 0>
ecs_move_t move_dtor(ecs_flags32_t &) {
    return nullptr;
}

// Component types must be move constructible and destructible
template <typename T, if_t<
    ! std::is_move_assignable<T>::value ||
    ! std::is_destructible<T>::value > = 0>
ecs_move_t move_dtor(ecs_flags32_t &flags) {
    flags |= ECS_TYPE_HOOK_MOVE_DTOR_ILLEGAL;
    return nullptr;
}

// Merge assign + dtor
template <typename T, if_t<
    !(std::is_trivially_move_assignable<T>::value &&
      std::is_trivially_destructible<T>::value) &&
    std::is_move_assignable<T>::value &&
    std::is_destructible<T>::value > = 0>
ecs_move_t move_dtor(ecs_flags32_t &) {
    return move_dtor_impl<T>;
}

// Traits to check for operator<, operator>, and operator==
template<typename...>
using void_t = void;

// These traits causes a "float comparison warning" in some compilers
// when `T` is float or double.
// Disable this warning with the following pragmas:
#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wfloat-equal"
#elif defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wfloat-equal"
#endif

// Trait to check for operator<
template <typename T, typename = void>
struct has_operator_less : std::false_type {};

// Only enable if T has an operator< that takes T as the right-hand side (no implicit conversion)
template <typename T>
struct has_operator_less<T, void_t<decltype(std::declval<const T&>() < std::declval<const T&>())>> :
    std::is_same<decltype(std::declval<const T&>() < std::declval<const T&>()), bool> {};

// Trait to check for operator>
template <typename T, typename = void>
struct has_operator_greater : std::false_type {};

// Only enable if T has an operator> that takes T as the right-hand side (no implicit conversion)
template <typename T>
struct has_operator_greater<T, void_t<decltype(std::declval<const T&>() > std::declval<const T&>())>> :
    std::is_same<decltype(std::declval<const T&>() > std::declval<const T&>()), bool> {};

// Trait to check for operator==
template <typename T, typename = void>
struct has_operator_equal : std::false_type {};

// Only enable if T has an operator== that takes T as the right-hand side (no implicit conversion)
template <typename T>
struct has_operator_equal<T, void_t<decltype(std::declval<const T&>() == std::declval<const T&>())>> :
    std::is_same<decltype(std::declval<const T&>() == std::declval<const T&>()), bool> {};

// 1. Compare function if `<`, `>`, are defined
template <typename T, if_t<
    has_operator_less<T>::value &&
    has_operator_greater<T>::value &&
    !has_operator_equal<T>::value > = 0>
int compare_impl(const void *a, const void *b, const ecs_type_info_t *) {
    const T& lhs = *static_cast<const T*>(a);
    const T& rhs = *static_cast<const T*>(b);
    if (lhs < rhs) return -1;
    if (lhs > rhs) return 1;
    return 0;
}

// 2. Compare function if `<` and `==` are defined, ignoring `>`
// if defined.
template <typename T, if_t<
    has_operator_less<T>::value &&
    has_operator_equal<T>::value > = 0>
int compare_impl(const void *a, const void *b, const ecs_type_info_t *) {
    const T& lhs = *static_cast<const T*>(a);
    const T& rhs = *static_cast<const T*>(b);
    if (lhs == rhs) return 0;
    if (lhs < rhs) return -1;
    return 1; // If not less and not equal, must be greater
}

// 3. Compare function if `>` and `==` are defined, deducing `<`
template <typename T, if_t<
    has_operator_greater<T>::value &&
    has_operator_equal<T>::value &&
    !has_operator_less<T>::value > = 0>
int compare_impl(const void *a, const void *b, const ecs_type_info_t *) {
    const T& lhs = *static_cast<const T*>(a);
    const T& rhs = *static_cast<const T*>(b);
    if (lhs == rhs) return 0;
    if (lhs > rhs) return 1;
    return -1; // If not greater and not equal, must be less
}

// 4. Compare function if only `<` is defined, deducing the rest
template <typename T, if_t<
    has_operator_less<T>::value &&
    !has_operator_greater<T>::value &&
    !has_operator_equal<T>::value > = 0>
int compare_impl(const void *a, const void *b, const ecs_type_info_t *) {
    const T& lhs = *static_cast<const T*>(a);
    const T& rhs = *static_cast<const T*>(b);
    if (lhs < rhs) return -1;
    if (rhs < lhs) return 1;
    return 0; // If neither is less, they must be equal
}

// 5. Compare function if only `>` is defined, deducing the rest
template <typename T, if_t<
    has_operator_greater<T>::value &&
    !has_operator_less<T>::value &&
    !has_operator_equal<T>::value > = 0>
int compare_impl(const void *a, const void *b, const ecs_type_info_t *) {
    const T& lhs = *static_cast<const T*>(a);
    const T& rhs = *static_cast<const T*>(b);
    if (lhs > rhs) return 1;
    if (rhs > lhs) return -1;
    return 0; // If neither is greater, they must be equal
}

// In order to have a generated compare hook, at least
// operator> or operator< must be defined:
template <typename T, if_t<
    has_operator_less<T>::value ||
    has_operator_greater<T>::value > = 0>
ecs_cmp_t compare() {
    return compare_impl<T>;
}

template <typename T, if_t<
    !has_operator_less<T>::value &&
    !has_operator_greater<T>::value > = 0>
ecs_cmp_t compare() {
    return NULL;
}

// Equals function enabled only if `==` is defined
template <typename T, if_t<
    has_operator_equal<T>::value > = 0>
bool equals_impl(const void *a, const void *b, const ecs_type_info_t *) {
    const T& lhs = *static_cast<const T*>(a);
    const T& rhs = *static_cast<const T*>(b);
    return lhs == rhs;
}

template <typename T, if_t<
    has_operator_equal<T>::value > = 0>
ecs_equals_t equals() {
    return equals_impl<T>;
}

template <typename T, if_t<
    !has_operator_equal<T>::value > = 0>
ecs_equals_t equals() {
    return NULL;
}

// re-enable the float comparison warning:
#if defined(__clang__)
    #pragma clang diagnostic pop
#elif defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic pop
#endif

} // _
} // flecs

/**
 * @file addons/cpp/world.hpp
 * @brief World class.
 */

#pragma once

export namespace flecs
{

/* Static helper functions to assign a component value */

// set(T&&)
template <typename T>
inline void set(world_t *world, flecs::entity_t entity, T&& value, flecs::id_t id) {
    ecs_assert(_::type<T>::size() != 0, ECS_INVALID_PARAMETER,
            "operation invalid for empty type");

    ecs_cpp_get_mut_t res = ecs_cpp_set(world, entity, id, &value, sizeof(T));

    T& dst = *static_cast<remove_reference_t<T>*>(res.ptr);
    if constexpr (std::is_copy_assignable_v<T>) {
        dst = FLECS_FWD(value);
    } else {
        dst = FLECS_MOV(value);
    }

    if (res.call_modified) {
        ecs_modified_id(world, entity, id);
    }
}

// set(const T&)
template <typename T>
inline void set(world_t *world, flecs::entity_t entity, const T& value, flecs::id_t id) {
    ecs_assert(_::type<T>::size() != 0, ECS_INVALID_PARAMETER,
            "operation invalid for empty type");

    ecs_cpp_get_mut_t res = ecs_cpp_set(world, entity, id, &value, sizeof(T));

    T& dst = *static_cast<remove_reference_t<T>*>(res.ptr);
    dst = value;

    if (res.call_modified) {
        ecs_modified_id(world, entity, id);
    }
}

// set(T&&)
template <typename T, typename A>
inline void set(world_t *world, entity_t entity, A&& value) {
    id_t id = _::type<T>::id(world);
    flecs::set(world, entity, FLECS_FWD(value), id);
}

// set(const T&)
template <typename T, typename A>
inline void set(world_t *world, entity_t entity, const A& value) {
    id_t id = _::type<T>::id(world);
    flecs::set(world, entity, value, id);
}

// assign(T&&)
template <typename T>
inline void assign(world_t *world, flecs::entity_t entity, T&& value, flecs::id_t id) {
    ecs_assert(_::type<remove_reference_t<T>>::size() != 0,
        ECS_INVALID_PARAMETER, "operation invalid for empty type");

    ecs_cpp_get_mut_t res = ecs_cpp_assign(
        world, entity, id, &value, sizeof(T));

    T& dst = *static_cast<remove_reference_t<T>*>(res.ptr);
    if constexpr (std::is_copy_assignable_v<T>) {
        dst = FLECS_FWD(value);
    } else {
        dst = FLECS_MOV(value);
    }

    if (res.call_modified) {
        ecs_modified_id(world, entity, id);
    }
}

// assign(const T&)
template <typename T>
inline void assign(world_t *world, flecs::entity_t entity, const T& value, flecs::id_t id) {
    ecs_assert(_::type<remove_reference_t<T>>::size() != 0,
        ECS_INVALID_PARAMETER, "operation invalid for empty type");

    ecs_cpp_get_mut_t res = ecs_cpp_assign(
        world, entity, id, &value, sizeof(T));

    T& dst = *static_cast<remove_reference_t<T>*>(res.ptr);
    dst = value;

    if (res.call_modified) {
        ecs_modified_id(world, entity, id);
    }
}

// set(T&&)
template <typename T, typename A>
inline void assign(world_t *world, entity_t entity, A&& value) {
    id_t id = _::type<T>::id(world);
    flecs::assign(world, entity, FLECS_FWD(value), id);
}

// set(const T&)
template <typename T, typename A>
inline void assign(world_t *world, entity_t entity, const A& value) {
    id_t id = _::type<T>::id(world);
    flecs::assign(world, entity, value, id);
}


// emplace for T(Args...)
template <typename T, typename ... Args, if_t<
    std::is_constructible<actual_type_t<T>, Args...>::value ||
    std::is_default_constructible<actual_type_t<T>>::value > = 0>
inline void emplace(world_t *world, flecs::entity_t entity, flecs::id_t id, Args&&... args) {
    ecs_assert(_::type<T>::size() != 0, ECS_INVALID_PARAMETER,
            "operation invalid for empty type");
    T& dst = *static_cast<T*>(ecs_emplace_id(world, entity, id, sizeof(T), nullptr));

    FLECS_PLACEMENT_NEW(&dst, T{FLECS_FWD(args)...});

    ecs_modified_id(world, entity, id);
}

/** Return id without generation.
 *
 * @see ecs_strip_generation()
 */
inline flecs::id_t strip_generation(flecs::entity_t e) {
    return ecs_strip_generation(e);
}

/** Return entity generation.
 */
inline uint32_t get_generation(flecs::entity_t e) {
    return ECS_GENERATION(e);
}

struct scoped_world;

/**
 * @defgroup cpp_world World
 * @ingroup cpp_core
 * World operations.
 *
 * @{
 */

/** The world.
 * The world is the container of all ECS data and systems. If the world is
 * deleted, all data in the world will be deleted as well.
 */
struct world {
    /** Create world.
     */
    explicit world()
        : world_( ecs_init() ) {
            init_builtin_components();
        }

    /** Create world with command line arguments.
     * Currently command line arguments are not interpreted, but they may be
     * used in the future to configure Flecs parameters.
     */
    explicit world(int argc, char *argv[])
        : world_( ecs_init_w_args(argc, argv) ) {
            init_builtin_components();
        }

    /** Create world from C world.
     */
    explicit world(world_t *w)
        : world_( w ) {
            if (w) {
                flecs_poly_claim(w);
            }
        }

    /** Not allowed to copy a world. May only take a reference.
     */
    world(const world& obj) {
        this->world_ = obj.world_;
        flecs_poly_claim(this->world_);
    }

    world& operator=(const world& obj) noexcept {
        release();
        this->world_ = obj.world_;
        flecs_poly_claim(this->world_);
        return *this;
    }

    world(world&& obj) noexcept {
        world_ = obj.world_;
        obj.world_ = nullptr;
    }

    world& operator=(world&& obj) noexcept {
        release();
        world_ = obj.world_;
        obj.world_ = nullptr;
        return *this;
    }

    /* Releases the underlying world object. If this is the last handle, the world
       will be finalized. */
    void release() {
        if (world_) {
            if (!flecs_poly_release(world_)) {
                if (ecs_stage_get_id(world_) == -1) {
                    ecs_stage_free(world_);
                } else {
                    // before we call ecs_fini(), we increment the reference count back to 1
                    // otherwise, copies of this object created during ecs_fini (e.g. a component on_remove hook)
                    // would call again this destructor and ecs_fini().
                    flecs_poly_claim(world_);
                    ecs_fini(world_);
                }
            }
            world_ = nullptr;
        }
    }

    ~world() {
        release();
    }

    /* Implicit conversion to world_t* */
    operator world_t*() const { return world_; }

    /** Make current world object owner of the world. This may only be called on
     * one flecs::world object, an may only be called  once. Failing to do so
     * will result in undefined behavior.
     *
     * This operation allows a custom (C) world to be wrapped by a C++ object,
     * and transfer ownership so that the world is automatically cleaned up.
     */
    void make_owner() {
        flecs_poly_release(world_);
    }

    /** Deletes and recreates the world. */
    void reset() {
        /* Make sure there's only one reference to the world */
        ecs_assert(flecs_poly_refcount(world_) == 1, ECS_INVALID_OPERATION,
            "reset would invalidate other handles");
        ecs_fini(world_);
        world_ = ecs_init();
    }

    /** Obtain pointer to C world object.
     */
    world_t* c_ptr() const {
        return world_;
    }

    /** Signal application should quit.
     * After calling this operation, the next call to progress() returns false.
     */
    void quit() const {
        ecs_quit(world_);
    }

    /** Register action to be executed when world is destroyed.
     */
    void atfini(ecs_fini_action_t action, void *ctx = nullptr) const {
        ecs_atfini(world_, action, ctx);
    }

    /** Test if quit() has been called.
     */
    bool should_quit() const {
        return ecs_should_quit(world_);
    }

    /** Begin frame.
     * When an application does not use progress() to control the main loop, it
     * can still use Flecs features such as FPS limiting and time measurements.
     * This operation needs to be invoked whenever a new frame is about to get
     * processed.
     *
     * Calls to frame_begin() must always be followed by frame_end().
     *
     * The function accepts a delta_time parameter, which will get passed to
     * systems. This value is also used to compute the amount of time the
     * function needs to sleep to ensure it does not exceed the target_fps, when
     * it is set. When 0 is provided for delta_time, the time will be measured.
     *
     * This function should only be ran from the main thread.
     *
     * @param delta_time Time elapsed since the last frame.
     * @return The provided delta_time, or measured time if 0 was provided.
     *
     * @see ecs_frame_begin()
     * @see flecs::world::frame_end()
     */
    ecs_ftime_t frame_begin(float delta_time = 0) const {
        return ecs_frame_begin(world_, delta_time);
    }

    /** End frame.
     * This operation must be called at the end of the frame, and always after
     * frame_begin().
     *
     * This function should only be ran from the main thread.
     *
     * @see ecs_frame_end()
     * @see flecs::world::frame_begin()
     */
    void frame_end() const {
        ecs_frame_end(world_);
    }

    /** Begin readonly mode.
     *
     * @param multi_threaded Whether to enable readonly/multi threaded mode.
     *
     * @return Whether world is currently readonly.
     *
     * @see ecs_readonly_begin()
     * @see flecs::world::is_readonly()
     * @see flecs::world::readonly_end()
     */
    bool readonly_begin(bool multi_threaded = false) const {
        return ecs_readonly_begin(world_, multi_threaded);
    }

    /** End readonly mode.
     *
     * @see ecs_readonly_end()
     * @see flecs::world::is_readonly()
     * @see flecs::world::readonly_begin()
     */
    void readonly_end() const {
        ecs_readonly_end(world_);
    }

    /** Defer operations until end of frame.
     * When this operation is invoked while iterating, operations inbetween the
     * defer_begin() and defer_end() operations are executed at the end of the frame.
     *
     * This operation is thread safe.
     *
     * @return true if world changed from non-deferred mode to deferred mode.
     *
     * @see ecs_defer_begin()
     * @see flecs::world::defer()
     * @see flecs::world::defer_end()
     * @see flecs::world::is_deferred()
     * @see flecs::world::defer_resume()
     * @see flecs::world::defer_suspend()
     */
    bool defer_begin() const {
        return ecs_defer_begin(world_);
    }

    /** End block of operations to defer.
     * See defer_begin().
     *
     * This operation is thread safe.
     *
     * @return true if world changed from deferred mode to non-deferred mode.
     *
     * @see ecs_defer_end()
     * @see flecs::world::defer()
     * @see flecs::world::defer_begin()
     * @see flecs::world::is_deferred()
     * @see flecs::world::defer_resume()
     * @see flecs::world::defer_suspend()
     */
    bool defer_end() const {
        return ecs_defer_end(world_);
    }

    /** Test whether deferring is enabled.
     *
     * @return True if deferred, false if not.
     *
     * @see ecs_is_deferred()
     * @see flecs::world::defer()
     * @see flecs::world::defer_begin()
     * @see flecs::world::defer_end()
     * @see flecs::world::defer_resume()
     * @see flecs::world::defer_suspend()
     */
    bool is_deferred() const {
        return ecs_is_deferred(world_);
    }

    /** Configure world to have N stages.
     * This initializes N stages, which allows applications to defer operations to
     * multiple isolated defer queues. This is typically used for applications with
     * multiple threads, where each thread gets its own queue, and commands are
     * merged when threads are synchronized.
     *
     * Note that set_threads() already creates the appropriate number of stages.
     * The set_stage_count() operation is useful for applications that want to manage
     * their own stages and/or threads.
     *
     * @param stages The number of stages.
     *
     * @see ecs_set_stage_count()
     * @see flecs::world::get_stage_count()
     */
    void set_stage_count(int32_t stages) const {
        ecs_set_stage_count(world_, stages);
    }

    /** Get number of configured stages.
     * Return number of stages set by set_stage_count().
     *
     * @return The number of stages used for threading.
     *
     * @see ecs_get_stage_count()
     * @see flecs::world::set_stage_count()
     */
    int32_t get_stage_count() const {
        return ecs_get_stage_count(world_);
    }

    /** Get current stage id.
     * The stage id can be used by an application to learn about which stage it
     * is using, which typically corresponds with the worker thread id.
     *
     * @return The stage id.
     */
    int32_t get_stage_id() const {
        return ecs_stage_get_id(world_);
    }

    /** Test if is a stage.
     * If this function returns false, it is guaranteed that this is a valid
     * world object.
     *
     * @return True if the world is a stage, false if not.
     */
    bool is_stage() const {
        ecs_assert(
            flecs_poly_is(world_, ecs_world_t) ||
            flecs_poly_is(world_, ecs_stage_t),
                ECS_INVALID_PARAMETER,
                "flecs::world instance contains invalid reference to world or stage");
        return flecs_poly_is(world_, ecs_stage_t);
    }

    /** Merge world or stage.
     * When automatic merging is disabled, an application can call this
     * operation on either an individual stage, or on the world which will merge
     * all stages. This operation may only be called when staging is not enabled
     * (either after progress() or after readonly_end()).
     *
     * This operation may be called on an already merged stage or world.
     *
     * @see ecs_merge()
     */
    void merge() const {
        ecs_merge(world_);
    }

    /** Get stage-specific world pointer.
     * Flecs threads can safely invoke the API as long as they have a private
     * context to write to, also referred to as the stage. This function returns a
     * pointer to a stage, disguised as a world pointer.
     *
     * Note that this function does not(!) create a new world. It simply wraps the
     * existing world in a thread-specific context, which the API knows how to
     * unwrap. The reason the stage is returned as an ecs_world_t is so that it
     * can be passed transparently to the existing API functions, vs. having to
     * create a dediated API for threading.
     *
     * @param stage_id The index of the stage to retrieve.
     * @return A thread-specific pointer to the world.
     */
    flecs::world get_stage(int32_t stage_id) const {
        return flecs::world(ecs_get_stage(world_, stage_id));
    }

    /** Create asynchronous stage.
     * An asynchronous stage can be used to asynchronously queue operations for
     * later merging with the world. An asynchronous stage is similar to a regular
     * stage, except that it does not allow reading from the world.
     *
     * Asynchronous stages are never merged automatically, and must therefore be
     * manually merged with the ecs_merge function. It is not necessary to call
     * defer_begin or defer_end before and after enqueuing commands, as an
     * asynchronous stage unconditionally defers operations.
     *
     * The application must ensure that no commands are added to the stage while the
     * stage is being merged.
     *
     * @return The stage.
     */
    flecs::world async_stage() const {
        ecs_world_t *as = ecs_stage_new(world_);
        flecs_poly_release(as); // world object will claim
        return flecs::world(as);
    }

    /** Get actual world.
     * If the current object points to a stage, this operation will return the
     * actual world.
     *
     * @return The actual world.
     */
    flecs::world get_world() const {
        /* Safe cast, mutability is checked */
        return flecs::world(
            world_ ? const_cast<flecs::world_t*>(ecs_get_world(world_)) : nullptr);
    }

    /** Test whether the current world object is readonly.
     * This function allows the code to test whether the currently used world
     * object is readonly or whether it allows for writing.
     *
     * @return True if the world or stage is readonly.
     *
     * @see ecs_stage_is_readonly()
     * @see flecs::world::readonly_begin()
     * @see flecs::world::readonly_end()
     */
    bool is_readonly() const {
        return ecs_stage_is_readonly(world_);
    }

    /** Set world context.
     * Set a context value that can be accessed by anyone that has a reference
     * to the world.
     *
     * @param ctx A pointer to a user defined structure.
     * @param ctx_free A function that is invoked with ctx when the world is freed.
     *
     *
     * @see ecs_set_ctx()
     * @see flecs::world::get_ctx()
     */
    void set_ctx(void* ctx, ecs_ctx_free_t ctx_free = nullptr) const {
        ecs_set_ctx(world_, ctx, ctx_free);
    }

    /** Get world context.
     * This operation retrieves a previously set world context.
     *
     * @return The context set with set_binding_ctx(). If no context was set, the
     *         function returns NULL.
     *
     * @see ecs_get_ctx()
     * @see flecs::world::set_ctx()
     */
    void* get_ctx() const {
        return ecs_get_ctx(world_);
    }

    /** Set world binding context.
     *
     * Same as set_ctx() but for binding context. A binding context is intended
     * specifically for language bindings to store binding specific data.
     *
     * @param ctx A pointer to a user defined structure.
     * @param ctx_free A function that is invoked with ctx when the world is freed.
     *
     * @see ecs_set_binding_ctx()
     * @see flecs::world::get_binding_ctx()
     */
    void set_binding_ctx(void* ctx, ecs_ctx_free_t ctx_free = nullptr) const {
        ecs_set_binding_ctx(world_, ctx, ctx_free);
    }

    /** Get world binding context.
     * This operation retrieves a previously set world binding context.
     *
     * @return The context set with set_binding_ctx(). If no context was set, the
     *         function returns NULL.
     *
     * @see ecs_get_binding_ctx()
     * @see flecs::world::set_binding_ctx()
     */
    void* get_binding_ctx() const {
        return ecs_get_binding_ctx(world_);
    }

    /** Preallocate memory for number of entities.
     * This function preallocates memory for the entity index.
     *
     * @param entity_count Number of entities to preallocate memory for.
     *
     * @see ecs_dim()
     */
    void dim(int32_t entity_count) const {
        ecs_dim(world_, entity_count);
    }

    /** Set entity range.
     * This function limits the range of issued entity ids between min and max.
     *
     * @param min Minimum entity id issued.
     * @param max Maximum entity id issued.
     *
     * @see ecs_set_entity_range()
     */
    void set_entity_range(entity_t min, entity_t max) const {
        ecs_set_entity_range(world_, min, max);
    }

    /** Enforce that operations cannot modify entities outside of range.
     * This function ensures that only entities within the specified range can
     * be modified. Use this function if specific parts of the code only are
     * allowed to modify a certain set of entities, as could be the case for
     * networked applications.
     *
     * @param enabled True if range check should be enabled, false if not.
     *
     * @see ecs_enable_range_check()
     */
    void enable_range_check(bool enabled = true) const {
        ecs_enable_range_check(world_, enabled);
    }

    /** Set current scope.
     *
     * @param scope The scope to set.
     * @return The current scope;
     *
     * @see ecs_set_scope()
     * @see flecs::world::get_scope()
     */
    flecs::entity set_scope(const flecs::entity_t scope) const;

    /** Get current scope.
     *
     * @return The current scope.
     *
     * @see ecs_get_scope()
     * @see flecs::world::set_scope()
     */
    flecs::entity get_scope() const;

    /** Same as set_scope but with type.
     *
     * @see ecs_set_scope()
     * @see flecs::world::get_scope()
     */
    template <typename T>
    flecs::entity set_scope() const;

    /** Set search path.
     *
     * @see ecs_set_lookup_path()
     * @see flecs::world::lookup()
     */
    flecs::entity_t* set_lookup_path(const flecs::entity_t *search_path) const {
        return ecs_set_lookup_path(world_, search_path);
    }

    /** Lookup entity by name.
     *
     * @param name Entity name.
     * @param recursive When false, only the current scope is searched.
     * @result The entity if found, or 0 if not found.
     */
    flecs::entity lookup(const char *name, const char *sep = "::", const char *root_sep = "::", bool recursive = true) const;

    /** Set singleton component.
     */
    template <typename T, if_t< !is_callable<T>::value > = 0>
    void set(const T& value) const {
        flecs::set<T>(world_, _::type<T>::id(world_), value);
    }

    /** Set singleton component.
     */
    template <typename T, if_t< !is_callable<T>::value > = 0>
    void set(T&& value) const {
        flecs::set<T>(world_, _::type<T>::id(world_),
            FLECS_FWD(value));
    }

    /** Set singleton pair.
     */
    template <typename First, typename Second, typename P = flecs::pair<First, Second>,
        typename A = actual_type_t<P>, if_not_t< flecs::is_pair<First>::value> = 0>
    void set(const A& value) const {
        flecs::set<P>(world_, _::type<First>::id(world_), value);
    }

    /** Set singleton pair.
     */
    template <typename First, typename Second, typename P = flecs::pair<First, Second>,
        typename A = actual_type_t<P>, if_not_t< flecs::is_pair<First>::value> = 0>
    void set(A&& value) const {
        flecs::set<P>(world_, _::type<First>::id(world_), FLECS_FWD(value));
    }

    /** Set singleton pair.
     */
    template <typename First, typename Second>
    void set(Second second, const First& value) const;

    /** Set singleton pair.
     */
    template <typename First, typename Second>
    void set(Second second, First&& value) const;

    /** Set singleton component inside a callback.
     */
    template <typename Func, if_t< is_callable<Func>::value > = 0 >
    void set(const Func& func) const;

    template <typename T, typename ... Args>
    void emplace(Args&&... args) const {
        flecs::id_t component_id = _::type<T>::id(world_);
        flecs::emplace<T>(world_, component_id, component_id, FLECS_FWD(args)...);
    }

    /** Ensure singleton component.
     */
    #ifndef ensure
    template <typename T>
    T& ensure() const;
    #endif

    /** Mark singleton component as modified.
     */
    template <typename T>
    void modified() const;

    /** Get ref singleton component.
     */
    template <typename T>
    ref<T> get_ref() const;


    /* try_get */

    /** Get singleton component.
     */
    const void* try_get(flecs::id_t id) const;

    /** Get singleton pair.
     */
    const void* try_get(flecs::entity_t r, flecs::entity_t t) const;

    /** Get singleton component.
     */
    template <typename T>
    const T* try_get() const;

    /** Get singleton pair.
     */
    template <typename First, typename Second, typename P = flecs::pair<First, Second>,
        typename A = actual_type_t<P>>
    const A* try_get() const;

    /** Get singleton pair.
     */
    template <typename First, typename Second>
    const First* try_get(Second second) const;


    /* get */

    /** Get singleton component.
     */
    const void* get(flecs::id_t id) const;

    /** Get singleton component.
     */
    const void* get(flecs::entity_t r, flecs::entity_t t) const;

    template <typename T>
    const T& get() const;

    /** Get singleton pair.
     */
    template <typename First, typename Second, typename P = flecs::pair<First, Second>,
        typename A = actual_type_t<P>>
    const A& get() const;

    /** Get singleton pair.
     */
    template <typename First, typename Second>
    const First& get(Second second) const;

    /** Get singleton component inside a callback.
     */
    template <typename Func, if_t< is_callable<Func>::value > = 0 >
    void get(const Func& func) const;


    /* try_get_mut */

    /** Get mutable singleton component.
     */
    void* try_get_mut(flecs::id_t id) const;

    /** Get mutable singleton pair.
     */
    void* try_get_mut(flecs::entity_t r, flecs::entity_t t) const;

    template <typename T>
    T* try_get_mut() const;

    /** Get mutable singleton pair.
     */
    template <typename First, typename Second, typename P = flecs::pair<First, Second>,
        typename A = actual_type_t<P>>
    A* try_get_mut() const;

    /** Get mutable singleton pair.
     */
    template <typename First, typename Second>
    First* try_get_mut(Second second) const;


    /* get_mut */

    /** Get mutable singleton component.
     */
    void* get_mut(flecs::id_t id) const;

    /** Get mutable singleton pair.
     */
    void* get_mut(flecs::entity_t r, flecs::entity_t t) const;

    template <typename T>
    T& get_mut() const;

    /** Get mutable singleton pair.
     */
    template <typename First, typename Second, typename P = flecs::pair<First, Second>,
        typename A = actual_type_t<P>>
    A& get_mut() const;

    /** Get mutable singleton pair.
     */
    template <typename First, typename Second>
    First& get_mut(Second second) const;


    /** Test if world has singleton component.
     *
     * @tparam T The component to check.
     * @return Whether the world has the singleton component.
     */
    template <typename T>
    bool has() const;

    /** Test if world has the provided pair.
     *
     * @tparam First The first element of the pair
     * @tparam Second The second element of the pair
     * @return Whether the world has the singleton pair.
     */
    template <typename First, typename Second>
    bool has() const;

    /** Test if world has the provided pair.
     *
     * @tparam First The first element of the pair
     * @param second The second element of the pair.
     * @return Whether the world has the singleton pair.
     */
    template <typename First>
    bool has(flecs::id_t second) const;

    /** Test if world has the provided pair.
     *
     * @param first The first element of the pair
     * @param second The second element of the pair
     * @return Whether the world has the singleton pair.
     */
    bool has(flecs::id_t first, flecs::id_t second) const;

    /** Check for enum singleton constant
     *
     * @tparam E The enum type.
     * @param value The enum constant to check.
     * @return Whether the world has the specified enum constant.
     */
    template <typename E, if_t< is_enum<E>::value > = 0>
    bool has(E value) const;

    /** Add singleton component.
     */
    template <typename T>
    void add() const;

    /** Adds a pair to the singleton component.
     *
     * @tparam First The first element of the pair
     * @tparam Second The second element of the pair
     */
    template <typename First, typename Second>
    void add() const;

    /** Adds a pair to the singleton component.
     *
     * @tparam First The first element of the pair
     * @param second The second element of the pair.
     */
    template <typename First>
    void add(flecs::entity_t second) const;

    /** Adds a pair to the singleton entity.
     *
     * @param first The first element of the pair
     * @param second The second element of the pair
     */
    void add(flecs::entity_t first, flecs::entity_t second) const;

    /** Add enum singleton constant
     *
     * @tparam E The enum type.
     * @param value The enum constant.
     */
    template <typename E, if_t< is_enum<E>::value > = 0>
    void add(E value) const;

    /** Remove singleton component.
     */
    template <typename T>
    void remove() const;

    /** Removes the pair singleton component.
     *
     * @tparam First The first element of the pair
     * @tparam Second The second element of the pair
     */
    template <typename First, typename Second>
    void remove() const;

    /** Removes the pair singleton component.
     *
     * @tparam First The first element of the pair
     * @param second The second element of the pair.
     */
    template <typename First>
    void remove(flecs::entity_t second) const;

    /** Removes the pair singleton component.
     *
     * @param first The first element of the pair
     * @param second The second element of the pair
     */
    void remove(flecs::entity_t first, flecs::entity_t second) const;

    /** Iterate entities in root of world
     * Accepts a callback with the following signature:
     *
     * @code
     * void(*)(flecs::entity e);
     * @endcode
     */
    template <typename Func>
    void children(Func&& f) const;

    /** Get singleton entity for type.
     */
    template <typename T>
    flecs::entity singleton() const;

    /** Get target for a given pair from a singleton entity.
     * This operation returns the target for a given pair. The optional
     * index can be used to iterate through targets, in case the entity has
     * multiple instances for the same relationship.
     *
     * @tparam First The first element of the pair.
     * @param index The index (0 for the first instance of the relationship).
     */
    template<typename First>
    flecs::entity target(int32_t index = 0) const;

    /** Get target for a given pair from a singleton entity.
     * This operation returns the target for a given pair. The optional
     * index can be used to iterate through targets, in case the entity has
     * multiple instances for the same relationship.
     *
     * @param first The first element of the pair for which to retrieve the target.
     * @param index The index (0 for the first instance of the relationship).
     */
    template<typename T>
    flecs::entity target(flecs::entity_t first, int32_t index = 0) const;

    /** Get target for a given pair from a singleton entity.
     * This operation returns the target for a given pair. The optional
     * index can be used to iterate through targets, in case the entity has
     * multiple instances for the same relationship.
     *
     * @param first The first element of the pair for which to retrieve the target.
     * @param index The index (0 for the first instance of the relationship).
     */
    flecs::entity target(flecs::entity_t first, int32_t index = 0) const;

    /** Create alias for component.
     *
     * @tparam T to create an alias for.
     * @param alias Alias for the component.
     * @return Entity representing the component.
     */
    template <typename T>
    flecs::entity use(const char *alias = nullptr) const;

    /** Create alias for entity.
     *
     * @param name Name of the entity.
     * @param alias Alias for the entity.
     */
    flecs::entity use(const char *name, const char *alias = nullptr) const;

    /** Create alias for entity.
     *
     * @param entity Entity for which to create the alias.
     * @param alias Alias for the entity.
     */
    void use(flecs::entity entity, const char *alias = nullptr) const;

    /** Count entities matching a component.
     *
     * @param component_id The component id.
     */
    int count(flecs::id_t component_id) const {
        return ecs_count_id(world_, component_id);
    }

    /** Count entities matching a pair.
     *
     * @param first The first element of the pair.
     * @param second The second element of the pair.
     */
    int count(flecs::entity_t first, flecs::entity_t second) const {
        return ecs_count_id(world_, ecs_pair(first, second));
    }

    /** Count entities matching a component.
     *
     * @tparam T The component type.
     */
    template <typename T>
    int count() const {
        return count(_::type<T>::id(world_));
    }

    /** Count entities matching a pair.
     *
     * @tparam First The first element of the pair.
     * @param second The second element of the pair.
     */
    template <typename First>
    int count(flecs::entity_t second) const {
        return count(_::type<First>::id(world_), second);
    }

    /** Count entities matching a pair.
     *
     * @tparam First The first element of the pair.
     * @tparam Second The second element of the pair.
     */
    template <typename First, typename Second>
    int count() const {
        return count(
            _::type<First>::id(world_),
            _::type<Second>::id(world_));
    }

    /** All entities created in function are created with id.
     */
    template <typename Func>
    void with(id_t with_id, const Func& func) const {
        ecs_id_t prev = ecs_set_with(world_, with_id);
        func();
        ecs_set_with(world_, prev);
    }

    /** All entities created in function are created with type.
     */
    template <typename T, typename Func>
    void with(const Func& func) const {
        with(this->id<T>(), func);
    }

    /** All entities created in function are created with pair.
     */
    template <typename First, typename Second, typename Func>
    void with(const Func& func) const {
        with(ecs_pair(this->id<First>(), this->id<Second>()), func);
    }

    /** All entities created in function are created with pair.
     */
    template <typename First, typename Func>
    void with(id_t second, const Func& func) const {
        with(ecs_pair(this->id<First>(), second), func);
    }

    /** All entities created in function are created with pair.
     */
    template <typename Func>
    void with(id_t first, id_t second, const Func& func) const {
        with(ecs_pair(first, second), func);
    }

    /** All entities created in function are created in scope. All operations
     * called in function (such as lookup) are relative to scope.
     */
    template <typename Func>
    void scope(id_t parent, const Func& func) const {
        ecs_entity_t prev = ecs_set_scope(world_, parent);
        func();
        ecs_set_scope(world_, prev);
    }

    /** Same as scope(parent, func), but with T as parent.
     */
    template <typename T, typename Func>
    void scope(const Func& func) const {
        flecs::id_t parent = _::type<T>::id(world_);
        scope(parent, func);
    }

    /** Use provided scope for operations ran on returned world.
     * Operations need to be ran in a single statement.
     */
    flecs::scoped_world scope(id_t parent) const;

    template <typename T>
    flecs::scoped_world scope() const;

    flecs::scoped_world scope(const char* name) const;

    /** Delete all entities with specified id. */
    void delete_with(id_t the_id) const {
        ecs_delete_with(world_, the_id);
    }

    /** Delete all entities with specified pair. */
    void delete_with(entity_t first, entity_t second) const {
        delete_with(ecs_pair(first, second));
    }

    /** Delete all entities with specified component. */
    template <typename T>
    void delete_with() const {
        delete_with(_::type<T>::id(world_));
    }

    /** Delete all entities with specified pair. */
    template <typename First, typename Second>
    void delete_with() const {
        delete_with(_::type<First>::id(world_), _::type<Second>::id(world_));
    }

    /** Delete all entities with specified pair. */
    template <typename First>
    void delete_with(entity_t second) const {
        delete_with(_::type<First>::id(world_), second);
    }

    /** Remove all instances of specified id. */
    void remove_all(id_t the_id) const {
        ecs_remove_all(world_, the_id);
    }

    /** Remove all instances of specified pair. */
    void remove_all(entity_t first, entity_t second) const {
        remove_all(ecs_pair(first, second));
    }

    /** Remove all instances of specified component. */
    template <typename T>
    void remove_all() const {
        remove_all(_::type<T>::id(world_));
    }

    /** Remove all instances of specified pair. */
    template <typename First, typename Second>
    void remove_all() const {
        remove_all(_::type<First>::id(world_), _::type<Second>::id(world_));
    }

    /** Remove all instances of specified pair. */
    template <typename First>
    void remove_all(entity_t second) const {
        remove_all(_::type<First>::id(world_), second);
    }

    /** Defer all operations called in function.
     *
     * @see flecs::world::defer_begin()
     * @see flecs::world::defer_end()
     * @see flecs::world::defer_is_deferred()
     * @see flecs::world::defer_resume()
     * @see flecs::world::defer_suspend()
     */
    template <typename Func>
    void defer(const Func& func) const {
        ecs_defer_begin(world_);
        func();
        ecs_defer_end(world_);
    }

    /** Suspend deferring operations.
     *
     * @see ecs_defer_suspend()
     * @see flecs::world::defer()
     * @see flecs::world::defer_begin()
     * @see flecs::world::defer_end()
     * @see flecs::world::defer_is_deferred()
     * @see flecs::world::defer_resume()
     */
    void defer_suspend() const {
        ecs_defer_suspend(world_);
    }

    /** Resume deferring operations.
     *
     * @see ecs_defer_resume()
     * @see flecs::world::defer()
     * @see flecs::world::defer_begin()
     * @see flecs::world::defer_end()
     * @see flecs::world::defer_is_deferred()
     * @see flecs::world::defer_suspend()
     */
    void defer_resume() const {
        ecs_defer_resume(world_);
    }

    /** Check if entity id exists in the world.
     *
     * @see ecs_exists()
     * @see flecs::world::is_alive()
     * @see flecs::world::is_valid()
     */
    bool exists(flecs::entity_t e) const {
        return ecs_exists(world_, e);
    }

    /** Check if entity id exists in the world.
     *
     * @see ecs_is_alive()
     * @see flecs::world::exists()
     * @see flecs::world::is_valid()
     */
    bool is_alive(flecs::entity_t e) const {
        return ecs_is_alive(world_, e);
    }

    /** Check if entity id is valid.
     * Invalid entities cannot be used with API functions.
     *
     * @see ecs_is_valid()
     * @see flecs::world::exists()
     * @see flecs::world::is_alive()
     */
    bool is_valid(flecs::entity_t e) const {
        return ecs_is_valid(world_, e);
    }

    /** Get alive entity for id.
     * Returns the entity with the current generation.
     *
     * @see ecs_get_alive()
     */
    flecs::entity get_alive(flecs::entity_t e) const;

    /**
     * @see ecs_make_alive()
     */
    flecs::entity make_alive(flecs::entity_t e) const;

    /** Set version of entity to provided.
     *
     * @see ecs_set_version()
     */
    void set_version(flecs::entity_t e) const {
        ecs_set_version(world_, e);
    }

    /* Run callback after completing frame */
    void run_post_frame(ecs_fini_action_t action, void *ctx) const {
        ecs_run_post_frame(world_, action, ctx);
    }

    /** Get the world info.
     *
     * @see ecs_get_world_info()
     */
    const flecs::world_info_t* get_info() const{
        return ecs_get_world_info(world_);
    }

    /** Get delta_time */
    ecs_ftime_t delta_time() const {
        return get_info()->delta_time;
    }

    /** Free unused memory.
     *
     * @see ecs_shrink()
    */
    void shrink() const {
        ecs_shrink(world_);
    }

    /** Begin exclusive access
     *
     * @param thread_name Optional thread name for improved debug messages.
     * @see ecs_exclusive_access_begin()
     */
    void exclusive_access_begin(const char *thread_name = nullptr) {
        ecs_exclusive_access_begin(world_, thread_name);
    }

    /** End exclusive access
     *
     * @param lock_world Lock world for all threads, allow readonly operations.
     * @see ecs_exclusive_access_end()
     */
    void exclusive_access_end(bool lock_world = false) {
        ecs_exclusive_access_end(world_, lock_world);
    }

    /** Return component id if it has been registered.
     * This operation is similar to world::id() but will never automatically
     * register the component.
     *
     * @tparam T The type for which to obtain the id.
     */
    template <typename T>
    flecs::id_t id_if_registered() {
        if (_::type<T>::registered(world_)) {
            return _::type<T>::id(world_);
        }
        else {
            return 0;
        }
    }

    /** Return type info */
    const flecs::type_info_t* type_info(flecs::id_t component) {
        return ecs_get_type_info(world_, component);
    }

    /** Return type info */
    const flecs::type_info_t* type_info(flecs::entity_t r, flecs::entity_t t) {
        return ecs_get_type_info(world_, ecs_pair(r, t));
    }

    /** Return type info */
    template <typename T>
    const flecs::type_info_t* type_info() {
        return ecs_get_type_info(world_, _::type<T>::id(world_));
    }

    /** Return type info */
    template <typename R>
    const flecs::type_info_t* type_info(flecs::entity_t t) {
        return type_info(_::type<R>::id(world_), t);
    }

    /** Return type info */
    template <typename R, typename T>
    const flecs::type_info_t* type_info() {
        return type_info<R>(_::type<T>::id(world_));
    }

/**
 * @file addons/cpp/mixins/id/mixin.inl
 * @brief Id world mixin.
 */

/** Get id from a type.
 *
 * @memberof flecs::world
 */
template <typename T>
flecs::id id() const;

/** Id factory.
 *
 * @memberof flecs::world
 */
template <typename ... Args>
flecs::id id(Args&&... args) const;

/** Get pair id from relationship, object.
 *
 * @memberof flecs::world
 */
template <typename First, typename Second>
flecs::id pair() const;

/** Get pair id from relationship, object.
 *
 * @memberof flecs::world
 */
template <typename First>
flecs::id pair(entity_t o) const;

/** Get pair id from relationship, object.
 *
 * @memberof flecs::world
 */
flecs::id pair(entity_t r, entity_t o) const;

/**
 * @file addons/cpp/mixins/component/mixin.inl
 * @brief Component mixin.
 */

/** Find or register component.
 *
 * @ingroup cpp_components
 * @memberof flecs::world
 */
template <typename T, typename... Args>
flecs::component<T> component(Args &&... args) const;

/** Find or register untyped component.
 * Method available on flecs::world class.
 *
 * @ingroup cpp_components
 * @memberof flecs::world
 */
template <typename... Args>
flecs::untyped_component component(Args &&... args) const;

/**
 * @file addons/cpp/mixins/entity/mixin.inl
 * @brief Entity world mixin.
 */

/** Create an entity.
 *
 * @memberof flecs::world
 * @ingroup cpp_entities
 */
template <typename... Args>
flecs::entity entity(Args &&... args) const;

/** Convert enum constant to entity.
 *
 * @memberof flecs::world
 * @ingroup cpp_entities
 */
template <typename E, if_t< is_enum<E>::value > = 0>
flecs::id id(E value) const;

/** Convert enum constant to entity.
 *
 * @memberof flecs::world
 * @ingroup cpp_entities
 */
template <typename E, if_t< is_enum<E>::value > = 0>
flecs::entity entity(E value) const;

/** Create a prefab.
 *
 * @memberof flecs::world
 * @ingroup cpp_entities
 */
template <typename... Args>
flecs::entity prefab(Args &&... args) const;

/** Create an entity that's associated with a type.
 *
 * @memberof flecs::world
 * @ingroup cpp_entities
 */
template <typename T>
flecs::entity entity(const char *name = nullptr) const;

/** Create a prefab that's associated with a type.
 *
 * @memberof flecs::world
 * @ingroup cpp_entities
 */
template <typename T>
flecs::entity prefab(const char *name = nullptr) const;

/**
 * @file addons/cpp/mixins/event/mixin.inl
 * @brief Event world mixin.
 */

/**
 * @defgroup cpp_addons_event Events
 * @ingroup cpp_addons
 * API for emitting events.
 *
 * @{
 */

/** Create a new event.
 *
 * @memberof flecs::world
 *
 * @param evt The event id.
 * @return Event builder.
 */
flecs::event_builder event(flecs::entity_t evt) const;

/** Create a new event.
 *
 * @memberof flecs::world
 *
 * @tparam E The event type.
 * @return Event builder.
 */
template <typename E>
flecs::event_builder_typed<E> event() const;

/** @} */

/**
 * @file addons/cpp/mixins/term/mixin.inl
 * @brief Term world mixin.
 */

/**
 * @memberof flecs::world
 * @ingroup cpp_core_queries
 *
 * @{
 */

/** Create a term.
 *
 */
template<typename... Args>
flecs::term term(Args &&... args) const;

/** Create a term for a (component) type.
 */
template<typename T>
flecs::term term() const;

/** Create a term for a pair.
 */
template<typename First, typename Second>
flecs::term term() const;

/** @} */

/**
 * @file addons/cpp/mixins/observer/mixin.inl
 * @brief Observer world mixin.
 */

/** Observer builder.
 *
 * @memberof flecs::world
 * @ingroup cpp_observers
 *
 * @{
 */

/** Upcast entity to an observer.
 * The provided entity must be an observer.
 *
 * @param e The entity.
 * @return An observer object.
 */
flecs::observer observer(flecs::entity e) const;

/** Create a new observer.
 *
 * @tparam Components The components to match on.
 * @tparam Args Arguments passed to the constructor of flecs::observer_builder.
 * @return Observer builder.
 */
template <typename... Components, typename... Args>
flecs::observer_builder<Components...> observer(Args &&... args) const;

/** @} */

/**
 * @file addons/cpp/mixins/query/mixin.inl
 * @brief Query world mixin.
 */

/**
 * @memberof flecs::world
 * @ingroup cpp_core_queries
 *
 * @{
 */

/** Create a query.
 *
 * @see ecs_query_init
 */
template <typename... Comps, typename... Args>
flecs::query<Comps...> query(Args &&... args) const;

/** Create a query from entity.
 *
 * @see ecs_query_init
 */
flecs::query<> query(flecs::entity query_entity) const;

/** Create a query builder.
 *
 * @see ecs_query_init
 */
template <typename... Comps, typename... Args>
flecs::query_builder<Comps...> query_builder(Args &&... args) const;

/** Iterate over all entities with components in argument list of function.
 * The function parameter must match the following signature:
 *
 * @code
 * void(*)(T&, U&, ...)
 * @endcode
 *
 * or:
 *
 * @code
 * void(*)(flecs::entity, T&, U&, ...)
 * @endcode
 *
 */
template <typename Func>
void each(Func&& func) const;

/** Iterate over all entities with provided component.
 * The function parameter must match the following signature:
 *
 * @code
 * void(*)(T&)
 * @endcode
 *
 * or:
 *
 * @code
 * void(*)(flecs::entity, T&)
 * @endcode
 *
 */
template <typename T, typename Func>
void each(Func&& func) const;

/** Iterate over all entities with provided (component) id. */
template <typename Func>
void each(flecs::id_t term_id, Func&& func) const;

/** @} */

/**
 * @file addons/cpp/mixins/enum/mixin.inl
 * @brief Enum world mixin.
 */

/** Convert enum constant to entity.
 *
 * @memberof flecs::world
 * @ingroup cpp_entities
 */
template <typename E, if_t< is_enum<E>::value > = 0>
flecs::entity to_entity(E constant) const;


#   ifdef FLECS_MODULE
/**
 * @file addons/cpp/mixins/module/mixin.inl
 * @brief Module world mixin.
 */

/**
 * @memberof flecs::world
 * @ingroup cpp_addons_modules
 *
 * @{
 */

/** Define a module.
 * This operation is not mandatory, but can be called inside the module ctor to
 * obtain the entity associated with the module, or override the module name.
 *
 * @tparam Module module class.
 * @return Module entity.
 */
template <typename Module>
flecs::entity module(const char *name = nullptr) const;

/** Import a module.
 *
 * @tparam Module module class.
 * @return Module entity.
 */
template <typename Module>
flecs::entity import();

/** @} */

#   endif
#   ifdef FLECS_PIPELINE
/**
 * @file addons/cpp/mixins/pipeline/mixin.inl
 * @brief Pipeline world mixin.
 */

/**
 * @memberof flecs::world
 * @ingroup cpp_pipelines
 *
 * @{
 */

/** Create a new pipeline.
 *
 * @return A pipeline builder.
 */
flecs::pipeline_builder<> pipeline() const;

/** Create a new pipeline.
 *
 * @tparam Pipeline Type associated with pipeline.
 * @return A pipeline builder.
 */
template <typename Pipeline, if_not_t< is_enum<Pipeline>::value > = 0>
flecs::pipeline_builder<> pipeline() const;

/** Set pipeline.
 * @see ecs_set_pipeline
 */
void set_pipeline(const flecs::entity pip) const;

/** Set pipeline.
 * @see ecs_set_pipeline
 */
template <typename Pipeline>
void set_pipeline() const;

/** Get pipeline.
 * @see ecs_get_pipeline
 */
flecs::entity get_pipeline() const;

/** Progress world one tick.
 * @see ecs_progress
 */
bool progress(ecs_ftime_t delta_time = 0.0) const;

/** Run pipeline.
 * @see ecs_run_pipeline
 */
void run_pipeline(const flecs::entity_t pip, ecs_ftime_t delta_time = 0.0) const;

/** Run pipeline.
 * @tparam Pipeline Type associated with pipeline.
 * @see ecs_run_pipeline
 */
template <typename Pipeline, if_not_t< is_enum<Pipeline>::value > = 0>
void run_pipeline(ecs_ftime_t delta_time = 0.0) const;

/** Set timescale.
 * @see ecs_set_time_scale
 */
void set_time_scale(ecs_ftime_t mul) const;

/** Set target FPS.
 * @see ecs_set_target_fps
 */
void set_target_fps(ecs_ftime_t target_fps) const;

/** Reset simulation clock.
 * @see ecs_reset_clock
 */
void reset_clock() const;

/** Set number of threads.
 * @see ecs_set_threads
 */
void set_threads(int32_t threads) const;

/** Set number of threads.
 * @see ecs_get_stage_count
 */
int32_t get_threads() const;

/** Set number of task threads.
 * @see ecs_set_task_threads
 */
void set_task_threads(int32_t task_threads) const;

/** Returns true if task thread use has been requested.
 * @see ecs_using_task_threads
 */
bool using_task_threads() const;

/** @} */

#   endif
#   ifdef FLECS_SYSTEM
/**
 * @file addons/cpp/mixins/system/mixin.inl
 * @brief System module world mixin.
 */

/**
 * @memberof flecs::world
 * @ingroup cpp_addons_systems
 *
 * @{
*/

/** Upcast entity to a system.
 * The provided entity must be a system.
 *
 * @param e The entity.
 * @return A system object.
 */
flecs::system system(flecs::entity e) const;

/** Create a new system.
 *
 * @tparam Components The components to match on.
 * @tparam Args Arguments passed to the constructor of flecs::system_builder.
 * @return System builder.
 */
template <typename... Components, typename... Args>
flecs::system_builder<Components...> system(Args &&... args) const;

/** @} */

#   endif
#   ifdef FLECS_TIMER
/**
 * @file addons/cpp/mixins/timer/mixin.inl
 * @brief Timer module mixin.
 */

/**
 * @memberof flecs::world
 * @ingroup cpp_addons_timer
 */

/** Find or register a singleton timer. */
template <typename T>
flecs::timer timer() const;

/** Find or register a timer. */
template <typename... Args>
flecs::timer timer(Args &&... args) const;

/** Enable randomization of initial time values for timers.
 * @see ecs_randomize_timers
 */
void randomize_timers() const;

#   endif
#   ifdef FLECS_SCRIPT
/**
 * @file addons/cpp/mixins/script/mixin.inl
 * @brief Script world mixin.
 */

/**
 * @defgroup cpp_addons_script Script
 * @ingroup cpp_addons
 * Data definition format for loading entity data.
 *
 * @{
 */

/** Run script.
 * @see ecs_script_run
 */
int script_run(const char *name, const char *str) const {
    return ecs_script_run(world_, name, str, nullptr);
}

/** Run script from file.
 * @see ecs_script_run_file
 */
int script_run_file(const char *filename) const {
    return ecs_script_run_file(world_, filename);
}

/** Build script.
 * @see ecs_script_init
 */
script_builder script(const char *name = nullptr) const {
    return script_builder(world_, name);
}

/** Convert value to string */
flecs::string to_expr(flecs::entity_t tid, const void* value) {
    char *expr = ecs_ptr_to_expr(world_, tid, value);
    return flecs::string(expr);
}

/** Convert value to string */
template <typename T>
flecs::string to_expr(const T* value) {
    flecs::entity_t tid = _::type<T>::id(world_);
    return to_expr(tid, value);
}

/** Get value of exported script variable.
 * This operation will panic if no const var with the provided name was found,
 * or if the type of the variable cannot be converted to the provided type.
 *
 * An exported variable can be created in a script like this:
 *
 * @code
 * export const x = f64: 10
 * @endcode
 *
 * See the Flecs script manual for more details.
 *
 * @tparam T The type of the value to obtain.
 * @param name The name of the exported variable.
 * @param default_value Optional default value. Returned when const var lookup failed.
 * @return The value of the variable.
 */
template <typename T>
T get_const_var(const char *name, const T& default_value = {}) const;

/** Get value of exported script variable.
 * This operation will panic if no const var with the provided name was found,
 * or if the type of the variable cannot be converted to the provided type.
 *
 * An exported variable can be created in a script like this:
 *
 * @code
 * export const x = f64: 10
 * @endcode
 *
 * See the Flecs script manual for more details.
 *
 * @tparam T The type of the value to obtain.
 * @param name The name of the exported variable.
 * @param out Optional pointer to out variable. Can be used to automatically deduce T.
 * @param default_value Optional default value. Returned when const var lookup failed.
 */
template <typename T>
void get_const_var(const char *name, T& out, const T& default_value = {}) const;


/** @} */

#   endif
#   ifdef FLECS_META
/**
 * @file addons/cpp/mixins/meta/world.inl
 * @brief Meta world mixin.
 */

/**
 * @memberof flecs::world
 * @ingroup cpp_addons_meta
 *
 * @{
 */

/** Return meta cursor to value */
flecs::cursor cursor(flecs::entity_t tid, void *ptr) {
    return flecs::cursor(world_, tid, ptr);
}

/** Return meta cursor to value */
template <typename T>
flecs::cursor cursor(void *ptr) {
    flecs::entity_t tid = _::type<T>::id(world_);
    return cursor(tid, ptr);
}

/** Create primitive type */
flecs::entity primitive(flecs::meta::primitive_kind_t kind);

/** Create array type. */
flecs::entity array(flecs::entity_t elem_id, int32_t array_count);

/** Create array type. */
template <typename T>
flecs::entity array(int32_t array_count);

/** Create vector type. */
flecs::entity vector(flecs::entity_t elem_id);

/** Create vector type. */
template <typename T>
flecs::entity vector();

/** @} */

#   endif
#   ifdef FLECS_JSON
/**
 * @file addons/cpp/mixins/json/world.inl
 * @brief JSON world mixin.
 */

/** Serialize untyped value to JSON.
 *
 * @memberof flecs::world
 * @ingroup cpp_addons_json
 */
flecs::string to_json(flecs::entity_t tid, const void* value) const {
    char *json = ecs_ptr_to_json(world_, tid, value);
    return flecs::string(json);
}

/** Serialize value to JSON.
 *
 * @memberof flecs::world
 * @ingroup cpp_addons_json
 */
template <typename T>
flecs::string to_json(const T* value) const {
    flecs::entity_t tid = _::type<T>::id(world_);
    return to_json(tid, value);
}

/** Serialize world to JSON.
 *
 * @memberof flecs::world
 * @ingroup cpp_addons_json
 */
flecs::string to_json() const {
    return flecs::string( ecs_world_to_json(world_, nullptr) );
}

/** Deserialize value from JSON.
 *
 * @memberof flecs::world
 * @ingroup cpp_addons_json
 */
const char* from_json(flecs::entity_t tid, void* value, const char *json, flecs::from_json_desc_t *desc = nullptr) {
    return ecs_ptr_from_json(world_, tid, value, json, desc);
}

/** Deserialize value from JSON.
 *
 * @memberof flecs::world
 * @ingroup cpp_addons_json
 */
template <typename T>
const char* from_json(T* value, const char *json, flecs::from_json_desc_t *desc = nullptr) {
    return ecs_ptr_from_json(world_, _::type<T>::id(world_),
        value, json, desc);
}

/** Deserialize JSON into world.
 *
 * @memberof flecs::world
 * @ingroup cpp_addons_json
 */
const char* from_json(const char *json, flecs::from_json_desc_t *desc = nullptr) {
    return ecs_world_from_json(world_, json, desc);
}

/** Deserialize JSON file into world.
 *
 * @memberof flecs::world
 * @ingroup cpp_addons_json
 */
const char* from_json_file(const char *json, flecs::from_json_desc_t *desc = nullptr) {
    return ecs_world_from_json_file(world_, json, desc);
}

#   endif
#   ifdef FLECS_APP
/**
 * @file addons/cpp/mixins/app/mixin.inl
 * @brief App world addon mixin.
 */

/**
 * @ingroup cpp_addons_app
 * @memberof flecs::world
 *
 * @{
 */

/** Return app builder.
 * The app builder is a convenience wrapper around a loop that runs
 * world::progress. An app allows for writing platform agnostic code,
 * as it provides hooks to modules for overtaking the main loop which is
 * required for frameworks like emscripten.
 */
flecs::app_builder app() {
    flecs::world_t *w = world_;
    world_ = nullptr; // Take ownership
    return flecs::app_builder(w);
}

/** @} */

#   endif
#   ifdef FLECS_METRICS

/** Create metric.
 *
 * @ingroup cpp_addons_metrics
 * @memberof flecs::world
 */
template <typename... Args>
flecs::metric_builder metric(Args &&... args) const;

#   endif
#   ifdef FLECS_ALERTS

/** Create alert.
 *
 * @ingroup cpp_addons_alerts
 * @memberof flecs::world
 */
template <typename... Comps, typename... Args>
flecs::alert_builder<Comps...> alert(Args &&... args) const;

#   endif

public:
    void init_builtin_components();

    world_t *world_;
};

/** Scoped world.
 * Utility class used by the world::scope method to create entities in a scope.
 */
struct scoped_world : world {
    scoped_world(
        flecs::world_t *w,
        flecs::entity_t s) : world(w)
    {
        prev_scope_ = ecs_set_scope(w, s);
    }

    ~scoped_world() {
        ecs_set_scope(world_, prev_scope_);
    }

    scoped_world(const scoped_world& obj) : world(nullptr) {
        prev_scope_ = obj.prev_scope_;
        world_ = obj.world_;
        flecs_poly_claim(world_);
    }

    flecs::entity_t prev_scope_;
};

/** @} */

} // export namespace flecs


/**
 * @file addons/cpp/field.hpp
 * @brief Wrapper classes for fields returned by flecs::iter.
 */

#pragma once

/**
 * @defgroup cpp_field Fields
 * @ingroup cpp_core
 * Field helper types.
 *
 * @{
 */

export namespace flecs
{

/** Unsafe wrapper class around a field.
 * This class can be used when a system does not know the type of a field at
 * compile time.
 *
 * @ingroup cpp_iterator
 */
struct untyped_field {
    untyped_field(void* array, size_t size, size_t count, bool is_shared = false)
        : data_(array)
        , size_(size)
        , count_(count)
        , is_shared_(is_shared) {}

    /** Return element in component array.
     * This operator may only be used if the field is not shared.
     *
     * @param index Index of element.
     * @return Reference to element.
     */
    void* operator[](size_t index) const {
        ecs_assert(!is_shared_ || !index, ECS_INVALID_PARAMETER,
            "invalid usage of [] operator for shared component field");
        ecs_assert(index < count_, ECS_COLUMN_INDEX_OUT_OF_RANGE,
            "index %d out of range for field", index);
        return ECS_OFFSET(data_, size_ * index);
    }

protected:
    void* data_;
    size_t size_;
    size_t count_;
    bool is_shared_;
};

/** Wrapper class around a field.
 *
 * @tparam T component type of the field.
 *
 * @ingroup cpp_iterator
 */
template <typename T>
struct field {
    static_assert(std::is_empty<T>::value == false,
        "invalid type for field, cannot iterate empty type");

    /** Create field from component array.
     *
     * @param array Pointer to the component array.
     * @param count Number of elements in component array.
     * @param is_shared Is the component shared or not.
     */
    field(T* array, size_t count, bool is_shared = false)
        : data_(array)
        , count_(count)
        , is_shared_(is_shared) {}

    /** Create field from iterator.
     *
     * @param iter Iterator object.
     * @param field Index of the signature of the query being iterated over.
     */
    field(iter &iter, int field);

    /** Return element in component array.
     * This operator may only be used if the field is not shared.
     *
     * @param index Index of element.
     * @return Reference to element.
     */
    T& operator[](size_t index) const;

    /** Return first element of component array.
     * This operator is typically used when the field is shared.
     *
     * @return Reference to the first element.
     */
    T& operator*() const;

    /** Return first element of component array.
     * This operator is typically used when the field is shared.
     *
     * @return Pointer to the first element.
     */
    T* operator->() const;

protected:
    T* data_;
    size_t count_;
    bool is_shared_;
};

} // export namespace flecs

/** @} */

/**
 * @file addons/cpp/iter.hpp
 * @brief Wrapper classes for ecs_iter_t and component arrays.
 */

#pragma once

/**
 * @defgroup cpp_iterator Iterators
 * @ingroup cpp_core
 * Iterator operations.
 *
 * @{
 */

export namespace flecs
{

////////////////////////////////////////////////////////////////////////////////

namespace _ {

////////////////////////////////////////////////////////////////////////////////

/** Iterate over an integer range (used to iterate over entity range).
 *
 * @tparam T of the iterator
 */
template <typename T>
struct range_iterator
{
    explicit range_iterator(T value)
        : value_(value){}

    bool operator!=(range_iterator const& other) const
    {
        return value_ != other.value_;
    }

    T const& operator*() const
    {
        return value_;
    }

    range_iterator& operator++()
    {
        ++value_;
        return *this;
    }

private:
    T value_;
};

} // namespace _

} // export namespace flecs

export namespace flecs
{

////////////////////////////////////////////////////////////////////////////////

/** Class for iterating over query results.
 *
 * @ingroup cpp_iterator
 */
struct iter {
private:
    using row_iterator = _::range_iterator<size_t>;

public:
    /** Construct iterator from C iterator object.
     * This operation is typically not invoked directly by the user.
     *
     * @param it Pointer to C iterator.
     */
    iter(ecs_iter_t *it) : iter_(it) { }

    row_iterator begin() const {
        return row_iterator(0);
    }

    row_iterator end() const {
        return row_iterator(static_cast<size_t>(iter_->count));
    }

    flecs::entity system() const;

    flecs::entity event() const;

    flecs::id event_id() const;

    flecs::world world() const;

    const flecs::iter_t* c_ptr() const {
        return iter_;
    }

    size_t count() const {
        ecs_check(iter_->flags & EcsIterIsValid, ECS_INVALID_PARAMETER,
            "operation invalid before calling next()");
        return static_cast<size_t>(iter_->count);
    error:
        return 0;
    }

    ecs_ftime_t delta_time() const {
        return iter_->delta_time;
    }

    ecs_ftime_t delta_system_time() const {
        return iter_->delta_system_time;
    }

    flecs::type type() const;

    flecs::table table() const;

    flecs::table other_table() const;

    flecs::table_range range() const;

    /** Access ctx.
     * ctx contains the context pointer assigned to a system.
     */
    void* ctx() {
        return iter_->ctx;
    }

    /** Access ctx.
     * ctx contains the context pointer assigned to a system.
     */
    template <typename T>
    T* ctx() {
        return static_cast<T*>(iter_->ctx);
    }

    /** Access param.
     * param contains the pointer passed to the param argument of system::run
     */
    void* param() {
        return iter_->param;
    }

    /** Access param.
     * param contains the pointer passed to the param argument of system::run
     */
    template <typename T>
    T* param() {
        /* TODO: type check */
        return static_cast<T*>(iter_->param);
    }

    /** Obtain mutable handle to entity being iterated over.
     *
     * @param row Row being iterated over.
     */
    flecs::entity entity(size_t row) const;

    /** Returns whether field is matched on self.
     *
     * @param index The field index.
     */
    bool is_self(int8_t index) const {
        return ecs_field_is_self(iter_, index);
    }

    /** Returns whether field is set.
     *
     * @param index The field index.
     */
    bool is_set(int8_t index) const {
        return ecs_field_is_set(iter_, index);
    }

    /** Returns whether field is readonly.
     *
     * @param index The field index.
     */
    bool is_readonly(int8_t index) const {
        return ecs_field_is_readonly(iter_, index);
    }

    /** Number of fields in iterator.
     */
    int32_t field_count() const {
        return iter_->field_count;
    }

    /** Size of field data type.
     *
     * @param index The field id.
     */
    size_t size(int8_t index) const {
        return ecs_field_size(iter_, index);
    }

    /** Obtain field source (0 if This).
     *
     * @param index The field index.
     */
    flecs::entity src(int8_t index) const;

    /** Obtain id matched for field.
     *
     * @param index The field index.
     */
    flecs::id id(int8_t index) const;

    /** Obtain pair id matched for field.
     * This operation will fail if the id is not a pair.
     *
     * @param index The field index.
     */
    flecs::id pair(int8_t index) const;

    /** Obtain column index for field.
     *
     * @param index The field index.
     */
    int32_t column_index(int8_t index) const {
        return ecs_field_column(iter_, index);
    }

    /** Obtain term that triggered an observer
     */
    int8_t term_index() const {
        return iter_->term_index;
    }

    /** Convert current iterator result to string.
     */
    flecs::string str() const {
        char *s = ecs_iter_str(iter_);
        return flecs::string(s);
    }

    /** Get readonly access to field data.
     * If the specified field index does not match with the provided type, the
     * function will assert.
     *
     * This function should not be used in each() callbacks, unless it is to
     * access a shared field. For access to non-shared fields in each(), use
     * field_at.
     *
     * @tparam T Type of the field.
     * @param index The field index.
     * @return The field data.
     */
    template <typename T, typename A = actual_type_t<T>,
        typename std::enable_if<std::is_const<T>::value, void>::type* = nullptr>
    flecs::field<A> field(int8_t index) const;

    /** Get read/write access to field data.
     * If the matched id for the specified field does not match with the provided
     * type or if the field is readonly, the function will assert.
     *
     * This function should not be used in each() callbacks, unless it is to
     * access a shared field. For access to non-shared fields in each(), use
     * field_at.
     *
     * @tparam T Type of the field.
     * @param index The field index.
     * @return The field data.
     */
    template <typename T, typename A = actual_type_t<T>,
        typename std::enable_if<
            std::is_const<T>::value == false, void>::type* = nullptr>
    flecs::field<A> field(int8_t index) const;

    /** Get unchecked access to field data.
     * Unchecked access is required when a system does not know the type of a
     * field at compile time.
     *
     * This function should not be used in each() callbacks, unless it is to
     * access a shared field. For access to non-shared fields in each(), use
     * field_at.
     *
     * @param index The field index.
     */
    flecs::untyped_field field(int8_t index) const {
        ecs_assert(!(iter_->flags & EcsIterCppEach) ||
               ecs_field_src(iter_, index) != 0, ECS_INVALID_OPERATION,
            "cannot .field from .each, use .field_at(%d, row) instead", index);
        return get_unchecked_field(index);
    }

    /** Get pointer to field at row.
     * This function may be used to access shared fields when row is set to 0.
     */
    void* field_at(int8_t index, size_t row) const {
        if (iter_->row_fields & (1llu << index)) {
            return get_unchecked_field_at(index, row)[0];
        } else {
            return get_unchecked_field(index)[row];
        }
    }

    /** Get reference to field at row.
     * This function may be used to access shared fields when row is set to 0.
     */
    template <typename T, typename A = actual_type_t<T>,
        typename std::enable_if<std::is_const<T>::value, void>::type* = nullptr>
    const A& field_at(int8_t index, size_t row) const {
        if (iter_->row_fields & (1llu << index)) {
            return get_field_at<A>(index, row)[0];
        } else {
            return get_field<A>(index)[row];
        }
    }

    /** Get reference to field at row.
     * This function may be used to access shared fields when row is set to 0.
     */
    template <typename T, typename A = actual_type_t<T>,
        typename std::enable_if<
            std::is_const<T>::value == false, void>::type* = nullptr>
    A& field_at(int8_t index, size_t row) const {
        ecs_assert(!ecs_field_is_readonly(iter_, index),
            ECS_ACCESS_VIOLATION, NULL);
        if (iter_->row_fields & (1llu << index)) {
            return get_field_at<A>(index, row)[0];
        } else {
            return get_field<A>(index)[row];
        }
    }

    /** Get readonly access to entity ids.
     *
     * @return The entity ids.
     */
    flecs::field<const flecs::entity_t> entities() const {
        return flecs::field<const flecs::entity_t>(
            iter_->entities, static_cast<size_t>(iter_->count), false);
    }

    /** Check if the current table has changed since the last iteration.
     * Can only be used when iterating queries and/or systems. */
    bool changed() {
        return ecs_iter_changed(iter_);
    }

    /** Skip current table.
     * This indicates to the query that the data in the current table is not
     * modified. By default, iterating a table with a query will mark the
     * iterated components as dirty if they are annotated with InOut or Out.
     *
     * When this operation is invoked, the components of the current table will
     * not be marked dirty. */
    void skip() {
        ecs_iter_skip(iter_);
    }

    /* Return group id for current table (grouped queries only) */
    uint64_t group_id() const {
        return ecs_iter_get_group(iter_);
    }

    /** Get value of variable by id.
     * Get value of a query variable for current result.
     */
    flecs::entity get_var(int var_id) const;

    /** Get value of variable by name.
     * Get value of a query variable for current result.
     */
    flecs::entity get_var(const char *name) const;

    /** Progress iterator.
     * This operation should only be called from a context where the iterator is
     * not being progressed automatically. An example of a valid context is
     * inside of a run() callback. An example of an invalid context is inside of
     * an each() callback.
     */
    bool next() {
        if (iter_->flags & EcsIterIsValid && iter_->table) {
            ECS_TABLE_UNLOCK(iter_->world, iter_->table);
        }
        bool result = iter_->next(iter_);
        iter_->flags |= EcsIterIsValid;
        if (result && iter_->table) {
            ECS_TABLE_LOCK(iter_->world, iter_->table);
        }
        return result;
    }

    /** Forward to each.
     * If a system has an each callback registered, this operation will forward
     * the current iterator to the each callback.
     */
    void each() {
        iter_->callback(iter_);
    }

    /** Iterate targets for pair field.
     *
     * @param index The field index.
     * @param func Callback invoked for each target
     */
    template <typename Func>
    void targets(int8_t index, const Func& func);

    /** Free iterator resources.
     * This operation only needs to be called when the iterator is not iterated
     * until completion (e.g. the last call to next() did not return false).
     *
     * Failing to call this operation on an unfinished iterator will throw a
     * fatal LEAK_DETECTED error.
     *
     * @see ecs_iter_fini()
     */
    void fini() {
        if (iter_->flags & EcsIterIsValid && iter_->table) {
            ECS_TABLE_UNLOCK(iter_->world, iter_->table);
        }
        ecs_iter_fini(iter_);
    }

private:
    /* Get field, check if correct type is used */
    template <typename T, typename A = actual_type_t<T>>
    flecs::field<T> get_field(int8_t index) const {

#ifndef FLECS_NDEBUG
        ecs_entity_t term_id = ecs_field_id(iter_, index);
        ecs_assert(ECS_HAS_ID_FLAG(term_id, PAIR) ||
            term_id == _::type<T>::id(iter_->world),
            ECS_COLUMN_TYPE_MISMATCH, NULL);
#endif

        size_t count;
        bool is_shared = !ecs_field_is_self(iter_, index);

        /* If a shared column is retrieved with 'column', there will only be a
         * single value. Ensure that the application does not accidentally read
         * out of bounds. */
        if (is_shared) {
            count = 1;
        } else {
            /* If column is owned, there will be as many values as there are
             * entities. */
            count = static_cast<size_t>(iter_->count);
        }

        return flecs::field<A>(
            static_cast<T*>(ecs_field_w_size(iter_, sizeof(A), index)),
            count, is_shared);
    }

    /* Get field, check if correct type is used */
    template <typename T, typename A = actual_type_t<T>>
    flecs::field<T> get_field_at(int8_t index, int32_t row) const {

#ifndef FLECS_NDEBUG
        ecs_entity_t term_id = ecs_field_id(iter_, index);
        ecs_assert(ECS_HAS_ID_FLAG(term_id, PAIR) ||
            term_id == _::type<T>::id(iter_->world),
            ECS_COLUMN_TYPE_MISMATCH, NULL);
#endif

        return flecs::field<A>(
            static_cast<T*>(ecs_field_at_w_size(iter_, sizeof(A), index, row)),
                1, false);
    }

    flecs::untyped_field get_unchecked_field(int8_t index) const {
        size_t count;
        size_t size = ecs_field_size(iter_, index);
        bool is_shared = !ecs_field_is_self(iter_, index);

        /* If a shared column is retrieved with 'column', there will only be a
         * single value. Ensure that the application does not accidentally read
         * out of bounds. */
        if (is_shared) {
            count = 1;
        } else {
            /* If column is owned, there will be as many values as there are
             * entities. */
            count = static_cast<size_t>(iter_->count);
        }

        return flecs::untyped_field(
            ecs_field_w_size(iter_, size, index), size, count, is_shared);
    }

    flecs::untyped_field get_unchecked_field_at(int8_t index, size_t row) const {
        size_t size = ecs_field_size(iter_, index);
        return flecs::untyped_field(
            ecs_field_at_w_size(iter_, size, index, static_cast<int32_t>(row)),
                size, 1, false);
    }

    flecs::iter_t *iter_;
};

} // export namespace flecs

/** @} */

/**
 * @file addons/cpp/ref.hpp
 * @brief Class that caches data to speedup get operations.
 */

#pragma once

export namespace flecs
{

/**
 * @defgroup cpp_ref Refs
 * @ingroup cpp_core
 * Refs are a fast mechanism for referring to a specific entity/component.
 *
 * @{
 */

/** Untyped component reference.
 * Reference to a component from a specific entity.
 */
struct untyped_ref {

    untyped_ref () : world_(nullptr), ref_{} {}

    untyped_ref(world_t *world, entity_t entity, flecs::id_t id)
        : ref_() {
        ecs_assert(id != 0, ECS_INVALID_PARAMETER,
            "invalid id");
        // the world we were called with may be a stage; convert it to a world
        // here if that is the case
        world_ = world ? const_cast<flecs::world_t *>(ecs_get_world(world))
            : nullptr;

#ifdef FLECS_DEBUG
        flecs::entity_t type = ecs_get_typeid(world, id);
        const flecs::type_info_t *ti = ecs_get_type_info(world, type);
        ecs_assert(ti && ti->size != 0, ECS_INVALID_PARAMETER,
            "cannot create ref to empty type");
#endif
        ref_ = ecs_ref_init_id(world_, entity, id);
    }

    untyped_ref(flecs::entity entity, flecs::id_t id);

    /** Return entity associated with reference. */
    flecs::entity entity() const;

    /** Return component associated with reference. */
    flecs::id component() const {
        return flecs::id(world_, ref_.id);
    }

    void* get() {
        return ecs_ref_get_id(world_, &ref_, this->ref_.id);
    }

    bool has() {
        return !!try_get();
    }

    flecs::world world() const {
        return flecs::world(world_);
    }

    /** implicit conversion to bool.  return true if there is a valid
     * component instance being referred to **/
    operator bool() {
        return has();
    }

    void* try_get() {
        if (!world_ || !ref_.entity) {
            return nullptr;
        }

        return get();
    }

private:
    world_t *world_;
    flecs::ref_t ref_;
};

/** Component reference.
 * Reference to a component from a specific entity.
 */
template <typename T>
struct ref : public untyped_ref {
    ref() : untyped_ref() { }

    ref(world_t *world, entity_t entity, flecs::id_t id = 0)
        : untyped_ref(world, entity, id ? id : _::type<T>::id(world))
    {    }

    ref(flecs::entity entity, flecs::id_t id = 0);

    T* operator->() {
        T* result = static_cast<T*>(get());

        ecs_assert(result != NULL, ECS_INVALID_PARAMETER,
            "nullptr dereference by flecs::ref");

        return result;
    }

    T* get() {
        return static_cast<T*>(untyped_ref::get());
    }

    T* try_get() {
        return static_cast<T*>(untyped_ref::try_get());
    }
};

/** @} */

}

/**
 * @file addons/cpp/entity.hpp
 * @brief Entity class.
 *
 * This class provides read/write access to entities.
 */

#pragma once

/**
 * @file addons/cpp/entity_view.hpp
 * @brief Entity class with only readonly operations.
 *
 * This class provides readonly access to entities. Using this class to store
 * entities in components ensures valid handles, as this class will always store
 * the actual world vs. a stage. The constructors of this class will never
 * create a new entity.
 *
 * To obtain a mutable handle to the entity, use the "mut" function.
 */

#pragma once

/**
 * @ingroup cpp_entities
 * @{
 */

export namespace flecs
{

/** Entity view.
 * Class with read operations for entities. Base for flecs::entity.
 *
 * @ingroup cpp_entities
 */
struct entity_view : public id {

    entity_view() : flecs::id() { }

    /** Wrap an existing entity id.
     *
     * @param world The world in which the entity is created.
     * @param id The entity id.
     */
    explicit entity_view(flecs::world_t *world, flecs::id_t id)
        : flecs::id(world
            ? const_cast<flecs::world_t*>(ecs_get_world(world))
            : nullptr
        , id ) { }

    /** Implicit conversion from flecs::entity_t to flecs::entity_view. */
    entity_view(entity_t id)
        : flecs::id( nullptr, id ) { }

    /** Get entity id.
     * @return The integer entity id.
     */
    entity_t id() const {
        return id_;
    }

    /** Check if entity is valid.
     * An entity is valid if:
     * - its id is not 0
     * - the id contains a valid bit pattern for an entity
     * - the entity is alive (see is_alive())
     *
     * @return True if the entity is valid, false otherwise.
     * @see ecs_is_valid()
     */
    bool is_valid() const {
        return world_ && ecs_is_valid(world_, id_);
    }

    explicit operator bool() const {
        return is_valid();
    }

    /** Check if entity is alive.
     *
     * @return True if the entity is alive, false otherwise.
     * @see ecs_is_alive()
     */
    bool is_alive() const {
        return world_ && ecs_is_alive(world_, id_);
    }

    /** Return the entity name.
     *
     * @return The entity name.
     */
    flecs::string_view name() const {
        return flecs::string_view(ecs_get_name(world_, id_));
    }

    /** Return the entity symbol.
     *
     * @return The entity symbol.
     */
    flecs::string_view symbol() const {
        return flecs::string_view(ecs_get_symbol(world_, id_));
    }

    /** Return the entity path.
     *
     * @return The hierarchical entity path.
     */
    flecs::string path(const char *sep = "::", const char *init_sep = "::") const {
        return path_from(0, sep, init_sep);
    }

    /** Return the entity path relative to a parent.
     *
     * @return The relative hierarchical entity path.
     */
    flecs::string path_from(flecs::entity_t parent, const char *sep = "::", const char *init_sep = "::") const {
        char *path = ecs_get_path_w_sep(world_, parent, id_, sep, init_sep);
        return flecs::string(path);
    }

    /** Return the entity path relative to a parent.
     *
     * @return The relative hierarchical entity path.
     */
    template <typename Parent>
    flecs::string path_from(const char *sep = "::", const char *init_sep = "::") const {
        return path_from(_::type<Parent>::id(world_), sep, init_sep);
    }

    bool enabled() const {
        return !ecs_has_id(world_, id_, flecs::Disabled);
    }

    /** Get the entity's type.
     *
     * @return The entity's type.
     */
    flecs::type type() const;

    /** Get the entity's table.
     *
     * @return Returns the entity's table.
     */
    flecs::table table() const;

    /** Get table range for the entity.
     * Returns a range with the entity's row as offset and count set to 1. If
     * the entity is not stored in a table, the function returns a range with
     * count 0.
     *
     * @return Returns the entity's table range.
     */
    flecs::table_range range() const;

    /** Iterate (component) ids of an entity.
     * The function parameter must match the following signature:
     *
     * @code
     * void(*)(flecs::id id)
     * @endcode
     *
     * @param func The function invoked for each id.
     */
    template <typename Func>
    void each(const Func& func) const;

    /** Iterate matching pair ids of an entity.
     * The function parameter must match the following signature:
     *
     * @code
     * void(*)(flecs::id id)
     * @endcode
     *
     * @param func The function invoked for each id.
     */
    template <typename Func>
    void each(flecs::id_t first, flecs::id_t second, const Func& func) const;

    /** Iterate targets for a given relationship.
     * The function parameter must match the following signature:
     *
     * @code
     * void(*)(flecs::entity target)
     * @endcode
     *
     * @param rel The relationship for which to iterate the targets.
     * @param func The function invoked for each target.
     */
    template <typename Func>
    void each(const flecs::entity_view& rel, const Func& func) const;

    /** Iterate targets for a given relationship.
     * The function parameter must match the following signature:
     *
     * @code
     * void(*)(flecs::entity target)
     * @endcode
     *
     * @tparam First The relationship for which to iterate the targets.
     * @param func The function invoked for each target.
     */
    template <typename First, typename Func>
    void each(const Func& func) const {
        return each(_::type<First>::id(world_), func);
    }

    /** Iterate children for entity.
     * The function parameter must match the following signature:
     *
     * @code
     * void(*)(flecs::entity target)
     * @endcode
     *
     * @param rel The relationship to follow.
     * @param func The function invoked for each child.
     */
    template <typename Func>
    void children(flecs::entity_t rel, Func&& func) const {
        /* When the entity is a wildcard, this would attempt to query for all
         * entities with (ChildOf, *) or (ChildOf, _) instead of querying for
         * the children of the wildcard entity. */
        if (id_ == flecs::Wildcard || id_ == flecs::Any) {
            /* This is correct, wildcard entities don't have children */
            return;
        }

        flecs::world world(world_);

        ecs_iter_t it = ecs_children_w_rel(world_, rel, id_);
        while (ecs_children_next(&it)) {
            _::each_delegate<Func>(FLECS_MOV(func)).invoke(&it);
        }
    }

    /** Iterate children for entity.
     * The function parameter must match the following signature:
     *
     * @code
     * void(*)(flecs::entity target)
     * @endcode
     *
     * @tparam Rel The relationship to follow.
     * @param func The function invoked for each child.
     */
    template <typename Rel, typename Func>
    void children(Func&& func) const {
        children(_::type<Rel>::id(world_), FLECS_MOV(func));
    }

    /** Iterate children for entity.
     * The function parameter must match the following signature:
     *
     * @code
     * void(*)(flecs::entity target)
     * @endcode
     *
     * This operation follows the ChildOf relationship.
     *
     * @param func The function invoked for each child.
     */
    template <typename Func>
    void children(Func&& func) const {
        children(flecs::ChildOf, FLECS_MOV(func));
    }


    /* try_get */

    /** Get component value.
     *
     * @tparam T The component to get.
     * @return Pointer to the component value, nullptr if the entity does not
     *         have the component.
     */
    template <typename T, if_t< is_actual<T>::value > = 0>
    const T* try_get() const {
        auto comp_id = _::type<T>::id(world_);
        ecs_assert(_::type<T>::size() != 0, ECS_INVALID_PARAMETER,
            "operation invalid for empty type");
        return static_cast<const T*>(ecs_get_id(world_, id_, comp_id));
    }

    /** Get component value.
     * Overload for when T is not the same as the actual type, which happens
     * when using pair types.
     *
     * @tparam T The component to get.
     * @return Pointer to the component value, nullptr if the entity does not
     *         have the component.
     */
    template <typename T, typename A = actual_type_t<T>,
        if_t< flecs::is_pair<T>::value > = 0>
    const A* try_get() const {
        auto comp_id = _::type<T>::id(world_);
        ecs_assert(_::type<A>::size() != 0, ECS_INVALID_PARAMETER,
            "operation invalid for empty type");
        return static_cast<const A*>(ecs_get_id(world_, id_, comp_id));
    }

    /** Get a pair.
     * This operation gets the value for a pair from the entity.
     *
     * @tparam First The first element of the pair.
     * @tparam Second the second element of a pair.
     */
    template <typename First, typename Second, typename P = pair<First, Second>,
        typename A = actual_type_t<P>, if_not_t< flecs::is_pair<First>::value > = 0>
    const A* try_get() const {
        return this->try_get<P>();
    }

    /** Get a pair.
     * This operation gets the value for a pair from the entity.
     *
     * @tparam First The first element of the pair.
     * @param second The second element of the pair.
     */
    template<typename First, typename Second, if_not_t< is_enum<Second>::value> = 0>
    const First* try_get(Second second) const {
        auto first = _::type<First>::id(world_);
        ecs_assert(_::type<First>::size() != 0, ECS_INVALID_PARAMETER,
            "operation invalid for empty type");
        return static_cast<const First*>(
            ecs_get_id(world_, id_, ecs_pair(first, second)));
    }

    /** Get a pair.
     * This operation gets the value for a pair from the entity.
     *
     * @tparam First The first element of the pair.
     * @param constant the enum constant.
     */
    template<typename First, typename Second, if_t< is_enum<Second>::value && !std::is_same<First, Second>::value > = 0>
    const First* try_get(Second constant) const {
        const auto& et = enum_type<Second>(this->world_);
        flecs::entity_t target = et.entity(constant);
        return try_get<First>(target);
    }

    /** Get component value (untyped).
     *
     * @param comp The component to get.
     * @return Pointer to the component value, nullptr if the entity does not
     *         have the component.
     */
    const void* try_get(flecs::id_t comp) const {
        return ecs_get_id(world_, id_, comp);
    }

    /** Get a pair (untyped).
     * This operation gets the value for a pair from the entity. If neither the
     * first nor the second part of the pair are components, the operation
     * will fail.
     *
     * @param first The first element of the pair.
     * @param second The second element of the pair.
     */
    const void* try_get(flecs::entity_t first, flecs::entity_t second) const {
        return ecs_get_id(world_, id_, ecs_pair(first, second));
    }

    /** Get the second part for a pair.
     * This operation gets the value for a pair from the entity. The first
     * part of the pair should not be a component.
     *
     * @tparam Second the second element of a pair.
     * @param first The first part of the pair.
     */
    template<typename Second>
    const Second* try_get_second(flecs::entity_t first) const {
        auto second = _::type<Second>::id(world_);
        ecs_assert( ecs_get_type_info(world_, ecs_pair(first, second)) != NULL,
            ECS_INVALID_PARAMETER, "pair is not a component");
        ecs_assert( ecs_get_type_info(world_, ecs_pair(first, second))->component == second,
            ECS_INVALID_PARAMETER, "type of pair is not Second");
        ecs_assert(_::type<Second>::size() != 0, ECS_INVALID_PARAMETER,
            "operation invalid for empty type");
        return static_cast<const Second*>(
            ecs_get_id(world_, id_, ecs_pair(first, second)));
    }

    /** Get the second part for a pair.
     * This operation gets the value for a pair from the entity. The first
     * part of the pair should not be a component.
     *
     * @tparam First The first element of the pair.
     * @tparam Second the second element of a pair.
     */
    template<typename First, typename Second>
    const Second* try_get_second() const {
        return try_get<pair_object<First, Second>>();
    }


    /* get */

    /** Get component value.
     *
     * @tparam T The component to get.
     * @return Ref to the component value, panics if the entity does not
     *         have the component.
     */
    template <typename T, if_t< is_actual<T>::value > = 0>
    const T& get() const {
        const T *r = try_get<T>();
        ecs_assert(r != nullptr, ECS_INVALID_OPERATION,
            "invalid get: entity does not have component (use try_get)");
        return *r;
    }

    /** Get component value.
     * Overload for when T is not the same as the actual type, which happens
     * when using pair types.
     *
     * @tparam T The component to get.
     * @return Ref to the component value, panics if the entity does not
     *         have the component.
     */
    template <typename T, typename A = actual_type_t<T>,
        if_t< flecs::is_pair<T>::value > = 0>
    const A& get() const {
        const A *r = try_get<T>();
        ecs_assert(r != nullptr, ECS_INVALID_OPERATION,
            "invalid get: entity does not have component (use try_get)");
        return *r;
    }

    /** Get a pair.
     * This operation gets the value for a pair from the entity.
     *
     * @tparam First The first element of the pair.
     * @tparam Second the second element of a pair.
     * @return Ref to the component value, panics if the entity does not
     *         have the component.
     */
    template <typename First, typename Second, typename P = pair<First, Second>,
        typename A = actual_type_t<P>, if_not_t< flecs::is_pair<First>::value > = 0>
    const A& get() const {
        return this->get<P>();
    }

    /** Get a pair.
     * This operation gets the value for a pair from the entity.
     *
     * @tparam First The first element of the pair.
     * @param second The second element of the pair.
     * @return Ref to the component value, panics if the entity does not
     *         have the component.
     */
    template<typename First, typename Second, if_not_t< is_enum<Second>::value> = 0>
    const First& get(Second second) const {
        const First *r = try_get<First>(second);
        ecs_assert(r != nullptr, ECS_INVALID_OPERATION,
            "invalid get: entity does not have component (use try_get)");
        return *r;
    }

    /** Get a pair.
     * This operation gets the value for a pair from the entity.
     *
     * @tparam First The first element of the pair.
     * @param constant the enum constant.
     * @return Ref to the component value, panics if the entity does not
     *         have the component.
     */
    template<typename First, typename Second, if_t< is_enum<Second>::value && !std::is_same<First, Second>::value > = 0>
    const First& get(Second constant) const {
        const auto& et = enum_type<Second>(this->world_);
        flecs::entity_t target = et.entity(constant);
        return get<First>(target);
    }

    /** Get component value (untyped).
     *
     * @param comp The component to get.
     * @return Pointer to the component value, panics if the entity does not
     *         have the component.
     */
    const void* get(flecs::id_t comp) const {
        const void *r = ecs_get_id(world_, id_, comp);
        ecs_assert(r != nullptr, ECS_INVALID_OPERATION,
            "invalid get: entity does not have component (use try_get)");
        return r;
    }

    /** Get a pair (untyped).
     * This operation gets the value for a pair from the entity. If neither the
     * first nor the second part of the pair are components, the operation
     * will fail.
     *
     * @param first The first element of the pair.
     * @param second The second element of the pair.
     * @return Pointer to the component value, panics if the entity does not
     *         have the component.
     */
    const void* get(flecs::entity_t first, flecs::entity_t second) const {
        const void *r = ecs_get_id(world_, id_, ecs_pair(first, second));
        ecs_assert(r != nullptr, ECS_INVALID_OPERATION,
            "invalid get: entity does not have component (use try_get)");
        return r;
    }

    /** Get 1..N components.
     * This operation accepts a callback with as arguments the components to
     * retrieve. The callback will only be invoked when the entity has all
     * the components.
     *
     * This operation is faster than individually calling get for each component
     * as it only obtains entity metadata once.
     *
     * While the callback is invoked the table in which the components are
     * stored is locked, which prevents mutations that could cause invalidation
     * of the component references. Note that this is not an actual lock:
     * invalid access causes a runtime panic and so it is still up to the
     * application to ensure access is protected.
     *
     * The component arguments must be references and can be either const or
     * non-const. When all arguments are const, the function will read-lock the
     * table (see ecs_read_begin). If one or more arguments are non-const the
     * function will write-lock the table (see ecs_write_begin).
     *
     * Example:
     *
     * @code
     * e.get([](Position& p, Velocity& v) { // write lock
     *   p.x += v.x;
     * });
     *
     * e.get([](const Position& p) {        // read lock
     *   std::cout << p.x << std::endl;
     * });
     * @endcode
     *
     * @param func The callback to invoke.
     * @return True if the entity has all components, false if not.
     */
    template <typename Func, if_t< is_callable<Func>::value > = 0>
    bool get(const Func& func) const;

    /** Get the second part for a pair.
     * This operation gets the value for a pair from the entity. The first
     * part of the pair should not be a component.
     *
     * @tparam Second the second element of a pair.
     * @param first The first part of the pair.
     */
    template<typename Second>
    const Second& get_second(flecs::entity_t first) const {
        const Second *r = try_get_second<Second>(first);
        ecs_assert(r != nullptr, ECS_INVALID_OPERATION,
            "invalid get: entity does not have component (use try_get)");
        return *r;
    }

    /** Get the second part for a pair.
     * This operation gets the value for a pair from the entity. The first
     * part of the pair should not be a component.
     *
     * @tparam First The first element of the pair.
     * @tparam Second the second element of a pair.
     */
    template<typename First, typename Second>
    const Second& get_second() const {
        const Second *r = try_get<First, Second>();
        ecs_assert(r != nullptr, ECS_INVALID_OPERATION,
            "invalid get: entity does not have component (use try_get)");
        return *r;
    }


    /* try_get_mut */

    /** Get mutable component value.
     *
     * @tparam T The component to get.
     * @return Pointer to the component value, nullptr if the entity does not
     *         have the component.
     */
    template <typename T, if_t< is_actual<T>::value > = 0>
    T* try_get_mut() const {
        auto comp_id = _::type<T>::id(world_);
        ecs_assert(_::type<T>::size() != 0, ECS_INVALID_PARAMETER,
            "operation invalid for empty type");
        return static_cast<T*>(ecs_get_mut_id(world_, id_, comp_id));
    }

    /** Get mutable component value.
     * Overload for when T is not the same as the actual type, which happens
     * when using pair types.
     *
     * @tparam T The component to get.
     * @return Pointer to the component value, nullptr if the entity does not
     *         have the component.
     */
    template <typename T, typename A = actual_type_t<T>,
        if_t< flecs::is_pair<T>::value > = 0>
    A* try_get_mut() const {
        auto comp_id = _::type<T>::id(world_);
        ecs_assert(_::type<A>::size() != 0, ECS_INVALID_PARAMETER,
            "operation invalid for empty type");
        return static_cast<A*>(ecs_get_mut_id(world_, id_, comp_id));
    }

    /** Get a mutable pair.
     * This operation gets the value for a pair from the entity.
     *
     * @tparam First The first element of the pair.
     * @tparam Second the second element of a pair.
     */
    template <typename First, typename Second, typename P = pair<First, Second>,
        typename A = actual_type_t<P>, if_not_t< flecs::is_pair<First>::value > = 0>
    A* try_get_mut() const {
        return this->try_get_mut<P>();
    }

    /** Get a mutable pair.
     * This operation gets the value for a pair from the entity.
     *
     * @tparam First The first element of the pair.
     * @param second The second element of the pair.
     */
    template<typename First, typename Second, if_not_t< is_enum<Second>::value> = 0>
    First* try_get_mut(Second second) const {
        auto first = _::type<First>::id(world_);
        ecs_assert(_::type<First>::size() != 0, ECS_INVALID_PARAMETER,
            "operation invalid for empty type");
        return static_cast<First*>(
            ecs_get_mut_id(world_, id_, ecs_pair(first, second)));
    }

    /** Get a mutable pair.
     * This operation gets the value for a pair from the entity.
     *
     * @tparam First The first element of the pair.
     * @param constant the enum constant.
     */
    template<typename First, typename Second, if_t< is_enum<Second>::value && !std::is_same<First, Second>::value > = 0>
    First* try_get_mut(Second constant) const {
        const auto& et = enum_type<Second>(this->world_);
        flecs::entity_t target = et.entity(constant);
        return get_mut<First>(target);
    }

    /** Get mutable component value (untyped).
     *
     * @param comp The component to get.
     * @return Pointer to the component value, nullptr if the entity does not
     *         have the component.
     */
    void* try_get_mut(flecs::id_t comp) const {
        return ecs_get_mut_id(world_, id_, comp);
    }

    /** Get a mutable pair (untyped).
     * This operation gets the value for a pair from the entity. If neither the
     * first nor the second part of the pair are components, the operation
     * will fail.
     *
     * @param first The first element of the pair.
     * @param second The second element of the pair.
     */
    void* try_get_mut(flecs::entity_t first, flecs::entity_t second) const {
        return ecs_get_mut_id(world_, id_, ecs_pair(first, second));
    }

    /** Get the second part for a pair.
     * This operation gets the value for a pair from the entity. The first
     * part of the pair should not be a component.
     *
     * @tparam Second the second element of a pair.
     * @param first The first part of the pair.
     */
    template<typename Second>
    Second* try_get_mut_second(flecs::entity_t first) const {
        auto second = _::type<Second>::id(world_);
        ecs_assert( ecs_get_type_info(world_, ecs_pair(first, second)) != NULL,
            ECS_INVALID_PARAMETER, "pair is not a component");
        ecs_assert( ecs_get_type_info(world_, ecs_pair(first, second))->component == second,
            ECS_INVALID_PARAMETER, "type of pair is not Second");
        ecs_assert(_::type<Second>::size() != 0, ECS_INVALID_PARAMETER,
            "operation invalid for empty type");
        return static_cast<Second*>(
            ecs_get_mut_id(world_, id_, ecs_pair(first, second)));
    }

    /** Get the second part for a pair.
     * This operation gets the value for a pair from the entity. The first
     * part of the pair should not be a component.
     *
     * @tparam First The first element of the pair.
     * @tparam Second the second element of a pair.
     */
    template<typename First, typename Second>
    Second* try_get_mut_second() const {
        return get_mut<pair_object<First, Second>>();
    }


    /* get_mut */

    /** Get mutable component value.
     *
     * @tparam T The component to get.
     * @return Pointer to the component value, nullptr if the entity does not
     *         have the component.
     */
    template <typename T, if_t< is_actual<T>::value > = 0>
    T& get_mut() const {
        T* r = try_get_mut<T>();
        ecs_assert(r != nullptr, ECS_INVALID_OPERATION,
            "invalid get_mut: entity does not have component (use try_get_mut)");
        return *r;
    }

    /** Get mutable component value.
     * Overload for when T is not the same as the actual type, which happens
     * when using pair types.
     *
     * @tparam T The component to get.
     * @return Pointer to the component value, nullptr if the entity does not
     *         have the component.
     */
    template <typename T, typename A = actual_type_t<T>,
        if_t< flecs::is_pair<T>::value > = 0>
    A& get_mut() const {
        A* r = try_get_mut<T>();
        ecs_assert(r != nullptr, ECS_INVALID_OPERATION,
            "invalid get_mut: entity does not have component (use try_get_mut)");
        return *r;
    }

    /** Get a mutable pair.
     * This operation gets the value for a pair from the entity.
     *
     * @tparam First The first element of the pair.
     * @tparam Second the second element of a pair.
     */
    template <typename First, typename Second, typename P = pair<First, Second>,
        typename A = actual_type_t<P>, if_not_t< flecs::is_pair<First>::value > = 0>
    A& get_mut() const {
        A* r = try_get_mut<First, Second>();
        ecs_assert(r != nullptr, ECS_INVALID_OPERATION,
            "invalid get_mut: entity does not have component (use try_get_mut)");
        return *r;
    }

    /** Get a mutable pair.
     * This operation gets the value for a pair from the entity.
     *
     * @tparam First The first element of the pair.
     * @param second The second element of the pair.
     */
    template<typename First, typename Second, if_not_t< is_enum<Second>::value> = 0>
    First& get_mut(Second second) const {
        First* r = try_get_mut<First>(second);
        ecs_assert(r != nullptr, ECS_INVALID_OPERATION,
            "invalid get_mut: entity does not have component (use try_get_mut)");
        return *r;
    }

    /** Get a mutable pair.
     * This operation gets the value for a pair from the entity.
     *
     * @tparam First The first element of the pair.
     * @param constant the enum constant.
     */
    template<typename First, typename Second, if_t< is_enum<Second>::value && !std::is_same<First, Second>::value > = 0>
    First& get_mut(Second constant) const {
        const auto& et = enum_type<Second>(this->world_);
        flecs::entity_t target = et.entity(constant);
        return get_mut<First>(target);
    }

    /** Get mutable component value (untyped).
     *
     * @param comp The component to get.
     * @return Pointer to the component value, nullptr if the entity does not
     *         have the component.
     */
    void* get_mut(flecs::id_t comp) const {
        void *r = ecs_get_mut_id(world_, id_, comp);
        ecs_assert(r != nullptr, ECS_INVALID_OPERATION,
            "invalid get_mut: entity does not have component (use try_get_mut)");
        return r;
    }

    /** Get a mutable pair (untyped).
     * This operation gets the value for a pair from the entity. If neither the
     * first nor the second part of the pair are components, the operation
     * will fail.
     *
     * @param first The first element of the pair.
     * @param second The second element of the pair.
     */
    void* get_mut(flecs::entity_t first, flecs::entity_t second) const {
        void *r = ecs_get_mut_id(world_, id_, ecs_pair(first, second));
        ecs_assert(r != nullptr, ECS_INVALID_OPERATION,
            "invalid get_mut: entity does not have component (use try_get_mut)");
        return r;
    }

    /** Get the second part for a pair.
     * This operation gets the value for a pair from the entity. The first
     * part of the pair should not be a component.
     *
     * @tparam Second the second element of a pair.
     * @param first The first part of the pair.
     */
    template<typename Second>
    Second& get_mut_second(flecs::entity_t first) const {
        Second *r = try_get_mut_second<Second>(first);
        ecs_assert(r != nullptr, ECS_INVALID_OPERATION,
            "invalid get_mut: entity does not have component (use try_get_mut)");
        return *r;
    }

    /** Get the second part for a pair.
     * This operation gets the value for a pair from the entity. The first
     * part of the pair should not be a component.
     *
     * @tparam First The first element of the pair.
     * @tparam Second the second element of a pair.
     */
    template<typename First, typename Second>
    Second* get_mut_second() const {
        Second *r = try_get_mut_second<First, Second>();
        ecs_assert(r != nullptr, ECS_INVALID_OPERATION,
            "invalid get_mut: entity does not have component (use try_get_mut)");
        return *r;
    }

    /** Get enum constant for enum relationship. */
    template<typename Enum>
    Enum get_constant() const;

    /** Get target for a given pair.
     * This operation returns the target for a given pair. The optional
     * index can be used to iterate through targets, in case the entity has
     * multiple instances for the same relationship.
     *
     * @tparam First The first element of the pair.
     * @param index The index (0 for the first instance of the relationship).
     */
    template<typename First>
    flecs::entity target(int32_t index = 0) const;

    /** Get target for a given pair.
     * This operation returns the target for a given pair. The optional
     * index can be used to iterate through targets, in case the entity has
     * multiple instances for the same relationship.
     *
     * @param first The first element of the pair for which to retrieve the target.
     * @param index The index (0 for the first instance of the relationship).
     */
    flecs::entity target(flecs::entity_t first, int32_t index = 0) const;

    /** Get the target of a pair for a given relationship id.
     * This operation returns the first entity that has the provided id by following
     * the specified relationship. If the entity itself has the id then entity will
     * be returned. If the id cannot be found on the entity or by following the
     * relationship, the operation will return 0.
     *
     * This operation can be used to lookup, for example, which prefab is providing
     * a component by specifying the IsA pair:
     *
     * @code
     * // Is Position provided by the entity or one of its base entities?
     * ecs_get_target_for_id(world, entity, EcsIsA, ecs_id(Position))
     * @endcode
     *
     * @param relationship The relationship to follow.
     * @param id The id to lookup.
     * @return The entity for which the target has been found.
     */
    flecs::entity target_for(flecs::entity_t relationship, flecs::id_t id) const;

    template <typename T>
    flecs::entity target_for(flecs::entity_t relationship) const;

    template <typename First, typename Second>
    flecs::entity target_for(flecs::entity_t relationship) const;

    /** Get depth for given relationship.
     *
     * @param rel The relationship.
     * @return The depth.
     */
    int32_t depth(flecs::entity_t rel) const {
        return ecs_get_depth(world_, id_, rel);
    }

    /** Get depth for given relationship.
     *
     * @tparam Rel The relationship.
     * @return The depth.
     */
    template<typename Rel>
    int32_t depth() const {
        return this->depth(_::type<Rel>::id(world_));
    }

    /** Get parent of entity.
     * Short for target(flecs::ChildOf).
     *
     * @return The parent of the entity.
     */
    flecs::entity parent() const;

    /** Lookup an entity by name.
     * Lookup an entity in the scope of this entity. The provided path may
     * contain double colons as scope separators, for example: "Foo::Bar".
     *
     * @param path The name of the entity to lookup.
     * @param search_path When false, only the entity's scope is searched.
     * @return The found entity, or entity::null if no entity matched.
     */
    flecs::entity lookup(const char *path, bool search_path = false) const;

    /** Check if entity has the provided entity.
     *
     * @param e The entity to check.
     * @return True if the entity has the provided entity, false otherwise.
     */
    bool has(flecs::id_t e) const {
        return ecs_has_id(world_, id_, e);
    }

    /** Check if entity has the provided component.
     *
     * @tparam T The component to check.
     * @return True if the entity has the provided component, false otherwise.
     */
    template <typename T>
    bool has() const {
        flecs::id_t cid = _::type<T>::id(world_);
        bool result = ecs_has_id(world_, id_, cid);
        if (result) {
            return result;
        }

        if (is_enum<T>::value) {
            return ecs_has_pair(world_, id_, cid, flecs::Wildcard);
        }

        return false;
    }

    /** Check if entity has the provided enum constant.
     *
     * @tparam E The enum type (can be deduced).
     * @param value The enum constant to check.
     * @return True if the entity has the provided constant, false otherwise.
     */
    template <typename E, if_t< is_enum<E>::value > = 0>
    bool has(E value) const {
        auto r = _::type<E>::id(world_);
        auto o = enum_type<E>(world_).entity(value);
        ecs_assert(o, ECS_INVALID_PARAMETER,
            "Constant was not found in Enum reflection data."
            " Did you mean to use has<E>() instead of has(E)?");
        return ecs_has_pair(world_, id_, r, o);
    }

    /** Check if entity has the provided pair.
     *
     * @tparam First The first element of the pair.
     * @tparam Second The second element of the pair.
     * @return True if the entity has the provided component, false otherwise.
     */
    template <typename First, typename Second>
    bool has() const {
        return this->has<First>(_::type<Second>::id(world_));
    }

    /** Check if entity has the provided pair.
     *
     * @tparam First The first element of the pair.
     * @param second The second element of the pair.
     * @return True if the entity has the provided component, false otherwise.
     */
    template<typename First, typename Second, if_not_t< is_enum<Second>::value > = 0>
    bool has(Second second) const {
        auto comp_id = _::type<First>::id(world_);
        return ecs_has_id(world_, id_, ecs_pair(comp_id, second));
    }

    /** Check if entity has the provided pair.
     *
     * @tparam Second The second element of the pair.
     * @param first The first element of the pair.
     * @return True if the entity has the provided component, false otherwise.
     */
    template <typename Second>
    bool has_second(flecs::entity_t first) const {
        return this->has(first, _::type<Second>::id(world_));
    }

    /** Check if entity has the provided pair.
     *
     * @tparam First The first element of the pair.
     * @param value The enum constant.
     * @return True if the entity has the provided component, false otherwise.
     */
    template<typename First, typename E, if_t< is_enum<E>::value && !std::is_same<First, E>::value > = 0>
    bool has(E value) const {
        const auto& et = enum_type<E>(this->world_);
        flecs::entity_t second = et.entity(value);
        return has<First>(second);
    }

    /** Check if entity has the provided pair.
     *
     * @param first The first element of the pair.
     * @param second The second element of the pair.
     * @return True if the entity has the provided component, false otherwise.
     */
    bool has(flecs::id_t first, flecs::id_t second) const {
        return ecs_has_id(world_, id_, ecs_pair(first, second));
    }

    /** Check if entity owns the provided entity.
     * An entity is owned if it is not shared from a base entity.
     *
     * @param e The entity to check.
     * @return True if the entity owns the provided entity, false otherwise.
     */
    bool owns(flecs::id_t e) const {
        return ecs_owns_id(world_, id_, e);
    }

    /** Check if entity owns the provided pair.
     *
     * @tparam First The first element of the pair.
     * @param second The second element of the pair.
     * @return True if the entity owns the provided component, false otherwise.
     */
    template <typename First>
    bool owns(flecs::id_t second) const {
        auto comp_id = _::type<First>::id(world_);
        return owns(ecs_pair(comp_id, second));
    }

    /** Check if entity owns the provided pair.
     *
     * @param first The first element of the pair.
     * @param second The second element of the pair.
     * @return True if the entity owns the provided component, false otherwise.
     */
    bool owns(flecs::id_t first, flecs::id_t second) const {
        return owns(ecs_pair(first, second));
    }

    /** Check if entity owns the provided component.
     * An component is owned if it is not shared from a base entity.
     *
     * @tparam T The component to check.
     * @return True if the entity owns the provided component, false otherwise.
     */
    template <typename T>
    bool owns() const {
        return owns(_::type<T>::id(world_));
    }

    /** Check if entity owns the provided pair.
     * An pair is owned if it is not shared from a base entity.
     *
     * @tparam First The first element of the pair.
     * @tparam Second The second element of the pair.
     * @return True if the entity owns the provided pair, false otherwise.
     */
    template <typename First, typename Second>
    bool owns() const {
        return owns(
            _::type<First>::id(world_),
            _::type<Second>::id(world_));
    }

    /** Test if id is enabled.
     *
     * @param id The id to test.
     * @return True if enabled, false if not.
     */
    bool enabled(flecs::id_t id) const {
        return ecs_is_enabled_id(world_, id_, id);
    }

    /** Test if component is enabled.
     *
     * @tparam T The component to test.
     * @return True if enabled, false if not.
     */
    template<typename T>
    bool enabled() const {
        return this->enabled(_::type<T>::id(world_));
    }

    /** Test if pair is enabled.
     *
     * @param first The first element of the pair.
     * @param second The second element of the pair.
     * @return True if enabled, false if not.
     */
    bool enabled(flecs::id_t first, flecs::id_t second) const {
        return this->enabled(ecs_pair(first, second));
    }

    /** Test if pair is enabled.
     *
     * @tparam First The first element of the pair.
     * @param second The second element of the pair.
     * @return True if enabled, false if not.
     */
    template <typename First>
    bool enabled(flecs::id_t second) const {
        return this->enabled(_::type<First>::id(world_), second);
    }

    /** Test if pair is enabled.
     *
     * @tparam First The first element of the pair.
     * @tparam Second The second element of the pair.
     * @return True if enabled, false if not.
     */
    template <typename First, typename Second>
    bool enabled() const {
        return this->enabled<First>(_::type<Second>::id(world_));
    }

    flecs::entity clone(bool clone_value = true, flecs::entity_t dst_id = 0) const;

    /** Return mutable entity handle for current stage
     * When an entity handle created from the world is used while the world is
     * in staged mode, it will only allow for readonly operations since
     * structural changes are not allowed on the world while in staged mode.
     *
     * To do mutations on the entity, this operation provides a handle to the
     * entity that uses the stage instead of the actual world.
     *
     * Note that staged entity handles should never be stored persistently, in
     * components or elsewhere. An entity handle should always point to the
     * main world.
     *
     * Also note that this operation is not necessary when doing mutations on an
     * entity outside of a system. It is allowed to do entity operations
     * directly on the world, as long as the world is not in staged mode.
     *
     * @param stage The current stage.
     * @return An entity handle that allows for mutations in the current stage.
     */
    flecs::entity mut(const flecs::world& stage) const;

    /** Same as mut(world), but for iterator.
     * This operation allows for the construction of a mutable entity handle
     * from an iterator.
     *
     * @param it An iterator that contains a reference to the world or stage.
     * @return An entity handle that allows for mutations in the current stage.
     */
    flecs::entity mut(const flecs::iter& it) const;

    /** Same as mut(world), but for entity.
     * This operation allows for the construction of a mutable entity handle
     * from another entity. This is useful in each() functions, which only
     * provide a handle to the entity being iterated over.
     *
     * @param e Another mutable entity.
     * @return An entity handle that allows for mutations in the current stage.
     */
    flecs::entity mut(const flecs::entity_view& e) const;

#   ifdef FLECS_JSON
/**
 * @file addons/cpp/mixins/json/entity_view.inl
 * @brief JSON entity mixin.
 */

/** Serialize entity to JSON.
 *
 * @memberof flecs::entity_view
 * @ingroup cpp_addons_json
 */
flecs::string to_json(const flecs::entity_to_json_desc_t *desc = nullptr) const {
    char *json = ecs_entity_to_json(world_, id_, desc);
    return flecs::string(json);
}

#   endif
#   ifdef FLECS_DOC
/**
 * @file addons/cpp/mixins/doc/entity_view.inl
 * @brief Doc entity view mixin.
 */

/** Get human readable name.
 *
 * @see ecs_doc_get_name()
 * @see flecs::doc::get_name()
 * @see flecs::entity_builder::set_doc_name()
 *
 * @memberof flecs::entity_view
 * @ingroup cpp_addons_doc
 */
const char* doc_name() const {
    return ecs_doc_get_name(world_, id_);
}

/** Get brief description.
 *
 * @see ecs_doc_get_brief()
 * @see flecs::doc::get_brief()
 * @see flecs::entity_builder::set_doc_brief()
 *
 * @memberof flecs::entity_view
 * @ingroup cpp_addons_doc
 */
const char* doc_brief() const {
    return ecs_doc_get_brief(world_, id_);
}

/** Get detailed description.
 *
 * @see ecs_doc_get_detail()
 * @see flecs::doc::get_detail()
 * @see flecs::entity_builder::set_doc_detail()
 *
 * @memberof flecs::entity_view
 * @ingroup cpp_addons_doc
 */
const char* doc_detail() const {
    return ecs_doc_get_detail(world_, id_);
}

/** Get link to external documentation.
 *
 * @see ecs_doc_get_link()
 * @see flecs::doc::get_link()
 * @see flecs::entity_builder::set_doc_link()
 *
 * @memberof flecs::entity_view
 * @ingroup cpp_addons_doc
 */
const char* doc_link() const {
    return ecs_doc_get_link(world_, id_);
}

/** Get color.
 *
 * @see ecs_doc_get_color()
 * @see flecs::doc::get_color()
 * @see flecs::entity_builder::set_doc_color()
 *
 * @memberof flecs::entity_view
 * @ingroup cpp_addons_doc
 */
const char* doc_color() const {
    return ecs_doc_get_color(world_, id_);
}

/** Get UUID.
 *
 * @see ecs_doc_get_uuid()
 * @see flecs::doc::get_uuid()
 * @see flecs::entity_builder::set_doc_uuid()
 *
 * @memberof flecs::entity_view
 * @ingroup cpp_addons_doc
 */
const char* doc_uuid() const {
    return ecs_doc_get_uuid(world_, id_);
}

#   endif
#   ifdef FLECS_ALERTS
/**
 * @file addons/cpp/mixins/alerts/entity_view.inl
 * @brief Alerts entity mixin.
 */

/** Return number of alerts for entity.
 *
 * @memberof flecs::entity_view
 * @ingroup cpp_addons_alerts
 */
int32_t alert_count(flecs::entity_t alert = 0) const {
    return ecs_get_alert_count(world_, id_, alert);
}

#   endif

/**
 * @file addons/cpp/mixins/enum/entity_view.inl
 * @brief Enum entity view mixin.
 */

/** Convert entity to enum constant.
 *
 * @memberof flecs::entity_view
 * @ingroup cpp_entities
 */
template <typename E>
E to_constant() const;


/**
 * @file addons/cpp/mixins/event/entity_view.inl
 * @brief Event entity mixin.
 */

/** Emit event for entity.
 *
 * @memberof flecs::entity_view
 *
 * @param evt The event to emit.
 */
void emit(flecs::entity_t evt) const {
    flecs::world(world_)
        .event(evt)
        .entity(id_)
        .emit();
}

/** Emit event for entity.
 *
 * @memberof flecs::entity_view
 *
 * @param evt The event to emit.
 */
void emit(flecs::entity evt) const;

/** Emit event for entity.
 *
 * @memberof flecs::entity_view
 *
 * @tparam Evt The event to emit.
 */
template <typename Evt, if_t<is_empty<Evt>::value> = 0>
void emit() const {
    this->emit(_::type<Evt>::id(world_));
}

/** Emit event with payload for entity.
 *
 * @memberof flecs::entity_view
 *
 * @tparam Evt The event to emit.
 */
template <typename Evt, if_not_t<is_empty<Evt>::value> = 0>
void emit(const Evt& payload) const {
    flecs::world(world_)
        .event(_::type<Evt>::id(world_))
        .entity(id_)
        .ctx(&payload)
        .emit();
}


/** Enqueue event for entity.
 *
 * @memberof flecs::entity_view
 *
 * @param evt The event to enqueue.
 */
void enqueue(flecs::entity_t evt) const {
    flecs::world(world_)
        .event(evt)
        .entity(id_)
        .enqueue();
}

/** Enqueue event for entity.
 *
 * @memberof flecs::entity_view
 *
 * @param evt The event to enqueue.
 */
void enqueue(flecs::entity evt) const;

/** Enqueue event for entity.
 *
 * @memberof flecs::entity_view
 *
 * @tparam Evt The event to enqueue.
 */
template <typename Evt, if_t<is_empty<Evt>::value> = 0>
void enqueue() const {
    this->enqueue(_::type<Evt>::id(world_));
}

/** Enqueue event with payload for entity.
 *
 * @memberof flecs::entity_view
 *
 * @tparam Evt The event to enqueue.
 */
template <typename Evt, if_not_t<is_empty<Evt>::value> = 0>
void enqueue(const Evt& payload) const {
    flecs::world(world_)
        .event(_::type<Evt>::id(world_))
        .entity(id_)
        .ctx(&payload)
        .enqueue();
}


private:
    flecs::entity set_stage(world_t *stage);
};

}

/** @} */

/**
 * @file addons/cpp/mixins/entity/builder.hpp
 * @brief Entity builder.
 */

#pragma once

export namespace flecs
{

/** Entity builder.
 * @ingroup cpp_entities
 */
template <typename Self>
struct entity_builder : entity_view {

    using entity_view::entity_view;

    /** Add a component to an entity.
     * To ensure the component is initialized, it should have a constructor.
     *
     * @tparam T the component type to add.
     */
    template <typename T>
    const Self& add() const  {
        flecs_static_assert(is_flecs_constructible<T>::value,
            "cannot default construct type: add T::T() or use emplace<T>()");
        ecs_add_id(this->world_, this->id_, _::type<T>::id(this->world_));
        return to_base();
    }

     /** Add pair for enum constant.
     * This operation will add a pair to the entity where the first element is
     * the enumeration type, and the second element the enumeration constant.
     *
     * The operation may be used with regular (C style) enumerations as well as
     * enum classes.
     *
     * @param value The enumeration value.
     */
    template <typename E, if_t< is_enum<E>::value > = 0>
    const Self& add(E value) const  {
        flecs::entity_t first = _::type<E>::id(this->world_);
        const auto& et = enum_type<E>(this->world_);
        flecs::entity_t second = et.entity(value);

        ecs_assert(second, ECS_INVALID_PARAMETER, "Component was not found in reflection data.");
        return this->add(first, second);
    }

    /** Add an entity to an entity.
     * Add an entity to the entity. This is typically used for tagging.
     *
     * @param component The component to add.
     */
    const Self& add(id_t component) const  {
        ecs_add_id(this->world_, this->id_, component);
        return to_base();
    }

    /** Add a pair.
     * This operation adds a pair to the entity.
     *
     * @param first The first element of the pair.
     * @param second The second element of the pair.
     */
    const Self& add(entity_t first, entity_t second) const  {
        ecs_add_pair(this->world_, this->id_, first, second);
        return to_base();
    }

    /** Add a pair.
     * This operation adds a pair to the entity.
     *
     * @tparam First The first element of the pair
     * @tparam Second The second element of the pair
     */
    template<typename First, typename Second>
    const Self& add() const  {
        return this->add<First>(_::type<Second>::id(this->world_));
    }

    /** Add a pair.
     * This operation adds a pair to the entity.
     *
     * @tparam First The first element of the pair
     * @param second The second element of the pair.
     */
    template<typename First, typename Second, if_not_t< is_enum<Second>::value > = 0>
    const Self& add(Second second) const  {
        flecs_static_assert(is_flecs_constructible<First>::value,
            "cannot default construct type: add T::T() or use emplace<T>()");
        return this->add(_::type<First>::id(this->world_), second);
    }

    /** Add a pair.
     * This operation adds a pair to the entity that consists out of a tag
     * combined with an enum constant.
     *
     * @tparam First The first element of the pair
     * @param constant the enum constant.
     */
    template<typename First, typename Second, if_t< is_enum<Second>::value && !std::is_same<First, Second>::value > = 0>
    const Self& add(Second constant) const  {
        flecs_static_assert(is_flecs_constructible<First>::value,
            "cannot default construct type: add T::T() or use emplace<T>()");
        const auto& et = enum_type<Second>(this->world_);
        return this->add<First>(et.entity(constant));
    }

    /** Add a pair.
     * This operation adds a pair to the entity.
     *
     * @param first The first element of the pair
     * @tparam Second The second element of the pair
     */
    template<typename Second>
    const Self& add_second(flecs::entity_t first) const  {
        return this->add(first, _::type<Second>::id(this->world_));
    }

    /** Conditional add.
     * This operation adds if condition is true, removes if condition is false.
     *
     * @param cond The condition to evaluate.
     * @param component The component to add.
     */
    const Self& add_if(bool cond, flecs::id_t component) const  {
        if (cond) {
            return this->add(component);
        } else {
            return this->remove(component);
        }
    }

    /** Conditional add.
     * This operation adds if condition is true, removes if condition is false.
     *
     * @tparam T The component to add.
     * @param cond The condition to evaluate.
     */
    template <typename T>
    const Self& add_if(bool cond) const  {
        if (cond) {
            return this->add<T>();
        } else {
            return this->remove<T>();
        }
    }

    /** Conditional add.
     * This operation adds if condition is true, removes if condition is false.
     *
     * @param cond The condition to evaluate.
     * @param first The first element of the pair.
     * @param second The second element of the pair.
     */
    const Self& add_if(bool cond, flecs::entity_t first, flecs::entity_t second) const  {
        if (cond) {
            return this->add(first, second);
        } else {
            /* If second is 0 or if relationship is exclusive, use wildcard for
             * second which will remove all instances of the relationship.
             * Replacing 0 with Wildcard will make it possible to use the second
             * as the condition. */
            if (!second || ecs_has_id(this->world_, first, flecs::Exclusive)) {
                second = flecs::Wildcard;
            }
            return this->remove(first, second);
        }
    }

    /** Conditional add.
     * This operation adds if condition is true, removes if condition is false.
     *
     * @tparam First The first element of the pair
     * @param cond The condition to evaluate.
     * @param second The second element of the pair.
     */
    template <typename First>
    const Self& add_if(bool cond, flecs::entity_t second) const  {
        return this->add_if(cond, _::type<First>::id(this->world_), second);
    }

    /** Conditional add.
     * This operation adds if condition is true, removes if condition is false.
     *
     * @tparam First The first element of the pair
     * @tparam Second The second element of the pair
     * @param cond The condition to evaluate.
     */
    template <typename First, typename Second>
    const Self& add_if(bool cond) const  {
        return this->add_if<First>(cond, _::type<Second>::id(this->world_));
    }

    /** Conditional add.
     * This operation adds if condition is true, removes if condition is false.
     *
     * @param cond The condition to evaluate.
     * @param constant The enumeration constant.
     */
    template <typename E, if_t< is_enum<E>::value > = 0>
    const Self& add_if(bool cond, E constant) const  {
        const auto& et = enum_type<E>(this->world_);
        return this->add_if<E>(cond, et.entity(constant));
    }

    /** Shortcut for `add(IsA, entity)`.
     *
     * @param second The second element of the pair.
     */
    const Self& is_a(entity_t second) const  {
        return this->add(flecs::IsA, second);
    }

    /** Shortcut for `add(IsA, entity)`.
     *
     * @tparam T the type associated with the entity.
     */
    template <typename T>
    const Self& is_a() const  {
        return this->add(flecs::IsA, _::type<T>::id(this->world_));
    }

    /** Shortcut for `add(ChildOf, entity)`.
     *
     * @param second The second element of the pair.
     */
    const Self& child_of(entity_t second) const  {
        return this->add(flecs::ChildOf, second);
    }

    /** Shortcut for `add(DependsOn, entity)`.
     *
     * @param second The second element of the pair.
     */
    const Self& depends_on(entity_t second) const  {
        return this->add(flecs::DependsOn, second);
    }

     /** Shortcut for `add(DependsOn, entity)`.
     *
     * @param second The second element of the pair.
     */
    template <typename E, if_t<is_enum<E>::value> = 0>
    const Self& depends_on(E second) const {
        const auto& et = enum_type<E>(this->world_);
        flecs::entity_t target = et.entity(second);
        return depends_on(target);
    }

    /** Shortcut for `add(SlotOf, entity)`.
     *
     * @param second The second element of the pair.
     */
    const Self& slot_of(entity_t second) const  {
        return this->add(flecs::SlotOf, second);
    }

    /** Shortcut for `add(SlotOf, target(ChildOf))`.
     */
    const Self& slot() const  {
        ecs_check(ecs_get_target(world_, id_, flecs::ChildOf, 0),
            ECS_INVALID_PARAMETER, "add ChildOf pair before using slot()");
        return this->slot_of(this->target(flecs::ChildOf));
    error:
        return to_base();
    }

    /** Shortcut for `add(ChildOf, entity)`.
     *
     * @tparam T the type associated with the entity.
     */
    template <typename T>
    const Self& child_of() const  {
        return this->child_of(_::type<T>::id(this->world_));
    }

    /** Shortcut for `add(DependsOn, entity)`.
     *
     * @tparam T the type associated with the entity.
     */
    template <typename T>
    const Self& depends_on() const  {
        return this->depends_on(_::type<T>::id(this->world_));
    }

    /** Shortcut for `add(SlotOf, entity)`.
     *
     * @tparam T the type associated with the entity.
     */
    template <typename T>
    const Self& slot_of() const  {
        return this->slot_of(_::type<T>::id(this->world_));
    }

    /** Remove a component from an entity.
     *
     * @tparam T the type of the component to remove.
     */
    template <typename T>
    const Self& remove() const {
        ecs_remove_id(this->world_, this->id_, _::type<T>::id(this->world_));
        return to_base();
    }

    /** Remove an entity from an entity.
     *
     * @param entity The entity to remove.
     */
    const Self& remove(entity_t entity) const  {
        ecs_remove_id(this->world_, this->id_, entity);
        return to_base();
    }

    /** Remove a pair.
     * This operation removes a pair from the entity.
     *
     * @param first The first element of the pair.
     * @param second The second element of the pair.
     */
    const Self& remove(entity_t first, entity_t second) const  {
        ecs_remove_pair(this->world_, this->id_, first, second);
        return to_base();
    }

    /** Removes a pair.
     * This operation removes a pair from the entity.
     *
     * @tparam First The first element of the pair
     * @tparam Second The second element of the pair
     */
    template<typename First, typename Second>
    const Self& remove() const  {
        return this->remove<First>(_::type<Second>::id(this->world_));
    }

    /** Remove a pair.
     * This operation removes the pair from the entity.
     *
     * @tparam First The first element of the pair
     * @param second The second element of the pair.
     */
    template<typename First, typename Second, if_not_t< is_enum<Second>::value > = 0>
    const Self& remove(Second second) const  {
        return this->remove(_::type<First>::id(this->world_), second);
    }

    /** Removes a pair.
     * This operation removes a pair from the entity.
     *
     * @tparam Second The second element of the pair
     * @param first The first element of the pair
     */
    template<typename Second>
    const Self& remove_second(flecs::entity_t first) const  {
        return this->remove(first, _::type<Second>::id(this->world_));
    }

    /** Remove a pair.
     * This operation removes the pair from the entity.
     *
     * @tparam First The first element of the pair
     * @param constant the enum constant.
     */
    template<typename First, typename Second, if_t< is_enum<Second>::value > = 0>
    const Self& remove(Second constant) const  {
        const auto& et = enum_type<Second>(this->world_);
        flecs::entity_t second = et.entity(constant);
        return this->remove<First>(second);
    }

    /** Mark id for auto-overriding.
     * When an entity inherits from a base entity (using the `IsA` relationship)
     * any ids marked for auto-overriding on the base will be overridden
     * automatically by the entity.
     *
     * @param id The id to mark for overriding.
     */
    const Self& auto_override(flecs::id_t id) const  {
        return this->add(ECS_AUTO_OVERRIDE | id);
    }

    /** Mark pair for auto-overriding.
     * @see auto_override(flecs::id_t) const
     *
     * @param first The first element of the pair.
     * @param second The second element of the pair.
     */
    const Self& auto_override(flecs::entity_t first, flecs::entity_t second) const  {
        return this->auto_override(ecs_pair(first, second));
    }

    /** Mark component for auto-overriding.
     * @see auto_override(flecs::id_t) const
     *
     * @tparam T The component to mark for overriding.
     */
    template <typename T>
    const Self& auto_override() const  {
        return this->auto_override(_::type<T>::id(this->world_));
    }

    /** Mark pair for auto-overriding.
     * @see auto_override(flecs::id_t) const
     *
     * @tparam First The first element of the pair.
     * @param second The second element of the pair.
     */
    template <typename First>
    const Self& auto_override(flecs::entity_t second) const  {
        return this->auto_override(_::type<First>::id(this->world_), second);
    }

    /** Mark pair for auto-overriding.
     * @see auto_override(flecs::id_t) const
     *
     * @tparam First The first element of the pair.
     * @tparam Second The second element of the pair.
     */
    template <typename First, typename Second>
    const Self& auto_override() const  {
        return this->auto_override<First>(_::type<Second>::id(this->world_));
    }

    /** Set component, mark component for auto-overriding.
     * @see auto_override(flecs::id_t) const
     *
     * @tparam T The component to set and for which to add the OVERRIDE flag
     * @param val The value to set.
     */
    template <typename T>
    const Self& set_auto_override(const T& val) const  {
        this->auto_override<T>();
        return this->set<T>(val);
    }

    /** Set component, mark component for auto-overriding.
     * @see auto_override(flecs::id_t) const
     *
     * @tparam T The component to set and for which to add the OVERRIDE flag
     * @param val The value to set.
     */
    template <typename T>
    const Self& set_auto_override(T&& val) const  {
        this->auto_override<T>();
        return this->set<T>(FLECS_FWD(val));
    }

    /** Set pair, mark component for auto-overriding.
     * @see auto_override(flecs::id_t) const
     *
     * @tparam First The first element of the pair.
     * @param second The second element of the pair.
     * @param val The value to set.
     */
    template <typename First>
    const Self& set_auto_override(flecs::entity_t second, const First& val) const  {
        this->auto_override<First>(second);
        return this->set<First>(second, val);
    }

    /** Set pair, mark component for auto-overriding.
     * @see auto_override(flecs::id_t) const
     *
     * @tparam First The first element of the pair.
     * @param second The second element of the pair.
     * @param val The value to set.
     */
    template <typename First>
    const Self& set_auto_override(flecs::entity_t second, First&& val) const  {
        this->auto_override<First>(second);
        return this->set<First>(second, FLECS_FWD(val));
    }

    /** Set component, mark component for auto-overriding.
     * @see auto_override(flecs::id_t) const
     *
     * @tparam First The first element of the pair.
     * @tparam Second The second element of the pair.
     * @param val The value to set.
     */
    template <typename First, typename Second, typename P = pair<First, Second>,
        typename A = actual_type_t<P>, if_not_t< flecs::is_pair<First>::value> = 0>
    const Self& set_auto_override(const A& val) const  {
        this->auto_override<First, Second>();
        return this->set<First, Second>(val);
    }

    /** Set component, mark component for auto-overriding.
     * @see auto_override(flecs::id_t) const
     *
     * @tparam First The first element of the pair.
     * @tparam Second The second element of the pair.
     * @param val The value to set.
     */
    template <typename First, typename Second, typename P = pair<First, Second>,
        typename A = actual_type_t<P>, if_not_t< flecs::is_pair<First>::value> = 0>
    const Self& set_auto_override(A&& val) const  {
        this->auto_override<First, Second>();
        return this->set<First, Second>(FLECS_FWD(val));
    }

    /** Emplace component, mark component for auto-overriding.
     * @see auto_override(flecs::id_t) const
     *
     * @tparam T The component to emplace and override.
     * @param args The arguments to pass to the constructor of `T`.
     */
    template <typename T, typename ... Args>
    const Self& emplace_auto_override(Args&&... args) const  {
        this->auto_override<T>();

        flecs::emplace<T>(this->world_, this->id_,
            _::type<T>::id(this->world_), FLECS_FWD(args)...);

        return to_base();
    }

    /** Emplace pair, mark pair for auto-overriding.
     * @see auto_override(flecs::id_t) const
     *
     * @tparam First The first element of the pair to emplace and override.
     * @tparam Second The second element of the pair to emplace and override.
     * @param args The arguments to pass to the constructor of `Second`.
     */
    template <typename First, typename Second, typename P = pair<First, Second>,
        typename A = actual_type_t<P>, if_not_t< flecs::is_pair<First>::value> = 0,
            typename ... Args>
    const Self& emplace_auto_override(Args&&... args) const  {
        this->auto_override<First, Second>();

        flecs::emplace<A>(this->world_, this->id_,
            ecs_pair(_::type<First>::id(this->world_),
                _::type<Second>::id(this->world_)),
            FLECS_FWD(args)...);

        return to_base();
    }

    /** Enable an entity.
     * Enabled entities are matched with systems and can be searched with
     * queries.
     */
    const Self& enable() const  {
        ecs_enable(this->world_, this->id_, true);
        return to_base();
    }

    /** Disable an entity.
     * Disabled entities are not matched with systems and cannot be searched
     * with queries, unless explicitly specified in the query expression.
     */
    const Self& disable() const  {
        ecs_enable(this->world_, this->id_, false);
        return to_base();
    }

    /** Enable an id.
     * This sets the enabled bit for this component. If this is the first time
     * the component is enabled or disabled, the bitset is added.
     *
     * @param id The id to enable.
     * @param toggle True to enable, false to disable (default = true).
     *
     * @see ecs_enable_id()
     */
    const Self& enable(flecs::id_t id, bool toggle = true) const  {
        ecs_enable_id(this->world_, this->id_, id, toggle);
        return to_base();
    }

    /** Enable a component.
     * @see enable(flecs::id_t) const
     *
     * @tparam T The component to enable.
     */
    template<typename T>
    const Self& enable() const  {
        return this->enable(_::type<T>::id(this->world_));
    }

    /** Enable a pair.
     * @see enable(flecs::id_t) const
     *
     * @param first The first element of the pair.
     * @param second The second element of the pair.
     */
    const Self& enable(flecs::id_t first, flecs::id_t second) const  {
        return this->enable(ecs_pair(first, second));
    }

    /** Enable a pair.
     * @see enable(flecs::id_t) const
     *
     * @tparam First The first element of the pair.
     * @param second The second element of the pair.
     */
    template<typename First>
    const Self& enable(flecs::id_t second) const  {
        return this->enable(_::type<First>::id(world_), second);
    }

    /** Enable a pair.
     * @see enable(flecs::id_t) const
     *
     * @tparam First The first element of the pair.
     * @tparam Second The second element of the pair.
     */
    template<typename First, typename Second>
    const Self& enable() const  {
        return this->enable<First>(_::type<Second>::id(world_));
    }

    /** Disable an id.
     * This sets the enabled bit for this id. If this is the first time
     * the id is enabled or disabled, the bitset is added.
     *
     * @param id The id to disable.
     *
     * @see ecs_enable_id()
     * @see enable(flecs::id_t) const
     */
    const Self& disable(flecs::id_t id) const  {
        return this->enable(id, false);
    }

    /** Disable a component.
     * @see disable(flecs::id_t) const
     *
     * @tparam T The component to enable.
     */
    template<typename T>
    const Self& disable() const  {
        return this->disable(_::type<T>::id(world_));
    }

    /** Disable a pair.
     * @see disable(flecs::id_t) const
     *
     * @param first The first element of the pair.
     * @param second The second element of the pair.
     */
    const Self& disable(flecs::id_t first, flecs::id_t second) const  {
        return this->disable(ecs_pair(first, second));
    }

    /** Disable a pair.
     * @see disable(flecs::id_t) const
     *
     * @tparam First The first element of the pair.
     * @param second The second element of the pair.
     */
    template<typename First>
    const Self& disable(flecs::id_t second) const  {
        return this->disable(_::type<First>::id(world_), second);
    }

    /** Disable a pair.
     * @see disable(flecs::id_t) const
     *
     * @tparam First The first element of the pair.
     * @tparam Second The second element of the pair.
     */
    template<typename First, typename Second>
    const Self& disable() const  {
        return this->disable<First>(_::type<Second>::id(world_));
    }

    const Self& set_ptr(entity_t comp, size_t size, const void *ptr) const  {
        ecs_set_id(this->world_, this->id_, comp, size, ptr);
        return to_base();
    }

    const Self& set_ptr(entity_t comp, const void *ptr) const  {
        const flecs::Component *cptr = ecs_get(
            this->world_, comp, EcsComponent);

        /* Can't set if it's not a component */
        ecs_assert(cptr != NULL, ECS_INVALID_PARAMETER, NULL);

        return set_ptr(comp, cptr->size, ptr);
    }

    /** Set a component for an entity.
     * This operation sets the component value. If the entity did not yet have
     * the component it will be added.
     *
     * @tparam T The component.
     * @param value The value to set.
     */
    template<typename T, if_t<is_actual<T>::value> = 0 >
    const Self& set(T&& value) const  {
        flecs::set<T>(this->world_, this->id_, FLECS_FWD(value));
        return to_base();
    }

    /** Set a component for an entity.
     * This operation sets the component value. If the entity did not yet have
     * the component it will be added.
     *
     * @tparam T The component.
     * @param value The value to set.
     */
    template<typename T, if_t<is_actual<T>::value > = 0>
    const Self& set(const T& value) const  {
        flecs::set<T>(this->world_, this->id_, value);
        return to_base();
    }

    /** Set a component for an entity.
     * This operation sets the component value. If the entity did not yet have
     * the component it will be added.
     *
     * @tparam T The component.
     * @param value The value to set.
     */
    template<typename T, typename A = actual_type_t<T>, if_not_t<
        is_actual<T>::value > = 0>
    const Self& set(A&& value) const  {
        flecs::set<T>(this->world_, this->id_, FLECS_FWD(value));
        return to_base();
    }

    /** Set a component for an entity.
     * This operation sets the component value. If the entity did not yet have
     * the component it will be added.
     *
     * @tparam T The component.
     * @param value The value to set.
     */
    template<typename T, typename A = actual_type_t<T>, if_not_t<
        is_actual<T>::value > = 0>
    const Self& set(const A& value) const  {
        flecs::set<T>(this->world_, this->id_, value);
        return to_base();
    }

    /** Set a pair for an entity.
     * This operation sets the pair value, and uses First as type. If the
     * entity did not yet have the pair, it will be added.
     *
     * @tparam First The first element of the pair.
     * @tparam Second The second element of the pair
     * @param value The value to set.
     */
    template <typename First, typename Second, typename P = pair<First, Second>,
        typename A = actual_type_t<P>, if_not_t< flecs::is_pair<First>::value> = 0>
    const Self& set(A&& value) const  {
        flecs::set<P>(this->world_, this->id_, FLECS_FWD(value));
        return to_base();
    }

    /** Set a pair for an entity.
     * This operation sets the pair value, and uses First as type. If the
     * entity did not yet have the pair, it will be added.
     *
     * @tparam First The first element of the pair.
     * @tparam Second The second element of the pair
     * @param value The value to set.
     */
    template <typename First, typename Second, typename P = pair<First, Second>,
        typename A = actual_type_t<P>, if_not_t< flecs::is_pair<First>::value> = 0>
    const Self& set(const A& value) const  {
        flecs::set<P>(this->world_, this->id_, value);
        return to_base();
    }

    /** Set a pair for an entity.
     * This operation sets the pair value, and uses First as type. If the
     * entity did not yet have the pair, it will be added.
     *
     * @tparam First The first element of the pair.
     * @param second The second element of the pair.
     * @param value The value to set.
     */
    template <typename First, typename Second, if_not_t< is_enum<Second>::value > = 0>
    const Self& set(Second second, const First& value) const  {
        auto first = _::type<First>::id(this->world_);
        flecs::set(this->world_, this->id_, value,
            ecs_pair(first, second));
        return to_base();
    }

    /** Set a pair for an entity.
     * This operation sets the pair value, and uses First as type. If the
     * entity did not yet have the pair, it will be added.
     *
     * @tparam First The first element of the pair.
     * @param second The second element of the pair.
     * @param value The value to set.
     */
    template <typename First, typename Second, if_not_t< is_enum<Second>::value > = 0>
    const Self& set(Second second, First&& value) const  {
        auto first = _::type<First>::id(this->world_);
        flecs::set(this->world_, this->id_, FLECS_FWD(value),
            ecs_pair(first, second));
        return to_base();
    }

    /** Set a pair for an entity.
     * This operation sets the pair value, and uses First as type. If the
     * entity did not yet have the pair, it will be added.
     *
     * @tparam First The first element of the pair.
     * @param constant The enum constant.
     * @param value The value to set.
     */
    template <typename First, typename Second, if_t< is_enum<Second>::value > = 0>
    const Self& set(Second constant, const First& value) const  {
        const auto& et = enum_type<Second>(this->world_);
        flecs::entity_t second = et.entity(constant);
        return set<First>(second, value);
    }

    /** Set a pair for an entity.
     * This operation sets the pair value, and uses Second as type. If the
     * entity did not yet have the pair, it will be added.
     *
     * @tparam Second The second element of the pair
     * @param first The first element of the pair.
     * @param value The value to set.
     */
    template <typename Second>
    const Self& set_second(entity_t first, const Second& value) const  {
        auto second = _::type<Second>::id(this->world_);
        ecs_assert( ecs_get_type_info(world_, ecs_pair(first, second)) != NULL,
            ECS_INVALID_PARAMETER, "pair is not a component");
        ecs_assert( ecs_get_type_info(world_, ecs_pair(first, second))->component == second,
            ECS_INVALID_PARAMETER, "type of pair is not Second");
        flecs::set(this->world_, this->id_, value,
            ecs_pair(first, second));
        return to_base();
    }

    /** Set a pair for an entity.
     * This operation sets the pair value, and uses Second as type. If the
     * entity did not yet have the pair, it will be added.
     *
     * @tparam Second The second element of the pair
     * @param first The first element of the pair.
     * @param value The value to set.
     */
    template <typename Second>
    const Self& set_second(entity_t first, Second&& value) const  {
        auto second = _::type<Second>::id(this->world_);
        ecs_assert( ecs_get_type_info(world_, ecs_pair(first, second)) != NULL,
            ECS_INVALID_PARAMETER, "pair is not a component");
        ecs_assert( ecs_get_type_info(world_, ecs_pair(first, second))->component == second,
            ECS_INVALID_PARAMETER, "type of pair is not Second");
        flecs::set(this->world_, this->id_, FLECS_FWD(value),
            ecs_pair(first, second));
        return to_base();
    }

    /** Set a pair for an entity.
     * This operation sets the pair value, and uses Second as type. If the
     * entity did not yet have the pair, it will be added.
     *
     * @tparam First The first element of the pair.
     * @tparam Second The second element of the pair
     * @param value The value to set.
     */
    template <typename First, typename Second>
    const Self& set_second(const Second& value) const  {
        flecs::set<pair_object<First, Second>>(this->world_, this->id_, value);
        return to_base();
    }


    /** Assign a component for an entity.
     * This operation sets the component value. If the entity did not yet have
     * the component the operation will panic.
     *
     * @tparam T The component.
     * @param value The value to set.
     */
    template<typename T, if_t<is_actual<T>::value> = 0 >
    const Self& assign(T&& value) const  {
        flecs::assign<T>(this->world_, this->id_, FLECS_FWD(value));
        return to_base();
    }

    /** Assign a component for an entity.
     * This operation sets the component value. If the entity did not yet have
     * the component the operation will panic.
     *
     * @tparam T The component.
     * @param value The value to set.
     */
    template<typename T, if_t<is_actual<T>::value > = 0>
    const Self& assign(const T& value) const  {
        flecs::assign<T>(this->world_, this->id_, value);
        return to_base();
    }

    /** Assign a component for an entity.
     * This operation sets the component value. If the entity did not yet have
     * the component the operation will panic.
     *
     * @tparam T The component.
     * @param value The value to set.
     */
    template<typename T, typename A = actual_type_t<T>, if_not_t<
        is_actual<T>::value > = 0>
    const Self& assign(A&& value) const  {
        flecs::assign<T>(this->world_, this->id_, FLECS_FWD(value));
        return to_base();
    }

    /** Assign a component for an entity.
     * This operation sets the component value. If the entity did not yet have
     * the component the operation will panic.
     *
     * @tparam T The component.
     * @param value The value to set.
     */
    template<typename T, typename A = actual_type_t<T>, if_not_t<
        is_actual<T>::value > = 0>
    const Self& assign(const A& value) const  {
        flecs::assign<T>(this->world_, this->id_, value);
        return to_base();
    }

    /** Assign a pair for an entity.
     * This operation sets the pair value, and uses First as type. If the
     * entity did not yet have the component the operation will panic.
     *
     * @tparam First The first element of the pair.
     * @tparam Second The second element of the pair
     * @param value The value to set.
     */
    template <typename First, typename Second, typename P = pair<First, Second>,
        typename A = actual_type_t<P>, if_not_t< flecs::is_pair<First>::value> = 0>
    const Self& assign(A&& value) const  {
        flecs::assign<P>(this->world_, this->id_, FLECS_FWD(value));
        return to_base();
    }

    /** Assign a pair for an entity.
     * This operation sets the pair value, and uses First as type. If the
     * entity did not yet have the component the operation will panic.
     *
     * @tparam First The first element of the pair.
     * @tparam Second The second element of the pair
     * @param value The value to set.
     */
    template <typename First, typename Second, typename P = pair<First, Second>,
        typename A = actual_type_t<P>, if_not_t< flecs::is_pair<First>::value> = 0>
    const Self& assign(const A& value) const  {
        flecs::assign<P>(this->world_, this->id_, value);
        return to_base();
    }

    /** Assign a pair for an entity.
     * This operation sets the pair value, and uses First as type. If the
     * entity did not yet have the component the operation will panic.
     *
     * @tparam First The first element of the pair.
     * @param second The second element of the pair.
     * @param value The value to set.
     */
    template <typename First, typename Second, if_not_t< is_enum<Second>::value > = 0>
    const Self& assign(Second second, const First& value) const  {
        auto first = _::type<First>::id(this->world_);
        flecs::assign(this->world_, this->id_, value,
            ecs_pair(first, second));
        return to_base();
    }

    /** Assign a pair for an entity.
     * This operation sets the pair value, and uses First as type. If the
     * entity did not yet have the component the operation will panic.
     *
     * @tparam First The first element of the pair.
     * @param second The second element of the pair.
     * @param value The value to set.
     */
    template <typename First, typename Second, if_not_t< is_enum<Second>::value > = 0>
    const Self& assign(Second second, First&& value) const  {
        auto first = _::type<First>::id(this->world_);
        flecs::assign(this->world_, this->id_, FLECS_FWD(value),
            ecs_pair(first, second));
        return to_base();
    }

    /** Assign a pair for an entity.
     * This operation sets the pair value, and uses First as type. If the
     * entity did not yet have the component the operation will panic.
     *
     * @tparam First The first element of the pair.
     * @param constant The enum constant.
     * @param value The value to set.
     */
    template <typename First, typename Second, if_t< is_enum<Second>::value > = 0>
    const Self& assign(Second constant, const First& value) const  {
        const auto& et = enum_type<Second>(this->world_);
        flecs::entity_t second = et.entity(constant);
        return assign<First>(second, value);
    }

    /** Assign a pair for an entity.
     * This operation sets the pair value, and uses Second as type. If the
     * entity did not yet have the component the operation will panic.
     *
     * @tparam Second The second element of the pair
     * @param first The first element of the pair.
     * @param value The value to set.
     */
    template <typename Second>
    const Self& assign_second(entity_t first, const Second& value) const  {
        auto second = _::type<Second>::id(this->world_);
        ecs_assert( ecs_get_type_info(world_, ecs_pair(first, second)) != NULL,
            ECS_INVALID_PARAMETER, "pair is not a component");
        ecs_assert( ecs_get_type_info(world_, ecs_pair(first, second))->component == second,
            ECS_INVALID_PARAMETER, "type of pair is not Second");
        flecs::assign(this->world_, this->id_, value,
            ecs_pair(first, second));
        return to_base();
    }

    /** Assign a pair for an entity.
     * This operation sets the pair value, and uses Second as type. If the
     * entity did not yet have the component the operation will panic.
     *
     * @tparam Second The second element of the pair
     * @param first The first element of the pair.
     * @param value The value to set.
     */
    template <typename Second>
    const Self& assign_second(entity_t first, Second&& value) const  {
        auto second = _::type<Second>::id(this->world_);
        ecs_assert( ecs_get_type_info(world_, ecs_pair(first, second)) != NULL,
            ECS_INVALID_PARAMETER, "pair is not a component");
        ecs_assert( ecs_get_type_info(world_, ecs_pair(first, second))->component == second,
            ECS_INVALID_PARAMETER, "type of pair is not Second");
        flecs::assign(this->world_, this->id_, FLECS_FWD(value),
            ecs_pair(first, second));
        return to_base();
    }

    template <typename First, typename Second>
    const Self& assign_second(const Second& value) const  {
        flecs::assign<pair_object<First, Second>>(this->world_, this->id_, value);
        return to_base();
    }


    /** Set 1..N components.
     * This operation accepts a callback with as arguments the components to
     * set. If the entity does not have all of the provided components, they
     * will be added.
     *
     * This operation is faster than individually calling get for each component
     * as it only obtains entity metadata once. When this operation is called
     * while deferred, its performance is equivalent to that of calling ensure
     * for each component separately.
     *
     * The operation will invoke modified for each component after the callback
     * has been invoked.
     *
     * @param func The callback to invoke.
     */
    template <typename Func>
    const Self& insert(const Func& func) const;

    /** Emplace component.
     * Emplace constructs a component in the storage, which prevents calling the
     * destructor on the value passed into the function.
     *
     * @tparam T the component to emplace
     * @param args The arguments to pass to the constructor of T
     */
    template<typename T, typename ... Args, typename A = actual_type_t<T>>
    const Self& emplace(Args&&... args) const  {
        flecs::emplace<A>(this->world_, this->id_,
            _::type<T>::id(this->world_), FLECS_FWD(args)...);
        return to_base();
    }

    template <typename First, typename Second, typename ... Args, typename P = pair<First, Second>,
        typename A = actual_type_t<P>, if_not_t< flecs::is_pair<First>::value> = 0>
    const Self& emplace(Args&&... args) const  {
        flecs::emplace<A>(this->world_, this->id_,
            ecs_pair(_::type<First>::id(this->world_),
                _::type<Second>::id(this->world_)),
            FLECS_FWD(args)...);
        return to_base();
    }

    template <typename First, typename ... Args>
    const Self& emplace_first(flecs::entity_t second, Args&&... args) const  {
        auto first = _::type<First>::id(this->world_);
        flecs::emplace<First>(this->world_, this->id_,
            ecs_pair(first, second),
            FLECS_FWD(args)...);
        return to_base();
    }

    template <typename Second, typename ... Args>
    const Self& emplace_second(flecs::entity_t first, Args&&... args) const  {
        auto second = _::type<Second>::id(this->world_);
        ecs_assert( ecs_get_type_info(world_, ecs_pair(first, second)) != NULL,
            ECS_INVALID_PARAMETER, "pair is not a component");
        ecs_assert( ecs_get_type_info(world_, ecs_pair(first, second))->component == second,
            ECS_INVALID_PARAMETER, "type of pair is not Second");
        flecs::emplace<Second>(this->world_, this->id_,
            ecs_pair(first, second),
            FLECS_FWD(args)...);
        return to_base();
    }

    /** Entities created in function will have the current entity.
     * This operation is thread safe.
     *
     * @param func The function to call.
     */
    template <typename Func>
    const Self& with(const Func& func) const  {
        ecs_id_t prev = ecs_set_with(this->world_, this->id_);
        func();
        ecs_set_with(this->world_, prev);
        return to_base();
    }

    /** Entities created in function will have `(First, this)`.
     * This operation is thread safe.
     *
     * @tparam First The first element of the pair
     * @param func The function to call.
     */
    template <typename First, typename Func>
    const Self& with(const Func& func) const  {
        with(_::type<First>::id(this->world_), func);
        return to_base();
    }

    /** Entities created in function will have `(first, this)`.
     * This operation is thread safe.
     *
     * @param first The first element of the pair.
     * @param func The function to call.
     */
    template <typename Func>
    const Self& with(entity_t first, const Func& func) const  {
        ecs_id_t prev = ecs_set_with(this->world_,
            ecs_pair(first, this->id_));
        func();
        ecs_set_with(this->world_, prev);
        return to_base();
    }

    /** The function will be ran with the scope set to the current entity. */
    template <typename Func>
    const Self& scope(const Func& func) const  {
        ecs_entity_t prev = ecs_set_scope(this->world_, this->id_);
        func();
        ecs_set_scope(this->world_, prev);
        return to_base();
    }

    /** Return world scoped to entity */
    scoped_world scope() const {
        return scoped_world(world_, id_);
    }

    /* Set the entity name.
     */
    const Self& set_name(const char *name) const  {
        ecs_set_name(this->world_, this->id_, name);
        return to_base();
    }

    /* Set entity alias.
     */
    const Self& set_alias(const char *name) const  {
        ecs_set_alias(this->world_, this->id_, name);
        return to_base();
    }

#   ifdef FLECS_DOC
/**
 * @file addons/cpp/mixins/doc/entity_builder.inl
 * @brief Doc entity builder mixin.
 */

/** Set human readable name.
 * This adds `(flecs.doc.Description, flecs.Name)` to the entity.
 *
 * @see ecs_doc_set_name()
 * @see flecs::doc::set_name()
 * @see flecs::entity_view::doc_name()
 *
 * @memberof flecs::entity_builder
 * @ingroup cpp_addons_doc
 */
const Self& set_doc_name(const char *name) const {
    ecs_doc_set_name(world_, id_, name);
    return to_base();
}

/** Set brief description.
 * This adds `(flecs.doc.Description, flecs.doc.Brief)` to the entity.
 *
 * @see ecs_doc_set_brief()
 * @see flecs::doc::set_brief()
 * @see flecs::entity_view::doc_brief()
 *
 * @memberof flecs::entity_builder
 * @ingroup cpp_addons_doc
 */
const Self& set_doc_brief(const char *brief) const {
    ecs_doc_set_brief(world_, id_, brief);
    return to_base();
}

/** Set detailed description.
 * This adds `(flecs.doc.Description, flecs.doc.Detail)` to the entity.
 *
 * @see ecs_doc_set_detail()
 * @see flecs::doc::set_detail()
 * @see flecs::entity_view::doc_detail()
 *
 * @memberof flecs::entity_builder
 * @ingroup cpp_addons_doc
 */
const Self& set_doc_detail(const char *detail) const {
    ecs_doc_set_detail(world_, id_, detail);
    return to_base();
}

/** Set link to external documentation.
 * This adds `(flecs.doc.Description, flecs.doc.Link)` to the entity.
 *
 * @see ecs_doc_set_link()
 * @see flecs::doc::set_link()
 * @see flecs::entity_view::doc_link()
 *
 * @memberof flecs::entity_builder
 * @ingroup cpp_addons_doc
 */
const Self& set_doc_link(const char *link) const {
    ecs_doc_set_link(world_, id_, link);
    return to_base();
}

/** Set doc color.
 * This adds `(flecs.doc.Description, flecs.doc.Color)` to the entity.
 *
 * @see ecs_doc_set_color()
 * @see flecs::doc::set_color()
 * @see flecs::entity_view::doc_color()
 *
 * @memberof flecs::entity_builder
 * @ingroup cpp_addons_doc
 */
const Self& set_doc_color(const char *color) const {
    ecs_doc_set_color(world_, id_, color);
    return to_base();
}

/** Set doc UUID.
 * This adds `(flecs.doc.Description, flecs.doc.Uuid)` to the entity.
 *
 * @see ecs_doc_set_uuid()
 * @see flecs::doc::set_uuid()
 * @see flecs::entity_view::doc_uuid()
 *
 * @memberof flecs::entity_builder
 * @ingroup cpp_addons_doc
 */
const Self& set_doc_uuid(const char *uuid) const {
    ecs_doc_set_uuid(world_, id_, uuid);
    return to_base();
}

#   endif

#   ifdef FLECS_META
/**
 * @file addons/cpp/mixins/meta/entity_builder.inl
 * @brief Meta entity builder mixin.
 */

/**
 * @memberof flecs::entity_view
 * @ingroup cpp_addons_meta
 *
 * @{
 */

/** Make entity a unit */
const Self& unit(
    const char *symbol,
    flecs::entity_t prefix = 0,
    flecs::entity_t base = 0,
    flecs::entity_t over = 0,
    int32_t factor = 0,
    int32_t power = 0) const
{
    ecs_unit_desc_t desc = {};
    desc.entity = this->id_;
    desc.symbol = const_cast<char*>(symbol); /* safe, will be copied in */
    desc.base = base;
    desc.over = over;
    desc.prefix = prefix;
    desc.translation.factor = factor;
    desc.translation.power = power;
    ecs_unit_init(this->world(), &desc);

    return to_base();
}

/** Make entity a derived unit */
const Self& unit(
    flecs::entity_t prefix = 0,
    flecs::entity_t base = 0,
    flecs::entity_t over = 0,
    int32_t factor = 0,
    int32_t power = 0) const
{
    ecs_unit_desc_t desc = {};
    desc.entity = this->id_;
    desc.base = base;
    desc.over = over;
    desc.prefix = prefix;
    desc.translation.factor = factor;
    desc.translation.power = power;
    ecs_unit_init(this->world(), &desc);

    return to_base();
}

/** Make entity a derived unit */
const Self& unit_prefix(
    const char *symbol,
    int32_t factor = 0,
    int32_t power = 0) const
{
    ecs_unit_prefix_desc_t desc = {};
    desc.entity = this->id_;
    desc.symbol = const_cast<char*>(symbol); /* safe, will be copied in */
    desc.translation.factor = factor;
    desc.translation.power = power;
    ecs_unit_prefix_init(this->world(), &desc);

    return to_base();
}

/** Add quantity to unit */
const Self& quantity(flecs::entity_t quantity) const {
    ecs_add_pair(this->world(), this->id(), flecs::Quantity, quantity);
    return to_base();
}

/** Make entity a unity prefix */
template <typename Quantity>
const Self& quantity() const {
    return this->quantity(_::type<Quantity>::id(this->world()));
}

/** Make entity a quantity */
const Self& quantity() const {
    ecs_add_id(this->world(), this->id(), flecs::Quantity);
    return to_base();
}

/** @} */

#   endif

#   ifdef FLECS_JSON
/**
 * @file addons/cpp/mixins/json/entity_builder.inl
 * @brief JSON entity mixin.
 */

/** Set component from JSON.
 *
 * @memberof flecs::entity_builder
 * @ingroup cpp_addons_json
 */
const Self& set_json(
    flecs::id_t e,
    const char *json,
    flecs::from_json_desc_t *desc = nullptr) const
{
    const flecs::type_info_t *ti = ecs_get_type_info(world_, e);
    if (!ti) {
        ecs_err("id is not a type");
        return to_base();
    }

    flecs::entity_t type = ti->component;

    void *ptr = ecs_ensure_id(world_, id_, e, static_cast<size_t>(ti->size));
    ecs_assert(ptr != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_ptr_from_json(world_, type, ptr, json, desc);
    ecs_modified_id(world_, id_, e);

    return to_base();
}

/** Set pair from JSON.
 *
 * @memberof flecs::entity_builder
 * @ingroup cpp_addons_json
 */
const Self& set_json(
    flecs::entity_t r,
    flecs::entity_t t,
    const char *json,
    flecs::from_json_desc_t *desc = nullptr) const
{
    return set_json(ecs_pair(r, t), json, desc);
}

/** Set component from JSON.
 *
 * @memberof flecs::entity_builder
 * @ingroup cpp_addons_json
 */
template <typename T>
const Self& set_json(
    const char *json,
    flecs::from_json_desc_t *desc = nullptr) const
{
    return set_json(_::type<T>::id(world_), json, desc);
}

/** Set pair from JSON.
 *
 * @memberof flecs::entity_builder
 * @ingroup cpp_addons_json
 */
template <typename R, typename T>
const Self& set_json(
    const char *json,
    flecs::from_json_desc_t *desc = nullptr) const
{
    return set_json(
        _::type<R>::id(world_),
        _::type<T>::id(world_),
        json, desc);
}

/** Set pair from JSON.
 *
 * @memberof flecs::entity_builder
 * @ingroup cpp_addons_json
 */
template <typename R>
const Self& set_json(
    flecs::entity_t t,
    const char *json,
    flecs::from_json_desc_t *desc = nullptr) const
{
    return set_json(
        _::type<R>::id(world_), t,
        json, desc);
}

/** Set pair from JSON.
 *
 * @memberof flecs::entity_builder
 * @ingroup cpp_addons_json
 */
template <typename T>
const Self& set_json_second(
    flecs::entity_t r,
    const char *json,
    flecs::from_json_desc_t *desc = nullptr) const
{
    return set_json(
        r, _::type<T>::id(world_),
        json, desc);
}

#   endif

/**
 * @file addons/cpp/mixins/event/entity_builder.inl
 * @brief Event entity mixin.
 */

/** Observe event on entity
 *
 * @memberof flecs::entity_builder
 *
 * @param evt The event id.
 * @param callback The observer callback.
 * @return Event builder.
 */
template <typename Func>
const Self& observe(flecs::entity_t evt, Func&& callback) const;

/** Observe event on entity
 *
 * @memberof flecs::entity_builder
 *
 * @tparam Evt The event type.
 * @param callback The observer callback.
 * @return Event builder.
 */
template <typename Evt, typename Func>
const Self& observe(Func&& callback) const;

/** Observe event on entity
 *
 * @memberof flecs::entity_builder
 *
 * @param callback The observer callback.
 * @return Event builder.
 */
template <typename Func>
const Self& observe(Func&& callback) const;




protected:
    const Self& to_base() const  {
        return *static_cast<const Self*>(this);
    }
};

}


/**
 * @defgroup cpp_entities Entities
 * @ingroup cpp_core
 * Entity operations.
 *
 * @{
 */

export namespace flecs
{

/** Entity.
 * Class with read/write operations for entities.
 *
 * @ingroup cpp_entities
*/
struct entity : entity_builder<entity>
{
    entity() : entity_builder<entity>() { }

    /** Create entity.
     *
     * @param world The world in which to create the entity.
     */
    explicit entity(world_t *world)
        : entity_builder()
    {
        world_ = world;
        if (!ecs_get_scope(world_) && !ecs_get_with(world_)) {
            id_ = ecs_new(world);
        } else {
            ecs_entity_desc_t desc = {};
            id_ = ecs_entity_init(world_, &desc);
        }
    }

    /** Wrap an existing entity id.
     *
     * @param world The world in which the entity is created.
     * @param id The entity id.
     */
    explicit entity(const flecs::world_t *world, flecs::entity_t id) {
        world_ = const_cast<flecs::world_t*>(world);
        id_ = id;
    }

    /** Create a named entity.
     * Named entities can be looked up with the lookup functions. Entity names
     * may be scoped, where each element in the name is separated by "::".
     * For example: "Foo::Bar". If parts of the hierarchy in the scoped name do
     * not yet exist, they will be automatically created.
     *
     * @param world The world in which to create the entity.
     * @param name The entity name.
     */
    explicit entity(world_t *world, const char *name)
        : entity_builder()
    {
        world_ = world;

        ecs_entity_desc_t desc = {};
        desc.name = name;
        desc.sep = "::";
        desc.root_sep = "::";
        id_ = ecs_entity_init(world, &desc);
    }

    /** Create a named entity.
     * Named entities can be looked up with the lookup functions. Entity names
     * may be scoped, where each element in the name is separated by sep.
     * For example: "Foo.Bar". If parts of the hierarchy in the scoped name do
     * not yet exist, they will be automatically created.
     *
     * @param world The world in which to create the entity.
     * @param name The entity name.
     * @param sep The separator to use for the scoped name.
     * @param root_sep The separator to use for the root of the scoped name.
     */
    explicit entity(world_t *world, const char *name, const char *sep, const char *root_sep)
        : entity_builder()
    {
        world_ = world;

        ecs_entity_desc_t desc = {};
        desc.name = name;
        desc.sep = sep;
        desc.root_sep = root_sep;
        id_ = ecs_entity_init(world, &desc);
    }

    /** Conversion from flecs::entity_t to flecs::entity.
     *
     * @param id The entity_t value to convert.
     */
    explicit entity(entity_t id)
        : entity_builder( nullptr, id ) { }

    #ifndef ensure

    /** Get mutable component value.
     * This operation returns a mutable pointer to the component. If the entity
     * did not yet have the component, it will be added. If a base entity had
     * the component, it will be overridden, and the value of the base component
     * will be copied to the entity before this function returns.
     *
     * @tparam T The component to get.
     * @return Pointer to the component value.
     */
    template <typename T>
    T& ensure() const {
        auto comp_id = _::type<T>::id(world_);
        ecs_assert(_::type<T>::size() != 0, ECS_INVALID_PARAMETER,
            "operation invalid for empty type");
        return *static_cast<T*>(ecs_ensure_id(world_, id_, comp_id, sizeof(T)));
    }

    /** Get mutable component value (untyped).
     * This operation returns a mutable pointer to the component. If the entity
     * did not yet have the component, it will be added. If a base entity had
     * the component, it will be overridden, and the value of the base component
     * will be copied to the entity before this function returns.
     *
     * @param comp The component to get.
     * @return Pointer to the component value.
     */
    void* ensure(entity_t comp) const {
        const flecs::type_info_t *ti = ecs_get_type_info(world_, comp);
        ecs_assert(ti != nullptr && ti->size != 0, ECS_INVALID_PARAMETER,
            "provided component is not a type or has size 0");
        return ecs_ensure_id(world_, id_, comp, static_cast<size_t>(ti->size));
    }

    /** Get mutable pointer for a pair.
     * This operation gets the value for a pair from the entity.
     *
     * @tparam First The first part of the pair.
     * @tparam Second the second part of the pair.
     */
    template <typename First, typename Second, typename P = pair<First, Second>,
        typename A = actual_type_t<P>, if_not_t< flecs::is_pair<First>::value> = 0>
    A& ensure() const {
        return *static_cast<A*>(ecs_ensure_id(world_, id_, ecs_pair(
            _::type<First>::id(world_),
            _::type<Second>::id(world_)), sizeof(A)));
    }

    /** Get mutable pointer for the first element of a pair.
     * This operation gets the value for a pair from the entity.
     *
     * @tparam First The first part of the pair.
     * @param second The second element of the pair.
     */
    template <typename First>
    First& ensure(entity_t second) const {
        auto first = _::type<First>::id(world_);
        ecs_assert(_::type<First>::size() != 0, ECS_INVALID_PARAMETER,
            "operation invalid for empty type");
        return *static_cast<First*>(
            ecs_ensure_id(world_, id_, ecs_pair(first, second), sizeof(First)));
    }

    /** Get mutable pointer for a pair (untyped).
     * This operation gets the value for a pair from the entity. If neither the
     * first nor second element of the pair is a component, the operation will
     * fail.
     *
     * @param first The first element of the pair.
     * @param second The second element of the pair.
     */
    void* ensure(entity_t first, entity_t second) const {
        return ensure(ecs_pair(first, second));
    }

    /** Get mutable pointer for the second element of a pair.
     * This operation gets the value for a pair from the entity.
     *
     * @tparam Second The second element of the pair.
     * @param first The first element of the pair.
     */
    template <typename Second>
    Second& ensure_second(entity_t first) const {
        auto second = _::type<Second>::id(world_);
        ecs_assert( ecs_get_type_info(world_, ecs_pair(first, second)) != NULL,
            ECS_INVALID_PARAMETER, "pair is not a component");
        ecs_assert( ecs_get_type_info(world_, ecs_pair(first, second))->component == second,
            ECS_INVALID_PARAMETER, "type of pair is not Second");
        ecs_assert(_::type<Second>::size() != 0, ECS_INVALID_PARAMETER,
            "operation invalid for empty type");
        return *static_cast<Second*>(
            ecs_ensure_id(world_, id_, ecs_pair(first, second), sizeof(Second)));
    }

    #endif

    /** Signal that component was modified.
     *
     * @tparam T component that was modified.
     */
    template <typename T>
    void modified() const {
        auto comp_id = _::type<T>::id(world_);
        ecs_assert(_::type<T>::size() != 0, ECS_INVALID_PARAMETER,
            "operation invalid for empty type");
        this->modified(comp_id);
    }

    /** Signal that the first element of a pair was modified.
     *
     * @tparam First The first part of the pair.
     * @tparam Second the second part of the pair.
     */
    template <typename First, typename Second, typename A = actual_type_t<flecs::pair<First, Second>>>
    void modified() const {
        auto first = _::type<First>::id(world_);
        auto second = _::type<Second>::id(world_);
        ecs_assert(_::type<A>::size() != 0, ECS_INVALID_PARAMETER,
            "operation invalid for empty type");
        this->modified(first, second);
    }

    /** Signal that the first part of a pair was modified.
     *
     * @tparam First The first part of the pair.
     * @param second The second element of the pair.
     */
    template <typename First>
    void modified(entity_t second) const {
        auto first = _::type<First>::id(world_);
        ecs_assert(_::type<First>::size() != 0, ECS_INVALID_PARAMETER,
            "operation invalid for empty type");
        this->modified(first, second);
    }

    /** Signal that a pair has modified (untyped).
     * If neither the first or second element of the pair are a component, the
     * operation will fail.
     *
     * @param first The first element of the pair.
     * @param second The second element of the pair.
     */
    void modified(entity_t first, entity_t second) const {
        this->modified(ecs_pair(first, second));
    }

    /** Signal that component was modified.
     *
     * @param comp component that was modified.
     */
    void modified(entity_t comp) const {
        ecs_modified_id(world_, id_, comp);
    }

    /** Get reference to component specified by id.
     * A reference allows for quick and safe access to a component value, and is
     * a faster alternative to repeatedly calling 'get' for the same component.
     *
     * The method accepts a component id argument, which can be used to create a
     * ref to a component that is different from the provided type. This allows
     * for creating a base type ref that points to a derived type:
     *
     * @code
     * flecs::ref<Base> r = e.get_ref<Base>(world.id<Derived>());
     * @endcode
     *
     * If the provided component id is not binary compatible with the specified
     * type, the behavior is undefined.
     *
     * @tparam T component for which to get a reference.
     * @return The reference.
     */
    template <typename T, if_t< is_actual<T>::value > = 0>
    ref<T> get_ref_w_id(flecs::id_t component) const {
        _::type<T>::id(world_); // ensure type is registered
        return ref<T>(world_, id_, component);
    }

    /** Get reference to component.
     * A reference allows for quick and safe access to a component value, and is
     * a faster alternative to repeatedly calling 'get' for the same component.
     *
     * @tparam T component for which to get a reference.
     * @return The reference.
     */
    template <typename T, if_t< is_actual<T>::value > = 0>
    ref<T> get_ref() const {
        return ref<T>(world_, id_, _::type<T>::id(world_));
    }

    /** Get reference to component.
     * Overload for when T is not the same as the actual type, which happens
     * when using pair types.
     * A reference allows for quick and safe access to a component value, and is
     * a faster alternative to repeatedly calling 'get' for the same component.
     *
     * @tparam T component for which to get a reference.
     * @return The reference.
     */
    template <typename T, typename A = actual_type_t<T>, if_t< flecs::is_pair<T>::value > = 0>
    ref<A> get_ref() const {
        return ref<A>(world_, id_,
                      ecs_pair(_::type<typename T::first>::id(world_),
                               _::type<typename T::second>::id(world_)));
    }


    template <typename First, typename Second, typename P = flecs::pair<First, Second>,
        typename A = actual_type_t<P>>
    ref<A> get_ref() const {
        return ref<A>(world_, id_,
            ecs_pair(_::type<First>::id(world_), _::type<Second>::id(world_)));
    }

    template <typename First>
    ref<First> get_ref(flecs::entity_t second) const {
        auto first = _::type<First>::id(world_);
        return ref<First>(world_, id_, ecs_pair(first, second));
    }

    untyped_ref get_ref(flecs::id_t component) const {
        return untyped_ref(world_, id_, component);
    }

    untyped_ref get_ref(flecs::id_t first, flecs::id_t second) const {
        return untyped_ref(world_, id_, ecs_pair(first, second));
    }

    template <typename Second>
    ref<Second> get_ref_second(flecs::entity_t first) const {
        auto second = _::type<Second>::id(world_);
        ecs_assert( ecs_get_type_info(world_, ecs_pair(first, second)) != NULL,
            ECS_INVALID_PARAMETER, "pair is not a component");
        ecs_assert( ecs_get_type_info(world_, ecs_pair(first, second))->component == second,
            ECS_INVALID_PARAMETER, "type of pair is not Second");
        return ref<Second>(world_, id_, ecs_pair(first, second));
    }

    /** Clear an entity.
     * This operation removes all components from an entity without recycling
     * the entity id.
     *
     * @see ecs_clear()
     */
    void clear() const {
        ecs_clear(world_, id_);
    }

    /** Delete an entity.
     * Entities have to be deleted explicitly, and are not deleted when the
     * entity object goes out of scope.
     *
     * @see ecs_delete()
     */
    void destruct() const {
        ecs_delete(world_, id_);
    }

    /** Create child */
    template <typename ... Args>
    flecs::entity child(flecs::entity_t r = flecs::ChildOf, Args&&... args) {
        flecs::world world(world_);
        return world.entity(FLECS_FWD(args)...).add(r, id_);
    }

    template <typename R, typename ... Args>
    flecs::entity child(Args&&... args) {
        flecs::world world(world_);
        return world.entity(FLECS_FWD(args)...).add(_::type<R>::id(world_), id_);
    }

    /** Set child order.
     * Changes the order of children as returned by entity::children(). Only
     * applicableo to entities with the flecs::OrderedChildren trait.
     *
     * @see ecs_set_child_order()
     */
    void set_child_order(flecs::entity_t *children, int32_t child_count) const {
        ecs_set_child_order(world_, id_, children, child_count);
    }

    /** Return entity as entity_view.
     * This returns an entity_view instance for the entity which is a readonly
     * version of the entity class.
     *
     * This is similar to a regular upcast, except that this method ensures that
     * the entity_view instance is instantiated with a world vs. a stage, which
     * a regular upcast does not guarantee.
     */
    flecs::entity_view view() const {
        return flecs::entity_view(
            const_cast<flecs::world_t*>(ecs_get_world(world_)), id_);
    }

    /** Entity id 0.
     * This function is useful when the API must provide an entity that
     * belongs to a world, but the entity id is 0.
     *
     * @param world The world.
     */
    static
    flecs::entity null(const flecs::world_t *world) {
        flecs::entity result;
        result.world_ = const_cast<flecs::world_t*>(world);
        return result;
    }

    static
    flecs::entity null() {
        return flecs::entity();
    }

#   ifdef FLECS_JSON

/** Deserialize entity to JSON.
 *
 * @memberof flecs::entity
 * @ingroup cpp_addons_json
 */
const char* from_json(const char *json) {
    return ecs_entity_from_json(world_, id_, json, nullptr);
}

#   endif
};

} // export namespace flecs

/** @} */

/**
 * @file addons/cpp/delegate.hpp
 * @brief Wrappers around C++ functions that provide callbacks for C APIs.
 */

#pragma once

#include <utility> // std::declval

export namespace flecs
{

namespace _
{

// Binding ctx for component hooks
struct component_binding_ctx {
    void *on_add = nullptr;
    void *on_remove = nullptr;
    void *on_set = nullptr;
    void *on_replace = nullptr;
    ecs_ctx_free_t free_on_add = nullptr;
    ecs_ctx_free_t free_on_remove = nullptr;
    ecs_ctx_free_t free_on_set = nullptr;
    ecs_ctx_free_t free_on_replace = nullptr;

    ~component_binding_ctx() {
        if (on_add && free_on_add) {
            free_on_add(on_add);
        }
        if (on_remove && free_on_remove) {
            free_on_remove(on_remove);
        }
        if (on_set && free_on_set) {
            free_on_set(on_set);
        }
        if (on_replace && free_on_replace) {
            free_on_replace(on_replace);
        }
    }
};

// Utility to convert template argument pack to array of term ptrs
struct field_ptr {
    void *ptr = nullptr;
    int8_t index = 0;
    bool is_ref = false;
    bool is_row = false;
};

template <typename ... Components>
struct field_ptrs {
    using array = flecs::array<_::field_ptr, sizeof...(Components)>;

    void populate(const ecs_iter_t *iter) {
        populate(iter, 0, static_cast<
            remove_reference_t<
                remove_pointer_t<Components>>
                    *>(nullptr)...);
    }

    void populate_self(const ecs_iter_t *iter) {
        populate_self(iter, 0, static_cast<
            remove_reference_t<
                remove_pointer_t<Components>>
                    *>(nullptr)...);
    }

    array fields_;

private:
    void populate(const ecs_iter_t*, size_t) { }

    template <typename T, typename... Targs,
        typename A = remove_pointer_t<actual_type_t<T>>,
            if_not_t< is_empty<A>::value > = 0>
    void populate(const ecs_iter_t *iter, size_t index, T, Targs... comps) {
        if (iter->row_fields & (1llu << index)) {
            /* Need to fetch the value with ecs_field_at() */
            fields_[index].is_row = true;
            fields_[index].is_ref = true;
            fields_[index].index = static_cast<int8_t>(index);
        } else {
            fields_[index].ptr = ecs_field_w_size(iter, sizeof(A),
                static_cast<int8_t>(index));
            fields_[index].is_ref = iter->sources[index] != 0;
        }

        populate(iter, index + 1, comps ...);
    }

    template <typename T, typename... Targs,
        typename A = remove_pointer_t<actual_type_t<T>>,
            if_t< is_empty<A>::value > = 0>
    void populate(const ecs_iter_t *iter, size_t index, T, Targs... comps) {
        populate(iter, index + 1, comps ...);
    }

    void populate_self(const ecs_iter_t*, size_t) { }

    template <typename T, typename... Targs,
        typename A = remove_pointer_t<actual_type_t<T>>,
            if_not_t< is_empty<A>::value > = 0>
    void populate_self(const ecs_iter_t *iter, size_t index, T, Targs... comps) {
        fields_[index].ptr = ecs_field_w_size(iter, sizeof(A),
            static_cast<int8_t>(index));
        fields_[index].is_ref = false;
        ecs_assert(iter->sources[index] == 0, ECS_INTERNAL_ERROR, NULL);
        populate_self(iter, index + 1, comps ...);
    }

    template <typename T, typename... Targs,
        typename A = remove_pointer_t<actual_type_t<T>>,
            if_t< is_empty<A>::value > = 0>
    void populate_self(const ecs_iter_t *iter, size_t index, T, Targs... comps) {
        populate(iter, index + 1, comps ...);
    }
};

struct delegate { };

// Template that figures out from the template parameters of a query/system
// how to pass the value to the each callback
template <typename T, typename = int>
struct each_field { };

// Base class
struct each_column_base {
    each_column_base(const _::field_ptr& field, size_t row)
        : field_(field), row_(row) {
    }

protected:
    const _::field_ptr& field_;
    size_t row_;
};

// If type is not a pointer, return a reference to the type (default case)
template <typename T>
struct each_field<T, if_t< !is_pointer<T>::value &&
        !is_empty<actual_type_t<T>>::value && is_actual<T>::value > >
    : each_column_base
{
    each_field(const flecs::iter_t*, _::field_ptr& field, size_t row)
        : each_column_base(field, row) { }

    T& get_row() {
        return static_cast<T*>(this->field_.ptr)[this->row_];
    }
};

// If argument type is not the same as actual component type, return by value.
// This requires that the actual type can be converted to the type.
// A typical scenario where this happens is when using flecs::pair types.
template <typename T>
struct each_field<T, if_t< !is_pointer<T>::value &&
        !is_empty<actual_type_t<T>>::value && !is_actual<T>::value> >
    : each_column_base
{
    each_field(const flecs::iter_t*, _::field_ptr& field, size_t row)
        : each_column_base(field, row) { }

    T get_row() {
        return static_cast<actual_type_t<T>*>(this->field_.ptr)[this->row_];
    }
};

// If type is empty (indicating a tag) the query will pass a nullptr. To avoid
// returning nullptr to reference arguments, return a temporary value.
template <typename T>
struct each_field<T, if_t< is_empty<actual_type_t<T>>::value &&
        !is_pointer<T>::value > >
    : each_column_base
{
    each_field(const flecs::iter_t*, _::field_ptr& field, size_t row)
        : each_column_base(field, row) { }

    T get_row() {
        return actual_type_t<T>();
    }
};

// If type is a pointer (indicating an optional value) don't index with row if
// the field is not set.
template <typename T>
struct each_field<T, if_t< is_pointer<T>::value &&
        !is_empty<actual_type_t<T>>::value > >
    : each_column_base
{
    each_field(const flecs::iter_t*, _::field_ptr& field, size_t row)
        : each_column_base(field, row) { }

    actual_type_t<T> get_row() {
        if (this->field_.ptr) {
            return &static_cast<actual_type_t<T>>(this->field_.ptr)[this->row_];
        } else {
            // optional argument doesn't have a value
            return nullptr;
        }
    }
};

// If the query contains component references to other entities, check if the
// current argument is one.
template <typename T, typename = int>
struct each_ref_field : public each_field<T> {
    each_ref_field(const flecs::iter_t *iter, _::field_ptr& field, size_t row)
        : each_field<T>(iter, field, row) {

        if (field.is_ref) {
            // If this is a reference, set the row to 0 as a ref always is a
            // single value, not an array. This prevents the application from
            // having to do an if-check on whether the column is owned.
            //
            // This check only happens when the current table being iterated
            // over caused the query to match a reference. The check is
            // performed once per iterated table.
            this->row_ = 0;
        }

        if (field.is_row) {
            field.ptr = ecs_field_at_w_size(iter, sizeof(T), field.index,
                static_cast<int32_t>(row));
        }
    }
};

// Type that handles passing components to each callbacks
template <typename Func, typename ... Components>
struct each_delegate : public delegate {
    using Terms = typename field_ptrs<Components ...>::array;

    template < if_not_t< is_same< decay_t<Func>, decay_t<Func>& >::value > = 0>
    explicit each_delegate(Func&& func) noexcept
        : func_(FLECS_MOV(func)) { }

    explicit each_delegate(const Func& func) noexcept
        : func_(func) { }

    // Invoke object directly. This operation is useful when the calling
    // function has just constructed the delegate, such as what happens when
    // iterating a query.
    void invoke(ecs_iter_t *iter) const {
        field_ptrs<Components...> terms;

        iter->flags |= EcsIterCppEach;

        if (iter->ref_fields | iter->up_fields) {
            terms.populate(iter);
            invoke_unpack< each_ref_field >(iter, func_, 0, terms.fields_);
        } else {
            terms.populate_self(iter);
            invoke_unpack< each_field >(iter, func_, 0, terms.fields_);
        }
    }

    // Static function that can be used as callback for systems/triggers
    static void run(ecs_iter_t *iter) {
        auto self = static_cast<const each_delegate*>(iter->callback_ctx);
        ecs_assert(self != nullptr, ECS_INTERNAL_ERROR, NULL);
        self->invoke(iter);
    }

    // Static function that can be used as callback for systems/triggers.
    // Different from run() in that it loops the iterator.
    static void run_each(ecs_iter_t *iter) {
        auto self = static_cast<const each_delegate*>(iter->run_ctx);
        ecs_assert(self != nullptr, ECS_INTERNAL_ERROR, NULL);
        while (iter->next(iter)) {
            self->invoke(iter);
        }
    }

    // Create instance of delegate
    static each_delegate* make(const Func& func) {
        return FLECS_NEW(each_delegate)(func);
    }

    // Function that can be used as callback to free delegate
    static void destruct(void *obj) {
        _::free_obj<each_delegate>(obj);
    }

    // Static function to call for component on_add hook
    static void run_add(ecs_iter_t *iter) {
        component_binding_ctx *ctx = reinterpret_cast<component_binding_ctx*>(
            iter->callback_ctx);
        iter->callback_ctx = ctx->on_add;
        run(iter);
    }

    // Static function to call for component on_remove hook
    static void run_remove(ecs_iter_t *iter) {
        component_binding_ctx *ctx = reinterpret_cast<component_binding_ctx*>(
            iter->callback_ctx);
        iter->callback_ctx = ctx->on_remove;
        run(iter);
    }

    // Static function to call for component on_set hook
    static void run_set(ecs_iter_t *iter) {
        component_binding_ctx *ctx = reinterpret_cast<component_binding_ctx*>(
            iter->callback_ctx);
        iter->callback_ctx = ctx->on_set;
        run(iter);
    }

    // Static function to call for component on_replace hook
    static void run_replace(ecs_iter_t *iter) {
        component_binding_ctx *ctx = reinterpret_cast<component_binding_ctx*>(
            iter->callback_ctx);
        iter->callback_ctx = ctx->on_replace;
        run(iter);
    }

private:
    // func(flecs::entity, Components...)
    template <template<typename X, typename = int> class ColumnType,
        typename... Args,
        typename Fn = Func,
        decltype(std::declval<const Fn&>()(
            std::declval<flecs::entity>(),
            std::declval<ColumnType< remove_reference_t<Components> > >().get_row()...), 0) = 0>
    static void invoke_callback(
        ecs_iter_t *iter, const Func& func, size_t i, Args... comps)
    {
        ecs_assert(iter->entities != nullptr, ECS_INVALID_PARAMETER,
            "query does not return entities ($this variable is not populated)");
        func(flecs::entity(iter->world, iter->entities[i]),
            (ColumnType< remove_reference_t<Components> >(iter, comps, i)
                .get_row())...);
    }

    // func(flecs::iter&, size_t row, Components...)
    template <template<typename X, typename = int> class ColumnType,
        typename... Args,
        typename Fn = Func,
        decltype(std::declval<const Fn&>()(
            std::declval<flecs::iter&>(),
            std::declval<size_t&>(),
            std::declval<ColumnType< remove_reference_t<Components> > >().get_row()...), 0) = 0>
    static void invoke_callback(
        ecs_iter_t *iter, const Func& func, size_t i, Args... comps)
    {
        flecs::iter it(iter);
        func(it, i, (ColumnType< remove_reference_t<Components> >(iter, comps, i)
            .get_row())...);
    }

    // func(Components...)
    template <template<typename X, typename = int> class ColumnType,
        typename... Args,
        typename Fn = Func,
        decltype(std::declval<const Fn&>()(
            std::declval<ColumnType< remove_reference_t<Components> > >().get_row()...), 0) = 0>
    static void invoke_callback(
        ecs_iter_t *iter, const Func& func, size_t i, Args... comps)
    {
        func((ColumnType< remove_reference_t<Components> >(iter, comps, i)
            .get_row())...);
    }

    template <template<typename X, typename = int> class ColumnType,
        typename... Args, if_t<
            sizeof...(Components) == sizeof...(Args)> = 0>
    static void invoke_unpack(
        ecs_iter_t *iter, const Func& func, size_t, Terms&, Args... comps)
    {
        ECS_TABLE_LOCK(iter->world, iter->table);

        size_t count = static_cast<size_t>(iter->count);
        if (count == 0 && !iter->table) {
            // If query has no This terms, count can be 0. Since each does not
            // have an entity parameter, just pass through components
            count = 1;
        }

        for (size_t i = 0; i < count; i ++) {
            invoke_callback<ColumnType>(iter, func, i, comps...);
        }

        ECS_TABLE_UNLOCK(iter->world, iter->table);
    }

    template <template<typename X, typename = int> class ColumnType,
        typename... Args, if_t< sizeof...(Components) != sizeof...(Args) > = 0>
    static void invoke_unpack(ecs_iter_t *iter, const Func& func,
        size_t index, Terms& columns, Args... comps)
    {
        invoke_unpack<ColumnType>(
            iter, func, index + 1, columns, comps..., columns[index]);
    }

public:
    Func func_;
};

template <typename Func, typename ... Components>
struct find_delegate : public delegate {
    using Terms = typename field_ptrs<Components ...>::array;

    template < if_not_t< is_same< decay_t<Func>, decay_t<Func>& >::value > = 0>
    explicit find_delegate(Func&& func) noexcept
        : func_(FLECS_MOV(func)) { }

    explicit find_delegate(const Func& func) noexcept
        : func_(func) { }

    // Invoke object directly. This operation is useful when the calling
    // function has just constructed the delegate, such as what happens when
    // iterating a query.
    flecs::entity invoke(ecs_iter_t *iter) const {
        field_ptrs<Components...> terms;

        iter->flags |= EcsIterCppEach;

        if (iter->ref_fields | iter->up_fields) {
            terms.populate(iter);
            return invoke_callback< each_ref_field >(iter, func_, 0, terms.fields_);
        } else {
            terms.populate_self(iter);
            return invoke_callback< each_field >(iter, func_, 0, terms.fields_);
        }
    }

private:
    // Number of function arguments is one more than number of components, pass
    // entity as argument.
    template <template<typename X, typename = int> class ColumnType,
        typename... Args,
        typename Fn = Func,
        if_t<sizeof...(Components) == sizeof...(Args)> = 0,
        decltype(bool(std::declval<const Fn&>()(
            std::declval<flecs::entity>(),
            std::declval<ColumnType< remove_reference_t<Components> > >().get_row()...))) = true>
    static flecs::entity invoke_callback(
        ecs_iter_t *iter, const Func& func, size_t, Terms&, Args... comps)
    {
        ECS_TABLE_LOCK(iter->world, iter->table);

        ecs_world_t *world = iter->world;
        size_t count = static_cast<size_t>(iter->count);
        flecs::entity result;

        for (size_t i = 0; i < count; i ++) {
            if (func(flecs::entity(world, iter->entities[i]),
                (ColumnType< remove_reference_t<Components> >(iter, comps, i)
                    .get_row())...))
            {
                result = flecs::entity(world, iter->entities[i]);
                break;
            }
        }

        ECS_TABLE_UNLOCK(iter->world, iter->table);

        return result;
    }

    // Number of function arguments is two more than number of components, pass
    // iter + index as argument.
    template <template<typename X, typename = int> class ColumnType,
        typename... Args,
        typename Fn = Func,
        if_t<sizeof...(Components) == sizeof...(Args)> = 0,
        decltype(bool(std::declval<const Fn&>()(
            std::declval<flecs::iter&>(),
            std::declval<size_t&>(),
            std::declval<ColumnType< remove_reference_t<Components> > >().get_row()...))) = true>
    static flecs::entity invoke_callback(
        ecs_iter_t *iter, const Func& func, size_t, Terms&, Args... comps)
    {
        size_t count = static_cast<size_t>(iter->count);
        if (count == 0) {
            // If query has no This terms, count can be 0. Since each does not
            // have an entity parameter, just pass through components
            count = 1;
        }

        flecs::iter it(iter);
        flecs::entity result;

        ECS_TABLE_LOCK(iter->world, iter->table);

        for (size_t i = 0; i < count; i ++) {
            if (func(it, i,
                (ColumnType< remove_reference_t<Components> >(iter, comps, i)
                    .get_row())...))
            {
                result = flecs::entity(iter->world, iter->entities[i]);
                break;
            }
        }

        ECS_TABLE_UNLOCK(iter->world, iter->table);

        return result;
    }

    // Number of function arguments is equal to number of components, no entity
    template <template<typename X, typename = int> class ColumnType,
        typename... Args,
        typename Fn = Func,
        if_t<sizeof...(Components) == sizeof...(Args)> = 0,
        decltype(bool(std::declval<const Fn&>()(
            std::declval<ColumnType< remove_reference_t<Components> > >().get_row()...))) = true>
    static flecs::entity invoke_callback(
        ecs_iter_t *iter, const Func& func, size_t, Terms&, Args... comps)
    {
        size_t count = static_cast<size_t>(iter->count);
        if (count == 0) {
            // If query has no This terms, count can be 0. Since each does not
            // have an entity parameter, just pass through components
            count = 1;
        }

        flecs::iter it(iter);
        flecs::entity result;

        ECS_TABLE_LOCK(iter->world, iter->table);

        for (size_t i = 0; i < count; i ++) {
            if (func(
                (ColumnType< remove_reference_t<Components> >(iter, comps, i)
                    .get_row())...))
            {
                result = flecs::entity(iter->world, iter->entities[i]);
                break;
            }
        }

        ECS_TABLE_UNLOCK(iter->world, iter->table);

        return result;
    }

    template <template<typename X, typename = int> class ColumnType,
        typename... Args, if_t< sizeof...(Components) != sizeof...(Args) > = 0>
    static flecs::entity invoke_callback(ecs_iter_t *iter, const Func& func,
        size_t index, Terms& columns, Args... comps)
    {
        return invoke_callback<ColumnType>(
            iter, func, index + 1, columns, comps..., columns[index]);
    }

    Func func_;
};

////////////////////////////////////////////////////////////////////////////////
//// Utility class to invoke a system iterate action
////////////////////////////////////////////////////////////////////////////////

template <typename Func>
struct run_delegate : delegate {
    template < if_not_t< is_same< decay_t<Func>, decay_t<Func>& >::value > = 0>
    explicit run_delegate(Func&& func) noexcept
        : func_(FLECS_MOV(func)) { }

    explicit run_delegate(const Func& func) noexcept
        : func_(func) { }

    // Invoke object directly. This operation is useful when the calling
    // function has just constructed the delegate, such as what happens when
    // iterating a query.
    void invoke(ecs_iter_t *iter) const {
        flecs::iter it(iter);
        iter->flags &= ~EcsIterIsValid;
        func_(it);
    }

    // Static function that can be used as callback for systems/triggers
    static void run(ecs_iter_t *iter) {
        auto self = static_cast<const run_delegate*>(iter->run_ctx);
        ecs_assert(self != nullptr, ECS_INTERNAL_ERROR, NULL);
        self->invoke(iter);
    }

    Func func_;
};


////////////////////////////////////////////////////////////////////////////////
//// Utility class to invoke an entity observer delegate
////////////////////////////////////////////////////////////////////////////////

template <typename Func>
struct entity_observer_delegate : delegate {
    explicit entity_observer_delegate(Func&& func) noexcept
        : func_(FLECS_MOV(func)) { }

    // Static function that can be used as callback for systems/triggers
    static void run(ecs_iter_t *iter) {
        invoke<Func>(iter);
    }

private:
    template <typename F,
        decltype(std::declval<const F&>()(std::declval<flecs::entity>()), 0) = 0>
    static void invoke(ecs_iter_t *iter) {
        auto self = static_cast<const entity_observer_delegate*>(iter->callback_ctx);
        ecs_assert(self != nullptr, ECS_INTERNAL_ERROR, NULL);
        self->func_(flecs::entity(iter->world, ecs_field_src(iter, 0)));
    }

    template <typename F,
        decltype(std::declval<const F&>()(), 0) = 0>
    static void invoke(ecs_iter_t *iter) {
        auto self = static_cast<const entity_observer_delegate*>(iter->callback_ctx);
        ecs_assert(self != nullptr, ECS_INTERNAL_ERROR, NULL);
        self->func_();
    }

    Func func_;
};

template <typename Func, typename Event>
struct entity_payload_observer_delegate : delegate {
    explicit entity_payload_observer_delegate(Func&& func) noexcept
        : func_(FLECS_MOV(func)) { }

    // Static function that can be used as callback for systems/triggers
    static void run(ecs_iter_t *iter) {
        invoke<Func>(iter);
    }

private:
    template <typename F,
        decltype(std::declval<const F&>()(
            std::declval<Event&>()), 0) = 0>
    static void invoke(ecs_iter_t *iter) {
        auto self = static_cast<const entity_payload_observer_delegate*>(
            iter->callback_ctx);
        ecs_assert(self != nullptr, ECS_INTERNAL_ERROR, NULL);
        ecs_assert(iter->param != nullptr, ECS_INVALID_OPERATION,
            "entity observer invoked without payload");

        Event *data = static_cast<Event*>(iter->param);
        self->func_(*data);
    }

    template <typename F,
        decltype(std::declval<const F&>()(
            std::declval<flecs::entity>(),
            std::declval<Event&>()), 0) = 0>
    static void invoke(ecs_iter_t *iter) {
        auto self = static_cast<const entity_payload_observer_delegate*>(
            iter->callback_ctx);
        ecs_assert(self != nullptr, ECS_INTERNAL_ERROR, NULL);
        ecs_assert(iter->param != nullptr, ECS_INVALID_OPERATION,
            "entity observer invoked without payload");

        Event *data = static_cast<Event*>(iter->param);
        self->func_(flecs::entity(iter->world, ecs_field_src(iter, 0)), *data);
    }

    Func func_;
};


////////////////////////////////////////////////////////////////////////////////
//// Utility to invoke callback on entity if it has components in signature
////////////////////////////////////////////////////////////////////////////////

template<typename ... Args>
struct entity_with_delegate_impl;

template<typename ... Args>
struct entity_with_delegate_impl<arg_list<Args ...>> {
    using ColumnArray = flecs::array<int32_t, sizeof...(Args)>;
    using ArrayType = flecs::array<void*, sizeof...(Args)>;
    using DummyArray = flecs::array<int, sizeof...(Args)>;
    using IdArray = flecs::array<id_t, sizeof...(Args)>;

    static bool const_args() {
        static flecs::array<bool, sizeof...(Args)> is_const_args ({
            flecs::is_const<flecs::remove_reference_t<Args>>::value...
        });

        for (auto is_const : is_const_args) {
            if (!is_const) {
                return false;
            }
        }
        return true;
    }

    static
    bool get_ptrs(world_t *world, flecs::entity_t e, const ecs_record_t *r, ecs_table_t *table,
        ArrayType& ptrs)
    {
        ecs_assert(table != NULL, ECS_INTERNAL_ERROR, NULL);

        /* table_index_of needs real world */
        const flecs::world_t *real_world = ecs_get_world(world);

        IdArray ids ({
            _::type<Args>().id(world)...
        });

        /* Get column indices for components */
        ColumnArray columns ({
            ecs_table_get_column_index(real_world, table,
                _::type<Args>().id(world))...
        });

        /* Get pointers for columns for entity */
        size_t i = 0;
        for (int32_t column : columns) {
            if (column == -1) {
                /* Component could be sparse */
                void *ptr = ecs_get_mut_id(world, e, ids[i]);
                if (!ptr) {
                    return false;
                }

                ptrs[i ++] = ptr;
                continue;
            }

            ptrs[i ++] = ecs_record_get_by_column(r, column, 0);
        }

        return true;
    }

    static bool ensure_ptrs(world_t *world, ecs_entity_t e, ArrayType& ptrs) {
        /* Get pointers w/ensure */
        size_t i = 0;
        DummyArray dummy ({
            (ptrs[i ++] = ecs_ensure_id(world, e,
                _::type<Args>().id(world), sizeof(Args)), 0)...
        });

        return true;
    }

    template <typename Func>
    static bool invoke_read(world_t *world, entity_t e, const Func& func) {
        const ecs_record_t *r = ecs_read_begin(world, e);
        if (!r) {
            return false;
        }

        ecs_table_t *table = r->table;
        if (!table) {
            return false;
        }

        ArrayType ptrs;
        bool has_components = get_ptrs(world, e, r, table, ptrs);
        if (has_components) {
            invoke_callback(func, 0, ptrs);
        }

        ecs_read_end(r);

        return has_components;
    }

    template <typename Func>
    static bool invoke_write(world_t *world, entity_t e, const Func& func) {
        ecs_record_t *r = ecs_write_begin(world, e);
        if (!r) {
            return false;
        }

        ecs_table_t *table = r->table;
        if (!table) {
            return false;
        }

        ArrayType ptrs;
        bool has_components = get_ptrs(world, e, r, table, ptrs);
        if (has_components) {
            invoke_callback(func, 0, ptrs);
        }

        ecs_write_end(r);

        return has_components;
    }

    template <typename Func>
    static bool invoke_get(world_t *world, entity_t e, const Func& func) {
        if (const_args()) {
            return invoke_read(world, e, func);
        } else {
            return invoke_write(world, e, func);
        }
    }

    // Utility for storing id in array in pack expansion
    static size_t store_added(IdArray& added, size_t elem, ecs_table_t *prev,
        ecs_table_t *next, id_t id)
    {
        // Array should only contain ids for components that are actually added,
        // so check if the prev and next tables are different.
        if (prev != next) {
            added[elem] = id;
            elem ++;
        }
        return elem;
    }

    struct InvokeCtx {
        InvokeCtx(flecs::table_t *table_arg) : table(table_arg) { }
        flecs::table_t *table;
        size_t component_count = 0;
        IdArray added = {};
    };

    static int invoke_add(
        flecs::world& w,
        flecs::entity_t entity,
        flecs::id_t component_id,
        InvokeCtx& ctx)
    {
        ecs_table_diff_t diff;
        flecs::table_t *next = flecs_table_traverse_add(
            w, ctx.table, &component_id, &diff);
        if (next != ctx.table) {
            ctx.added[ctx.component_count] = component_id;
            ctx.component_count ++;
        } else {
            if (diff.added_flags & EcsTableHasDontFragment) {
                w.entity(entity).add(component_id);

                ctx.added[ctx.component_count] = component_id;
                ctx.component_count ++;
            }
        }

        ctx.table = next;

        return 0;
    }

    template <typename Func>
    static bool invoke_ensure(
        world_t *world,
        entity_t id,
        const Func& func)
    {
        flecs::world w(world);

        ArrayType ptrs;
        ecs_table_t *table = NULL;

        // When not deferred take the fast path.
        if (!w.is_deferred()) {
            // Bit of low level code so we only do at most one table move & one
            // entity lookup for the entire operation.

            // Make sure the object is not a stage. Operations on a stage are
            // only allowed when the stage is in deferred mode, which is when
            // the world is in readonly mode.
            ecs_assert(!w.is_stage(), ECS_INVALID_PARAMETER, NULL);

            // Find table for entity
            ecs_record_t *r = ecs_record_find(world, id);
            if (r) {
                table = r->table;
            }

            // Iterate components, only store added component ids in added array
            InvokeCtx ctx(table);
            DummyArray dummy_before ({ (
                invoke_add(w, id, w.id<Args>(), ctx)
            )... });

            (void)dummy_before;

            // If table is different, move entity straight to it
            if (table != ctx.table) {
                ecs_type_t ids;
                ids.array = ctx.added.ptr();
                ids.count = static_cast<ecs_size_t>(ctx.component_count);
                ecs_commit(world, id, r, ctx.table, &ids, NULL);
                table = ctx.table;
            }

            if (!get_ptrs(w, id, r, table, ptrs)) {
                ecs_abort(ECS_INTERNAL_ERROR, NULL);
            }

            ECS_TABLE_LOCK(world, table);

        // When deferred, obtain pointers with regular ensure
        } else {
            ensure_ptrs(world, id, ptrs);
        }

        invoke_callback(func, 0, ptrs);

        if (!w.is_deferred()) {
            ECS_TABLE_UNLOCK(world, table);
        }

        // Call modified on each component
        DummyArray dummy_after ({
            ( ecs_modified_id(world, id, w.id<Args>()), 0)...
        });
        (void)dummy_after;

        return true;
    }

private:
    template <typename Func, typename ... TArgs,
        if_t<sizeof...(TArgs) == sizeof...(Args)> = 0>
    static void invoke_callback(
        const Func& f, size_t, ArrayType&, TArgs&& ... comps)
    {
        f(*static_cast<typename base_arg_type<Args>::type*>(comps)...);
    }

    template <typename Func, typename ... TArgs,
        if_t<sizeof...(TArgs) != sizeof...(Args)> = 0>
    static void invoke_callback(const Func& f, size_t arg, ArrayType& ptrs,
        TArgs&& ... comps)
    {
        invoke_callback(f, arg + 1, ptrs, comps..., ptrs[arg]);
    }
};

template <typename Func, typename U = int>
struct entity_with_delegate {
    static_assert(function_traits<Func>::value, "type is not callable");
};

template <typename Func>
struct entity_with_delegate<Func, if_t< is_callable<Func>::value > >
    : entity_with_delegate_impl< arg_list_t<Func> >
{
    static_assert(function_traits<Func>::arity > 0,
        "function must have at least one argument");
};

} // namespace _

// Experimental: allows using the each delegate for use cases outside of flecs
template <typename Func, typename ... Args>
using delegate = _::each_delegate<typename std::decay<Func>::type, Args...>;

} // export namespace flecs

/**
 * @file addons/cpp/component.hpp
 * @brief Registering/obtaining info from components.
 */

#pragma once

/**
 * @defgroup cpp_components Components
 * @ingroup cpp_core
 * Registering and working with components.
 *
 * @{
 */

export namespace flecs {

namespace _ {

// Trick to obtain typename from type, as described here
// https://blog.molecular-matters.com/2015/12/11/getting-the-type-of-a-template-argument-as-string-without-rtti/
//
// The code from the link has been modified to work with more types, and across
// multiple compilers. The resulting string should be the same on all platforms
// for all compilers.
//

#if defined(__GNUC__) || defined(_WIN32)
template <typename T>
inline const char* type_name() {
    const size_t len = ECS_FUNC_TYPE_LEN(const char*, type_name, ECS_FUNC_NAME);
    static char result[len + 1] = {};
    const size_t front_len = ECS_FUNC_NAME_FRONT(const char*, type_name);
    const char* cppTypeName = ecs_cpp_get_type_name(result, ECS_FUNC_NAME, len, front_len);
    return cppTypeName;
}
#else
#error "implicit component registration not supported"
#endif

// Translate a typename into a language-agnostic identifier. This allows for
// registration of components/modules across language boundaries.
template <typename T>
inline const char* symbol_name() {
    const size_t len = ECS_FUNC_TYPE_LEN(const char*, symbol_name, ECS_FUNC_NAME);
    static char result[len + 1] = {};
    const char* cppSymbolName = ecs_cpp_get_symbol_name(result, type_name<T>(), len);
    return cppSymbolName;
}

template <> inline const char* symbol_name<uint8_t>() {
    return "u8";
}
template <> inline const char* symbol_name<uint16_t>() {
    return "u16";
}
template <> inline const char* symbol_name<uint32_t>() {
    return "u32";
}
template <> inline const char* symbol_name<uint64_t>() {
    return "u64";
}
template <> inline const char* symbol_name<int8_t>() {
    return "i8";
}
template <> inline const char* symbol_name<int16_t>() {
    return "i16";
}
template <> inline const char* symbol_name<int32_t>() {
    return "i32";
}
template <> inline const char* symbol_name<int64_t>() {
    return "i64";
}
template <> inline const char* symbol_name<float>() {
    return "f32";
}
template <> inline const char* symbol_name<double>() {
    return "f64";
}

// If type is trivial, don't register lifecycle actions. While the functions
// that obtain the lifecycle callback do detect whether the callback is required
// adding a special case for trivial types eases the burden a bit on the
// compiler as it reduces the number of templates to evaluate.
template<typename T, enable_if_t<
    std::is_trivial<T>::value == true
        >* = nullptr>
void register_lifecycle_actions(ecs_world_t*, ecs_entity_t) { }

// If the component is non-trivial, register component lifecycle actions.
// Depending on the type not all callbacks may be available.
template<typename T, enable_if_t<
    std::is_trivial<T>::value == false
        >* = nullptr>
void register_lifecycle_actions(
    ecs_world_t *world,
    ecs_entity_t component)
{
    ecs_type_hooks_t cl{};
    cl.ctor = ctor<T>(cl.flags);
    cl.dtor = dtor<T>(cl.flags);

    cl.copy = copy<T>(cl.flags);
    cl.copy_ctor = copy_ctor<T>(cl.flags);
    cl.move = move<T>(cl.flags);
    cl.move_ctor = move_ctor<T>(cl.flags);

    cl.ctor_move_dtor = ctor_move_dtor<T>(cl.flags);
    cl.move_dtor = move_dtor<T>(cl.flags);

    cl.flags &= ECS_TYPE_HOOKS_ILLEGAL;
    ecs_set_hooks_id(world, component, &cl);

    if (cl.flags & (ECS_TYPE_HOOK_MOVE_ILLEGAL|ECS_TYPE_HOOK_MOVE_CTOR_ILLEGAL))
    {
        ecs_add_id(world, component, flecs::Sparse);
    }
}

template <typename T>
struct type_impl {
    static_assert(is_pointer<T>::value == false,
        "pointer types are not allowed for components");

    // Initialize component identifier
    static void init(
        bool allow_tag = true)
    {
        s_index = flecs_component_ids_index_get();
        s_allow_tag = allow_tag;
        s_size = sizeof(T);
        s_alignment = alignof(T);
        if (is_empty<T>::value && allow_tag) {
            s_size = 0;
            s_alignment = 0;
        }
    }

    static void init_builtin(
        flecs::world_t *world,
        flecs::entity_t id,
        bool allow_tag = true)
    {
        init(allow_tag);
        flecs_component_ids_set(world, s_index, id);
    }

    // Register component id.
    static entity_t register_id(
        world_t *world,                     // The world
        const char *name = nullptr,         // User provided name (overrides typename)
        bool allow_tag = true,              // Register empty types as zero-sized components
        bool is_component = true,           // Add flecs::Component to result
        bool explicit_registration = false, // Entered from world.component<T>()?
        flecs::id_t id = 0)                 // User provided component id
    {
        if (!s_index) {
            // This is the first time (in this binary image) that this type is
            // being used. Generate a static index that will identify the type
            // across worlds.
            init(allow_tag);
            ecs_assert(s_index != 0, ECS_INTERNAL_ERROR, NULL);
        }

        bool registered = false, existing = false;

        flecs::entity_t c = ecs_cpp_component_register(
            world, id, s_index, name, type_name<T>(),
            symbol_name<T>(), size(), alignment(),
            is_component, explicit_registration, &registered, &existing);

        ecs_assert(c != 0, ECS_INTERNAL_ERROR, NULL);

        if (registered) {
            // Register lifecycle callbacks, but only if the component has a
            // size. Components that don't have a size are tags, and tags don't
            // require construction/destruction/copy/move's.
            if (size() && !existing) {
                register_lifecycle_actions<T>(world, c);
            }

            // If component is enum type, register constants. Make sure to do
            // this after setting the component id, because the enum code will
            // be calling type<T>::id().
            #if FLECS_CPP_ENUM_REFLECTION_SUPPORT
            _::init_enum<T>(world, c);
            #endif
        }

        return c;
    }

    // Get type (component) id.
    // If type was not yet registered and automatic registration is allowed,
    // this function will also register the type.
    static entity_t id(world_t *world)
    {
#ifdef FLECS_CPP_NO_AUTO_REGISTRATION
        ecs_assert(registered(world), ECS_INVALID_OPERATION,
            "component '%s' must be registered before use",
            type_name<T>());

        flecs::entity_t c = flecs_component_ids_get(world, s_index);
        ecs_assert(c != 0, ECS_INTERNAL_ERROR, NULL);
        ecs_assert(ecs_is_alive(world, c), ECS_INVALID_OPERATION,
            "component '%s' was deleted, reregister before using",
            type_name<T>());
#else
        flecs::entity_t c = flecs_component_ids_get_alive(world, s_index);
        if (!c) {
            c = register_id(world);
        }
#endif
        return c;
    }

    // Return the size of a component.
    static size_t size() {
        ecs_assert(s_index != 0, ECS_INTERNAL_ERROR, NULL);
        return s_size;
    }

    // Return the alignment of a component.
    static size_t alignment() {
        ecs_assert(s_index != 0, ECS_INTERNAL_ERROR, NULL);
        return s_alignment;
    }

    // Was the component already registered.
    static bool registered(flecs::world_t *world) {
        ecs_assert(world != nullptr, ECS_INVALID_PARAMETER, NULL);

        if (s_index == 0) {
            return false;
        }

        if (!flecs_component_ids_get(world, s_index)) {
            return false;
        }

        return true;
    }

    // This function is only used to test cross-translation unit features. No
    // code other than test cases should invoke this function.
    static void reset() {
        s_index = 0;
        s_size = 0;
        s_alignment = 0;
        s_allow_tag = true;
    }

    static int32_t s_index;
    static size_t s_size;
    static size_t s_alignment;
    static bool s_allow_tag;
};

// Global templated variables that hold component identifier and other info
template <typename T> int32_t  type_impl<T>::s_index;
template <typename T> size_t   type_impl<T>::s_size;
template <typename T> size_t   type_impl<T>::s_alignment;
template <typename T> bool     type_impl<T>::s_allow_tag( true );

// Front facing class for implicitly registering a component & obtaining
// static component data

// Regular type
template <typename T>
struct type<T, if_not_t< is_pair<T>::value >>
    : type_impl<base_type_t<T>> { };

// Pair type
template <typename T>
struct type<T, if_t< is_pair<T>::value >>
{
    // Override id method to return id of pair
    static id_t id(world_t *world = nullptr) {
        return ecs_pair(
            type< pair_first_t<T> >::id(world),
            type< pair_second_t<T> >::id(world));
    }
};

} // namespace _

/** Untyped component class.
 * Generic base class for flecs::component.
 *
 * @ingroup cpp_components
 */
struct untyped_component : entity {
    using entity::entity;

    untyped_component() : entity() { }
    explicit untyped_component(flecs::world_t *world, flecs::entity_t id) : entity(world, id) { }
    explicit untyped_component(flecs::entity_t id) : entity(id) { }

    explicit untyped_component(flecs::world_t *world, const char *name)
    {
        world_ = world;

        ecs_entity_desc_t desc = {};
        desc.name = name;
        desc.sep = "::";
        desc.root_sep = "::";
        desc.use_low_id = true;
        id_ = ecs_entity_init(world, &desc);
    }

    explicit untyped_component(world_t *world, const char *name, const char *sep, const char *root_sep)
    {
        world_ = world;

        ecs_entity_desc_t desc = {};
        desc.name = name;
        desc.sep = sep;
        desc.root_sep = root_sep;
        desc.use_low_id = true;
        id_ = ecs_entity_init(world, &desc);
    }

protected:

flecs::type_hooks_t get_hooks() const {
    const flecs::type_hooks_t* h = ecs_get_hooks_id(world_, id_);
    if (h) {
        return *h;
    } else {
        return {};
    }
}

void set_hooks(flecs::type_hooks_t &h) {
    h.flags &= ECS_TYPE_HOOKS_ILLEGAL;
    ecs_set_hooks_id(world_, id_, &h);
}

public:

untyped_component& on_compare(
    ecs_cmp_t compare_callback)
{
    ecs_assert(compare_callback, ECS_INVALID_PARAMETER, NULL);
    flecs::type_hooks_t h = get_hooks();
    h.cmp = compare_callback;
    h.flags &= ~ECS_TYPE_HOOK_CMP_ILLEGAL;
    if(h.flags & ECS_TYPE_HOOK_EQUALS_ILLEGAL) {
        h.flags &= ~ECS_TYPE_HOOK_EQUALS_ILLEGAL;
        h.equals = NULL;
    }
    set_hooks(h);
    return *this;
}

untyped_component& on_equals(
    ecs_equals_t equals_callback)
{
    ecs_assert(equals_callback, ECS_INVALID_PARAMETER, NULL);
    flecs::type_hooks_t h = get_hooks();
    h.equals = equals_callback;
    h.flags &= ~ECS_TYPE_HOOK_EQUALS_ILLEGAL;
    set_hooks(h);
    return *this;
}

#   ifdef FLECS_META
/**
 * @file addons/cpp/mixins/meta/untyped_component.inl
 * @brief Meta component mixin.
 */

/**
 * @memberof flecs::component
 * @ingroup cpp_addons_meta
 *
 * @{
 */

private:

/** Private method that adds member to component. */
untyped_component& internal_member(
    flecs::entity_t type_id,
    flecs::entity_t unit,
    const char *name,
    int32_t count = 0,
    size_t offset = 0,
    bool use_offset = false)
{
    ecs_entity_desc_t desc = {};
    desc.name = name;
    desc.parent = id_;
    ecs_entity_t eid = ecs_entity_init(world_, &desc);
    ecs_assert(eid != 0, ECS_INTERNAL_ERROR, NULL);

    flecs::entity e(world_, eid);

    Member m = {};
    m.type = type_id;
    m.unit = unit;
    m.count = count;
    m.offset = static_cast<int32_t>(offset);
    m.use_offset = use_offset;
    e.set<Member>(m);

    return *this;
}

public:

/** Add member with unit. */
untyped_component& member(
    flecs::entity_t type_id,
    flecs::entity_t unit,
    const char *name,
    int32_t count = 0)
{
    return internal_member(type_id, unit, name, count, 0, false);
}

/** Add member with unit, count and offset. */
untyped_component& member(
    flecs::entity_t type_id,
    flecs::entity_t unit,
    const char *name,
    int32_t count,
    size_t offset)
{
    return internal_member(type_id, unit, name, count, offset, true);
}

/** Add member. */
untyped_component& member(
    flecs::entity_t type_id,
    const char* name,
    int32_t count = 0)
{
    return member(type_id, 0, name, count);
}

/** Add member with count and offset. */
untyped_component& member(
    flecs::entity_t type_id,
    const char* name,
    int32_t count,
    size_t offset)
{
    return member(type_id, 0, name, count, offset);
}

/** Add member. */
template <typename MemberType>
untyped_component& member(
    const char *name,
    int32_t count = 0)
{
    flecs::entity_t type_id = _::type<MemberType>::id(world_);
    return member(type_id, name, count);
}

/** Add member. */
template <typename MemberType>
untyped_component& member(
    const char *name,
    int32_t count,
    size_t offset)
{
    flecs::entity_t type_id = _::type<MemberType>::id(world_);
    return member(type_id, name, count, offset);
}

/** Add member with unit. */
template <typename MemberType>
untyped_component& member(
    flecs::entity_t unit,
    const char *name,
    int32_t count = 0)
{
    flecs::entity_t type_id = _::type<MemberType>::id(world_);
    return member(type_id, unit, name, count);
}

/** Add member with unit. */
template <typename MemberType>
untyped_component& member(
    flecs::entity_t unit,
    const char *name,
    int32_t count,
    size_t offset)
{
    flecs::entity_t type_id = _::type<MemberType>::id(world_);
    return member(type_id, unit, name, count, offset);
}

/** Add member with unit. */
template <typename MemberType, typename UnitType>
untyped_component& member(
    const char *name,
    int32_t count = 0)
{
    flecs::entity_t type_id = _::type<MemberType>::id(world_);
    flecs::entity_t unit_id = _::type<UnitType>::id(world_);
    return member(type_id, unit_id, name, count);
}

/** Add member with unit. */
template <typename MemberType, typename UnitType>
untyped_component& member(
    const char *name,
    int32_t count,
    size_t offset)
{
    flecs::entity_t type_id = _::type<MemberType>::id(world_);
    flecs::entity_t unit_id = _::type<UnitType>::id(world_);
    return member(type_id, unit_id, name, count, offset);
}

/** Add member using pointer-to-member. */
template <typename MemberType, typename ComponentType,
    typename RealType = typename std::remove_extent<MemberType>::type>
untyped_component& member(
    const char* name,
    const MemberType ComponentType::* ptr)
{
    flecs::entity_t type_id = _::type<RealType>::id(world_);
    size_t offset = reinterpret_cast<size_t>(&(static_cast<ComponentType*>(nullptr)->*ptr));
    return member(type_id, name, std::extent<MemberType>::value, offset);
}

/** Add member with unit using pointer-to-member. */
template <typename MemberType, typename ComponentType,
    typename RealType = typename std::remove_extent<MemberType>::type>
untyped_component& member(
    flecs::entity_t unit,
    const char* name,
    const MemberType ComponentType::* ptr)
{
    flecs::entity_t type_id = _::type<RealType>::id(world_);
    size_t offset = reinterpret_cast<size_t>(&(static_cast<ComponentType*>(nullptr)->*ptr));
    return member(type_id, unit, name, std::extent<MemberType>::value, offset);
}

/** Add member with unit using pointer-to-member. */
template <typename UnitType, typename MemberType, typename ComponentType,
    typename RealType = typename std::remove_extent<MemberType>::type>
untyped_component& member(
    const char* name,
    const MemberType ComponentType::* ptr)
{
    flecs::entity_t type_id = _::type<RealType>::id(world_);
    flecs::entity_t unit_id = _::type<UnitType>::id(world_);
    size_t offset = reinterpret_cast<size_t>(&(static_cast<ComponentType*>(nullptr)->*ptr));
    return member(type_id, unit_id, name, std::extent<MemberType>::value, offset);
}

/** Add constant. */
template <typename T = int32_t>
untyped_component& constant(
    const char *name,
    T value)
{
    ecs_add_id(world_, id_, _::type<flecs::Enum>::id(world_));

    ecs_entity_desc_t desc = {};
    desc.name = name;
    desc.parent = id_;
    ecs_entity_t eid = ecs_entity_init(world_, &desc);
    ecs_assert(eid != 0, ECS_INTERNAL_ERROR, NULL);

    ecs_set_id(world_, eid,
        ecs_pair(flecs::Constant, _::type<T>::id(world_)), sizeof(T),
        &value);

    return *this;
}

/** Add bitmask constant. */
template <typename T = uint32_t>
untyped_component& bit(
    const char *name,
    T value)
{
    ecs_add_id(world_, id_, _::type<flecs::Bitmask>::id(world_));

    ecs_entity_desc_t desc = {};
    desc.name = name;
    desc.parent = id_;
    ecs_entity_t eid = ecs_entity_init(world_, &desc);
    ecs_assert(eid != 0, ECS_INTERNAL_ERROR, NULL);

    ecs_set_id(world_, eid,
        ecs_pair(flecs::Constant, _::type<T>::id(world_)), sizeof(T),
        &value);

    return *this;
}

/** Register array metadata for component */
template <typename Elem>
untyped_component& array(
    int32_t elem_count)
{
    ecs_array_desc_t desc = {};
    desc.entity = id_;
    desc.type = _::type<Elem>::id(world_);
    desc.count = elem_count;
    ecs_array_init(world_, &desc);
    return *this;
}

/** Add member value range */
untyped_component& range(
    double min,
    double max)
{
    const flecs::member_t *m = ecs_cpp_last_member(world_, id_);
    if (!m) {
        return *this;
    }

    flecs::world w(world_);
    flecs::entity me = w.entity(m->member);

    // Don't use C++ ensure because Unreal defines a macro called ensure
    flecs::MemberRanges *mr = static_cast<flecs::MemberRanges*>(
        ecs_ensure_id(w, me, w.id<flecs::MemberRanges>(),
            sizeof(flecs::MemberRanges)));
    mr->value.min = min;
    mr->value.max = max;
    me.modified<flecs::MemberRanges>();
    return *this;
}

/** Add member warning range */
untyped_component& warning_range(
    double min,
    double max)
{
    const flecs::member_t *m = ecs_cpp_last_member(world_, id_);
    if (!m) {
        return *this;
    }

    flecs::world w(world_);
    flecs::entity me = w.entity(m->member);

    // Don't use C++ ensure because Unreal defines a macro called ensure
    flecs::MemberRanges *mr = static_cast<flecs::MemberRanges*>(
        ecs_ensure_id(w, me, w.id<flecs::MemberRanges>(),
            sizeof(flecs::MemberRanges)));
    mr->warning.min = min;
    mr->warning.max = max;
    me.modified<flecs::MemberRanges>();
    return *this;
}

/** Add member error range */
untyped_component& error_range(
    double min,
    double max)
{
    const flecs::member_t *m = ecs_cpp_last_member(world_, id_);
    if (!m) {
        return *this;
    }

    flecs::world w(world_);
    flecs::entity me = w.entity(m->member);

    // Don't use C++ ensure because Unreal defines a macro called ensure
    flecs::MemberRanges *mr = static_cast<flecs::MemberRanges*>(ecs_ensure_id(
        w, me, w.id<flecs::MemberRanges>(), sizeof(flecs::MemberRanges)));
    mr->error.min = min;
    mr->error.max = max;
    me.modified<flecs::MemberRanges>();
    return *this;
}

/** @} */

#   endif
#   ifdef FLECS_METRICS
/**
 * @file addons/cpp/mixins/meta/untyped_component.inl
 * @brief Metrics component mixin.
 */

/**
 * @memberof flecs::component
 * @ingroup cpp_addons_metrics
 *
 * @{
 */

/** Register member as metric.
 * When no explicit name is provided, this operation will derive the metric name
 * from the member name. When the member name is "value", the operation will use
 * the name of the component.
 *
 * When the brief parameter is provided, it is set on the metric as if
 * set_doc_brief is used. The brief description can be obtained with
 * get_doc_brief.
 *
 * @tparam Kind Metric kind (Counter, CounterIncrement or Gauge).
 * @param parent Parent entity of the metric (optional).
 * @param brief Description for metric (optional).
 * @param name Name of metric (optional).
 */
template <typename Kind>
untyped_component& metric(
    flecs::entity_t parent = 0,
    const char *brief = nullptr,
    const char *name = nullptr);

/** @} */

#   endif
};

/** Component class.
 * Class used to register components and component metadata.
 *
 * @ingroup cpp_components
 */
template <typename T>
struct component : untyped_component {
    /** Register a component.
     * If the component was already registered, this operation will return a handle
     * to the existing component.
     *
     * @param world The world for which to register the component.
     * @param name Optional name (overrides typename).
     * @param allow_tag If true, empty types will be registered with size 0.
     * @param id Optional id to register component with.
     */
    component(
        flecs::world_t *world,
        const char *name = nullptr,
        bool allow_tag = true,
        flecs::id_t id = 0)
    {
        world_ = world;
        id_ = _::type<T>::register_id(world, name, allow_tag, true, true, id);
    }

    /** Register on_add hook. */
    template <typename Func>
    component<T>& on_add(Func&& func) {
        using Delegate = typename _::each_delegate<typename std::decay<Func>::type, T>;
        flecs::type_hooks_t h = get_hooks();
        ecs_assert(h.on_add == nullptr, ECS_INVALID_OPERATION,
            "on_add hook is already set");
        BindingCtx *ctx = get_binding_ctx(h);
        h.on_add = Delegate::run_add;
        ctx->on_add = FLECS_NEW(Delegate)(FLECS_FWD(func));
        ctx->free_on_add = _::free_obj<Delegate>;
        set_hooks(h);
        return *this;
    }

    /** Register on_remove hook. */
    template <typename Func>
    component<T>& on_remove(Func&& func) {
        using Delegate = typename _::each_delegate<
            typename std::decay<Func>::type, T>;
        flecs::type_hooks_t h = get_hooks();
        ecs_assert(h.on_remove == nullptr, ECS_INVALID_OPERATION,
            "on_remove hook is already set");
        BindingCtx *ctx = get_binding_ctx(h);
        h.on_remove = Delegate::run_remove;
        ctx->on_remove = FLECS_NEW(Delegate)(FLECS_FWD(func));
        ctx->free_on_remove = _::free_obj<Delegate>;
        set_hooks(h);
        return *this;
    }

    /** Register on_set hook. */
    template <typename Func>
    component<T>& on_set(Func&& func) {
        using Delegate = typename _::each_delegate<
            typename std::decay<Func>::type, T>;
        flecs::type_hooks_t h = get_hooks();
        ecs_assert(h.on_set == nullptr, ECS_INVALID_OPERATION,
            "on_set hook is already set");
        BindingCtx *ctx = get_binding_ctx(h);
        h.on_set = Delegate::run_set;
        ctx->on_set = FLECS_NEW(Delegate)(FLECS_FWD(func));
        ctx->free_on_set = _::free_obj<Delegate>;
        set_hooks(h);
        return *this;
    }

    /** Register on_replace hook. */
    template <typename Func>
    component<T>& on_replace(Func&& func) {
        using Delegate = typename _::each_delegate<
            typename std::decay<Func>::type, T, T>;
        flecs::type_hooks_t h = get_hooks();
        ecs_assert(h.on_set == nullptr, ECS_INVALID_OPERATION,
            "on_set hook is already set");
        BindingCtx *ctx = get_binding_ctx(h);
        h.on_replace = Delegate::run_replace;
        ctx->on_replace = FLECS_NEW(Delegate)(FLECS_FWD(func));
        ctx->free_on_replace = _::free_obj<Delegate>;
        set_hooks(h);
        return *this;
    }

    /** Register operator compare hook. */
    using untyped_component::on_compare;
    component<T>& on_compare() {
        ecs_cmp_t handler = _::compare<T>();
        ecs_assert(handler != NULL, ECS_INVALID_OPERATION,
            "Type does not have operator> or operator< const or is inaccessible");
        on_compare(handler);
        return *this;
    }

    /** Type safe variant of compare op function */
    using cmp_hook = int(*)(const T* a, const T* b, const ecs_type_info_t *ti);
    component<T>& on_compare(cmp_hook callback) {
        on_compare(reinterpret_cast<ecs_cmp_t>(callback));
        return *this;
    }

    /** Register operator equals hook. */
    using untyped_component::on_equals;
    component<T>& on_equals() {
        ecs_equals_t handler = _::equals<T>();
        ecs_assert(handler != NULL, ECS_INVALID_OPERATION,
            "Type does not have operator== const or is inaccessible");
        on_equals(handler);
        return *this;
    }

    /** Type safe variant of equals op function */
    using equals_hook = bool(*)(const T* a, const T* b, const ecs_type_info_t *ti);
    component<T>& on_equals(equals_hook callback) {
        on_equals(reinterpret_cast<ecs_equals_t>(callback));
        return *this;
    }

#   ifdef FLECS_META

/** Register opaque type interface */
template <typename Func>
component& opaque(const Func& type_support) {
    flecs::world world(world_);
    auto ts = type_support(world);
    ts.desc.entity = _::type<T>::id(world_);
    ecs_opaque_init(world_, &ts.desc);
    return *this;
}

flecs::opaque<T> opaque(flecs::entity_t as_type) {
    return flecs::opaque<T>(world_).as_type(as_type);
}

flecs::opaque<T> opaque(flecs::entity as_type) {
    return this->opaque(as_type.id());
}

flecs::opaque<T> opaque(flecs::untyped_component as_type) {
    return this->opaque(as_type.id());
}

/** Return opaque type builder for collection type */
template <typename ElemType>
flecs::opaque<T, ElemType> opaque(flecs::id_t as_type) {
    return flecs::opaque<T, ElemType>(world_).as_type(as_type);
}

/** Add constant. */
component<T>& constant(const char *name, T value) {
    using U = typename std::underlying_type<T>::type;

    ecs_add_id(world_, id_, _::type<flecs::Enum>::id(world_));

    ecs_entity_desc_t desc = {};
    desc.name = name;
    desc.parent = id_;
    ecs_entity_t eid = ecs_entity_init(world_, &desc);
    ecs_assert(eid != 0, ECS_INTERNAL_ERROR, NULL);

    flecs::id_t pair = ecs_pair(flecs::Constant, _::type<U>::id(world_));
    U *ptr = static_cast<U*>(ecs_ensure_id(world_, eid, pair, sizeof(U)));
    *ptr = static_cast<U>(value);
    ecs_modified_id(world_, eid, pair);

    // If we're not using automatic enum reflection, manually set static data
    #ifdef FLECS_CPP_NO_ENUM_REFLECTION
    auto et = enum_type<T>(world_);
    et.register_constant(world_, static_cast<U>(value), eid);
    #endif

    return *this;
}

#   endif

private:
    using BindingCtx = _::component_binding_ctx;

    BindingCtx* get_binding_ctx(flecs::type_hooks_t& h){
        BindingCtx *result = static_cast<BindingCtx*>(h.binding_ctx);
        if (!result) {
            result = FLECS_NEW(BindingCtx);
            h.binding_ctx = result;
            h.binding_ctx_free = _::free_obj<BindingCtx>;
        }
        return result;
    }
};

}

/** @} */

/**
 * @file addons/cpp/type.hpp
 * @brief Utility functions for id vector.
 */

#pragma once

export namespace flecs {

/**
 * @defgroup cpp_types Types
 * @ingroup cpp_core
 * @brief Type operations.
 *
 * @{
 */

/** Type class.
 * A type is a vector of component ids which can be requested from entities or tables.
 */
struct type {
    type() : world_(nullptr), type_(nullptr) { }

    type(world_t *world, const type_t *t)
        : world_(world)
        , type_(t) { }

    /** Convert type to comma-separated string */
    flecs::string str() const {
        return flecs::string(ecs_type_str(world_, type_));
    }

    /** Return number of ids in type */
    int32_t count() const {
        if (!type_) {
            return 0;
        }
        return type_->count;
    }

    /** Return pointer to array. */
    flecs::id_t* array() const {
        if (!type_) {
            return nullptr;
        }
        return type_->array;
    }

    /** Get id at specified index in type */
    flecs::id get(int32_t index) const {
        ecs_assert(type_ != NULL, ECS_INVALID_PARAMETER, NULL);
        ecs_assert(type_->count > index, ECS_OUT_OF_RANGE, NULL);
        if (!type_) {
            return flecs::id();
        }
        return flecs::id(world_, type_->array[index]);
    }

    const flecs::id_t* begin() const {
        if (type_ && type_->count) {
            return type_->array;
        } else {
            return &empty_;
        }
    }

    const flecs::id_t* end() const {
        if (type_ && type_->count) {
            return &type_->array[type_->count];
        } else {
            return &empty_;
        }
    }

    /** Implicit conversion to type_t */
    operator const type_t*() const {
        return type_;
    }
private:
    world_t *world_;
    const type_t *type_;
    flecs::id_t empty_;
};

/** #} */

}

/**
 * @file addons/cpp/table.hpp
 * @brief Direct access to table data.
 */

#pragma once

export namespace flecs {

/**
 * @defgroup cpp_tables Tables
 * @ingroup cpp_core
 * Table operations.
 *
 * @{
 */

struct table {
    table() : world_(nullptr), table_(nullptr) { }

    table(world_t *world, table_t *t)
        : world_(world)
        , table_(t) { }

    virtual ~table() { }

    /** Convert table type to string. */
    flecs::string str() const {
        return flecs::string(ecs_table_str(world_, table_));
    }

    /** Get table type. */
    flecs::type type() const {
        return flecs::type(world_, ecs_table_get_type(table_));
    }

    /** Get table count. */
    int32_t count() const {
        return ecs_table_count(table_);
    }

    /** Get number of allocated elements in table. */
    int32_t size() const {
        return ecs_table_size(table_);
    }

    /** Get array with entity ids. */
    const flecs::entity_t* entities() const {
        return ecs_table_entities(table_);
    }

    /** Delete entities in table. */
    void clear_entities() const {
        ecs_table_clear_entities(world_, table_);
    }

    /** Find type index for (component) id.
     *
     * @param id The (component) id.
     * @return The index of the id in the table type, -1 if not found/
     */
    int32_t type_index(flecs::id_t id) const {
        return ecs_table_get_type_index(world_, table_, id);
    }

    /** Find type index for type.
     *
     * @tparam T The type.
     * @return True if the table has the type, false if not.
     */
    template <typename T>
    int32_t type_index() const {
        return type_index(_::type<T>::id(world_));
    }

    /** Find type index for pair.
     * @param first First element of pair.
     * @param second Second element of pair.
     * @return True if the table has the pair, false if not.
     */
    int32_t type_index(flecs::entity_t first, flecs::entity_t second) const {
        return type_index(ecs_pair(first, second));
    }

    /** Find type index for pair.
     * @tparam First First element of pair.
     * @param second Second element of pair.
     * @return True if the table has the pair, false if not.
     */
    template <typename First>
    int32_t type_index(flecs::entity_t second) const {
        return type_index(_::type<First>::id(world_), second);
    }

    /** Find type index for pair.
     * @tparam First First element of pair.
     * @tparam Second Second element of pair.
     * @return True if the table has the pair, false if not.
     */
    template <typename First, typename Second>
    int32_t type_index() const {
        return type_index<First>(_::type<Second>::id(world_));
    }

    /** Find column index for (component) id.
     *
     * @param id The (component) id.
     * @return The index of the id in the table type, -1 if not found/
     */
    int32_t column_index(flecs::id_t id) const {
        return ecs_table_get_column_index(world_, table_, id);
    }

    /** Find column index for type.
     *
     * @tparam T The type.
     * @return True if the table has the type, false if not.
     */
    template <typename T>
    int32_t column_index() const {
        return column_index(_::type<T>::id(world_));
    }

    /** Find column index for pair.
     * @param first First element of pair.
     * @param second Second element of pair.
     * @return True if the table has the pair, false if not.
     */
    int32_t column_index(flecs::entity_t first, flecs::entity_t second) const {
        return column_index(ecs_pair(first, second));
    }

    /** Find column index for pair.
     * @tparam First First element of pair.
     * @param second Second element of pair.
     * @return True if the table has the pair, false if not.
     */
    template <typename First>
    int32_t column_index(flecs::entity_t second) const {
        return column_index(_::type<First>::id(world_), second);
    }

    /** Find column index for pair.
     * @tparam First First element of pair.
     * @tparam Second Second element of pair.
     * @return True if the table has the pair, false if not.
     */
    template <typename First, typename Second>
    int32_t column_index() const {
        return column_index<First>(_::type<Second>::id(world_));
    }

    /** Test if table has (component) id.
     *
     * @param id The (component) id.
     * @return True if the table has the id, false if not.
     */
    bool has(flecs::id_t id) const {
        return type_index(id) != -1;
    }

    /** Test if table has the type.
     *
     * @tparam T The type.
     * @return True if the table has the type, false if not.
     */
    template <typename T>
    bool has() const {
        return type_index<T>() != -1;
    }

    /** Test if table has the pair.
     *
     * @param first First element of pair.
     * @param second Second element of pair.
     * @return True if the table has the pair, false if not.
     */
    bool has(flecs::entity_t first, flecs::entity_t second) const {
        return type_index(first, second) != -1;
    }

    /** Test if table has the pair.
     *
     * @tparam First First element of pair.
     * @param second Second element of pair.
     * @return True if the table has the pair, false if not.
     */
    template <typename First>
    bool has(flecs::entity_t second) const {
        return type_index<First>(second) != -1;
    }

    /** Test if table has the pair.
     *
     * @tparam First First element of pair.
     * @tparam Second Second element of pair.
     * @return True if the table has the pair, false if not.
     */
    template <typename First, typename Second>
    bool has() const {
        return type_index<First, Second>() != -1;
    }

    /** Get pointer to component array by column index.
     *
     * @param index The column index.
     * @return Pointer to the column, NULL if not a component.
     */
    virtual void* get_column(int32_t index) const {
        return ecs_table_get_column(table_, index, 0);
    }


    /* get */

    /** Get pointer to component array by component.
     *
     * @param id The component id.
     * @return Pointer to the column, NULL if not found.
     */
    void* try_get(flecs::id_t id) const {
        int32_t index = column_index(id);
        if (index == -1) {
            return NULL;
        }
        return get_column(index);
    }

    /** Get pointer to component array by pair.
     *
     * @param first The first element of the pair.
     * @param second The second element of the pair.
     * @return Pointer to the column, NULL if not found.
     */
    void* try_get(flecs::entity_t first, flecs::entity_t second) const {
        return try_get(ecs_pair(first, second));
    }

    /** Get pointer to component array by component.
     *
     * @tparam T The component.
     * @return Pointer to the column, NULL if not found.
     */
    template <typename T, if_t< is_actual<T>::value > = 0>
    T* try_get() const {
        return static_cast<T*>(try_get(_::type<T>::id(world_)));
    }

    /** Get pointer to component array by component.
     *
     * @tparam T The component.
     * @return Pointer to the column, NULL if not found.
     */
    template <typename T, typename A = actual_type_t<T>,
        if_t< flecs::is_pair<T>::value > = 0>
    A* try_get() const {
        return static_cast<A*>(try_get(_::type<T>::id(world_)));
    }

    /** Get pointer to component array by pair.
     *
     * @tparam First The first element of the pair.
     * @param second The second element of the pair.
     * @return Pointer to the column, NULL if not found.
     */
    template <typename First>
    First* try_get(flecs::entity_t second) const {
        return static_cast<First*>(try_get(_::type<First>::id(world_), second));
    }


    /* get */

    /** Get pointer to component array by component.
     *
     * @param id The component id.
     * @return Pointer to the column, NULL if not found.
     */
    void* get(flecs::id_t id) const {
        int32_t index = column_index(id);
        if (index == -1) {
            return NULL;
        }
        void *r = get_column(index);
        ecs_assert(r != nullptr, ECS_INVALID_OPERATION,
            "invalid get_mut: table does not have component (use try_get)");
        return r;
    }

    /** Get pointer to component array by pair.
     *
     * @param first The first element of the pair.
     * @param second The second element of the pair.
     * @return Pointer to the column, NULL if not found.
     */
    void* get(flecs::entity_t first, flecs::entity_t second) const {
        return get(ecs_pair(first, second));
    }

    /** Get pointer to component array by component.
     *
     * @tparam T The component.
     * @return Pointer to the column, NULL if not found.
     */
    template <typename T, if_t< is_actual<T>::value > = 0>
    T* get() const {
        return static_cast<T*>(get(_::type<T>::id(world_)));
    }

    /** Get pointer to component array by component.
     *
     * @tparam T The component.
     * @return Pointer to the column, NULL if not found.
     */
    template <typename T, typename A = actual_type_t<T>,
        if_t< flecs::is_pair<T>::value > = 0>
    A* get() const {
        return static_cast<A*>(get(_::type<T>::id(world_)));
    }

    /** Get pointer to component array by pair.
     *
     * @tparam First The first element of the pair.
     * @param second The second element of the pair.
     * @return Pointer to the column, NULL if not found.
     */
    template <typename First>
    First* get(flecs::entity_t second) const {
        return static_cast<First*>(get(_::type<First>::id(world_), second));
    }


    /** Get pointer to component array by pair.
     *
     * @tparam First The first element of the pair.
     * @tparam Second The second element of the pair.
     * @return Pointer to the column, NULL if not found.
     */
    template <typename First, typename Second, typename P = flecs::pair<First, Second>,
        typename A = actual_type_t<P>, if_not_t< flecs::is_pair<First>::value> = 0>
    A* get() const {
        return static_cast<A*>(get<First>(_::type<Second>::id(world_)));
    }

    /** Get column size. */
    size_t column_size(int32_t index) const {
        return ecs_table_get_column_size(table_, index);
    }

    /** Get depth for given relationship.
     *
     * @param rel The relationship.
     * @return The depth.
     */
    int32_t depth(flecs::entity_t rel) const  {
        return ecs_table_get_depth(world_, table_, rel);
    }

    /** Get depth for given relationship.
     *
     * @tparam Rel The relationship.
     * @return The depth.
     */
    template <typename Rel>
    int32_t depth() const {
        return depth(_::type<Rel>::id(world_));
    }

    /** Get table records array */
    ecs_table_records_t records() const {
        return flecs_table_records(table_);
    }

    /** Get table id. */
    uint64_t id() const {
        return flecs_table_id(table_);
    }

    /** Lock table. */
    void lock() const {
        ecs_table_lock(world_, table_);
    }

    /** Unlock table. */
    void unlock() const {
        ecs_table_unlock(world_, table_);
    }

    /** Check if table has flags. */
    bool has_flags(ecs_flags32_t flags) const {
        return ecs_table_has_flags(table_, flags);
    }

    /** Get table.
     *
     * @return The table.
     */
    table_t* get_table() const {
        return table_;
    }

    /* Implicit conversion to table_t */
    operator table_t*() const {
        return table_;
    }

protected:
    world_t *world_;
    table_t *table_;
};

struct table_range : table {
    table_range()
        : table()
        , offset_(0)
        , count_(0) { }

    table_range(world_t *world, table_t *t, int32_t offset, int32_t count)
        : table(world, t)
        , offset_(offset)
        , count_(count) { }

    int32_t offset() const {
        return offset_;
    }

    int32_t count() const {
        return count_;
    }

    /** Get pointer to component array by column index.
     *
     * @param index The column index.
     * @return Pointer to the column, NULL if not a component.
     */
    void* get_column(int32_t index) const override {
        return ecs_table_get_column(table_, index, offset_);
    }

private:
    int32_t offset_ = 0;
    int32_t count_ = 0;
};

/** @} */

}

/**
 * @file addons/cpp/utils/iterable.hpp
 * @brief Base class for iterable objects, like queries.
 */

export namespace flecs {

template <typename ... Components>
struct iter_iterable;

template <typename ... Components>
struct page_iterable;

template <typename ... Components>
struct worker_iterable;

template <typename ... Components>
struct iterable {

    /** Each iterator.
     * The "each" iterator accepts a function that is invoked for each matching
     * entity. The following function signatures are valid:
     *  - func(flecs::entity e, Components& ...)
     *  - func(flecs::iter& it, size_t index, Components& ....)
     *  - func(Components& ...)
     */
    template <typename Func>
    void each(Func&& func) const {
        ecs_iter_t it = this->get_iter(nullptr);
        ecs_iter_next_action_t next = this->next_action();
        while (next(&it)) {
            _::each_delegate<Func, Components...>(func).invoke(&it);
        }
    }

    /** Run iterator.
     * The "each" iterator accepts a function that is invoked once for a query
     * with a valid iterator. The following signature is valid:
     *  - func(flecs::iter&)
     */
    template <typename Func>
    void run(Func&& func) const {
        ecs_iter_t it = this->get_iter(nullptr);
        _::run_delegate<Func>(func).invoke(&it);
    }

    template <typename Func>
    flecs::entity find(Func&& func) const {
        ecs_iter_t it = this->get_iter(nullptr);
        ecs_iter_next_action_t next = this->next_action();

        flecs::entity result;
        while (!result && next(&it)) {
            result = _::find_delegate<Func, Components...>(func).invoke(&it);
        }

        if (result) {
            ecs_iter_fini(&it);
        }

        return result;
    }

    /** Create iterator.
     * Create an iterator object that can be modified before iterating.
     */
    iter_iterable<Components...> iter(flecs::world_t *world = nullptr) const;

    /** Create iterator.
     * Create an iterator object that can be modified before iterating.
     */
    iter_iterable<Components...> iter(flecs::iter& iter) const;

    /** Create iterator.
     * Create an iterator object that can be modified before iterating.
     */
    iter_iterable<Components...> iter(flecs::entity e) const;

    /** Page iterator.
     * Create an iterator that limits the returned entities with offset/limit.
     *
     * @param offset How many entities to skip.
     * @param limit The maximum number of entities to return.
     * @return Iterable that can be iterated with each/iter.
     */
    page_iterable<Components...> page(int32_t offset, int32_t limit);

    /** Worker iterator.
     * Create an iterator that divides the number of matched entities across
     * a number of resources.
     *
     * @param index The index of the current resource.
     * @param count The total number of resources to divide entities between.
     * @return Iterable that can be iterated with each/iter.
     */
    worker_iterable<Components...> worker(int32_t index, int32_t count);

    /** Return number of entities matched by iterable. */
    int32_t count() const {
        return this->iter().count();
    }

    /** Return whether iterable has any matches. */
    bool is_true() const {
        return this->iter().is_true();
    }

    /** Return first entity matched by iterable. */
    flecs::entity first() const {
        return this->iter().first();
    }

    iter_iterable<Components...> set_var(int var_id, flecs::entity_t value) const {
        return this->iter().set_var(var_id, value);
    }

    iter_iterable<Components...> set_var(const char *name, flecs::entity_t value) const {
        return this->iter().set_var(name, value);
    }

    iter_iterable<Components...> set_var(const char *name, flecs::table_t *value) const {
        return this->iter().set_var(name, value);
    }

    iter_iterable<Components...> set_var(const char *name, ecs_table_range_t value) const {
        return this->iter().set_var(name, value);
    }

    iter_iterable<Components...> set_var(const char *name, flecs::table_range value) const {
        return this->iter().set_var(name, value);
    }

    // Limit results to tables with specified group id (grouped queries only)
    iter_iterable<Components...> set_group(uint64_t group_id) const {
        return this->iter().set_group(group_id);
    }

    // Limit results to tables with specified group id (grouped queries only)
    template <typename Group>
    iter_iterable<Components...> set_group() const {
        return this->iter().template set_group<Group>();
    }

    virtual ~iterable() { }
protected:
    friend iter_iterable<Components...>;
    friend page_iterable<Components...>;
    friend worker_iterable<Components...>;

    virtual ecs_iter_t get_iter(flecs::world_t *stage) const = 0;
    virtual ecs_iter_next_action_t next_action() const = 0;
};

template <typename ... Components>
struct iter_iterable final : iterable<Components...> {
    template <typename Iterable>
    iter_iterable(Iterable *it, flecs::world_t *world)
    {
        it_ = it->get_iter(world);
        next_ = it->next_action();
        next_each_ = it->next_action();
        ecs_assert(next_ != nullptr, ECS_INTERNAL_ERROR, NULL);
        ecs_assert(next_each_ != nullptr, ECS_INTERNAL_ERROR, NULL);
    }

    iter_iterable<Components...>& set_var(int var_id, flecs::entity_t value) {
        ecs_assert(var_id != -1, ECS_INVALID_PARAMETER, 0);
        ecs_iter_set_var(&it_, var_id, value);
        return *this;
    }

    iter_iterable<Components...>& set_var(const char *name, flecs::entity_t value) {
        int var_id = ecs_query_find_var(it_.query, name);
        ecs_assert(var_id != -1, ECS_INVALID_PARAMETER, name);
        ecs_iter_set_var(&it_, var_id, value);
        return *this;
    }

    iter_iterable<Components...>& set_var(const char *name, flecs::table_t *value) {
        int var_id = ecs_query_find_var(it_.query, name);
        ecs_assert(var_id != -1, ECS_INVALID_PARAMETER, name);
        ecs_iter_set_var_as_table(&it_, var_id, value);
        return *this;
    }

    iter_iterable<Components...>& set_var(const char *name, ecs_table_range_t value) {
        int var_id = ecs_query_find_var(it_.query, name);
        ecs_assert(var_id != -1, ECS_INVALID_PARAMETER, name);
        ecs_iter_set_var_as_range(&it_, var_id, &value);
        return *this;
    }

    iter_iterable<Components...>& set_var(const char *name, flecs::table_range value) {
        ecs_table_range_t range;
        range.table = value.get_table();
        range.offset = value.offset();
        range.count = value.count();
        return set_var(name, range);
    }

#   ifdef FLECS_JSON
/**
 * @file addons/cpp/mixins/json/iterable.inl
 * @brief JSON iterable mixin.
 */

/** Serialize iterator result to JSON.
 *
 * @memberof flecs::iter
 * @ingroup cpp_addons_json
 */
flecs::string to_json(flecs::iter_to_json_desc_t *desc = nullptr) {
    char *json = ecs_iter_to_json(&it_, desc);
    return flecs::string(json);
}

#   endif

    // Return total number of entities in result.
    int32_t count() {
        int32_t result = 0;
        while (next_each_(&it_)) {
            result += it_.count;
        }
        return result;
    }

    // Returns true if iterator yields at least once result.
    bool is_true() {
        bool result = next_each_(&it_);
        if (result) {
            ecs_iter_fini(&it_);
        }
        return result;
    }

    // Return first matching entity.
    flecs::entity first() {
        flecs::entity result;
        if (next_each_(&it_) && it_.count) {
            result = flecs::entity(it_.world, it_.entities[0]);
            ecs_iter_fini(&it_);
        }
        return result;
    }

    // Limit results to tables with specified group id (grouped queries only)
    iter_iterable<Components...>& set_group(uint64_t group_id) {
        ecs_iter_set_group(&it_, group_id);
        return *this;
    }

    // Limit results to tables with specified group id (grouped queries only)
    template <typename Group>
    iter_iterable<Components...>& set_group() {
        ecs_iter_set_group(&it_, _::type<Group>().id(it_.real_world));
        return *this;
    }

protected:
    ecs_iter_t get_iter(flecs::world_t *world) const override {
        if (world) {
            ecs_iter_t result = it_;
            result.world = world;
            return result;
        }
        return it_;
    }

    ecs_iter_next_action_t next_action() const override {
        return next_;
    }

private:
    ecs_iter_t it_;
    ecs_iter_next_action_t next_;
    ecs_iter_next_action_t next_each_;
};

template <typename ... Components>
iter_iterable<Components...> iterable<Components...>::iter(flecs::world_t *world) const
{
    return iter_iterable<Components...>(this, world);
}

template <typename ... Components>
iter_iterable<Components...> iterable<Components...>::iter(flecs::iter& it) const
{
    return iter_iterable<Components...>(this, it.world());
}

template <typename ... Components>
iter_iterable<Components...> iterable<Components...>::iter(flecs::entity e) const
{
    return iter_iterable<Components...>(this, e.world());
}

template <typename ... Components>
struct page_iterable final : iterable<Components...> {
    template <typename Iterable>
    page_iterable(int32_t offset, int32_t limit, Iterable *it)
        : offset_(offset)
        , limit_(limit)
    {
        chain_it_ = it->get_iter(nullptr);
    }

protected:
    ecs_iter_t get_iter(flecs::world_t*) const {
        return ecs_page_iter(&chain_it_, offset_, limit_);
    }

    ecs_iter_next_action_t next_action() const {
        return ecs_page_next;
    }

private:
    ecs_iter_t chain_it_;
    int32_t offset_;
    int32_t limit_;
};

template <typename ... Components>
page_iterable<Components...> iterable<Components...>::page(
    int32_t offset,
    int32_t limit)
{
    return page_iterable<Components...>(offset, limit, this);
}

template <typename ... Components>
struct worker_iterable final : iterable<Components...> {
    worker_iterable(int32_t offset, int32_t limit, iterable<Components...> *it)
        : offset_(offset)
        , limit_(limit)
    {
        chain_it_ = it->get_iter(nullptr);
    }

protected:
    ecs_iter_t get_iter(flecs::world_t*) const {
        return ecs_worker_iter(&chain_it_, offset_, limit_);
    }

    ecs_iter_next_action_t next_action() const {
        return ecs_worker_next;
    }

private:
    ecs_iter_t chain_it_;
    int32_t offset_;
    int32_t limit_;
};

template <typename ... Components>
worker_iterable<Components...> iterable<Components...>::worker(
    int32_t index,
    int32_t count)
{
    return worker_iterable<Components...>(index, count, this);
}

}


// Mixin implementations
/**
 * @file addons/cpp/mixins/id/impl.hpp
 * @brief Id class implementation.
 */

#pragma once

export namespace flecs {

inline flecs::entity id::entity() const {
    ecs_assert(!is_pair(), ECS_INVALID_OPERATION, NULL);
    ecs_assert(!flags(), ECS_INVALID_OPERATION, NULL);
    return flecs::entity(world_, id_);
}

inline flecs::entity id::flags() const {
    return flecs::entity(world_, id_ & ECS_ID_FLAGS_MASK);
}

inline flecs::entity id::first() const {
    ecs_assert(is_pair(), ECS_INVALID_OPERATION, NULL);

    flecs::entity_t e = ECS_PAIR_FIRST(id_);
    if (world_) {
        return flecs::entity(world_, ecs_get_alive(world_, e));
    } else {
        return flecs::entity(e);
    }
}

inline flecs::entity id::second() const {
    flecs::entity_t e = ECS_PAIR_SECOND(id_);
    if (world_) {
        return flecs::entity(world_, ecs_get_alive(world_, e));
    } else {
        return flecs::entity(e);
    }
}

inline flecs::entity id::add_flags(flecs::id_t flags) const {
    return flecs::entity(world_, id_ | flags);
}

inline flecs::entity id::remove_flags(flecs::id_t flags) const {
    (void)flags;
    ecs_assert((id_ & ECS_ID_FLAGS_MASK) == flags, ECS_INVALID_PARAMETER, NULL);
    return flecs::entity(world_, id_ & ECS_COMPONENT_MASK);
}

inline flecs::entity id::remove_flags() const {
    return flecs::entity(world_, id_ & ECS_COMPONENT_MASK);
}

inline flecs::entity id::remove_generation() const {
    return flecs::entity(world_, static_cast<uint32_t>(id_));
}

inline flecs::world id::world() const {
    return flecs::world(world_);
}

inline flecs::entity id::type_id() const {
    return flecs::entity(world_, ecs_get_typeid(world_, id_));
}


// Id mixin implementation

template <typename T>
inline flecs::id world::id() const {
    return flecs::id(world_, _::type<T>::id(world_));
}

template <typename ... Args>
inline flecs::id world::id(Args&&... args) const {
    return flecs::id(world_, FLECS_FWD(args)...);
}

template <typename First, typename Second>
inline flecs::id world::pair() const {
    return flecs::id(
        world_,
        ecs_pair(
            _::type<First>::id(world_),
            _::type<Second>::id(world_)));
}

template <typename First>
inline flecs::id world::pair(entity_t o) const {
    ecs_assert(!ECS_IS_PAIR(o), ECS_INVALID_PARAMETER,
        "cannot create nested pairs");

    return flecs::id(
        world_,
        ecs_pair(
            _::type<First>::id(world_),
            o));
}

inline flecs::id world::pair(entity_t r, entity_t o) const {
    ecs_assert(!ECS_IS_PAIR(r) && !ECS_IS_PAIR(o), ECS_INVALID_PARAMETER,
        "cannot create nested pairs");

    return flecs::id(
        world_,
        ecs_pair(r, o));
}

}

/**
 * @file addons/cpp/mixins/entity/impl.hpp
 * @brief Entity implementation.
 */

#pragma once

export namespace flecs {

inline untyped_ref::untyped_ref(flecs::entity entity, flecs::id_t id)
    : untyped_ref(entity.world(), entity.id(), id) { }

inline flecs::entity untyped_ref::entity() const {
        return flecs::entity(world_, ref_.entity);
}

template <typename T>
flecs::ref<T>::ref(flecs::entity entity, flecs::id_t id)
        : ref(entity.world(), entity.id(), id) { }

template <typename Self>
template <typename Func>
inline const Self& entity_builder<Self>::insert(const Func& func) const  {
    _::entity_with_delegate<Func>::invoke_ensure(
        this->world_, this->id_, func);
    return to_base();
}

template<typename Enum>
inline Enum entity_view::get_constant() const {
    flecs::entity tgt = this->target<Enum>();
    return tgt.to_constant<Enum>();
}

template<typename First>
inline flecs::entity entity_view::target(int32_t index) const
{
    return flecs::entity(world_,
        ecs_get_target(world_, id_, _::type<First>::id(world_), index));
}

inline flecs::entity entity_view::target(
    flecs::entity_t relationship,
    int32_t index) const
{
    return flecs::entity(world_,
        ecs_get_target(world_, id_, relationship, index));
}

inline flecs::entity entity_view::target_for(
    flecs::entity_t relationship,
    flecs::id_t id) const
{
    return flecs::entity(world_,
        ecs_get_target_for_id(world_, id_, relationship, id));
}

template <typename T>
inline flecs::entity entity_view::target_for(flecs::entity_t relationship) const {
    return target_for(relationship, _::type<T>::id(world_));
}

template <typename First, typename Second>
inline flecs::entity entity_view::target_for(flecs::entity_t relationship) const {
    return target_for(relationship, _::type<First, Second>::id(world_));
}

inline flecs::entity entity_view::parent() const {
    return flecs::entity(world_, ecs_get_parent(world_, id_));
}

inline flecs::entity entity_view::mut(const flecs::world& stage) const {
    ecs_assert(!stage.is_readonly(), ECS_INVALID_PARAMETER,
        "cannot use readonly world/stage to create mutable handle");
    return flecs::entity(id_).set_stage(stage.c_ptr());
}

inline flecs::entity entity_view::mut(const flecs::iter& it) const {
    ecs_assert(!it.world().is_readonly(), ECS_INVALID_PARAMETER,
        "cannot use iterator created for readonly world/stage to create mutable handle");
    return flecs::entity(id_).set_stage(it.world().c_ptr());
}

inline flecs::entity entity_view::mut(const flecs::entity_view& e) const {
    ecs_assert(!e.world().is_readonly(), ECS_INVALID_PARAMETER,
        "cannot use entity created for readonly world/stage to create mutable handle");
    return flecs::entity(id_).set_stage(e.world_);
}

inline flecs::entity entity_view::set_stage(world_t *stage) {
    return flecs::entity(stage, id_);
}

inline flecs::type entity_view::type() const {
    return flecs::type(world_, ecs_get_type(world_, id_));
}

inline flecs::table entity_view::table() const {
    return flecs::table(world_, ecs_get_table(world_, id_));
}

inline flecs::table_range entity_view::range() const {
    ecs_record_t *r = ecs_record_find(world_, id_);
    if (r) {
        return flecs::table_range(world_, r->table,
            ECS_RECORD_TO_ROW(r->row), 1);
    }
    return flecs::table_range();
}

template <typename Func>
inline void entity_view::each(const Func& func) const {
    const ecs_type_t *type = ecs_get_type(world_, id_);
    if (!type) {
        return;
    }

    const ecs_id_t *ids = type->array;
    int32_t count = type->count;

    for (int i = 0; i < count; i ++) {
        ecs_id_t id = ids[i];
        flecs::id ent(world_, id);
        func(ent);
    }
}

template <typename Func>
inline void entity_view::each(flecs::id_t pred, flecs::id_t obj, const Func& func) const {
    flecs::world_t *real_world = const_cast<flecs::world_t*>(
        ecs_get_world(world_));

    const ecs_table_t *table = ecs_get_table(world_, id_);
    if (!table) {
        return;
    }

    const ecs_type_t *type = ecs_table_get_type(table);
    if (!type) {
        return;
    }

    flecs::id_t pattern = pred;
    if (obj) {
        pattern = ecs_pair(pred, obj);
    }

    int32_t cur = 0;
    id_t *ids = type->array;

    while (-1 != (cur = ecs_search_offset(real_world, table, cur, pattern, 0)))
    {
        flecs::id ent(world_, ids[cur]);
        func(ent);
        cur ++;
    }
}

template <typename Func>
inline void entity_view::each(const flecs::entity_view& rel, const Func& func) const {
    return this->each(rel, flecs::Wildcard, [&](flecs::id id) {
        flecs::entity obj = id.second();
        func(obj);
    });
}

template <typename Func, if_t< is_callable<Func>::value > >
inline bool entity_view::get(const Func& func) const {
    return _::entity_with_delegate<Func>::invoke_get(world_, id_, func);
}

inline flecs::entity entity_view::lookup(const char *path, bool search_path) const {
    ecs_assert(id_ != 0, ECS_INVALID_PARAMETER, "invalid lookup from null handle");
    auto id = ecs_lookup_path_w_sep(world_, id_, path, "::", "::", search_path);
    return flecs::entity(world_, id);
}

inline flecs::entity entity_view::clone(bool copy_value, flecs::entity_t dst_id) const {
    if (!dst_id) {
        dst_id = ecs_new(world_);
    }

    flecs::entity dst = flecs::entity(world_, dst_id);
    ecs_clone(world_, dst_id, id_, copy_value);
    return dst;
}

// Entity mixin implementation
template <typename... Args>
inline flecs::entity world::entity(Args &&... args) const {
    return flecs::entity(world_, FLECS_FWD(args)...);
}

template <typename E, if_t< is_enum<E>::value >>
inline flecs::id world::id(E value) const {
    flecs::entity_t constant = enum_type<E>(world_).entity(value);
    return flecs::id(world_, constant);
}

template <typename E, if_t< is_enum<E>::value >>
inline flecs::entity world::entity(E value) const {
    flecs::entity_t constant = enum_type<E>(world_).entity(value);
    return flecs::entity(world_, constant);
}

template <typename T>
inline flecs::entity world::entity(const char *name) const {
    return flecs::entity(world_, _::type<T>::register_id(world_, name, true, false) );
}

template <typename... Args>
inline flecs::entity world::prefab(Args &&... args) const {
    flecs::entity result = flecs::entity(world_, FLECS_FWD(args)...);
    result.add(flecs::Prefab);
    return result;
}

template <typename T>
inline flecs::entity world::prefab(const char *name) const {
    flecs::entity result = this->entity<T>(name);
    result.add(flecs::Prefab);
    return result;
}

}

/**
 * @file addons/cpp/mixins/component/impl.hpp
 * @brief Component mixin implementation
 */

#pragma once

export namespace flecs {

template <typename T, typename... Args>
inline flecs::component<T> world::component(Args &&... args) const {
    return flecs::component<T>(world_, FLECS_FWD(args)...);
}

template <typename... Args>
inline flecs::untyped_component world::component(Args &&... args) const {
    return flecs::untyped_component(world_, FLECS_FWD(args)...);
}

} // export namespace flecs

/**
 * @file addons/cpp/mixins/term/impl.hpp
 * @brief Term implementation.
 */

#pragma once

/**
 * @file addons/cpp/mixins/term/builder_i.hpp
 * @brief Term builder interface.
 */

#pragma once

/**
 * @file addons/cpp/utils/signature.hpp
 * @brief Compile time utilities for deriving query attributes from param pack.
 */

#pragma once

export namespace flecs {
namespace _ {

    template <typename T, if_t< is_const_p<T>::value > = 0>
    constexpr flecs::inout_kind_t type_to_inout() {
        return flecs::In;
    }

    template <typename T, if_t< is_reference<T>::value > = 0>
    constexpr flecs::inout_kind_t type_to_inout() {
        return flecs::InOut;
    }

    template <typename T, if_not_t<
        is_const_p<T>::value || is_reference<T>::value > = 0>
    constexpr flecs::inout_kind_t type_to_inout() {
        return flecs::InOutDefault;
    }

    template <typename T, if_t< is_pointer<T>::value > = 0>
    constexpr flecs::oper_kind_t type_to_oper() {
        return flecs::Optional;
    }

    template <typename T, if_not_t< is_pointer<T>::value > = 0>
    constexpr flecs::oper_kind_t type_to_oper() {
        return flecs::And;
    }

    template <typename ... Components>
    struct sig {
        sig(flecs::world_t *world)
            : world_(world)
            , ids({ (_::type<remove_pointer_t<Components>>::id(world))... })
            , inout ({ (type_to_inout<Components>())... })
            , oper ({ (type_to_oper<Components>())... })
        { }

        flecs::world_t *world_;
        flecs::array<flecs::id_t, sizeof...(Components)> ids;
        flecs::array<flecs::inout_kind_t, sizeof...(Components)> inout;
        flecs::array<flecs::oper_kind_t, sizeof...(Components)> oper;

        template <typename Builder>
        void populate(const Builder& b) {
            size_t i = 0;
            for (auto id : ids) {
                b->with(id).inout(inout[i]).oper(oper[i]);
                i ++;
            }
        }
    };

} // namespace _
} // export namespace flecs

#include <stdio.h>

export namespace flecs
{

/** Term identifier builder.
 * A term identifier describes a single identifier in a term. Identifier
 * descriptions can reference entities by id, name or by variable, which means
 * the entity will be resolved when the term is evaluated.
 *
 * @ingroup cpp_core_queries
 */
template<typename Base>
struct term_ref_builder_i {
    term_ref_builder_i() : term_ref_(nullptr) { }

    virtual ~term_ref_builder_i() { }

    /* The self flag indicates the term identifier itself is used */
    Base& self() {
        this->assert_term_ref();
        term_ref_->id |= flecs::Self;
        return *this;
    }

    /* Specify value of identifier by id */
    Base& id(flecs::entity_t id) {
        this->assert_term_ref();
        term_ref_->id = id;
        return *this;
    }

    /* Specify value of identifier by id. Almost the same as id(entity), but this
     * operation explicitly sets the flecs::IsEntity flag. This forces the id to
     * be interpreted as entity, whereas not setting the flag would implicitly
     * convert ids for builtin variables such as flecs::This to a variable.
     *
     * This function can also be used to disambiguate id(0), which would match
     * both id(entity_t) and id(const char*).
     */
    Base& entity(flecs::entity_t entity) {
        this->assert_term_ref();
        term_ref_->id = entity | flecs::IsEntity;
        return *this;
    }

    /* Specify value of identifier by name */
    Base& name(const char *name) {
        this->assert_term_ref();
        term_ref_->id |= flecs::IsEntity;
        term_ref_->name = const_cast<char*>(name);
        return *this;
    }

    /* Specify identifier is a variable (resolved at query evaluation time) */
    Base& var(const char *var_name) {
        this->assert_term_ref();
        term_ref_->id |= flecs::IsVariable;
        term_ref_->name = const_cast<char*>(var_name);
        return *this;
    }

    /* Override term id flags */
    Base& flags(flecs::flags64_t flags) {
        this->assert_term_ref();
        term_ref_->id = flags;
        return *this;
    }

    ecs_term_ref_t *term_ref_;

protected:
    virtual flecs::world_t* world_v() = 0;

    void assert_term_ref() {
        ecs_assert(term_ref_ != NULL, ECS_INVALID_PARAMETER,
            "no active term (call .with() first)");
    }

private:
    operator Base&() {
        return *static_cast<Base*>(this);
    }
};

/** Term builder interface.
 * A term is a single element of a query expression.
 *
 * @ingroup cpp_core_queries
 */
template<typename Base>
struct term_builder_i : term_ref_builder_i<Base> {
    term_builder_i() : term_(nullptr) { }

    term_builder_i(ecs_term_t *term_ptr) {
        set_term(term_ptr);
    }

    Base& term(id_t id) {
        return this->id(id);
    }

    /* Call prior to setting values for src identifier */
    Base& src() {
        this->assert_term();
        this->term_ref_ = &term_->src;
        return *this;
    }

    /* Call prior to setting values for first identifier. This is either the
     * component identifier, or first element of a pair (in case second is
     * populated as well). */
    Base& first() {
        this->assert_term();
        this->term_ref_ = &term_->first;
        return *this;
    }

    /* Call prior to setting values for second identifier. This is the second
     * element of a pair. Requires that first() is populated as well. */
    Base& second() {
        this->assert_term();
        this->term_ref_ = &term_->second;
        return *this;
    }

    /* Select src identifier, initialize it with entity id */
    Base& src(flecs::entity_t id) {
        this->src();
        this->id(id);
        return *this;
    }

    /* Select src identifier, initialize it with id associated with type */
    template<typename T>
    Base& src() {
        this->src(_::type<T>::id(this->world_v()));
        return *this;
    }

    /* Select src identifier, initialize it with name. If name starts with a $
     * the name is interpreted as a variable. */
    Base& src(const char *name) {
        ecs_assert(name != NULL, ECS_INVALID_PARAMETER, NULL);
        this->src();
        if (name[0] == '$') {
            this->var(&name[1]);
        } else {
            this->name(name);
        }
        return *this;
    }

    /* Select first identifier, initialize it with entity id */
    Base& first(flecs::entity_t id) {
        this->first();
        this->id(id);
        return *this;
    }

    /* Select first identifier, initialize it with id associated with type */
    template<typename T>
    Base& first() {
        this->first(_::type<T>::id(this->world_v()));
        return *this;
    }

    /* Select first identifier, initialize it with name. If name starts with a $
     * the name is interpreted as a variable. */
    Base& first(const char *name) {
        ecs_assert(name != NULL, ECS_INVALID_PARAMETER, NULL);
        this->first();
        if (name[0] == '$') {
            this->var(&name[1]);
        } else {
            this->name(name);
        }
        return *this;
    }

    /* Select second identifier, initialize it with entity id */
    Base& second(flecs::entity_t id) {
        this->second();
        this->id(id);
        return *this;
    }

    /* Select second identifier, initialize it with id associated with type */
    template<typename T>
    Base& second() {
        this->second(_::type<T>::id(this->world_v()));
        return *this;
    }

    /* Select second identifier, initialize it with name. If name starts with a $
     * the name is interpreted as a variable. */
    Base& second(const char *name) {
        ecs_assert(name != NULL, ECS_INVALID_PARAMETER, NULL);
        this->second();
        if (name[0] == '$') {
            this->var(&name[1]);
        } else {
            this->name(name);
        }
        return *this;
    }

    /* The up flag indicates that the term identifier may be substituted by
     * traversing a relationship upwards. For example: substitute the identifier
     * with its parent by traversing the ChildOf relationship. */
    Base& up(flecs::entity_t trav = 0) {
        this->assert_term_ref();
        ecs_check(this->term_ref_ != &term_->first, ECS_INVALID_PARAMETER,
            "up traversal can only be applied to term source");
        ecs_check(this->term_ref_ != &term_->second, ECS_INVALID_PARAMETER,
            "up traversal can only be applied to term source");
        this->term_ref_->id |= flecs::Up;
        if (trav) {
            term_->trav = trav;
        }
    error:
        return *this;
    }

    template <typename Trav>
    Base& up() {
        return this->up(_::type<Trav>::id(this->world_v()));
    }

    /* The cascade flag is like up, but returns results in breadth-first order.
     * Only supported for flecs::query */
    Base& cascade(flecs::entity_t trav = 0) {
        this->assert_term_ref();
        this->up();
        this->term_ref_->id |= flecs::Cascade;
        if (trav) {
            term_->trav = trav;
        }
        return *this;
    }

    template <typename Trav>
    Base& cascade() {
        return this->cascade(_::type<Trav>::id(this->world_v()));
    }

    /* Use with cascade to iterate results in descending (bottom -> top) order */
    Base& desc() {
        this->assert_term_ref();
        this->term_ref_->id |= flecs::Desc;
        return *this;
    }

    /* Same as up(), exists for backwards compatibility */
    Base& parent() {
        return this->up();
    }

    /* Specify relationship to traverse, and flags to indicate direction */
    Base& trav(flecs::entity_t trav, flecs::flags32_t flags = 0) {
        this->assert_term_ref();
        term_->trav = trav;
        this->term_ref_->id |= flags;
        return *this;
    }

    /** Set id flags for term. */
    Base& id_flags(id_t flags) {
        this->assert_term();
        term_->id |= flags;
        return *this;
    }

    /** Set read/write access of term. */
    Base& inout(flecs::inout_kind_t inout) {
        this->assert_term();
        term_->inout = static_cast<int16_t>(inout);
        return *this;
    }

    /** Set read/write access for stage. Use this when a system reads or writes
     * components other than the ones provided by the query. This information
     * can be used by schedulers to insert sync/merge points between systems
     * where deferred operations are flushed.
     *
     * Setting this is optional. If not set, the value of the accessed component
     * may be out of sync for at most one frame.
     */
    Base& inout_stage(flecs::inout_kind_t inout) {
        this->assert_term();
        term_->inout = static_cast<int16_t>(inout);
        if (term_->oper != EcsNot) {
            this->src().entity(0);
        }
        return *this;
    }

    /** Short for inout_stage(flecs::Out).
     *   Use when system uses add, remove or set.
     */
    Base& write() {
        return this->inout_stage(flecs::Out);
    }

    /** Short for inout_stage(flecs::In).
     *   Use when system uses get.
     */
    Base& read() {
        return this->inout_stage(flecs::In);
    }

    /** Short for inout_stage(flecs::InOut).
     *   Use when system uses ensure.
     */
    Base& read_write() {
        return this->inout_stage(flecs::InOut);
    }

    /** Short for inout(flecs::In) */
    Base& in() {
        return this->inout(flecs::In);
    }

    /** Short for inout(flecs::Out) */
    Base& out() {
        return this->inout(flecs::Out);
    }

    /** Short for inout(flecs::InOut) */
    Base& inout() {
        return this->inout(flecs::InOut);
    }

    /** Short for inout(flecs::In) */
    Base& inout_none() {
        return this->inout(flecs::InOutNone);
    }

    /** Set operator of term. */
    Base& oper(flecs::oper_kind_t oper) {
        this->assert_term();
        term_->oper = static_cast<int16_t>(oper);
        return *this;
    }

    /* Short for oper(flecs::And) */
    Base& and_() {
        return this->oper(flecs::And);
    }

    /* Short for oper(flecs::Or) */
    Base& or_() {
        return this->oper(flecs::Or);
    }

    /* Short for oper(flecs::Or) */
    Base& not_() {
        return this->oper(flecs::Not);
    }

    /* Short for oper(flecs::Or) */
    Base& optional() {
        return this->oper(flecs::Optional);
    }

    /* Short for oper(flecs::AndFrom) */
    Base& and_from() {
        return this->oper(flecs::AndFrom);
    }

    /* Short for oper(flecs::OrFrom) */
    Base& or_from() {
        return this->oper(flecs::OrFrom);
    }

    /* Short for oper(flecs::NotFrom) */
    Base& not_from() {
        return this->oper(flecs::NotFrom);
    }

    /* Query terms are not triggered on by observers */
    Base& filter() {
        term_->inout = EcsInOutFilter;
        return *this;
    }

    ecs_term_t *term_;

protected:
    virtual flecs::world_t* world_v() override = 0;

    void set_term(ecs_term_t *term) {
        term_ = term;
        if (term) {
            this->term_ref_ = &term_->src; // default to subject
        } else {
            this->term_ref_ = nullptr;
        }
    }

private:
    void assert_term() {
        ecs_assert(term_ != NULL, ECS_INVALID_PARAMETER,
            "no active term (call .with() first)");
    }

    operator Base&() {
        return *static_cast<Base*>(this);
    }
};

}


export namespace flecs {

/** Class that describes a term.
 *
 * @ingroup cpp_core_queries
 */
struct term final : term_builder_i<term> {
    term()
        : term_builder_i<term>(&value)
        , value({})
        , world_(nullptr) { }

    term(flecs::world_t *world_ptr)
        : term_builder_i<term>(&value)
        , value({})
        , world_(world_ptr) { }

    term(flecs::world_t *world_ptr, ecs_term_t t)
        : term_builder_i<term>(&value)
        , value({})
        , world_(world_ptr) {
            value = t;
            this->set_term(&value);
        }

    term(flecs::world_t *world_ptr, id_t component_id)
        : term_builder_i<term>(&value)
        , value({})
        , world_(world_ptr) {
            if (component_id & ECS_ID_FLAGS_MASK) {
                value.id = component_id;
            } else {
                value.first.id = component_id;
            }
            this->set_term(&value);
        }

    term(flecs::world_t *world_ptr, entity_t first, entity_t second)
        : term_builder_i<term>(&value)
        , value({})
        , world_(world_ptr) {
            value.id = ecs_pair(first, second);
            this->set_term(&value);
        }

    term(id_t component_id)
        : term_builder_i<term>(&value)
        , value({})
        , world_(nullptr) {
            if (component_id & ECS_ID_FLAGS_MASK) {
                value.id = component_id;
            } else {
                value.first.id = component_id;
            }
        }

    term(id_t first, id_t second)
        : term_builder_i<term>(&value)
        , value({})
        , world_(nullptr) {
            value.first.id = first;
            value.second.id = second;
        }

    void reset() {
        value = {};
        this->set_term(nullptr);
    }

    bool is_set() {
        return ecs_term_is_initialized(&value);
    }

    flecs::id id() {
        return flecs::id(world_, value.id);
    }

    flecs::inout_kind_t inout() {
        return static_cast<flecs::inout_kind_t>(value.inout);
    }

    flecs::oper_kind_t oper() {
        return static_cast<flecs::oper_kind_t>(value.oper);
    }

    flecs::entity get_src() {
        return flecs::entity(world_, ECS_TERM_REF_ID(&value.src));
    }

    flecs::entity get_first() {
        return flecs::entity(world_, ECS_TERM_REF_ID(&value.first));
    }

    flecs::entity get_second() {
        return flecs::entity(world_, ECS_TERM_REF_ID(&value.second));
    }

    operator flecs::term_t() const {
        return value;
    }

    flecs::term_t value;

protected:
    flecs::world_t* world_v() override { return world_; }

private:
    flecs::world_t *world_;
};

// Term mixin implementation
template <typename... Args>
inline flecs::term world::term(Args &&... args) const {
    return flecs::term(world_, FLECS_FWD(args)...);
}

template <typename T>
inline flecs::term world::term() const {
    return flecs::term(world_, _::type<T>::id(world_));
}

template <typename First, typename Second>
inline flecs::term world::term() const {
    return flecs::term(world_, ecs_pair(
        _::type<First>::id(world_),
        _::type<Second>::id(world_)));
}

}

/**
 * @file addons/cpp/mixins/query/impl.hpp
 * @brief Query implementation.
 */

#pragma once

/**
 * @file addons/cpp/mixins/query/builder.hpp
 * @brief Query builder.
 */

#pragma once

/**
 * @file addons/cpp/utils/builder.hpp
 * @brief Builder base class.
 *
 * Generic functionality for builder classes.
 */

#pragma once

export namespace flecs {
namespace _ {

// Macros for template types so we don't go cross-eyed
#define FLECS_TBUILDER template<typename ... Components> class
#define FLECS_IBUILDER template<typename IBase, typename ... Components> class

template<FLECS_TBUILDER T, typename TDesc, typename Base, FLECS_IBUILDER IBuilder, typename ... Components>
struct builder : IBuilder<Base, Components ...>
{
    using IBase = IBuilder<Base, Components ...>;

public:
    builder(flecs::world_t *world)
        : IBase(&desc_)
        , desc_{}
        , world_(world) { }

    builder(const builder& f)
        : IBase(&desc_, f.term_index_)
    {
        world_ = f.world_;
        desc_ = f.desc_;
    }

    builder(builder&& f)  noexcept
        : builder<T, TDesc, Base, IBuilder, Components...>(f) { }

    operator TDesc*() {
        return &desc_;
    }

    T<Components ...> build() {
        return T<Components...>(world_, *static_cast<Base*>(this));
    }

protected:
    flecs::world_t* world_v() override { return world_; }
    TDesc desc_;
    flecs::world_t *world_;
};

#undef FLECS_TBUILDER
#undef FLECS_IBUILDER

} // namespace _
} // export namespace flecs

/**
 * @file addons/cpp/mixins/query/builder_i.hpp
 * @brief Query builder interface.
 */

#pragma once


export namespace flecs
{

/** Query builder interface.
 *
 * @ingroup cpp_core_queries
 */
template<typename Base, typename ... Components>
struct query_builder_i : term_builder_i<Base> {
    query_builder_i(ecs_query_desc_t *desc, int32_t term_index = 0)
        : term_index_(term_index)
        , expr_count_(0)
        , desc_(desc) { }

    Base& query_flags(ecs_flags32_t flags) {
        desc_->flags |= flags;
        return *this;
    }

    Base& cache_kind(query_cache_kind_t kind) {
        desc_->cache_kind = static_cast<ecs_query_cache_kind_t>(kind);
        return *this;
    }

    Base& cached() {
        return cache_kind(flecs::QueryCacheAuto);
    }

    Base& detect_changes() {
        desc_->flags |= EcsQueryDetectChanges;
        return *this;
    }

    Base& expr(const char *expr) {
        ecs_check(expr_count_ == 0, ECS_INVALID_OPERATION,
            "query_builder::expr() called more than once");
        desc_->expr = expr;
        expr_count_ ++;

    error:
        return *this;
    }

    /* With methods */

    template<typename T>
    Base& with() {
        this->term();
        *this->term_ = flecs::term(_::type<T>::id(this->world_v()));
        this->term_->inout = static_cast<ecs_inout_kind_t>(
            _::type_to_inout<T>());
        return *this;
    }

    Base& with(id_t component_id) {
        this->term();
        *this->term_ = flecs::term(component_id);
        return *this;
    }

    Base& with(const char *component_name) {
        this->term();
        *this->term_ = flecs::term().first(component_name);
        return *this;
    }

    Base& with(const char *first, const char *second) {
        this->term();
        *this->term_ = flecs::term().first(first).second(second);
        return *this;
    }

    Base& with(entity_t first, entity_t second) {
        this->term();
        *this->term_ = flecs::term(first, second);
        return *this;
    }

    Base& with(entity_t first, const char *second) {
        this->term();
        *this->term_ = flecs::term(first).second(second);
        return *this;
    }

    Base& with(const char *first, entity_t second) {
        this->term();
        *this->term_ = flecs::term().first(first).second(second);
        return *this;
    }

    template<typename First>
    Base& with(entity_t second) {
        return this->with(_::type<First>::id(this->world_v()), second);
    }

    template<typename First>
    Base& with(const char *second) {
        return this->with(_::type<First>::id(this->world_v())).second(second);
    }

    template<typename First, typename Second>
    Base& with() {
        return this->with<First>(_::type<Second>::id(this->world_v()));
    }

    template <typename E, if_t< is_enum<E>::value > = 0>
    Base& with(E value) {
        flecs::entity_t r = _::type<E>::id(this->world_v());
        auto o = enum_type<E>(this->world_v()).entity(value);
        return this->with(r, o);
    }

    Base& with(flecs::term& term) {
        this->term();
        *this->term_ = term;
        return *this;
    }

    Base& with(flecs::term&& term) {
        this->term();
        *this->term_ = term;
        return *this;
    }

    /* Without methods, shorthand for .with(...).not_(). */

    template <typename ... Args>
    Base& without(Args&&... args) {
        return this->with(FLECS_FWD(args)...).not_();
    }

    template <typename T, typename ... Args>
    Base& without(Args&&... args) {
        return this->with<T>(FLECS_FWD(args)...).not_();
    }

    template <typename First, typename Second>
    Base& without() {
        return this->with<First, Second>().not_();
    }

    /* Write/read methods */

    Base& write() {
        term_builder_i<Base>::write();
        return *this;
    }

    template <typename ... Args>
    Base& write(Args&&... args) {
        return this->with(FLECS_FWD(args)...).write();
    }

    template <typename T, typename ... Args>
    Base& write(Args&&... args) {
        return this->with<T>(FLECS_FWD(args)...).write();
    }

    template <typename First, typename Second>
    Base& write() {
        return this->with<First, Second>().write();
    }

    Base& read() {
        term_builder_i<Base>::read();
        return *this;
    }

    template <typename ... Args>
    Base& read(Args&&... args) {
        return this->with(FLECS_FWD(args)...).read();
    }

    template <typename T, typename ... Args>
    Base& read(Args&&... args) {
        return this->with<T>(FLECS_FWD(args)...).read();
    }

    template <typename First, typename Second>
    Base& read() {
        return this->with<First, Second>().read();
    }

    /* Scope_open/scope_close shorthand notation. */
    Base& scope_open() {
        return this->with(flecs::ScopeOpen).entity(0);
    }

    Base& scope_close() {
        return this->with(flecs::ScopeClose).entity(0);
    }

    /* Term notation for more complex query features */

    /** Sets the current term to next one in term list.
     */
    Base& term() {
        if (this->term_) {
            ecs_check(ecs_term_is_initialized(this->term_),
                ECS_INVALID_OPERATION,
                    "query_builder::term() called without initializing term");
        }

        ecs_check(term_index_ < FLECS_TERM_COUNT_MAX,
            ECS_INVALID_PARAMETER, "maximum number of terms exceeded");

        this->set_term(&desc_->terms[term_index_]);

        term_index_ ++;

    error:
        return *this;
    }

    /** Sets the current term to the one with the provided type.
     * This loops over all terms to find the one with the provided type.
     * For performance-critical paths, use term_at(int32_t) instead.
     */
    template <typename T>
    Base& term_at() {
        flecs::id_t term_id = _::type<T>::id(this->world_v());
        for (int i = 0; i < term_index_; i ++) {
            ecs_term_t cur_term = desc_->terms[i];
            ecs_id_t cur_term_id = cur_term.id;
            ecs_id_t cur_term_pair = ecs_pair(cur_term.first.id, cur_term.second.id);

            if ((term_id == cur_term_id || (cur_term_id != 0 && term_id == ecs_get_typeid(this->world_v(), cur_term_id))) ||
                (term_id == cur_term_pair || (cur_term_pair != 0 && term_id == ecs_get_typeid(this->world_v(), cur_term_pair)))) {
                return term_at(i);
            }
        }

        ecs_err("term not found");
        return *this;
    }

    /** Sets the current term to the one at the provided index.
     */
    Base& term_at(int32_t term_index) {
        ecs_assert(term_index >= 0, ECS_INVALID_PARAMETER, NULL);
        int32_t prev_index = term_index_;
        term_index_ = term_index;
        this->term();
        term_index_ = prev_index;
        ecs_assert(ecs_term_is_initialized(this->term_),
            ECS_INVALID_PARAMETER, NULL);
        return *this;
    }

    /** Sets the current term to the one at the provided index and asserts that the type matches.
     */
    template <typename T>
    Base& term_at(int32_t term_index) {
        this->term_at(term_index);
#if !defined(FLECS_NDEBUG) || defined(FLECS_KEEP_ASSERT)
        flecs::id_t term_id = _::type<T>::id(this->world_v());
        ecs_term_t cur_term = *this->term_;
        ecs_id_t cur_term_id = cur_term.id;
        ecs_id_t cur_term_pair = ecs_pair(cur_term.first.id, cur_term.second.id);

        ecs_assert((term_id == cur_term_id || (cur_term_id != 0 && term_id == ecs_get_typeid(this->world_v(), cur_term_id))) ||
            (term_id == cur_term_pair || (cur_term_pair != 0 && term_id == ecs_get_typeid(this->world_v(), cur_term_pair))),
            ECS_INVALID_PARAMETER, "term type mismatch");
#endif
        return *this;
    }

    /** Sort the output of a query.
     * This enables sorting of entities across matched tables. As a result of this
     * operation, the order of entities in the matched tables may be changed.
     * Resorting happens when a query iterator is obtained, and only if the table
     * data has changed.
     *
     * If multiple queries that match the same (down)set of tables specify different
     * sorting functions, resorting is likely to happen every time an iterator is
     * obtained, which can significantly slow down iterations.
     *
     * The sorting function will be applied to the specified component. Resorting
     * only happens if that component has changed, or when the entity order in the
     * table has changed. If no component is provided, resorting only happens when
     * the entity order changes.
     *
     * @tparam T The component used to sort.
     * @param compare The compare function used to sort the components.
     */
    template <typename T>
    Base& order_by(int(*compare)(flecs::entity_t, const T*, flecs::entity_t, const T*)) {
        ecs_order_by_action_t cmp = reinterpret_cast<ecs_order_by_action_t>(compare);
        return this->order_by(_::type<T>::id(this->world_v()), cmp);
    }

    /** Sort the output of a query.
     * Same as order_by<T>, but with component identifier.
     *
     * @param component The component used to sort.
     * @param compare The compare function used to sort the components.
     */
    Base& order_by(flecs::entity_t component, int(*compare)(flecs::entity_t, const void*, flecs::entity_t, const void*)) {
        desc_->order_by_callback = reinterpret_cast<ecs_order_by_action_t>(compare);
        desc_->order_by = component;
        return *this;
    }

    /** Group and sort matched tables.
     * Similar to ecs_query_order_by(), but instead of sorting individual entities, this
     * operation only sorts matched tables. This can be useful of a query needs to
     * enforce a certain iteration order upon the tables it is iterating, for
     * example by giving a certain component or tag a higher priority.
     *
     * The sorting function assigns a "rank" to each type, which is then used to
     * sort the tables. Tables with higher ranks will appear later in the iteration.
     *
     * Resorting happens when a query iterator is obtained, and only if the set of
     * matched tables for a query has changed. If table sorting is enabled together
     * with entity sorting, table sorting takes precedence, and entities will be
     * sorted within each set of tables that are assigned the same rank.
     *
     * @tparam T The component used to determine the group rank.
     * @param group_by_action Callback that determines group id for table.
     */
    template <typename T>
    Base& group_by(uint64_t(*group_by_action)(flecs::world_t*, flecs::table_t *table, flecs::id_t id, void* ctx)) {
        ecs_group_by_action_t action = reinterpret_cast<ecs_group_by_action_t>(group_by_action);
        return this->group_by(_::type<T>::id(this->world_v()), action);
    }

    /** Group and sort matched tables.
     * Same as group_by<T>, but with component identifier.
     *
     * @param component The component used to determine the group rank.
     * @param group_by_action Callback that determines group id for table.
     */
    Base& group_by(flecs::entity_t component, uint64_t(*group_by_action)(flecs::world_t*, flecs::table_t *table, flecs::id_t id, void* ctx)) {
        desc_->group_by_callback = reinterpret_cast<ecs_group_by_action_t>(group_by_action);
        desc_->group_by = component;
        return *this;
    }

    /** Group and sort matched tables.
     * Same as group_by<T>, but with default group_by action.
     *
     * @tparam T The component used to determine the group rank.
     */
    template <typename T>
    Base& group_by() {
        return this->group_by(_::type<T>::id(this->world_v()), nullptr);
    }

    /** Group and sort matched tables.
     * Same as group_by, but with default group_by action.
     *
     * @param component The component used to determine the group rank.
     */
    Base& group_by(flecs::entity_t component) {
        return this->group_by(component, nullptr);
    }

    /** Specify context to be passed to group_by function.
     *
     * @param ctx Context to pass to group_by function.
     * @param ctx_free Function to cleanup context (called when query is deleted).
     */
    Base& group_by_ctx(void *ctx, ecs_ctx_free_t ctx_free = nullptr) {
        desc_->group_by_ctx = ctx;
        desc_->group_by_ctx_free = ctx_free;
        return *this;
    }

    /** Specify on_group_create action.
     */
    Base& on_group_create(ecs_group_create_action_t action) {
        desc_->on_group_create = action;
        return *this;
    }

    /** Specify on_group_delete action.
     */
    Base& on_group_delete(ecs_group_delete_action_t action) {
        desc_->on_group_delete = action;
        return *this;
    }

protected:
    virtual flecs::world_t* world_v() override = 0;
    int32_t term_index_;
    int32_t expr_count_;

private:
    operator Base&() {
        return *static_cast<Base*>(this);
    }

    ecs_query_desc_t *desc_;
};

}


export namespace flecs {
namespace _ {
    template <typename ... Components>
    using query_builder_base = builder<
        query, ecs_query_desc_t, query_builder<Components...>,
        query_builder_i, Components ...>;
}

/** Query builder.
 *
 * @ingroup cpp_core_queries
 */
template <typename ... Components>
struct query_builder final : _::query_builder_base<Components...> {
    query_builder(flecs::world_t* world, flecs::entity query_entity)
        : _::query_builder_base<Components...>(world)
    {
        _::sig<Components...>(world).populate(this);
        this->desc_.entity = query_entity.id();
    }

    query_builder(flecs::world_t* world, const char *name = nullptr)
        : _::query_builder_base<Components...>(world)
    {
        _::sig<Components...>(world).populate(this);
        if (name != nullptr) {
            ecs_entity_desc_t entity_desc = {};
            entity_desc.name = name;
            entity_desc.sep = "::";
            entity_desc.root_sep = "::";
            this->desc_.entity = ecs_entity_init(world, &entity_desc);
        }
    }

    template <typename Func>
    void each(Func&& func) {
        this->build().each(FLECS_FWD(func));
    }
};

}


export namespace flecs
{

struct query_base {
    query_base() { }

    query_base(query_t *q)
        : query_(q) {
            flecs_poly_claim(q);
        }

    query_base(const query_t *q)
        : query_(ECS_CONST_CAST(query_t*, q)) {
            flecs_poly_claim(q);
        }

    query_base(world_t *world, ecs_query_desc_t *desc) {
        if (desc->entity && desc->terms[0].id == 0) {
            const flecs::Poly *query_poly = ecs_get_pair(
                world, desc->entity, EcsPoly, EcsQuery);
            if (query_poly) {
                query_ = static_cast<flecs::query_t*>(query_poly->poly);
                flecs_poly_claim(query_);
                return;
            }
        }

        query_ = ecs_query_init(world, desc);
    }

    query_base(const query_base& obj) {
        this->query_ = obj.query_;
        if (this->query_)
        {
            flecs_poly_claim(this->query_);
        }
    }

    query_base& operator=(const query_base& obj) {
        this->~query_base();
        this->query_ = obj.query_;
        if (this->query_)
        {
            flecs_poly_claim(this->query_);
        }
        return *this;
    }

    query_base(query_base&& obj) noexcept {
        this->query_ = obj.query_;
        obj.query_ = nullptr;
    }

    query_base& operator=(query_base&& obj) noexcept {
        this->query_ = obj.query_;
        obj.query_ = nullptr;
        return *this;
    }

    flecs::entity entity() {
        return flecs::entity(query_->world, query_->entity);
    }

    const flecs::query_t* c_ptr() const {
        return query_;
    }

    operator const flecs::query_t*() const {
        return query_;
    }

    operator bool() const {
        return query_ != nullptr;
    }

    /** Free persistent query.
     * A persistent query is a query that is associated with an entity, such as
     * system queries and named queries. Persistent queries must be deleted with
     * destruct(), or will be deleted automatically at world cleanup.
     */
    void destruct() {
        ecs_assert(query_->entity != 0, ECS_INVALID_OPERATION, "destruct() "
            "should only be called on queries associated with entities");
        ecs_query_fini(query_);
        query_ = nullptr;
    }

    ~query_base() {
        /* Only free if query is not associated with entity, such as system
         * queries and named queries. Named queries have to be either explicitly
         * deleted with the .destruct() method, or will be deleted when the
         * world is deleted. */
        if (query_ && !query_->entity) {
            if (!flecs_poly_release(query_)) {
                ecs_query_fini(query_);
                query_ = nullptr;
            }
        }
    }

    /** Returns whether the query data changed since the last iteration.
     * This operation must be invoked before obtaining the iterator, as this will
     * reset the changed state. The operation will return true after:
     * - new entities have been matched with
     * - matched entities were deleted
     * - matched components were changed
     *
     * @return true if entities changed, otherwise false.
     */
    bool changed() const {
        return ecs_query_changed(query_);
    }

    /** Get info for group.
     *
     * @param group_id The group id for which to retrieve the info.
     * @return The group info.
     */
    const flecs::query_group_info_t* group_info(uint64_t group_id) const {
        return ecs_query_get_group_info(query_, group_id);
    }

    /** Get context for group.
     *
     * @param group_id The group id for which to retrieve the context.
     * @return The group context.
     */
    void* group_ctx(uint64_t group_id) const {
        const flecs::query_group_info_t *gi = group_info(group_id);
        if (gi) {
            return gi->ctx;
        } else {
            return NULL;
        }
    }

    template <typename Func>
    void each_term(const Func& func) {
        for (int i = 0; i < query_->term_count; i ++) {
            flecs::term t(query_->world, query_->terms[i]);
            func(t);
            t.reset(); // prevent freeing resources
        }
    }

    flecs::term term(int32_t index) {
        return flecs::term(query_->world, query_->terms[index]);
    }

    int32_t term_count() {
        return query_->term_count;
    }

    int32_t field_count() {
        return query_->field_count;
    }

    int32_t find_var(const char *name) {
        return ecs_query_find_var(query_, name);
    }

    flecs::string str() {
        char *result = ecs_query_str(query_);
        return flecs::string(result);
    }

    /** Returns a string representing the query plan.
     * This can be used to analyze the behavior & performance of the query.
     * @see ecs_query_plan
     */
    flecs::string plan() const {
        char *result = ecs_query_plan(query_);
        return flecs::string(result);
    }

    operator query<>() const;

#   ifdef FLECS_JSON

/** Serialize query to JSON.
 *
 * @memberof flecs::query_base
 * @ingroup cpp_addons_json
 */
flecs::string to_json(flecs::iter_to_json_desc_t *desc = nullptr) {
    ecs_iter_t it = ecs_query_iter(ecs_get_world(query_), query_);
    char *json = ecs_iter_to_json(&it, desc);
    return flecs::string(json);
}
#   endif

protected:
    query_t *query_ = nullptr;
};

template<typename ... Components>
struct query : query_base, iterable<Components...> {
private:
    using Fields = typename _::field_ptrs<Components...>::array;

public:
    using query_base::query_base;

    query() : query_base() { } // necessary not to confuse msvc

    query(const query& obj) : query_base(obj) { }

    query& operator=(const query& obj) {
        query_base::operator=(obj);
        return *this;
    }

    query(query&& obj) noexcept : query_base(FLECS_MOV(obj)) { }

    query& operator=(query&& obj) noexcept {
        query_base::operator=(FLECS_FWD(obj));
        return *this;
    }

    flecs::query<> cache_query() const {
        const flecs::query_t *q = ecs_query_get_cache_query(query_);
        return flecs::query<>(q);
    }

private:
    ecs_iter_t get_iter(flecs::world_t *world) const override {
        ecs_assert(query_ != nullptr, ECS_INVALID_PARAMETER,
            "cannot iterate invalid query");
        if (!world) {
            world = query_->world;
        }
        return ecs_query_iter(world, query_);
    }

    ecs_iter_next_action_t next_action() const override {
        return ecs_query_next;
    }
};

// World mixin implementation
template <typename... Comps, typename... Args>
inline flecs::query<Comps...> world::query(Args &&... args) const {
    return flecs::query_builder<Comps...>(world_, FLECS_FWD(args)...)
        .build();
}

inline flecs::query<> world::query(flecs::entity query_entity) const {
    ecs_query_desc_t desc = {};
    desc.entity = query_entity;
    return flecs::query<>(world_, &desc);
}

template <typename... Comps, typename... Args>
inline flecs::query_builder<Comps...> world::query_builder(Args &&... args) const {
    return flecs::query_builder<Comps...>(world_, FLECS_FWD(args)...);
}

// world::each
namespace _ {

// Each with entity parameter
template<typename Func, typename ... Args>
struct query_delegate_w_ent;

template<typename Func, typename E, typename ... Args>
struct query_delegate_w_ent<Func, arg_list<E, Args ...> >
{
    query_delegate_w_ent(const flecs::world& world, Func&& func) {
        auto f = world.query<Args ...>();
        f.each(FLECS_MOV(func));
    }
};

// Each without entity parameter
template<typename Func, typename ... Args>
struct query_delegate_no_ent;

template<typename Func, typename ... Args>
struct query_delegate_no_ent<Func, arg_list<Args ...> >
{
    query_delegate_no_ent(const flecs::world& world, Func&& func) {
        auto f = world.query<Args ...>();
        f.each(FLECS_MOV(func));
    }
};

// Switch between function with & without entity parameter
template<typename Func, typename T = int>
struct query_delegate;

template <typename Func>
struct query_delegate<Func, if_t<is_same<first_arg_t<Func>, flecs::entity>::value> > {
    query_delegate(const flecs::world& world, Func&& func) {
        query_delegate_w_ent<Func, arg_list_t<Func>>(world, FLECS_MOV(func));
    }
};

template <typename Func>
struct query_delegate<Func, if_not_t<is_same<first_arg_t<Func>, flecs::entity>::value> > {
    query_delegate(const flecs::world& world, Func&& func) {
        query_delegate_no_ent<Func, arg_list_t<Func>>(world, FLECS_MOV(func));
    }
};

}

template <typename Func>
inline void world::each(Func&& func) const {
    _::query_delegate<Func> f_delegate(*this, FLECS_MOV(func));
}

template <typename T, typename Func>
inline void world::each(Func&& func) const {
    ecs_iter_t it = ecs_each_id(world_, _::type<T>::id(world_));

    while (ecs_each_next(&it)) {
        _::each_delegate<Func, T>(func).invoke(&it);
    }
}

template <typename Func>
inline void world::each(flecs::id_t each_id, Func&& func) const {
    ecs_iter_t it = ecs_each_id(world_, each_id);

    while (ecs_each_next(&it)) {
        _::each_delegate<Func>(func).invoke(&it);
    }
}

// query_base implementation
inline query_base::operator flecs::query<> () const {
    return flecs::query<>(query_);
}

}

/**
 * @file addons/cpp/mixins/observer/impl.hpp
 * @brief Observer implementation.
 */

#pragma once

/**
 * @file addons/cpp/mixins/observer/builder.hpp
 * @brief Observer builder.
 */

#pragma once

/**
 * @file addons/cpp/utils/node_builder.hpp
 * @brief Base builder class for node objects, like systems, observers.
 */

#pragma once

export namespace flecs {
namespace _ {

// Macros for template types so we don't go cross-eyed
#define FLECS_IBUILDER template<typename IBase, typename ... Components> class

template<typename T, typename TDesc, typename Base, FLECS_IBUILDER IBuilder, typename ... Components>
struct node_builder : IBuilder<Base, Components ...>
{
    using IBase = IBuilder<Base, Components ...>;

public:
    explicit node_builder(flecs::world_t* world, const char *name = nullptr)
        : IBase(&desc_)
        , desc_{}
        , world_(world)
    {
        ecs_entity_desc_t entity_desc = {};
        entity_desc.name = name;
        entity_desc.sep = "::";
        entity_desc.root_sep = "::";
        desc_.entity = ecs_entity_init(world_, &entity_desc);
    }

    template <typename Func>
    T run(Func&& func) {
        using Delegate = typename _::run_delegate<
            typename std::decay<Func>::type>;

        auto ctx = FLECS_NEW(Delegate)(FLECS_FWD(func));
        desc_.run = Delegate::run;
        desc_.run_ctx = ctx;
        desc_.run_ctx_free = _::free_obj<Delegate>;
        return T(world_, &desc_);
    }

    template <typename Func, typename EachFunc>
    T run(Func&& func, EachFunc&& each_func) {
        using Delegate = typename _::run_delegate<
            typename std::decay<Func>::type>;

        auto ctx = FLECS_NEW(Delegate)(FLECS_FWD(func));
        desc_.run = Delegate::run;
        desc_.run_ctx = ctx;
        desc_.run_ctx_free = _::free_obj<Delegate>;
        return each(FLECS_FWD(each_func));
    }

    template <typename Func>
    T each(Func&& func) {
        using Delegate = typename _::each_delegate<
            typename std::decay<Func>::type, Components...>;
        auto ctx = FLECS_NEW(Delegate)(FLECS_FWD(func));
        desc_.callback = Delegate::run;
        desc_.callback_ctx = ctx;
        desc_.callback_ctx_free = _::free_obj<Delegate>;
        return T(world_, &desc_);
    }

    template <typename Func>
    T run_each(Func&& func) {
        using Delegate = typename _::each_delegate<
            typename std::decay<Func>::type, Components...>;
        auto ctx = FLECS_NEW(Delegate)(FLECS_FWD(func));
        desc_.run = Delegate::run_each;
        desc_.run_ctx = ctx;
        desc_.run_ctx_free = _::free_obj<Delegate>;
        return T(world_, &desc_);
    }

protected:
    flecs::world_t* world_v() override { return world_; }
    TDesc desc_;
    flecs::world_t *world_;
};

#undef FLECS_IBUILDER

} // namespace _
} // export namespace flecs

/**
 * @file addons/cpp/mixins/observer/builder_i.hpp
 * @brief Observer builder interface.
 */

#pragma once


export namespace flecs {

/** Observer builder interface.
 *
 * @ingroup cpp_observers
 */
template<typename Base, typename ... Components>
struct observer_builder_i : query_builder_i<Base, Components ...> {
    using BaseClass = query_builder_i<Base, Components ...>;
    observer_builder_i()
        : BaseClass(nullptr)
        , desc_(nullptr)
        , event_count_(0) { }

    observer_builder_i(ecs_observer_desc_t *desc)
        : BaseClass(&desc->query)
        , desc_(desc)
        , event_count_(0) { }

    /** Specify the event(s) for when the observer should run.
     * @param evt The event.
     */
    Base& event(entity_t evt) {
        desc_->events[event_count_ ++] = evt;
        return *this;
    }

    /** Specify the event(s) for when the observer should run.
     * @tparam E The event.
     */
    template <typename E>
    Base& event() {
        desc_->events[event_count_ ++] = _::type<E>().id(world_v());
        return *this;
    }

    /** Invoke observer for anything that matches its query on creation */
    Base& yield_existing(bool value = true) {
        desc_->yield_existing = value;
        return *this;
    }

    /** Set observer flags */
    Base& observer_flags(ecs_flags32_t flags) {
        desc_->flags_ |= flags;
        return *this;
    }

    /** Set observer context */
    Base& ctx(void *ptr) {
        desc_->ctx = ptr;
        return *this;
    }

    /** Set observer run callback */
    Base& run(ecs_iter_action_t action) {
        desc_->run = action;
        return *this;
    }

protected:
    virtual flecs::world_t* world_v() override = 0;

private:
    operator Base&() {
        return *static_cast<Base*>(this);
    }

    ecs_observer_desc_t *desc_;
    int32_t event_count_;
};

}


export namespace flecs {
namespace _ {
    template <typename ... Components>
    using observer_builder_base = node_builder<
        observer, ecs_observer_desc_t, observer_builder<Components...>,
        observer_builder_i, Components ...>;
}

/** Observer builder.
 *
 * @ingroup cpp_observers
 */
template <typename ... Components>
struct observer_builder final : _::observer_builder_base<Components...> {
    observer_builder(flecs::world_t* world, const char *name = nullptr)
        : _::observer_builder_base<Components...>(world, name)
    {
        _::sig<Components...>(world).populate(this);
    }
};

}


export namespace flecs
{

struct observer final : entity
{
    using entity::entity;

    explicit observer() : entity() { }

    observer(flecs::world_t *world, ecs_observer_desc_t *desc) {
        world_ = world;
        id_ = ecs_observer_init(world, desc);
    }

    void ctx(void *ctx) {
        ecs_observer_desc_t desc = {};
        desc.entity = id_;
        desc.ctx = ctx;
        ecs_observer_init(world_, &desc);
    }

    void* ctx() const {
        return ecs_observer_get(world_, id_)->ctx;
    }

    flecs::query<> query() const {
        return flecs::query<>(ecs_observer_get(world_, id_)->query);
    }
};

// Mixin implementation
inline observer world::observer(flecs::entity e) const {
    return flecs::observer(world_, e);
}

template <typename... Comps, typename... Args>
inline observer_builder<Comps...> world::observer(Args &&... args) const {
    return flecs::observer_builder<Comps...>(world_, FLECS_FWD(args)...);
}

} // export namespace flecs

/**
 * @file addons/cpp/mixins/event/impl.hpp
 * @brief Event implementation.
 */

#pragma once


export namespace flecs
{

// Mixin implementation

inline flecs::event_builder world::event(flecs::entity_t evt) const {
    return flecs::event_builder(world_, evt);
}

template <typename E>
inline flecs::event_builder_typed<E> world::event() const {
    return flecs::event_builder_typed<E>(world_, _::type<E>().id(world_));
}

namespace _ {
    inline void entity_observer_create(
        flecs::world_t *world,
        flecs::entity_t event,
        flecs::entity_t entity,
        ecs_iter_action_t callback,
        void *callback_ctx,
        ecs_ctx_free_t callback_ctx_free)
    {
        ecs_observer_desc_t desc = {};
        desc.events[0] = event;
        desc.query.terms[0].id = EcsAny;
        desc.query.terms[0].src.id = entity;
        desc.callback = callback;
        desc.callback_ctx = callback_ctx;
        desc.callback_ctx_free = callback_ctx_free;

        flecs::entity_t o = ecs_observer_init(world, &desc);
        ecs_add_pair(world, o, EcsChildOf, entity);
    }

    template <typename Func>
    struct entity_observer_factory {
        template <typename Evt, if_t<is_empty<Evt>::value> = 0>
        static void create(
            flecs::world_t *world,
            flecs::entity_t entity,
            Func&& f)
        {
            using Delegate = _::entity_observer_delegate<Func>;
            auto ctx = FLECS_NEW(Delegate)(FLECS_FWD(f));
            entity_observer_create(world, _::type<Evt>::id(world), entity, Delegate::run, ctx, _::free_obj<Delegate>);
        }

        template <typename Evt, if_not_t<is_empty<Evt>::value> = 0>
        static void create(
            flecs::world_t *world,
            flecs::entity_t entity,
            Func&& f)
        {
            using Delegate = _::entity_payload_observer_delegate<Func, Evt>;
            auto ctx = FLECS_NEW(Delegate)(FLECS_FWD(f));
            entity_observer_create(world, _::type<Evt>::id(world), entity, Delegate::run, ctx, _::free_obj<Delegate>);
        }
    };
}

template <typename Self>
template <typename Func>
inline const Self& entity_builder<Self>::observe(flecs::entity_t evt, Func&& f) const {
    using Delegate = _::entity_observer_delegate<Func>;
    auto ctx = FLECS_NEW(Delegate)(FLECS_FWD(f));

    _::entity_observer_create(world_, evt, id_, Delegate::run, ctx, _::free_obj<Delegate>);

    return to_base();
}

template <typename Self>
template <typename Evt, typename Func>
inline const Self& entity_builder<Self>::observe(Func&& f) const {
    _::entity_observer_factory<Func>::template create<Evt>(
        world_, id_, FLECS_FWD(f));
    return to_base();
}

template <typename Self>
template <typename Func>
inline const Self& entity_builder<Self>::observe(Func&& f) const {
    return this->observe<_::event_from_func_t<Func>>(FLECS_FWD(f));
}

inline void entity_view::emit(flecs::entity evt) const {
    this->emit(evt.id());
}

inline void entity_view::enqueue(flecs::entity evt) const {
    this->enqueue(evt.id());
}

} // export namespace flecs

/**
 * @file addons/cpp/mixins/enum/impl.hpp
 * @brief Enum implementation.
 */

#pragma once

export namespace flecs {

template <typename E>
inline E entity_view::to_constant() const {
    using U = typename std::underlying_type<E>::type;
    const E* ptr = static_cast<const E*>(ecs_get_id(world_, id_,
        ecs_pair(flecs::Constant, _::type<U>::id(world_))));
    ecs_assert(ptr != NULL, ECS_INVALID_PARAMETER, "entity is not a constant");
    return ptr[0];
}

template <typename E, if_t< is_enum<E>::value >>
inline flecs::entity world::to_entity(E constant) const {
    const auto& et = enum_type<E>(world_);
    return flecs::entity(world_, et.entity(constant));
}

}
#ifdef FLECS_MODULE
/**
 * @file addons/cpp/mixins/module/impl.hpp
 * @brief Module implementation.
 */

#pragma once

export namespace flecs {

namespace _ {

template <typename T>
ecs_entity_t do_import(world& world, const char *symbol) {
    ecs_trace("#[magenta]import#[reset] %s", _::type_name<T>());
    ecs_log_push();

    ecs_entity_t scope = ecs_set_scope(world, 0);

    // Initialize module component type & don't allow it to be registered as a
    // tag, as this would prevent calling emplace()
    auto c_ = component<T>(world, nullptr, false);

    // Make module component sparse so that it'll never move in memory. This
    // guarantees that a module destructor can be reliably used to cleanup
    // module resources.
    c_.add(flecs::Sparse);

    c_.add(flecs::Singleton);

    ecs_set_scope(world, c_);
    world.emplace<T>(world);
    ecs_set_scope(world, scope);

    ecs_add_id(world, c_, EcsModule);

    // It should now be possible to lookup the module
    ecs_entity_t m = ecs_lookup_symbol(world, symbol, false, false);
    ecs_assert(m != 0, ECS_MODULE_UNDEFINED, symbol);
    ecs_assert(m == c_, ECS_INTERNAL_ERROR, NULL);

    ecs_log_pop();

    return m;
}

template <typename T>
flecs::entity import(world& world) {
    const char *symbol = _::symbol_name<T>();

    ecs_entity_t m = ecs_lookup_symbol(world, symbol, true, false);

    if (!_::type<T>::registered(world)) {
        /* Module is registered with world, initialize static data */
        if (m) {
            _::type<T>::init_builtin(world, m, false);

        /* Module is not yet registered, register it now */
        } else {
            m = _::do_import<T>(world, symbol);
        }

    /* Module has been registered, but could have been for another world. Import
     * if module hasn't been registered for this world. */
    } else if (!m) {
        m = _::do_import<T>(world, symbol);
    }

    return flecs::entity(world, m);
}

}

/**
 * @defgroup cpp_addons_modules Modules
 * @ingroup cpp_addons
 * Modules organize components, systems and more in reusable units of code.
 *
 * @{
 */

template <typename Module>
inline flecs::entity world::module(const char *name) const {
    flecs::entity result = this->entity(
        _::type<Module>::register_id(world_, nullptr, false));

    if (name) {
        flecs::entity prev_parent = result.parent();
        ecs_add_path_w_sep(world_, result, 0, name, "::", "::");
        flecs::entity parent = result.parent();
        if (prev_parent != parent) {
            // Module was reparented, cleanup old parent(s)
            flecs::entity cur = prev_parent, next;
            while (cur) {
                next = cur.parent();

                ecs_iter_t it = ecs_each_id(world_, ecs_pair(EcsChildOf, cur));
                if (!ecs_iter_is_true(&it)) {
                    cur.destruct();

                    // Prevent increasing the generation count of the temporary
                    // parent. This allows entities created during
                    // initialization to keep non-recycled ids.
                    this->set_version(cur);
                }

                cur = next;
            }
        }
    }

    return result;
}

template <typename Module>
inline flecs::entity world::import() {
    return flecs::_::import<Module>(*this);
}

/** @} */

}

#endif
#ifdef FLECS_SYSTEM
/**
 * @file addons/cpp/mixins/system/impl.hpp
 * @brief System module implementation.
 */

#pragma once

/**
 * @file addons/cpp/mixins/system/builder.hpp
 * @brief System builder.
 */

#pragma once

/**
 * @file addons/cpp/mixins/system/builder_i.hpp
 * @brief System builder interface.
 */

#pragma once


export namespace flecs
{

/** System builder interface.
 *
 * @ingroup cpp_addons_systems
 */
template<typename Base, typename ... Components>
struct system_builder_i : query_builder_i<Base, Components ...> {
private:
    using BaseClass = query_builder_i<Base, Components ...>;

public:
    system_builder_i(ecs_system_desc_t *desc)
        : BaseClass(&desc->query)
        , desc_(desc) { }

    /** Specify in which phase the system should run.
     *
     * @param phase The phase.
     */
    Base& kind(entity_t phase) {
        flecs::entity_t cur_phase = ecs_get_target(
            world_v(), desc_->entity, EcsDependsOn, 0);
        if (cur_phase) {
            ecs_remove_id(world_v(), desc_->entity, ecs_dependson(cur_phase));
            ecs_remove_id(world_v(), desc_->entity, cur_phase);
        }
        if (phase) {
            ecs_add_id(world_v(), desc_->entity, ecs_dependson(phase));
            ecs_add_id(world_v(), desc_->entity, phase);
        }
        return *this;
    }

    template <typename E, if_t<is_enum<E>::value> = 0>
    Base& kind(E phase)
    {
        const auto& et = enum_type<E>(this->world_v());
        flecs::entity_t target = et.entity(phase);
        return this->kind(target);
    }

    /** Specify in which phase the system should run.
     *
     * @tparam Phase The phase.
     */
    template <typename Phase>
    Base& kind() {
        return this->kind(_::type<Phase>::id(world_v()));
    }

    /** Specify whether system can run on multiple threads.
     *
     * @param value If false system will always run on a single thread.
     */
    Base& multi_threaded(bool value = true) {
        desc_->multi_threaded = value;
        return *this;
    }

    /** Specify whether system should be ran in staged context.
     *
     * @param value If false system will always run staged.
     */
    Base& immediate(bool value = true) {
        desc_->immediate = value;
        return *this;
    }

    /** Set system interval.
     * This operation will cause the system to be ran at the specified interval.
     *
     * The timer is synchronous, and is incremented each frame by delta_time.
     *
     * @param interval The interval value.
     */
    Base& interval(ecs_ftime_t interval) {
        desc_->interval = interval;
        return *this;
    }

    /** Set system rate.
     * This operation will cause the system to be ran at a multiple of the
     * provided tick source. The tick source may be any entity, including
     * another system.
     *
     * @param tick_source The tick source.
     * @param rate The multiple at which to run the system.
     */
    Base& rate(const entity_t tick_source, int32_t rate) {
        desc_->rate = rate;
        desc_->tick_source = tick_source;
        return *this;
    }

    /** Set system rate.
     * This operation will cause the system to be ran at a multiple of the
     * frame tick frequency. If a tick source was provided, this just updates
     * the rate of the system.
     *
     * @param rate The multiple at which to run the system.
     */
    Base& rate(int32_t rate) {
        desc_->rate = rate;
        return *this;
    }

    /** Set tick source.
     * This operation sets a shared tick source for the system.
     *
     * @tparam T The type associated with the singleton tick source to use for the system.
     */
    template<typename T>
    Base& tick_source() {
        desc_->tick_source = _::type<T>::id(world_v());
        return *this;
    }

    /** Set tick source.
     * This operation sets a shared tick source for the system.
     *
     * @param tick_source The tick source to use for the system.
     */
    Base& tick_source(flecs::entity_t tick_source) {
        desc_->tick_source = tick_source;
        return *this;
    }

    /** Set system context */
    Base& ctx(void *ptr) {
        desc_->ctx = ptr;
        return *this;
    }

    /** Set system run callback */
    Base& run(ecs_iter_action_t action) {
        desc_->run = action;
        return *this;
    }

protected:
    virtual flecs::world_t* world_v() override = 0;

private:
    operator Base&() {
        return *static_cast<Base*>(this);
    }

    ecs_system_desc_t *desc_;
};

}


export namespace flecs {
namespace _ {
    template <typename ... Components>
    using system_builder_base = node_builder<
        system, ecs_system_desc_t, system_builder<Components...>,
        system_builder_i, Components ...>;
}

/** System builder.
 *
 * @ingroup cpp_addons_systems
 */
template <typename ... Components>
struct system_builder final : _::system_builder_base<Components...> {
    system_builder(flecs::world_t* world, const char *name = nullptr)
        : _::system_builder_base<Components...>(world, name)
    {
        _::sig<Components...>(world).populate(this);

#ifdef FLECS_PIPELINE
        ecs_add_id(world, this->desc_.entity, ecs_dependson(flecs::OnUpdate));
        ecs_add_id(world, this->desc_.entity, flecs::OnUpdate);
#endif
    }

    template <typename Func>
    system each(Func&& func);
};

}


export namespace flecs
{

struct system_runner_fluent {
    system_runner_fluent(
        world_t *world,
        entity_t id,
        int32_t stage_current,
        int32_t stage_count,
        ecs_ftime_t delta_time,
        void *param)
        : stage_(world)
        , id_(id)
        , delta_time_(delta_time)
        , param_(param)
        , stage_current_(stage_current)
        , stage_count_(stage_count) { }

    system_runner_fluent& offset(int32_t offset) {
        offset_ = offset;
        return *this;
    }

    system_runner_fluent& limit(int32_t limit) {
        limit_ = limit;
        return *this;
    }

    system_runner_fluent& stage(flecs::world& stage) {
        stage_ = stage.c_ptr();
        return *this;
    }

    ~system_runner_fluent() {
        if (stage_count_) {
            ecs_run_worker(
                stage_, id_, stage_current_, stage_count_, delta_time_,
                param_);
        } else {
            ecs_run(stage_, id_, delta_time_, param_);
        }
    }

private:
    world_t *stage_;
    entity_t id_;
    ecs_ftime_t delta_time_;
    void *param_;
    int32_t offset_;
    int32_t limit_;
    int32_t stage_current_;
    int32_t stage_count_;
};

struct system final : entity
{
    using entity::entity;

    explicit system() {
        id_ = 0;
        world_ = nullptr;
    }

    explicit system(flecs::world_t *world, ecs_system_desc_t *desc) {
        world_ = world;
        id_ = ecs_system_init(world, desc);
    }

    void ctx(void *ctx) {
        ecs_system_desc_t desc = {};
        desc.entity = id_;
        desc.ctx = ctx;
        ecs_system_init(world_, &desc);
    }

    void* ctx() const {
        return ecs_system_get(world_, id_)->ctx;
    }

    flecs::query<> query() const {
        return flecs::query<>(ecs_system_get(world_, id_)->query);
    }

    system_runner_fluent run(ecs_ftime_t delta_time = 0.0f, void *param = nullptr) const {
        return system_runner_fluent(world_, id_, 0, 0, delta_time, param);
    }

    system_runner_fluent run_worker(
        int32_t stage_current,
        int32_t stage_count,
        ecs_ftime_t delta_time = 0.0f,
        void *param = nullptr) const
    {
        return system_runner_fluent(
            world_, id_, stage_current, stage_count, delta_time, param);
    }

#   ifdef FLECS_TIMER
/**
 * @file addons/cpp/mixins/timer/system_mixin.inl
 * @brief Timer module system mixin.
 */

/**
 * @memberof flecs::system
 * @ingroup cpp_addons_timer
 *
 * @{
 */

/** Set interval.
 * @see ecs_set_interval
 */
void interval(ecs_ftime_t interval);

/** Get interval.
 * @see ecs_get_interval.
 */
ecs_ftime_t interval();

/** Set timeout.
 * @see ecs_set_timeout
 */
void timeout(ecs_ftime_t timeout);

/** Get timeout.
 * @see ecs_get_timeout
 */
ecs_ftime_t timeout();

/** Set system rate (system is its own tick source).
 * @see ecs_set_rate
 */
void rate(int32_t rate);

/** Start timer.
 * @see ecs_start_timer
 */
void start();

/** Stop timer.
 * @see ecs_start_timer
 */
void stop();

/** Set external tick source.
 * @see ecs_set_tick_source
 */
template<typename T>
void set_tick_source();

/** Set external tick source.
 * @see ecs_set_tick_source
 */
void set_tick_source(flecs::entity e);

/** @} */

#   endif

};

// Mixin implementation
inline system world::system(flecs::entity e) const {
    return flecs::system(world_, e);
}

template <typename... Comps, typename... Args>
inline system_builder<Comps...> world::system(Args &&... args) const {
    return flecs::system_builder<Comps...>(world_, FLECS_FWD(args)...);
}

namespace _ {

inline void system_init(flecs::world& world) {
    world.component<TickSource>("flecs::system::TickSource");
}

} // namespace _

template <typename ... Components>
template <typename Func>
inline system system_builder<Components...>::each(Func&& func) {
    // Faster version of each() that iterates the query on the C++ side.
    return this->run_each(FLECS_FWD(func));
}

} // export namespace flecs

#endif
#ifdef FLECS_PIPELINE
/**
 * @file addons/cpp/mixins/pipeline/impl.hpp
 * @brief Pipeline module implementation.
 */

#pragma once

/**
 * @file addons/cpp/mixins/pipeline/builder.hpp
 * @brief Pipeline builder.
 */

#pragma once

/**
 * @file addons/cpp/mixins/pipeline/builder_i.hpp
 * @brief Pipeline builder interface.
 */

#pragma once


export namespace flecs {

/** Pipeline builder interface.
 *
 * @ingroup cpp_pipelines
 */
template<typename Base>
struct pipeline_builder_i : query_builder_i<Base> {
    pipeline_builder_i(ecs_pipeline_desc_t *desc, int32_t term_index = 0)
        : query_builder_i<Base>(&desc->query, term_index)
        , desc_(desc) { }

private:
    ecs_pipeline_desc_t *desc_;
};

}


export namespace flecs {
namespace _ {
    template <typename ... Components>
    using pipeline_builder_base = builder<
        pipeline, ecs_pipeline_desc_t, pipeline_builder<Components...>,
        pipeline_builder_i, Components ...>;
}

/** Pipeline builder.
 *
 * @ingroup cpp_pipelines
 */
template <typename ... Components>
struct pipeline_builder final : _::pipeline_builder_base<Components...> {
    pipeline_builder(flecs::world_t* world, flecs::entity_t id = 0)
        : _::pipeline_builder_base<Components...>(world)
    {
        _::sig<Components...>(world).populate(this);
        this->desc_.entity = id;
    }
};

}


export namespace flecs {

template <typename ... Components>
struct pipeline : entity {
    pipeline(world_t *world, ecs_pipeline_desc_t *desc)
        : entity(world)
    {
        id_ = ecs_pipeline_init(world, desc);

        if (!id_) {
            ecs_abort(ECS_INVALID_PARAMETER, NULL);
        }
    }
};

inline flecs::pipeline_builder<> world::pipeline() const {
    return flecs::pipeline_builder<>(world_);
}

template <typename Pipeline, if_not_t< is_enum<Pipeline>::value >>
inline flecs::pipeline_builder<> world::pipeline() const {
    return flecs::pipeline_builder<>(world_, _::type<Pipeline>::id(world_));
}

inline void world::set_pipeline(const flecs::entity pip) const {
    return ecs_set_pipeline(world_, pip);
}

template <typename Pipeline>
inline void world::set_pipeline() const {
    return ecs_set_pipeline(world_, _::type<Pipeline>::id(world_));
}

inline flecs::entity world::get_pipeline() const {
    return flecs::entity(world_, ecs_get_pipeline(world_));
}

inline bool world::progress(ecs_ftime_t delta_time) const {
    return ecs_progress(world_, delta_time);
}

inline void world::run_pipeline(const flecs::entity_t pip, ecs_ftime_t delta_time) const {
    return ecs_run_pipeline(world_, pip, delta_time);
}

template <typename Pipeline, if_not_t< is_enum<Pipeline>::value >>
inline void world::run_pipeline(ecs_ftime_t delta_time) const {
    return ecs_run_pipeline(world_, _::type<Pipeline>::id(world_), delta_time);
}

inline void world::set_time_scale(ecs_ftime_t mul) const {
    ecs_set_time_scale(world_, mul);
}

inline void world::set_target_fps(ecs_ftime_t target_fps) const {
    ecs_set_target_fps(world_, target_fps);
}

inline void world::reset_clock() const {
    ecs_reset_clock(world_);
}

inline void world::set_threads(int32_t threads) const {
    ecs_set_threads(world_, threads);
}

inline int32_t world::get_threads() const {
    return ecs_get_stage_count(world_);
}

inline void world::set_task_threads(int32_t task_threads) const {
    ecs_set_task_threads(world_, task_threads);
}

inline bool world::using_task_threads() const {
    return ecs_using_task_threads(world_);
}

}

#endif
#ifdef FLECS_TIMER
/**
 * @file addons/cpp/mixins/timer/impl.hpp
 * @brief Timer module implementation.
 */

#pragma once

export namespace flecs {

// Timer class
struct timer final : entity {
    using entity::entity;

    timer& interval(ecs_ftime_t interval) {
        ecs_set_interval(world_, id_, interval);
        return *this;
    }

    ecs_ftime_t interval() {
        return ecs_get_interval(world_, id_);
    }

    timer& timeout(ecs_ftime_t timeout) {
        ecs_set_timeout(world_, id_, timeout);
        return *this;
    }

    ecs_ftime_t timeout() {
        return ecs_get_timeout(world_, id_);
    }

    timer& rate(int32_t rate, flecs::entity_t tick_source = 0) {
        ecs_set_rate(world_, id_, rate, tick_source);
        return *this;
    }

    void start() {
        ecs_start_timer(world_, id_);
    }

    void stop() {
        ecs_stop_timer(world_, id_);
    }
};

template <typename T>
inline flecs::timer world::timer() const {
    return flecs::timer(world_, _::type<T>::id(world_));
}

template <typename... Args>
inline flecs::timer world::timer(Args &&... args) const {
    return flecs::timer(world_, FLECS_FWD(args)...);
}

inline void world::randomize_timers() const {
    ecs_randomize_timers(world_);
}

inline void system::interval(ecs_ftime_t interval) {
    ecs_set_interval(world_, id_, interval);
}

inline ecs_ftime_t system::interval() {
    return ecs_get_interval(world_, id_);
}

inline void system::timeout(ecs_ftime_t timeout) {
    ecs_set_timeout(world_, id_, timeout);
}

inline ecs_ftime_t system::timeout() {
    return ecs_get_timeout(world_, id_);
}

inline void system::rate(int32_t rate) {
    ecs_set_rate(world_, id_, rate, 0);
}

inline void system::start() {
    ecs_start_timer(world_, id_);
}

inline void system::stop() {
    ecs_stop_timer(world_, id_);
}

template<typename T>
inline void system::set_tick_source() {
    ecs_set_tick_source(world_, id_, _::type<T>::id(world_));
}

inline void system::set_tick_source(flecs::entity e) {
    ecs_set_tick_source(world_, id_, e);
}

namespace _ {

inline void timer_init(flecs::world& world) {
    world.component<RateFilter>("flecs::timer::RateFilter");
    world.component<Timer>("flecs::timer::Timer");
}

}
}

#endif
#ifdef FLECS_DOC
/**
 * @file addons/cpp/mixins/doc/impl.hpp
 * @brief Doc mixin implementation.
 */

#pragma once

export namespace flecs {
namespace doc {

/** Get UUID for an entity.
 *
 * @see ecs_doc_get_uuid()
 * @see flecs::doc::set_uuid()
 * @see flecs::entity_view::doc_uuid()
 *
 * @ingroup cpp_addons_doc
 */
inline const char* get_uuid(const flecs::entity_view& e) {
    return ecs_doc_get_uuid(e.world(), e);
}

/** Get human readable name for an entity.
 *
 * @see ecs_doc_get_name()
 * @see flecs::doc::set_name()
 * @see flecs::entity_view::doc_name()
 *
 * @ingroup cpp_addons_doc
 */
inline const char* get_name(const flecs::entity_view& e) {
    return ecs_doc_get_name(e.world(), e);
}

/** Get brief description for an entity.
 *
 * @see ecs_doc_get_brief()
 * @see flecs::doc::set_brief()
 * @see flecs::entity_view::doc_brief()
 *
 * @ingroup cpp_addons_doc
 */
inline const char* get_brief(const flecs::entity_view& e) {
    return ecs_doc_get_brief(e.world(), e);
}

/** Get detailed description for an entity.
 *
 * @see ecs_doc_get_detail()
 * @see flecs::doc::set_detail()
 * @see flecs::entity_view::doc_detail()
 *
 * @ingroup cpp_addons_doc
 */
inline const char* get_detail(const flecs::entity_view& e) {
    return ecs_doc_get_detail(e.world(), e);
}

/** Get link to external documentation for an entity.
 *
 * @see ecs_doc_get_link()
 * @see flecs::doc::set_link()
 * @see flecs::entity_view::doc_link()
 *
 * @ingroup cpp_addons_doc
 */
inline const char* get_link(const flecs::entity_view& e) {
    return ecs_doc_get_link(e.world(), e);
}

/** Get color for an entity.
 *
 * @see ecs_doc_get_color()
 * @see flecs::doc::set_color()
 * @see flecs::entity_view::doc_color()
 *
 * @ingroup cpp_addons_doc
 */
inline const char* get_color(const flecs::entity_view& e) {
    return ecs_doc_get_color(e.world(), e);
}

/** Set UUID for an entity.
 *
 * @see ecs_doc_set_uuid()
 * @see flecs::doc::get_uuid()
 * @see flecs::entity_builder::set_doc_uuid()
 *
 * @ingroup cpp_addons_doc
 */
inline void set_uuid(flecs::entity& e, const char *uuid) {
    ecs_doc_set_uuid(e.world(), e, uuid);
}

/** Set human readable name for an entity.
 *
 * @see ecs_doc_set_name()
 * @see flecs::doc::get_name()
 * @see flecs::entity_builder::set_doc_name()
 *
 * @ingroup cpp_addons_doc
 */
inline void set_name(flecs::entity& e, const char *name) {
    ecs_doc_set_name(e.world(), e, name);
}

/** Set brief description for an entity.
 *
 * @see ecs_doc_set_brief()
 * @see flecs::doc::get_brief()
 * @see flecs::entity_builder::set_doc_brief()
 *
 * @ingroup cpp_addons_doc
 */
inline void set_brief(flecs::entity& e, const char *description) {
    ecs_doc_set_brief(e.world(), e, description);
}

/** Set detailed description for an entity.
 *
 * @see ecs_doc_set_detail()
 * @see flecs::doc::get_detail()
 * @see flecs::entity_builder::set_doc_detail()
 *
 * @ingroup cpp_addons_doc
 */
inline void set_detail(flecs::entity& e, const char *description) {
    ecs_doc_set_detail(e.world(), e, description);
}

/** Set link to external documentation for an entity.
 *
 * @see ecs_doc_set_link()
 * @see flecs::doc::get_link()
 * @see flecs::entity_builder::set_doc_link()
 *
 * @ingroup cpp_addons_doc
 */
inline void set_link(flecs::entity& e, const char *link) {
    ecs_doc_set_link(e.world(), e, link);
}

/** Set color for an entity.
 *
 * @see ecs_doc_set_color()
 * @see flecs::doc::get_color()
 * @see flecs::entity_builder::set_doc_color()
 *
 * @ingroup cpp_addons_doc
 */
inline void set_color(flecs::entity& e, const char *color) {
    ecs_doc_set_color(e.world(), e, color);
}

/** @private */
namespace _ {

/** @private */
inline void init(flecs::world& world) {
    world.component<doc::Description>("flecs::doc::Description");
}

} // namespace _
} // namespace doc
} // export namespace flecs

#endif
#ifdef FLECS_DOC
#endif
#ifdef FLECS_REST
/**
 * @file addons/cpp/mixins/rest/impl.hpp
 * @brief Rest module implementation.
 */

#pragma once

export namespace flecs {
namespace rest {
namespace _ {

inline void init(flecs::world& world) {
    world.component<Rest>("flecs::rest::Rest");
}

} // namespace _
} // namespace rest
} // export namespace flecs

#endif
#ifdef FLECS_META
/**
 * @file addons/cpp/mixins/meta/impl.hpp
 * @brief Meta implementation.
 */

#pragma once

FLECS_ENUM_LAST(flecs::meta::type_kind_t, flecs::meta::TypeKindLast)
FLECS_ENUM_LAST(flecs::meta::primitive_kind_t, flecs::meta::PrimitiveKindLast)

export namespace flecs {
namespace meta {
namespace _ {

/* Type support for entity wrappers */
template <typename EntityType>
inline flecs::opaque<EntityType> flecs_entity_support(flecs::world&) {
    return flecs::opaque<EntityType>()
        .as_type(flecs::Entity)
        .serialize([](const flecs::serializer *ser, const EntityType *data) {
            flecs::entity_t id = data->id();
            return ser->value(flecs::Entity, &id);
        })
        .assign_entity(
            [](EntityType *dst, flecs::world_t *world, flecs::entity_t e) {
                *dst = EntityType(world, e);
            });
}

inline void init(flecs::world& world) {
    world.component<bool_t>("flecs::meta::bool");
    world.component<char_t>("flecs::meta::char");
    world.component<u8_t>("flecs::meta::u8");
    world.component<u16_t>("flecs::meta::u16");
    world.component<u32_t>("flecs::meta::u32");
    world.component<u64_t>("flecs::meta::u64");
    world.component<i8_t>("flecs::meta::i8");
    world.component<i16_t>("flecs::meta::i16");
    world.component<i32_t>("flecs::meta::i32");
    world.component<i64_t>("flecs::meta::i64");
    world.component<f32_t>("flecs::meta::f32");
    world.component<f64_t>("flecs::meta::f64");

    world.component<type_kind_t>("flecs::meta::type_kind");
    world.component<primitive_kind_t>("flecs::meta::primitive_kind");
    world.component<member_t>("flecs::meta::member_t");
    world.component<enum_constant_t>("flecs::meta::enum_constant");
    world.component<bitmask_constant_t>("flecs::meta::bitmask_constant");

    world.component<Type>("flecs::meta::type");
    world.component<TypeSerializer>("flecs::meta::TypeSerializer");
    world.component<Primitive>("flecs::meta::primitive");
    world.component<Enum>("flecs::meta::enum");
    world.component<Bitmask>("flecs::meta::bitmask");
    world.component<Member>("flecs::meta::member");
    world.component<MemberRanges>("flecs::meta::member_ranges");
    world.component<Struct>("flecs::meta::struct");
    world.component<Array>("flecs::meta::array");
    world.component<Vector>("flecs::meta::vector");

    world.component<Unit>("flecs::meta::unit");

    // To support member<uintptr_t> and member<intptr_t> register components
    // (that do not have conflicting symbols with builtin ones) for platform
    // specific types.

    if (!flecs::is_same<i32_t, iptr_t>() && !flecs::is_same<i64_t, iptr_t>()) {
        flecs::_::type<iptr_t>::init_builtin(world, flecs::Iptr, true);
        // Remove symbol to prevent validation errors, as it doesn't match with
        // the typename
        ecs_remove_pair(world, flecs::Iptr, ecs_id(EcsIdentifier), EcsSymbol);
    }

    if (!flecs::is_same<u32_t, uptr_t>() && !flecs::is_same<u64_t, uptr_t>()) {
        flecs::_::type<uptr_t>::init_builtin(world, flecs::Uptr, true);
        // Remove symbol to prevent validation errors, as it doesn't match with
        // the typename
        ecs_remove_pair(world, flecs::Uptr, ecs_id(EcsIdentifier), EcsSymbol);
    }

    // Register opaque type support for C++ entity wrappers
    world.entity("::flecs::cpp").add(flecs::Module).scope([&]{
        world.component<flecs::entity_view>()
            .opaque(flecs_entity_support<flecs::entity_view>);
        world.component<flecs::entity>()
            .opaque(flecs_entity_support<flecs::entity>);
    });
}

} // namespace _

} // namespace meta


inline flecs::entity cursor::get_type() const {
    return flecs::entity(cursor_.world, ecs_meta_get_type(&cursor_));
}

inline flecs::entity cursor::get_unit() const {
    return flecs::entity(cursor_.world, ecs_meta_get_unit(&cursor_));
}

inline flecs::entity cursor::get_entity() const {
    return flecs::entity(cursor_.world, ecs_meta_get_entity(&cursor_));
}

/** Create primitive type */
inline flecs::entity world::primitive(flecs::meta::primitive_kind_t kind) {
    ecs_primitive_desc_t desc = {};
    desc.kind = kind;
    flecs::entity_t eid = ecs_primitive_init(world_, &desc);
    ecs_assert(eid != 0, ECS_INVALID_OPERATION, NULL);
    return flecs::entity(world_, eid);
}

/** Create array type. */
inline flecs::entity world::array(flecs::entity_t elem_id, int32_t array_count) {
    ecs_array_desc_t desc = {};
    desc.type = elem_id;
    desc.count = array_count;
    flecs::entity_t eid = ecs_array_init(world_, &desc);
    ecs_assert(eid != 0, ECS_INVALID_OPERATION, NULL);
    return flecs::entity(world_, eid);
}

/** Create array type. */
template <typename T>
inline flecs::entity world::array(int32_t array_count) {
    return this->array(_::type<T>::id(world_), array_count);
}

inline flecs::entity world::vector(flecs::entity_t elem_id) {
    ecs_vector_desc_t desc = {};
    desc.type = elem_id;
    flecs::entity_t eid = ecs_vector_init(world_, &desc);
    ecs_assert(eid != 0, ECS_INVALID_OPERATION, NULL);
    return flecs::entity(world_, eid);
}

template <typename T>
inline flecs::entity world::vector() {
    return this->vector(_::type<T>::id(world_));
}

} // export namespace flecs

inline int ecs_serializer_t::value(ecs_entity_t type, const void *v) const {
    return this->value_(this, type, v);
}

template <typename T>
inline int ecs_serializer_t::value(const T& v) const {
    return this->value(flecs::_::type<T>::id(
        const_cast<flecs::world_t*>(this->world)), &v);
}

inline int ecs_serializer_t::member(const char *name) const {
    return this->member_(this, name);
}

#endif
#ifdef FLECS_UNITS
/**
 * @file addons/cpp/mixins/units/impl.hpp
 * @brief Units module implementation.
 */

#pragma once

export namespace flecs {

inline units::units(flecs::world& world) {
    /* Import C module  */
    FlecsUnitsImport(world);

    /* Bridge between C++ types and flecs.units entities */
    world.module<units>();

    // Initialize world.entity(prefixes) scope
    world.entity<Prefixes>("::flecs::units::prefixes");

    // Initialize prefixes
    world.entity<Yocto>("::flecs::units::prefixes::Yocto");
    world.entity<Zepto>("::flecs::units::prefixes::Zepto");
    world.entity<Atto>("::flecs::units::prefixes::Atto");
    world.entity<Femto>("::flecs::units::prefixes::Femto");
    world.entity<Pico>("::flecs::units::prefixes::Pico");
    world.entity<Nano>("::flecs::units::prefixes::Nano");
    world.entity<Micro>("::flecs::units::prefixes::Micro");
    world.entity<Milli>("::flecs::units::prefixes::Milli");
    world.entity<Centi>("::flecs::units::prefixes::Centi");
    world.entity<Deci>("::flecs::units::prefixes::Deci");
    world.entity<Deca>("::flecs::units::prefixes::Deca");
    world.entity<Hecto>("::flecs::units::prefixes::Hecto");
    world.entity<Kilo>("::flecs::units::prefixes::Kilo");
    world.entity<Mega>("::flecs::units::prefixes::Mega");
    world.entity<Giga>("::flecs::units::prefixes::Giga");
    world.entity<Tera>("::flecs::units::prefixes::Tera");
    world.entity<Peta>("::flecs::units::prefixes::Peta");
    world.entity<Exa>("::flecs::units::prefixes::Exa");
    world.entity<Zetta>("::flecs::units::prefixes::Zetta");
    world.entity<Yotta>("::flecs::units::prefixes::Yotta");
    world.entity<Kibi>("::flecs::units::prefixes::Kibi");
    world.entity<Mebi>("::flecs::units::prefixes::Mebi");
    world.entity<Gibi>("::flecs::units::prefixes::Gibi");
    world.entity<Tebi>("::flecs::units::prefixes::Tebi");
    world.entity<Pebi>("::flecs::units::prefixes::Pebi");
    world.entity<Exbi>("::flecs::units::prefixes::Exbi");
    world.entity<Zebi>("::flecs::units::prefixes::Zebi");
    world.entity<Yobi>("::flecs::units::prefixes::Yobi");

    // Initialize quantities
    world.entity<Duration>("::flecs::units::Duration");
    world.entity<Time>("::flecs::units::Time");
    world.entity<Mass>("::flecs::units::Mass");
    world.entity<Force>("::flecs::units::Force");
    world.entity<ElectricCurrent>("::flecs::units::ElectricCurrent");
    world.entity<Amount>("::flecs::units::Amount");
    world.entity<LuminousIntensity>("::flecs::units::LuminousIntensity");
    world.entity<Length>("::flecs::units::Length");
    world.entity<Pressure>("::flecs::units::Pressure");
    world.entity<Speed>("::flecs::units::Speed");
    world.entity<Temperature>("::flecs::units::Temperature");
    world.entity<Data>("::flecs::units::Data");
    world.entity<DataRate>("::flecs::units::DataRate");
    world.entity<Angle>("::flecs::units::Angle");
    world.entity<Frequency>("::flecs::units::Frequency");
    world.entity<Uri>("::flecs::units::Uri");
    world.entity<Color>("::flecs::units::Color");

    // Initialize duration units
    world.entity<duration::PicoSeconds>(
        "::flecs::units::Duration::PicoSeconds");
    world.entity<duration::NanoSeconds>(
        "::flecs::units::Duration::NanoSeconds");
    world.entity<duration::MicroSeconds>(
        "::flecs::units::Duration::MicroSeconds");
    world.entity<duration::MilliSeconds>(
        "::flecs::units::Duration::MilliSeconds");
    world.entity<duration::Seconds>(
        "::flecs::units::Duration::Seconds");
    world.entity<duration::Minutes>(
        "::flecs::units::Duration::Minutes");
    world.entity<duration::Hours>(
        "::flecs::units::Duration::Hours");
    world.entity<duration::Days>(
        "::flecs::units::Duration::Days");

    // Initialize time units
    world.entity<time::Date>("::flecs::units::Time::Date");

    // Initialize mass units
    world.entity<mass::Grams>("::flecs::units::Mass::Grams");
    world.entity<mass::KiloGrams>("::flecs::units::Mass::KiloGrams");

    // Initialize current units
    world.entity<electric_current::Ampere>
    ("::flecs::units::ElectricCurrent::Ampere");

    // Initialize amount units
    world.entity<amount::Mole>("::flecs::units::Amount::Mole");

    // Initialize luminous intensity units
    world.entity<luminous_intensity::Candela>(
        "::flecs::units::LuminousIntensity::Candela");

    // Initialize force units
    world.entity<force::Newton>("::flecs::units::Force::Newton");

    // Initialize length units
    world.entity<length::Meters>("::flecs::units::Length::Meters");
    world.entity<length::PicoMeters>("::flecs::units::Length::PicoMeters");
    world.entity<length::NanoMeters>("::flecs::units::Length::NanoMeters");
    world.entity<length::MicroMeters>("::flecs::units::Length::MicroMeters");
    world.entity<length::MilliMeters>("::flecs::units::Length::MilliMeters");
    world.entity<length::CentiMeters>("::flecs::units::Length::CentiMeters");
    world.entity<length::KiloMeters>("::flecs::units::Length::KiloMeters");
    world.entity<length::Miles>("::flecs::units::Length::Miles");
    world.entity<length::Pixels>("::flecs::units::Length::Pixels");

    // Initialize pressure units
    world.entity<pressure::Pascal>("::flecs::units::Pressure::Pascal");
    world.entity<pressure::Bar>("::flecs::units::Pressure::Bar");

    // Initialize speed units
    world.entity<speed::MetersPerSecond>(
        "::flecs::units::Speed::MetersPerSecond");
    world.entity<speed::KiloMetersPerSecond>(
        "::flecs::units::Speed::KiloMetersPerSecond");
    world.entity<speed::KiloMetersPerHour>(
        "::flecs::units::Speed::KiloMetersPerHour");
    world.entity<speed::MilesPerHour>(
        "::flecs::units::Speed::MilesPerHour");

    // Initialize temperature units
    world.entity<temperature::Kelvin>(
        "::flecs::units::Temperature::Kelvin");
    world.entity<temperature::Celsius>(
        "::flecs::units::Temperature::Celsius");
    world.entity<temperature::Fahrenheit>(
        "::flecs::units::Temperature::Fahrenheit");

    // Initialize data units
    world.entity<data::Bits>(
        "::flecs::units::Data::Bits");
    world.entity<data::KiloBits>(
        "::flecs::units::Data::KiloBits");
    world.entity<data::MegaBits>(
        "::flecs::units::Data::MegaBits");
    world.entity<data::GigaBits>(
        "::flecs::units::Data::GigaBits");
    world.entity<data::Bytes>(
        "::flecs::units::Data::Bytes");
    world.entity<data::KiloBytes>(
        "::flecs::units::Data::KiloBytes");
    world.entity<data::MegaBytes>(
        "::flecs::units::Data::MegaBytes");
    world.entity<data::GigaBytes>(
        "::flecs::units::Data::GigaBytes");
    world.entity<data::KibiBytes>(
        "::flecs::units::Data::KibiBytes");
    world.entity<data::MebiBytes>(
        "::flecs::units::Data::MebiBytes");
    world.entity<data::GibiBytes>(
        "::flecs::units::Data::GibiBytes");

    // Initialize datarate units
    world.entity<datarate::BitsPerSecond>(
        "::flecs::units::DataRate::BitsPerSecond");
    world.entity<datarate::KiloBitsPerSecond>(
        "::flecs::units::DataRate::KiloBitsPerSecond");
    world.entity<datarate::MegaBitsPerSecond>(
        "::flecs::units::DataRate::MegaBitsPerSecond");
    world.entity<datarate::GigaBitsPerSecond>(
        "::flecs::units::DataRate::GigaBitsPerSecond");
    world.entity<datarate::BytesPerSecond>(
        "::flecs::units::DataRate::BytesPerSecond");
    world.entity<datarate::KiloBytesPerSecond>(
        "::flecs::units::DataRate::KiloBytesPerSecond");
    world.entity<datarate::MegaBytesPerSecond>(
        "::flecs::units::DataRate::MegaBytesPerSecond");
    world.entity<datarate::GigaBytesPerSecond>(
        "::flecs::units::DataRate::GigaBytesPerSecond");

    // Initialize hertz units
    world.entity<frequency::Hertz>(
        "::flecs::units::Frequency::Hertz");
    world.entity<frequency::KiloHertz>(
        "::flecs::units::Frequency::KiloHertz");
    world.entity<frequency::MegaHertz>(
        "::flecs::units::Frequency::MegaHertz");
    world.entity<frequency::GigaHertz>(
        "::flecs::units::Frequency::GigaHertz");

    // Initialize uri units
    world.entity<uri::Hyperlink>(
        "::flecs::units::Uri::Hyperlink");
    world.entity<uri::Image>(
        "::flecs::units::Uri::Image");
    world.entity<uri::File>(
        "::flecs::units::Uri::File");

    // Initialize angles
    world.entity<angle::Radians>(
        "::flecs::units::Angle::Radians");
    world.entity<angle::Degrees>(
        "::flecs::units::Angle::Degrees");

    // Initialize color
    world.entity<color::Rgb>("::flecs::units::Color::Rgb");
    world.entity<color::Hsl>("::flecs::units::Color::Hsl");
    world.entity<color::Css>("::flecs::units::Color::Css");

    // Initialize percentage
    world.entity<Percentage>("::flecs::units::Percentage");

    // Initialize Bel
    world.entity<Bel>("::flecs::units::Bel");
    world.entity<DeciBel>("::flecs::units::DeciBel");
}

}

#endif
#ifdef FLECS_STATS
/**
 * @file addons/cpp/mixins/stats/impl.hpp
 * @brief Monitor module implementation.
 */

#pragma once

export namespace flecs {

inline stats::stats(flecs::world& world) {
#ifdef FLECS_UNITS
    world.import<flecs::units>();
#endif

    /* Import C module  */
    FlecsStatsImport(world);

    world.component<WorldSummary>();
    world.component<WorldStats>();
    world.component<PipelineStats>();
}

}

#endif
#ifdef FLECS_METRICS
/**
 * @file addons/cpp/mixins/metrics/impl.hpp
 * @brief Metrics module implementation.
 */

#pragma once

export namespace flecs {

inline metrics::metrics(flecs::world& world) {
    world.import<flecs::units>();

    /* Import C module  */
    FlecsMetricsImport(world);

    world.component<Value>();
    world.component<Source>();

    world.entity<metrics::Instance>("::flecs::metrics::Instance");
    world.entity<metrics::Metric>("::flecs::metrics::Metric");
    world.entity<metrics::Counter>("::flecs::metrics::Metric::Counter");
    world.entity<metrics::CounterId>("::flecs::metrics::Metric::CounterId");
    world.entity<metrics::CounterIncrement>("::flecs::metrics::Metric::CounterIncrement");
    world.entity<metrics::Gauge>("::flecs::metrics::Metric::Gauge");
}

inline metric_builder::~metric_builder() {
    if (!created_) {
        ecs_metric_init(world_, &desc_);
    }
}

inline metric_builder& metric_builder::member(const char *name) {
    flecs::entity m;
    if (desc_.id) {
        flecs::entity_t type = ecs_get_typeid(world_, desc_.id);
        m = flecs::entity(world_, type).lookup(name);
    } else {
        m = flecs::world(world_).lookup(name);
    }
    if (!m) {
        flecs::log::err("member '%s' not found", name);
    }
    return member(m);
}

template <typename T>
inline metric_builder& metric_builder::member(const char *name) {
    flecs::entity e (world_, _::type<T>::id(world_));
    flecs::entity_t m = e.lookup(name);
    if (!m) {
        flecs::log::err("member '%s' not found in type '%s'",
            name, e.path().c_str());
        return *this;
    }
    return member(m);
}

inline metric_builder& metric_builder::dotmember(const char *expr) {
    desc_.dotmember = expr;
    return *this;
}

template <typename T>
inline metric_builder& metric_builder::dotmember(const char *expr) {
    desc_.dotmember = expr;
    desc_.id = _::type<T>::id(world_);
    return *this;
}

inline metric_builder::operator flecs::entity() {
    if (!created_) {
        created_ = true;
        flecs::entity result(world_, ecs_metric_init(world_, &desc_));
        desc_.entity = result;
        return result;
    } else {
        return flecs::entity(world_, desc_.entity);
    }
}

template <typename... Args>
inline flecs::metric_builder world::metric(Args &&... args) const {
    flecs::entity result(world_, FLECS_FWD(args)...);
    return flecs::metric_builder(world_, result);
}

template <typename Kind>
inline untyped_component& untyped_component::metric(
    flecs::entity_t parent,
    const char *brief,
    const char *metric_name)
{
    flecs::world w(world_);
    flecs::entity e(world_, id_);

    const flecs::member_t *m = ecs_cpp_last_member(w, e);
    if (!m) {
        return *this;
    }

    flecs::entity me = w.entity(m->member);
    flecs::entity metric_entity = me;
    if (parent) {
        const char *component_name = e.name();
        if (!metric_name) {
            if (ecs_os_strcmp(m->name, "value") || !component_name) {
                metric_entity = w.scope(parent).entity(m->name);
            } else {
                // If name of member is "value", use name of type.
                char *snake_name = flecs_to_snake_case(component_name);
                metric_entity = w.scope(parent).entity(snake_name);
                ecs_os_free(snake_name);
            }
        } else {
            metric_entity = w.scope(parent).entity(metric_name);
        }
    }

    w.metric(metric_entity).member(me).kind<Kind>().brief(brief);

    return *this;
}

}

#endif
#ifdef FLECS_ALERTS
/**
 * @file addons/cpp/mixins/alerts/impl.hpp
 * @brief Alerts module implementation.
 */

#pragma once

/**
 * @file addons/cpp/mixins/alerts/builder.hpp
 * @brief Alert builder.
 */

#pragma once

/**
 * @file addons/cpp/mixins/alerts/builder_i.hpp
 * @brief Alert builder interface.
 */

#pragma once


export namespace flecs {

/** Alert builder interface.
 *
 * @ingroup cpp_addons_alerts
 */
template<typename Base, typename ... Components>
struct alert_builder_i : query_builder_i<Base, Components ...> {
private:
    using BaseClass = query_builder_i<Base, Components ...>;

public:
    alert_builder_i()
        : BaseClass(nullptr)
        , desc_(nullptr) { }

    alert_builder_i(ecs_alert_desc_t *desc, int32_t term_index = 0)
        : BaseClass(&desc->query, term_index)
        , desc_(desc) { }

    /** Alert message.
     *
     * @see ecs_alert_desc_t::message
     */
    Base& message(const char *message) {
        desc_->message = message;
        return *this;
    }

    /** Set brief description for alert.
     *
     * @see ecs_alert_desc_t::brief
     */
    Base& brief(const char *brief) {
        desc_->brief = brief;
        return *this;
    }

    /** Set doc name for alert.
     *
     * @see ecs_alert_desc_t::doc_name
     */
    Base& doc_name(const char *doc_name) {
        desc_->doc_name = doc_name;
        return *this;
    }

    /** Set severity of alert (default is Error)
     *
     * @see ecs_alert_desc_t::severity
     */
    Base& severity(flecs::entity_t kind) {
        desc_->severity = kind;
        return *this;
    }

    /* Set retain period of alert.
     *
     * @see ecs_alert_desc_t::retain_period
     */
    Base& retain_period(ecs_ftime_t period) {
        desc_->retain_period = period;
        return *this;
    }

    /** Set severity of alert (default is Error)
     *
     * @see ecs_alert_desc_t::severity
     */
    template <typename Severity>
    Base& severity() {
        return severity(_::type<Severity>::id(world_v()));
    }

    /** Add severity filter */
    Base& severity_filter(flecs::entity_t kind, flecs::id_t with, const char *var = nullptr) {
        ecs_assert(severity_filter_count < ECS_ALERT_MAX_SEVERITY_FILTERS,
            ECS_INVALID_PARAMETER, "Maximum number of severity filters reached");

        ecs_alert_severity_filter_t *filter =
            &desc_->severity_filters[severity_filter_count ++];

        filter->severity = kind;
        filter->with = with;
        filter->var = var;
        return *this;
    }

    /** Add severity filter */
    template <typename Severity>
    Base& severity_filter(flecs::id_t with, const char *var = nullptr) {
        return severity_filter(_::type<Severity>::id(world_v()), with, var);
    }

    /** Add severity filter */
    template <typename Severity, typename T, if_not_t< is_enum<T>::value > = 0>
    Base& severity_filter(const char *var = nullptr) {
        return severity_filter(_::type<Severity>::id(world_v()),
            _::type<T>::id(world_v()), var);
    }

    /** Add severity filter */
    template <typename Severity, typename T, if_t< is_enum<T>::value > = 0 >
    Base& severity_filter(T with, const char *var = nullptr) {
        flecs::world w(world_v());
        flecs::entity constant = w.to_entity<T>(with);
        return severity_filter(_::type<Severity>::id(world_v()),
            w.pair<T>(constant), var);
    }

    /** Set member to create an alert for out of range values */
    Base& member(flecs::entity_t m) {
        desc_->member = m;
        return *this;
    }

    /** Set (component) id for member (optional). If .member() is set and id
     * is not set, the id will default to the member parent. */
    Base& id(flecs::id_t id) {
        desc_->id = id;
        return *this;
    }

    /** Set member to create an alert for out of range values */
    template <typename T>
    Base& member(const char *m, const char *v = nullptr) {
        flecs::entity_t id = _::type<T>::id(world_v());
        flecs::entity_t mid = ecs_lookup_path_w_sep(
            world_v(), id, m, "::", "::", false);
        ecs_assert(m != 0, ECS_INVALID_PARAMETER, NULL);
        desc_->var = v;
        return this->member(mid);
    }

    /** Set source variable for member (optional, defaults to $this) */
    Base& var(const char *v) {
        desc_->var = v;
        return *this;
    }

protected:
    virtual flecs::world_t* world_v() = 0;

private:
    operator Base&() {
        return *static_cast<Base*>(this);
    }

    ecs_alert_desc_t *desc_;
    int32_t severity_filter_count = 0;
};

}


export namespace flecs {
namespace _ {
    template <typename ... Components>
    using alert_builder_base = builder<
        alert, ecs_alert_desc_t, alert_builder<Components...>,
        alert_builder_i, Components ...>;
}

/** Alert builder.
 *
 * @ingroup cpp_addons_alerts
 */
template <typename ... Components>
struct alert_builder final : _::alert_builder_base<Components...> {
    alert_builder(flecs::world_t* world, const char *name = nullptr)
        : _::alert_builder_base<Components...>(world)
    {
        _::sig<Components...>(world).populate(this);
        if (name != nullptr) {
            ecs_entity_desc_t entity_desc = {};
            entity_desc.name = name;
            entity_desc.sep = "::";
            entity_desc.root_sep = "::";
            this->desc_.entity = ecs_entity_init(world, &entity_desc);
        }
    }
};

}


export namespace flecs {

template <typename ... Components>
struct alert final : entity
{
    using entity::entity;

    explicit alert() {
        id_ = 0;
        world_ = nullptr;
    }

    explicit alert(flecs::world_t *world, ecs_alert_desc_t *desc) {
        world_ = world;
        id_ = ecs_alert_init(world, desc);
    }
};

inline alerts::alerts(flecs::world& world) {
    world.import<metrics>();

    /* Import C module  */
    FlecsAlertsImport(world);

    world.component<AlertsActive>();
    world.component<Instance>();

    world.entity<alerts::Alert>("::flecs::alerts::Alert");
    world.entity<alerts::Info>("::flecs::alerts::Info");
    world.entity<alerts::Warning>("::flecs::alerts::Warning");
    world.entity<alerts::Error>("::flecs::alerts::Error");
}

template <typename... Comps, typename... Args>
inline flecs::alert_builder<Comps...> world::alert(Args &&... args) const {
    return flecs::alert_builder<Comps...>(world_, FLECS_FWD(args)...);
}

}

#endif
#ifdef FLECS_SCRIPT
/**
 * @file addons/cpp/mixins/script/impl.hpp
 * @brief Script implementation.
 */

#pragma once


export namespace flecs
{

inline flecs::entity script_builder::run() const {
    ecs_entity_t e = ecs_script_init(world_, &desc_);
    return flecs::entity(world_, e);
}

namespace _ {

    inline ecs_value_t get_const_var(const flecs::world_t *world, const char *name) {
        flecs::entity_t v = ecs_lookup_path_w_sep(
            world, 0, name, "::", "::", false);
        if (!v) {
            ecs_warn("unresolved const variable '%s', returning default", name);
            return {};
        }

        ecs_value_t value = ecs_const_var_get(world, v);
        if (value.ptr == nullptr) {
            ecs_warn("entity '%s' is not a const variable, returning default",
                name);
            return {};
        }

        return value;
    }

    template <typename T>
    inline T get_const_value(
        flecs::world_t *world, const char *name, ecs_value_t value, ecs_entity_t type, const T& default_value)
    {
        char *requested_type = ecs_get_path(world, type);
        char *var_type = ecs_get_path(world, value.type);
        ecs_warn("cannot cast const variable '%s' from type '%s' to '%s', "
                    "returning default", name, var_type, requested_type);
        ecs_os_free(requested_type);
        ecs_os_free(var_type);
        return default_value;
    }

    template <>
    inline char get_const_value<char>(
        flecs::world_t *world, const char *name, ecs_value_t value, ecs_entity_t type, const char& default_value)
    {
        (void)name; (void)type; (void)default_value;
        ecs_meta_cursor_t cur = ecs_meta_cursor(world, value.type, value.ptr);
        return ecs_meta_get_char(&cur);
    }

    template <>
    inline int8_t get_const_value<int8_t>(
        flecs::world_t *world, const char *name, ecs_value_t value, ecs_entity_t type, const int8_t& default_value)
    {
        (void)name; (void)type; (void)default_value;
        ecs_meta_cursor_t cur = ecs_meta_cursor(world, value.type, value.ptr);
        return static_cast<int8_t>(ecs_meta_get_int(&cur));
    }

    template <>
    inline int16_t get_const_value<int16_t>(
        flecs::world_t *world, const char *name, ecs_value_t value, ecs_entity_t type, const int16_t& default_value)
    {
        (void)name; (void)type; (void)default_value;
        ecs_meta_cursor_t cur = ecs_meta_cursor(world, value.type, value.ptr);
        return static_cast<int16_t>(ecs_meta_get_int(&cur));
    }

    template <>
    inline int32_t get_const_value<int32_t>(
        flecs::world_t *world, const char *name, ecs_value_t value, ecs_entity_t type, const int32_t& default_value)
    {
        (void)name; (void)type; (void)default_value;
        ecs_meta_cursor_t cur = ecs_meta_cursor(world, value.type, value.ptr);
        return static_cast<int32_t>(ecs_meta_get_int(&cur));
    }

    template <>
    inline int64_t get_const_value<int64_t>(
        flecs::world_t *world, const char *name, ecs_value_t value, ecs_entity_t type, const int64_t& default_value)
    {
        (void)name; (void)type; (void)default_value;
        ecs_meta_cursor_t cur = ecs_meta_cursor(world, value.type, value.ptr);
        return ecs_meta_get_int(&cur);
    }

    template <>
    inline uint8_t get_const_value<uint8_t>(
        flecs::world_t *world, const char *name, ecs_value_t value, ecs_entity_t type, const uint8_t& default_value)
    {
        (void)name; (void)type; (void)default_value;
        ecs_meta_cursor_t cur = ecs_meta_cursor(world, value.type, value.ptr);
        return static_cast<uint8_t>(ecs_meta_get_uint(&cur));
    }

    template <>
    inline uint16_t get_const_value<uint16_t>(
        flecs::world_t *world, const char *name, ecs_value_t value, ecs_entity_t type, const uint16_t& default_value)
    {
        (void)name; (void)type; (void)default_value;
        ecs_meta_cursor_t cur = ecs_meta_cursor(world, value.type, value.ptr);
        return static_cast<uint16_t>(ecs_meta_get_uint(&cur));
    }

    template <>
    inline uint32_t get_const_value<uint32_t>(
        flecs::world_t *world, const char *name, ecs_value_t value, ecs_entity_t type, const uint32_t& default_value)
    {
        (void)name; (void)type; (void)default_value;
        ecs_meta_cursor_t cur = ecs_meta_cursor(world, value.type, value.ptr);
        return static_cast<uint32_t>(ecs_meta_get_uint(&cur));
    }

    template <>
    inline uint64_t get_const_value<uint64_t>(
        flecs::world_t *world, const char *name, ecs_value_t value, ecs_entity_t type, const uint64_t& default_value)
    {
        (void)name; (void)type; (void)default_value;
        ecs_meta_cursor_t cur = ecs_meta_cursor(world, value.type, value.ptr);
        return ecs_meta_get_uint(&cur);
    }

    template <>
    inline float get_const_value<float>(
        flecs::world_t *world, const char *name, ecs_value_t value, ecs_entity_t type, const float& default_value)
    {
        (void)name; (void)type; (void)default_value;
        ecs_meta_cursor_t cur = ecs_meta_cursor(world, value.type, value.ptr);
        return static_cast<float>(ecs_meta_get_float(&cur));
    }

    template <>
    inline double get_const_value<double>(
        flecs::world_t *world, const char *name, ecs_value_t value, ecs_entity_t type, const double& default_value)
    {
        (void)name; (void)type; (void)default_value;
        ecs_meta_cursor_t cur = ecs_meta_cursor(world, value.type, value.ptr);
        return ecs_meta_get_float(&cur);
    }
}

template <typename T>
inline T world::get_const_var(
    const char *name,
    const T& default_value) const
{
    ecs_value_t value = flecs::_::get_const_var(world_, name);
    if (!value.ptr) {
        return default_value;
    }

    flecs::id_t type = flecs::_::type<T>::id(world_);
    if (type == value.type) {
        return *(static_cast<T*>(value.ptr));
    }

    return flecs::_::get_const_value<T>(
        world_, name, value, type, default_value);
}

template <typename T>
void world::get_const_var(
    const char *name,
    T& out,
    const T& default_value) const
{
    ecs_value_t value = flecs::_::get_const_var(world_, name);
    if (!value.ptr) {
        out = default_value;
        return;
    }

    flecs::id_t type = flecs::_::type<T>::id(world_);
    if (type == value.type) {
        out = *(static_cast<T*>(value.ptr));
        return;
    }

    out = flecs::_::get_const_value<T>(
        world_, name, value, type, default_value);
}


namespace script {
namespace _ {

inline void init(flecs::world& world) {
    world.component<Script>("flecs::script::Script");
}

}
}

}

#endif

/**
 * @file addons/cpp/impl/field.hpp
 * @brief Field implementation.
 */

#pragma once

export namespace flecs
{

template <typename T>
inline field<T>::field(iter &iter, int32_t index) {
    *this = iter.field<T>(index);
}

template <typename T>
T& field<T>::operator[](size_t index) const {
    ecs_assert(data_ != nullptr, ECS_INVALID_OPERATION,
        "invalid nullptr dereference of component type %s",
            _::type_name<T>());
    ecs_assert(index < count_, ECS_COLUMN_INDEX_OUT_OF_RANGE,
        "index %d out of range for array of component type %s",
            index, _::type_name<T>());
    ecs_assert(!index || !is_shared_, ECS_INVALID_PARAMETER,
        "non-zero index invalid for shared field of component type %s",
            _::type_name<T>());
    return data_[index];
}

/** Return first element of component array.
 * This operator is typically used when the field is shared.
 *
 * @return Reference to the first element.
 */
template <typename T>
T& field<T>::operator*() const {
    ecs_assert(data_ != nullptr, ECS_INVALID_OPERATION,
        "invalid nullptr dereference of component type %s",
            _::type_name<T>());
    return *data_;
}

/** Return first element of component array.
 * This operator is typically used when the field is shared.
 *
 * @return Pointer to the first element.
 */
template <typename T>
T* field<T>::operator->() const {
    ecs_assert(data_ != nullptr, ECS_INVALID_OPERATION,
        "invalid nullptr dereference of component type %s",
            _::type_name<T>());
    ecs_assert(data_ != nullptr, ECS_INVALID_OPERATION,
        "-> operator invalid for array with >1 element of "
        "component type %s, use [row] instead",
            _::type_name<T>());
    return data_;
}

}

/**
 * @file addons/cpp/impl/iter.hpp
 * @brief Iterator implementation.
 */

#pragma once

export namespace flecs
{

inline flecs::entity iter::system() const {
    return flecs::entity(iter_->world, iter_->system);
}

inline flecs::entity iter::event() const {
    return flecs::entity(iter_->world, iter_->event);
}

inline flecs::id iter::event_id() const {
    return flecs::id(iter_->world, iter_->event_id);
}

inline flecs::world iter::world() const {
    return flecs::world(iter_->world);
}

inline flecs::entity iter::entity(size_t row) const {
    ecs_assert(row < static_cast<size_t>(iter_->count),
        ECS_COLUMN_INDEX_OUT_OF_RANGE, NULL);
    return flecs::entity(iter_->world, iter_->entities[row]);
}

inline flecs::entity iter::src(int8_t index) const {
    return flecs::entity(iter_->world, ecs_field_src(iter_, index));
}

inline flecs::id iter::id(int8_t index) const {
    return flecs::id(iter_->world, ecs_field_id(iter_, index));
}

inline flecs::id iter::pair(int8_t index) const {
    flecs::id_t id = ecs_field_id(iter_, index);
    ecs_check(ECS_HAS_ID_FLAG(id, PAIR), ECS_INVALID_PARAMETER, NULL);
    return flecs::id(iter_->world, id);
error:
    return flecs::id();
}

inline flecs::type iter::type() const {
    return flecs::type(iter_->world, ecs_table_get_type(iter_->table));
}

inline flecs::table iter::table() const {
    return flecs::table(iter_->real_world, iter_->table);
}

inline flecs::table iter::other_table() const {
    return flecs::table(iter_->real_world, iter_->other_table);
}

inline flecs::table_range iter::range() const {
    return flecs::table_range(iter_->real_world, iter_->table,
        iter_->offset, iter_->count);
}

template <typename T, typename A,
    typename std::enable_if<std::is_const<T>::value, void>::type*>
inline flecs::field<A> iter::field(int8_t index) const {
    ecs_assert(!(iter_->flags & EcsIterCppEach) ||
               ecs_field_src(iter_, index) != 0, ECS_INVALID_OPERATION,
        "cannot .field from .each, use .field_at<%s>(%d, row) instead",
            _::type_name<T>(), index);
    return get_field<A>(index);
}

template <typename T, typename A,
    typename std::enable_if<
        std::is_const<T>::value == false, void>::type*>
inline flecs::field<A> iter::field(int8_t index) const {
    ecs_assert(!(iter_->flags & EcsIterCppEach) ||
               ecs_field_src(iter_, index) != 0, ECS_INVALID_OPERATION,
        "cannot .field from .each, use .field_at<%s>(%d, row) instead",
            _::type_name<T>(), index);
    ecs_assert(!ecs_field_is_readonly(iter_, index),
        ECS_ACCESS_VIOLATION, NULL);
    return get_field<A>(index);
}

inline flecs::entity iter::get_var(int var_id) const {
    ecs_assert(var_id != -1, ECS_INVALID_PARAMETER, 0);
    return flecs::entity(iter_->world, ecs_iter_get_var(iter_, var_id));
}

/** Get value of variable by name.
 * Get value of a query variable for current result.
 */
inline flecs::entity iter::get_var(const char *name) const {
    const flecs::query_t *q = iter_->query;

    int var_id = ecs_query_find_var(q, name);
    ecs_assert(var_id != -1, ECS_INVALID_PARAMETER, name);
    return flecs::entity(iter_->world, ecs_iter_get_var(iter_, var_id));
}

template <typename Func>
void iter::targets(int8_t index, const Func& func) {
    ecs_assert(iter_->table != nullptr, ECS_INVALID_OPERATION, NULL);
    ecs_assert(index < iter_->field_count, ECS_INVALID_PARAMETER, NULL);
    ecs_assert(ecs_field_is_set(iter_, index), ECS_INVALID_PARAMETER, NULL);
    const ecs_type_t *table_type = ecs_table_get_type(iter_->table);
    const ecs_table_record_t *tr = iter_->trs[index];
    int32_t i = tr->index, end = i + tr->count;
    for (; i < end; i ++) {
        ecs_id_t id = table_type->array[i];
        ecs_assert(ECS_IS_PAIR(id), ECS_INVALID_PARAMETER,
            "field does not match a pair");
        flecs::entity tgt(iter_->world,
            ecs_pair_second(iter_->real_world, id));
        func(tgt);
    }
}

} // export namespace flecs

/**
 * @file addons/cpp/impl/world.hpp
 * @brief World implementation.
 */

#pragma once

export namespace flecs
{

inline void world::init_builtin_components() {
    this->component<Component>();
    this->component<Identifier>();
    this->component<Poly>();

    /* If meta is not defined and we're using enum reflection, make sure that
     * primitive types are registered. This makes sure we can set components of
     * underlying_type_t<E> when registering constants. */
#   if !defined(FLECS_META) && !defined(FLECS_CPP_NO_ENUM_REFLECTION)
    this->component<uint8_t>("flecs::meta::u8");
    this->component<uint16_t>("flecs::meta::u16");
    this->component<uint32_t>("flecs::meta::u32");
    this->component<uint64_t>("flecs::meta::u64");
    this->component<int8_t>("flecs::meta::i8");
    this->component<int16_t>("flecs::meta::i16");
    this->component<int32_t>("flecs::meta::i32");
    this->component<int64_t>("flecs::meta::i64");
#   endif

#   ifdef FLECS_SYSTEM
    _::system_init(*this);
#   endif
#   ifdef FLECS_TIMER
    _::timer_init(*this);
#   endif
#   ifdef FLECS_DOC
    doc::_::init(*this);
#   endif
#   ifdef FLECS_REST
    rest::_::init(*this);
#   endif
#   ifdef FLECS_META
    meta::_::init(*this);
#   endif
#   ifdef FLECS_SCRIPT
    script::_::init(*this);
#   endif
}

template <typename T>
inline flecs::entity world::use(const char *alias) const {
    entity_t e = _::type<T>::id(world_);
    const char *name = alias;
    if (!name) {
        // If no name is defined, use the entity name without the scope
        name = ecs_get_name(world_, e);
    }
    ecs_set_alias(world_, e, name);
    return flecs::entity(world_, e);
}

inline flecs::entity world::use(const char *name, const char *alias) const {
    entity_t e = ecs_lookup_path_w_sep(world_, 0, name, "::", "::", true);
    ecs_assert(e != 0, ECS_INVALID_PARAMETER, NULL);

    ecs_set_alias(world_, e, alias);
    return flecs::entity(world_, e);
}

inline void world::use(flecs::entity e, const char *alias) const {
    entity_t eid = e.id();
    const char *name = alias;
    if (!name) {
        // If no name is defined, use the entity name without the scope
        name = ecs_get_name(world_, eid);
    }
    ecs_set_alias(world_, eid, name);
}

inline flecs::entity world::set_scope(const flecs::entity_t s) const {
    return flecs::entity(ecs_set_scope(world_, s));
}

inline flecs::entity world::get_scope() const {
    return flecs::entity(world_, ecs_get_scope(world_));
}

template <typename T>
inline flecs::entity world::set_scope() const {
    return set_scope( _::type<T>::id(world_) );
}

inline entity world::lookup(const char *name, const char *sep, const char *root_sep, bool recursive) const {
    auto e = ecs_lookup_path_w_sep(world_, 0, name, sep, root_sep, recursive);
    return flecs::entity(*this, e);
}

#ifndef ensure
template <typename T>
inline T& world::ensure() const {
    flecs::entity e(world_, _::type<T>::id(world_));
    return e.ensure<T>();
}
#endif

template <typename T>
inline void world::modified() const {
    flecs::entity e(world_, _::type<T>::id(world_));
    e.modified<T>();
}

template <typename First, typename Second>
inline void world::set(Second second, const First& value) const {
    flecs::entity e(world_, _::type<First>::id(world_));
    e.set<First>(second, value);
}

template <typename First, typename Second>
inline void world::set(Second second, First&& value) const {
    flecs::entity e(world_, _::type<First>::id(world_));
    e.set<First>(second, value);
}

template <typename T>
inline ref<T> world::get_ref() const {
    flecs::entity e(world_, _::type<T>::id(world_));
    return e.get_ref<T>();
}

inline const void* world::try_get(flecs::id_t id) const {
    flecs::entity e(world_, id);
    return e.try_get(id);
}

inline const void* world::try_get(flecs::entity_t r, flecs::entity_t t) const {
    flecs::entity e(world_, r);
    return e.try_get(r, t);
}

template <typename T>
inline const T* world::try_get() const {
    flecs::entity e(world_, _::type<T>::id(world_));
    return e.try_get<T>();
}

template <typename First, typename Second, typename P, typename A>
inline const A* world::try_get() const {
    flecs::entity e(world_, _::type<First>::id(world_));
    return e.try_get<First, Second>();
}

template <typename First, typename Second>
inline const First* world::try_get(Second second) const {
    flecs::entity e(world_, _::type<First>::id(world_));
    return e.try_get<First>(second);
}

inline const void* world::get(flecs::id_t id) const {
    flecs::entity e(world_, id);
    return e.get(id);
}

inline const void* world::get(flecs::entity_t r, flecs::entity_t t) const {
    flecs::entity e(world_, r);
    return e.get(r, t);
}

template <typename T>
inline const T& world::get() const {
    flecs::entity e(world_, _::type<T>::id(world_));
    return e.get<T>();
}

template <typename First, typename Second, typename P, typename A>
inline const A& world::get() const {
    flecs::entity e(world_, _::type<First>::id(world_));
    return e.get<First, Second>();
}

template <typename First, typename Second>
const First& world::get(Second second) const {
    flecs::entity e(world_, _::type<First>::id(world_));
    return e.get<First>(second);
}

inline void* world::try_get_mut(flecs::id_t id) const {
    flecs::entity e(world_, id);
    return e.try_get_mut(id);
}

inline void* world::try_get_mut(flecs::entity_t r, flecs::entity_t t) const {
    flecs::entity e(world_, r);
    return e.try_get_mut(r, t);
}

template <typename T>
inline T* world::try_get_mut() const {
    flecs::entity e(world_, _::type<T>::id(world_));
    return e.try_get_mut<T>();
}

template <typename First, typename Second, typename P, typename A>
inline A* world::try_get_mut() const {
    flecs::entity e(world_, _::type<First>::id(world_));
    return e.try_get_mut<First, Second>();
}

template <typename First, typename Second>
inline First* world::try_get_mut(Second second) const {
    flecs::entity e(world_, _::type<First>::id(world_));
    return e.try_get_mut<First>(second);
}

inline void* world::get_mut(flecs::id_t id) const {
    flecs::entity e(world_, id);
    return e.get_mut(id);
}

inline void* world::get_mut(flecs::entity_t r, flecs::entity_t t) const {
    flecs::entity e(world_, r);
    return e.get_mut(r, t);
}

template <typename T>
inline T& world::get_mut() const {
    flecs::entity e(world_, _::type<T>::id(world_));
    return e.get_mut<T>();
}

template <typename First, typename Second, typename P, typename A>
inline A& world::get_mut() const {
    flecs::entity e(world_, _::type<First>::id(world_));
    return e.get_mut<First, Second>();
}

template <typename First, typename Second>
inline First& world::get_mut(Second second) const {
    flecs::entity e(world_, _::type<First>::id(world_));
    return e.get_mut<First>(second);
}

template <typename T>
inline bool world::has() const {
    flecs::entity e(world_, _::type<T>::id(world_));
    return e.has<T>();
}

template <typename First, typename Second>
inline bool world::has() const {
    flecs::entity e(world_, _::type<First>::id(world_));
    return e.has<First, Second>();
}

template <typename First>
inline bool world::has(flecs::id_t second) const {
    flecs::entity e(world_, _::type<First>::id(world_));
    return e.has<First>(second);
}

inline bool world::has(flecs::id_t first, flecs::id_t second) const {
    flecs::entity e(world_, first);
    return e.has(first, second);
}

template <typename E, if_t< is_enum<E>::value > >
inline bool world::has(E value) const {
    flecs::entity e(world_, _::type<E>::id(world_));
    return e.has(value);
}

template <typename T>
inline void world::add() const {
    flecs::entity e(world_, _::type<T>::id(world_));
    e.add<T>();
}

template <typename First, typename Second>
inline void world::add() const {
    flecs::entity e(world_, _::type<First>::id(world_));
    e.add<First, Second>();
}

template <typename First>
inline void world::add(flecs::entity_t second) const {
    flecs::entity e(world_, _::type<First>::id(world_));
    e.add<First>(second);
}

inline void world::add(flecs::entity_t first, flecs::entity_t second) const {
    flecs::entity e(world_, first);
    e.add(first, second);
}

template <typename E, if_t< is_enum<E>::value > >
inline void world::add(E value) const {
    flecs::entity e(world_, _::type<E>::id(world_));
    e.add(value);
}

template <typename T>
inline void world::remove() const {
    flecs::entity e(world_, _::type<T>::id(world_));
    e.remove<T>();
}

template <typename First, typename Second>
inline void world::remove() const {
    flecs::entity e(world_, _::type<First>::id(world_));
    e.remove<First, Second>();
}

template <typename First>
inline void world::remove(flecs::entity_t second) const {
    flecs::entity e(world_, _::type<First>::id(world_));
    e.remove<First>(second);
}

inline void world::remove(flecs::entity_t first, flecs::entity_t second) const {
    flecs::entity e(world_, first);
    e.remove(first, second);
}

template <typename Func>
inline void world::children(Func&& f) const {
    this->entity(0).children(FLECS_FWD(f));
}

template <typename T>
inline flecs::entity world::singleton() const {
    return flecs::entity(world_, _::type<T>::id(world_));
}

template <typename First>
inline flecs::entity world::target(int32_t index) const
{
    return flecs::entity(world_,
        ecs_get_target(world_, _::type<First>::id(world_), _::type<First>::id(world_), index));
}

template <typename T>
inline flecs::entity world::target(
    flecs::entity_t relationship,
    int32_t index) const
{
    return flecs::entity(world_,
        ecs_get_target(world_, _::type<T>::id(world_), relationship, index));
}

inline flecs::entity world::target(
    flecs::entity_t relationship,
    int32_t index) const
{
    return flecs::entity(world_,
        ecs_get_target(world_, relationship, relationship, index));
}

template <typename Func, if_t< is_callable<Func>::value > >
inline void world::get(const Func& func) const {
    static_assert(arity<Func>::value == 1, "singleton component must be the only argument");
    _::entity_with_delegate<Func>::invoke_get(
        this->world_, this->singleton<first_arg_t<Func>>(), func);
}

template <typename Func, if_t< is_callable<Func>::value > >
inline void world::set(const Func& func) const {
    static_assert(arity<Func>::value == 1, "singleton component must be the only argument");
    _::entity_with_delegate<Func>::invoke_ensure(
        this->world_, this->singleton<first_arg_t<Func>>(), func);
}

inline flecs::entity world::get_alive(flecs::entity_t e) const {
    e = ecs_get_alive(world_, e);
    return flecs::entity(world_, e);
}

inline flecs::entity world::make_alive(flecs::entity_t e) const {
    ecs_make_alive(world_, e);
    return flecs::entity(world_, e);
}

template <typename E>
inline flecs::entity enum_data<E>::entity() const {
    return flecs::entity(world_, _::type<E>::id(world_));
}

template <typename E>
inline flecs::entity enum_data<E>::entity(underlying_type_t<E> value) const {
    int index = index_by_value(value);
    if (index >= 0) {
        int32_t constant_i = impl_.constants[index].index;
        flecs::entity_t entity = flecs_component_ids_get(world_, constant_i);
        return flecs::entity(world_, entity);
    }
#ifdef FLECS_META
    // Reflection data lookup failed. Try value lookup amongst flecs::Constant relationships
    flecs::world world = flecs::world(world_);
    return world.query_builder()
        .with(flecs::ChildOf, world.id<E>())
        .with(flecs::Constant, world.id<int32_t>())
        .build()
        .find([value](flecs::entity constant) {
            const int32_t& constant_value = constant.get_second<int32_t>(flecs::Constant);
            return value == static_cast<underlying_type_t<E>>(constant_value);
        });
#else
    return flecs::entity::null(world_);
#endif
}

template <typename E>
inline flecs::entity enum_data<E>::entity(E value) const {
    return entity(static_cast<underlying_type_t<E>>(value));
}

/** Use provided scope for operations ran on returned world.
 * Operations need to be ran in a single statement.
 */
inline flecs::scoped_world world::scope(id_t parent) const {
    return scoped_world(world_, parent);
}

template <typename T>
inline flecs::scoped_world world::scope() const {
    flecs::id_t parent = _::type<T>::id(world_);
    return scoped_world(world_, parent);
}

inline flecs::scoped_world world::scope(const char* name) const {
  return scope(entity(name));
}

} // export namespace flecs


/**
 * @defgroup cpp_core Core
 * Core ECS functionality (entities, storage, queries)
 *
 * @{
 * @}
 */

/**
 * @defgroup cpp_addons Addons
 * C++ APIs for addons.
 *
 * @{
 * @}
 */

/** @} */

#endif // __cplusplus

