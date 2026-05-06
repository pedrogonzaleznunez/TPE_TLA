/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 1

/* Pull parsers.  */
#define YYPULL 0




/* First part of user prologue.  */
#line 1 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"


#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"
#include "BisonActions.h"
#include <stdbool.h>
#include <stdlib.h>

/**
 * The error reporting function for Bison parser.
 *
 * @todo Add location to the grammar and "pushToken" API function.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Error-Reporting-Function.html
 * @see https://www.gnu.org/software/bison/manual/html_node/Tracking-Locations.html
 */
void yyerror(const YYLTYPE * location, const char * message) {}


#line 91 "src/main/c/frontend/syntactic-analysis/BisonParser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "BisonParser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_HARDWARE = 3,                   /* HARDWARE  */
  YYSYMBOL_ROUTINE = 4,                    /* ROUTINE  */
  YYSYMBOL_VAR = 5,                        /* VAR  */
  YYSYMBOL_IF = 6,                         /* IF  */
  YYSYMBOL_ELSE = 7,                       /* ELSE  */
  YYSYMBOL_REPEAT = 8,                     /* REPEAT  */
  YYSYMBOL_EVERY = 9,                      /* EVERY  */
  YYSYMBOL_WAIT = 10,                      /* WAIT  */
  YYSYMBOL_FOR = 11,                       /* FOR  */
  YYSYMBOL_FROM = 12,                      /* FROM  */
  YYSYMBOL_TO = 13,                        /* TO  */
  YYSYMBOL_TIMES = 14,                     /* TIMES  */
  YYSYMBOL_ON = 15,                        /* ON  */
  YYSYMBOL_KW_LED = 16,                    /* KW_LED  */
  YYSYMBOL_KW_BUZZER = 17,                 /* KW_BUZZER  */
  YYSYMBOL_KW_BUTTON = 18,                 /* KW_BUTTON  */
  YYSYMBOL_KW_POTENTIOMETER = 19,          /* KW_POTENTIOMETER  */
  YYSYMBOL_KW_SERVO = 20,                  /* KW_SERVO  */
  YYSYMBOL_KW_ULTRASONIC = 21,             /* KW_ULTRASONIC  */
  YYSYMBOL_KW_DHT11 = 22,                  /* KW_DHT11  */
  YYSYMBOL_KW_LCD = 23,                    /* KW_LCD  */
  YYSYMBOL_TRIG = 24,                      /* TRIG  */
  YYSYMBOL_ECHO = 25,                      /* ECHO  */
  YYSYMBOL_TRUE = 26,                      /* TRUE  */
  YYSYMBOL_FALSE = 27,                     /* FALSE  */
  YYSYMBOL_AND = 28,                       /* AND  */
  YYSYMBOL_OR = 29,                        /* OR  */
  YYSYMBOL_NOT = 30,                       /* NOT  */
  YYSYMBOL_PLUS = 31,                      /* PLUS  */
  YYSYMBOL_MINUS = 32,                     /* MINUS  */
  YYSYMBOL_STAR = 33,                      /* STAR  */
  YYSYMBOL_SLASH = 34,                     /* SLASH  */
  YYSYMBOL_ASSIGN = 35,                    /* ASSIGN  */
  YYSYMBOL_EQ = 36,                        /* EQ  */
  YYSYMBOL_NE = 37,                        /* NE  */
  YYSYMBOL_LT = 38,                        /* LT  */
  YYSYMBOL_GT = 39,                        /* GT  */
  YYSYMBOL_LE = 40,                        /* LE  */
  YYSYMBOL_GE = 41,                        /* GE  */
  YYSYMBOL_LBRACE = 42,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 43,                    /* RBRACE  */
  YYSYMBOL_LPAREN = 44,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 45,                    /* RPAREN  */
  YYSYMBOL_SEMI = 46,                      /* SEMI  */
  YYSYMBOL_COMMA = 47,                     /* COMMA  */
  YYSYMBOL_COLON = 48,                     /* COLON  */
  YYSYMBOL_DOT = 49,                       /* DOT  */
  YYSYMBOL_INTEGER = 50,                   /* INTEGER  */
  YYSYMBOL_FLOAT = 51,                     /* FLOAT  */
  YYSYMBOL_TIME = 52,                      /* TIME  */
  YYSYMBOL_ANALOG_PIN = 53,                /* ANALOG_PIN  */
  YYSYMBOL_IDENTIFIER = 54,                /* IDENTIFIER  */
  YYSYMBOL_STRING = 55,                    /* STRING  */
  YYSYMBOL_IGNORED = 56,                   /* IGNORED  */
  YYSYMBOL_UNKNOWN = 57,                   /* UNKNOWN  */
  YYSYMBOL_UMINUS = 58,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 59,                  /* $accept  */
  YYSYMBOL_program = 60,                   /* program  */
  YYSYMBOL_hardware_block = 61,            /* hardware_block  */
  YYSYMBOL_hardware_decls = 62,            /* hardware_decls  */
  YYSYMBOL_hardware_decl = 63,             /* hardware_decl  */
  YYSYMBOL_pin = 64,                       /* pin  */
  YYSYMBOL_digital_pin = 65,               /* digital_pin  */
  YYSYMBOL_analog_pin = 66,                /* analog_pin  */
  YYSYMBOL_routine_block = 67,             /* routine_block  */
  YYSYMBOL_stmt_list = 68,                 /* stmt_list  */
  YYSYMBOL_block = 69,                     /* block  */
  YYSYMBOL_stmt = 70,                      /* stmt  */
  YYSYMBOL_call = 71,                      /* call  */
  YYSYMBOL_arg_list_opt = 72,              /* arg_list_opt  */
  YYSYMBOL_arg_list = 73,                  /* arg_list  */
  YYSYMBOL_expr = 74                       /* expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   266

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  16
/* YYNRULES -- Number of rules.  */
#define YYNRULES  57
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  153

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   313


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    97,    97,   100,   103,   104,   107,   108,   109,   110,
     111,   112,   113,   115,   119,   120,   123,   126,   129,   132,
     133,   136,   139,   140,   141,   142,   143,   144,   145,   146,
     149,   152,   153,   156,   157,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  static const char *const yy_sname[] =
  {
  "end of file", "error", "invalid token", "HARDWARE", "ROUTINE", "VAR",
  "IF", "ELSE", "REPEAT", "EVERY", "WAIT", "FOR", "FROM", "TO", "TIMES",
  "ON", "KW_LED", "KW_BUZZER", "KW_BUTTON", "KW_POTENTIOMETER", "KW_SERVO",
  "KW_ULTRASONIC", "KW_DHT11", "KW_LCD", "TRIG", "ECHO", "TRUE", "FALSE",
  "AND", "OR", "NOT", "PLUS", "MINUS", "STAR", "SLASH", "ASSIGN", "EQ",
  "NE", "LT", "GT", "LE", "GE", "LBRACE", "RBRACE", "LPAREN", "RPAREN",
  "SEMI", "COMMA", "COLON", "DOT", "INTEGER", "FLOAT", "TIME",
  "ANALOG_PIN", "IDENTIFIER", "STRING", "IGNORED", "UNKNOWN", "UMINUS",
  "$accept", "program", "hardware_block", "hardware_decls",
  "hardware_decl", "pin", "digital_pin", "analog_pin", "routine_block",
  "stmt_list", "block", "stmt", "call", "arg_list_opt", "arg_list", "expr", YY_NULLPTR
  };
  return yy_sname[yysymbol];
}
#endif

#define YYPACT_NINF (-94)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      14,   -21,    23,    18,   -94,   -94,   -18,   -94,    -7,   -94,
     -28,   -25,   -23,   -16,     8,    12,    16,    17,   -94,    28,
      22,    62,    63,    64,    65,    69,    71,    73,    74,   -94,
      37,    85,    55,    40,    39,   -94,    45,   -94,    49,   -45,
     -45,   -45,   -45,   -45,    53,   -45,    56,    66,   -94,   -94,
      85,    85,    85,   -94,   -94,   -94,    45,   -94,   -94,   174,
      50,   129,    57,    96,    59,   -94,   -94,   -94,   -94,   -94,
     -94,   -94,   -94,   -94,   -94,    90,   -94,    68,    85,   214,
     -94,   159,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,   -94,   109,    77,    77,   -94,    85,
      76,    75,    78,   143,   -94,   214,   203,   -27,   -27,   -94,
     -94,   225,   225,   225,   225,   225,   225,    29,    77,   -94,
     -94,   113,    85,    68,    68,   -94,   -94,   -94,    85,    79,
      80,   189,    81,    83,   174,   -94,    85,    97,    68,   -94,
     189,    84,    86,    68,    68,    89,    91,   -94,    68,   101,
      68,   110,   -94
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     4,     1,     0,     2,     0,    19,
       0,     0,     0,     0,     0,     0,     0,     0,     3,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     5,
       0,     0,     0,     0,     0,    18,     0,    20,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,    57,
       0,     0,     0,    52,    53,    54,    51,    55,    50,     0,
       0,     0,     0,     0,     0,    23,    16,    17,     6,    14,
      15,     7,     8,     9,    10,     0,    11,     0,     0,    37,
      48,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    19,    25,     0,     0,    24,     0,
       0,     0,     0,     0,    49,    36,    35,    44,    45,    46,
      47,    38,    39,    40,    41,    42,    43,     0,     0,    27,
      28,     0,    31,     0,     0,    22,    21,    26,     0,     0,
      32,    33,     0,     0,     0,    30,     0,     0,     0,    29,
      34,     0,     0,     0,     0,     0,     0,    12,     0,     0,
       0,     0,    13
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -94,   -94,   -94,   -94,   -94,     2,   -75,   -94,   -94,    27,
     -93,   -94,   -19,   -94,   -94,   -32
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     3,     8,    19,    68,    69,    70,     7,    20,
      95,    37,    58,   129,   130,    59
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      61,    38,   102,   119,   120,    66,    86,    87,    67,    10,
      11,    12,    13,    14,    15,    16,    17,     1,    79,    80,
      81,     4,     6,     5,     9,   127,    21,    30,    31,    22,
      32,    23,    33,    34,    30,    31,    18,    32,    24,    33,
      34,   139,    71,    72,    73,    74,   103,    76,   132,   133,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    25,   142,    60,    35,    26,   121,   145,   146,
      27,    28,   126,   149,    29,   151,    36,    39,    40,    41,
      42,    48,    49,    36,    43,    50,    44,    51,    45,    46,
     131,    47,    62,    63,    64,    65,   134,    75,    38,    52,
      77,    78,    96,    98,   140,    53,    54,    55,    99,    56,
      57,    48,    49,   100,   101,    50,   118,    51,    66,    94,
     122,   117,   141,   123,   135,   124,   128,   136,   137,    52,
     138,     0,   143,   144,   147,    53,    54,    55,   148,    56,
      57,    82,    83,    97,    84,    85,    86,    87,   150,    88,
      89,    90,    91,    92,    93,   152,     0,    82,    83,     0,
      84,    85,    86,    87,     0,    88,    89,    90,    91,    92,
      93,    82,    83,     0,    84,    85,    86,    87,     0,    88,
      89,    90,    91,    92,    93,     0,     0,    82,    83,   125,
      84,    85,    86,    87,     0,    88,    89,    90,    91,    92,
      93,     0,    82,    83,   104,    84,    85,    86,    87,     0,
      88,    89,    90,    91,    92,    93,    94,    82,    83,     0,
      84,    85,    86,    87,     0,    88,    89,    90,    91,    92,
      93,    82,     0,     0,    84,    85,    86,    87,     0,    88,
      89,    90,    91,    92,    93,    84,    85,    86,    87,     0,
      88,    89,    90,    91,    92,    93,    84,    85,    86,    87,
       0,    -1,    -1,    -1,    -1,    -1,    -1
};

static const yytype_int16 yycheck[] =
{
      32,    20,    77,    96,    97,    50,    33,    34,    53,    16,
      17,    18,    19,    20,    21,    22,    23,     3,    50,    51,
      52,    42,     4,     0,    42,   118,    54,     5,     6,    54,
       8,    54,    10,    11,     5,     6,    43,     8,    54,    10,
      11,   134,    40,    41,    42,    43,    78,    45,   123,   124,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    54,   138,     9,    43,    54,    99,   143,   144,
      54,    54,    43,   148,    46,   150,    54,    15,    15,    15,
      15,    26,    27,    54,    15,    30,    15,    32,    15,    15,
     122,    54,    52,    54,    49,    46,   128,    44,   117,    44,
      44,    35,    52,    46,   136,    50,    51,    52,    12,    54,
      55,    26,    27,    54,    24,    30,     7,    32,    50,    42,
      44,    94,    25,    48,    45,    47,    13,    47,    47,    44,
      47,    -1,    48,    47,    45,    50,    51,    52,    47,    54,
      55,    28,    29,    14,    31,    32,    33,    34,    47,    36,
      37,    38,    39,    40,    41,    45,    -1,    28,    29,    -1,
      31,    32,    33,    34,    -1,    36,    37,    38,    39,    40,
      41,    28,    29,    -1,    31,    32,    33,    34,    -1,    36,
      37,    38,    39,    40,    41,    -1,    -1,    28,    29,    46,
      31,    32,    33,    34,    -1,    36,    37,    38,    39,    40,
      41,    -1,    28,    29,    45,    31,    32,    33,    34,    -1,
      36,    37,    38,    39,    40,    41,    42,    28,    29,    -1,
      31,    32,    33,    34,    -1,    36,    37,    38,    39,    40,
      41,    28,    -1,    -1,    31,    32,    33,    34,    -1,    36,
      37,    38,    39,    40,    41,    31,    32,    33,    34,    -1,
      36,    37,    38,    39,    40,    41,    31,    32,    33,    34,
      -1,    36,    37,    38,    39,    40,    41
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    60,    61,    42,     0,     4,    67,    62,    42,
      16,    17,    18,    19,    20,    21,    22,    23,    43,    63,
      68,    54,    54,    54,    54,    54,    54,    54,    54,    46,
       5,     6,     8,    10,    11,    43,    54,    70,    71,    15,
      15,    15,    15,    15,    15,    15,    15,    54,    26,    27,
      30,    32,    44,    50,    51,    52,    54,    55,    71,    74,
       9,    74,    52,    54,    49,    46,    50,    53,    64,    65,
      66,    64,    64,    64,    64,    44,    64,    44,    35,    74,
      74,    74,    28,    29,    31,    32,    33,    34,    36,    37,
      38,    39,    40,    41,    42,    69,    52,    14,    46,    12,
      54,    24,    65,    74,    45,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    68,     7,    69,
      69,    74,    44,    48,    47,    46,    43,    69,    13,    72,
      73,    74,    65,    65,    74,    45,    47,    47,    47,    69,
      74,    25,    65,    48,    47,    65,    65,    45,    47,    65,
      47,    65,    45
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    59,    60,    61,    62,    62,    63,    63,    63,    63,
      63,    63,    63,    63,    64,    64,    65,    66,    67,    68,
      68,    69,    70,    70,    70,    70,    70,    70,    70,    70,
      71,    72,    72,    73,    73,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     4,     0,     3,     4,     4,     4,     4,
       4,     4,    12,    16,     1,     1,     1,     1,     4,     0,
       2,     3,     5,     2,     3,     3,     5,     4,     4,     7,
       6,     0,     1,     1,     3,     3,     3,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     3,
       1,     1,     1,     1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif
/* Parser data structure.  */
struct yypstate
  {
    /* Number of syntax errors so far.  */
    int yynerrs;

    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;
    /* Whether this instance has not started parsing yet.
     * If 2, it corresponds to a finished parsing.  */
    int yynew;
  };


/* Context of a parse error.  */
typedef struct
{
  yypstate* yyps;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypstate_expected_tokens (yypstate *yyps,
                          yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyps->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}


/* Similar to the previous function.  */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  return yypstate_expected_tokens (yyctx->yyps, yyarg, yyargn);
}


#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif



static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yystrlen (yysymbol_name (yyarg[yyi]));
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp = yystpcpy (yyp, yysymbol_name (yyarg[yyi++]));
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yykind)
    {
    case YYSYMBOL_IDENTIFIER: /* IDENTIFIER  */
#line 48 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
            { free(((*yyvaluep).string)); }
#line 1331 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
        break;

    case YYSYMBOL_STRING: /* STRING  */
#line 48 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
            { free(((*yyvaluep).string)); }
#line 1337 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
        break;

    case YYSYMBOL_hardware_block: /* hardware_block  */
#line 53 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
            { destroyEzHardwareBlock(((*yyvaluep).hardwareBlock)); }
#line 1343 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
        break;

    case YYSYMBOL_hardware_decls: /* hardware_decls  */
#line 53 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
            { destroyEzHardwareBlock(((*yyvaluep).hardwareBlock)); }
#line 1349 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
        break;

    case YYSYMBOL_routine_block: /* routine_block  */
#line 52 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
            { destroyEzBlock(((*yyvaluep).block)); }
#line 1355 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
        break;

    case YYSYMBOL_stmt_list: /* stmt_list  */
#line 52 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
            { destroyEzBlock(((*yyvaluep).block)); }
#line 1361 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
        break;

    case YYSYMBOL_block: /* block  */
#line 52 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
            { destroyEzBlock(((*yyvaluep).block)); }
#line 1367 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
        break;

    case YYSYMBOL_stmt: /* stmt  */
#line 51 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
            { destroyEzStatement(((*yyvaluep).statement)); }
#line 1373 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
        break;

    case YYSYMBOL_call: /* call  */
#line 49 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
            { destroyEzExpression(((*yyvaluep).expression)); }
#line 1379 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
        break;

    case YYSYMBOL_arg_list_opt: /* arg_list_opt  */
#line 50 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
            { destroyEzExpressionList(((*yyvaluep).expressionList)); }
#line 1385 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
        break;

    case YYSYMBOL_arg_list: /* arg_list  */
#line 50 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
            { destroyEzExpressionList(((*yyvaluep).expressionList)); }
#line 1391 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
        break;

    case YYSYMBOL_expr: /* expr  */
#line 49 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
            { destroyEzExpression(((*yyvaluep).expression)); }
#line 1397 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
        break;

      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}





#define yynerrs yyps->yynerrs
#define yystate yyps->yystate
#define yyerrstatus yyps->yyerrstatus
#define yyssa yyps->yyssa
#define yyss yyps->yyss
#define yyssp yyps->yyssp
#define yyvsa yyps->yyvsa
#define yyvs yyps->yyvs
#define yyvsp yyps->yyvsp
#define yylsa yyps->yylsa
#define yyls yyps->yyls
#define yylsp yyps->yylsp
#define yystacksize yyps->yystacksize

/* Initialize the parser data structure.  */
static void
yypstate_clear (yypstate *yyps)
{
  yynerrs = 0;
  yystate = 0;
  yyerrstatus = 0;

  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

  /* Initialize the state stack, in case yypcontext_expected_tokens is
     called before the first call to yyparse. */
  *yyssp = 0;
  yyps->yynew = 1;
}

/* Initialize the parser data structure.  */
yypstate *
yypstate_new (void)
{
  yypstate *yyps;
  yyps = YY_CAST (yypstate *, YYMALLOC (sizeof *yyps));
  if (!yyps)
    return YY_NULLPTR;
  yystacksize = YYINITDEPTH;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yypstate_clear (yyps);
  return yyps;
}

void
yypstate_delete (yypstate *yyps)
{
  if (yyps)
    {
#ifndef yyoverflow
      /* If the stack was reallocated but the parse did not complete, then the
         stack still needs to be freed.  */
      if (yyss != yyssa)
        YYSTACK_FREE (yyss);
#endif
      YYFREE (yyps);
    }
}



/*---------------.
| yypush_parse.  |
`---------------*/

int
yypush_parse (yypstate *yyps,
              int yypushed_char, YYSTYPE const *yypushed_val, YYLTYPE *yypushed_loc)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  switch (yyps->yynew)
    {
    case 0:
      yyn = yypact[yystate];
      goto yyread_pushed_token;

    case 2:
      yypstate_clear (yyps);
      break;

    default:
      break;
    }

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = *yypushed_loc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      if (!yyps->yynew)
        {
          YYDPRINTF ((stderr, "Return for a new token:\n"));
          yyresult = YYPUSH_MORE;
          goto yypushreturn;
        }
      yyps->yynew = 0;
yyread_pushed_token:
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yypushed_char;
      if (yypushed_val)
        yylval = *yypushed_val;
      if (yypushed_loc)
        yylloc = *yypushed_loc;
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: hardware_block routine_block  */
#line 97 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                        { (yyval.program) = EzProgramSemanticAction((yyvsp[-1].hardwareBlock), (yyvsp[0].block)); }
#line 1766 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 3: /* hardware_block: HARDWARE LBRACE hardware_decls RBRACE  */
#line 100 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                        { (yyval.hardwareBlock) = (yyvsp[-1].hardwareBlock); }
#line 1772 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 4: /* hardware_decls: %empty  */
#line 103 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                        { (yyval.hardwareBlock) = EzHardwareBlockSemanticAction(); }
#line 1778 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 5: /* hardware_decls: hardware_decls hardware_decl SEMI  */
#line 104 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                { (yyval.hardwareBlock) = EzHardwareBlockAppendDeclSemanticAction((yyvsp[-2].hardwareBlock), (yyvsp[-1].hardwareDecl)); }
#line 1784 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 6: /* hardware_decl: KW_LED IDENTIFIER ON pin  */
#line 107 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                        { (yyval.hardwareDecl) = EzSinglePinDeclSemanticAction(EZ_COMPONENT_LED, (yyvsp[-2].string), (yyvsp[0].pin)); }
#line 1790 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 7: /* hardware_decl: KW_BUZZER IDENTIFIER ON pin  */
#line 108 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                { (yyval.hardwareDecl) = EzSinglePinDeclSemanticAction(EZ_COMPONENT_BUZZER, (yyvsp[-2].string), (yyvsp[0].pin)); }
#line 1796 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 8: /* hardware_decl: KW_BUTTON IDENTIFIER ON pin  */
#line 109 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                { (yyval.hardwareDecl) = EzSinglePinDeclSemanticAction(EZ_COMPONENT_BUTTON, (yyvsp[-2].string), (yyvsp[0].pin)); }
#line 1802 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 9: /* hardware_decl: KW_POTENTIOMETER IDENTIFIER ON pin  */
#line 110 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                        { (yyval.hardwareDecl) = EzSinglePinDeclSemanticAction(EZ_COMPONENT_POTENTIOMETER, (yyvsp[-2].string), (yyvsp[0].pin)); }
#line 1808 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 10: /* hardware_decl: KW_SERVO IDENTIFIER ON pin  */
#line 111 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                { (yyval.hardwareDecl) = EzSinglePinDeclSemanticAction(EZ_COMPONENT_SERVO, (yyvsp[-2].string), (yyvsp[0].pin)); }
#line 1814 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 11: /* hardware_decl: KW_DHT11 IDENTIFIER ON pin  */
#line 112 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                { (yyval.hardwareDecl) = EzSinglePinDeclSemanticAction(EZ_COMPONENT_DHT11, (yyvsp[-2].string), (yyvsp[0].pin)); }
#line 1820 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 12: /* hardware_decl: KW_ULTRASONIC IDENTIFIER ON LPAREN TRIG COLON digital_pin COMMA ECHO COLON digital_pin RPAREN  */
#line 114 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                                                { (yyval.hardwareDecl) = EzUltrasonicDeclSemanticAction((yyvsp[-10].string), (yyvsp[-5].pin), (yyvsp[-1].pin)); }
#line 1826 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 13: /* hardware_decl: KW_LCD IDENTIFIER ON LPAREN digital_pin COMMA digital_pin COMMA digital_pin COMMA digital_pin COMMA digital_pin COMMA digital_pin RPAREN  */
#line 116 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                                                { (yyval.hardwareDecl) = EzLcdDeclSemanticAction((yyvsp[-14].string), (yyvsp[-11].pin), (yyvsp[-9].pin), (yyvsp[-7].pin), (yyvsp[-5].pin), (yyvsp[-3].pin), (yyvsp[-1].pin)); }
#line 1832 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 14: /* pin: digital_pin  */
#line 119 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                { (yyval.pin) = (yyvsp[0].pin); }
#line 1838 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 15: /* pin: analog_pin  */
#line 120 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                { (yyval.pin) = (yyvsp[0].pin); }
#line 1844 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 16: /* digital_pin: INTEGER  */
#line 123 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                        { (yyval.pin) = (EzPinValue){ .isAnalog = false, .number = (yyvsp[0].integer) }; }
#line 1850 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 17: /* analog_pin: ANALOG_PIN  */
#line 126 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                        { (yyval.pin) = (EzPinValue){ .isAnalog = true, .number = (yyvsp[0].analogPin) }; }
#line 1856 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 18: /* routine_block: ROUTINE LBRACE stmt_list RBRACE  */
#line 129 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                        { (yyval.block) = (yyvsp[-1].block); }
#line 1862 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 19: /* stmt_list: %empty  */
#line 132 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                        { (yyval.block) = EzBlockSemanticAction(); }
#line 1868 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 20: /* stmt_list: stmt_list stmt  */
#line 133 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                { (yyval.block) = EzBlockAppendStatementSemanticAction((yyvsp[-1].block), (yyvsp[0].statement)); }
#line 1874 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 21: /* block: LBRACE stmt_list RBRACE  */
#line 136 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                { (yyval.block) = (yyvsp[-1].block); }
#line 1880 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 22: /* stmt: VAR IDENTIFIER ASSIGN expr SEMI  */
#line 139 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                        { (yyval.statement) = EzVarDeclStatementSemanticAction((yyvsp[-3].string), (yyvsp[-1].expression)); }
#line 1886 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 23: /* stmt: call SEMI  */
#line 140 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                        { (yyval.statement) = EzExprStatementSemanticAction((yyvsp[-1].expression)); }
#line 1892 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 24: /* stmt: WAIT TIME SEMI  */
#line 141 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                        { (yyval.statement) = EzWaitStatementSemanticAction((yyvsp[-1].timeMs)); }
#line 1898 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 25: /* stmt: IF expr block  */
#line 142 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                        { (yyval.statement) = EzIfStatementSemanticAction((yyvsp[-1].expression), (yyvsp[0].block), NULL); }
#line 1904 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 26: /* stmt: IF expr block ELSE block  */
#line 143 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                { (yyval.statement) = EzIfStatementSemanticAction((yyvsp[-3].expression), (yyvsp[-2].block), (yyvsp[0].block)); }
#line 1910 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 27: /* stmt: REPEAT EVERY TIME block  */
#line 144 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                { (yyval.statement) = EzRepeatEveryStatementSemanticAction((yyvsp[-1].timeMs), (yyvsp[0].block)); }
#line 1916 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 28: /* stmt: REPEAT expr TIMES block  */
#line 145 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                { (yyval.statement) = EzRepeatTimesStatementSemanticAction((yyvsp[-2].expression), (yyvsp[0].block)); }
#line 1922 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 29: /* stmt: FOR IDENTIFIER FROM expr TO expr block  */
#line 146 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                { (yyval.statement) = EzForRangeStatementSemanticAction((yyvsp[-5].string), (yyvsp[-3].expression), (yyvsp[-1].expression), (yyvsp[0].block)); }
#line 1928 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 30: /* call: IDENTIFIER DOT IDENTIFIER LPAREN arg_list_opt RPAREN  */
#line 149 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                        { (yyval.expression) = EzCallSemanticAction((yyvsp[-5].string), (yyvsp[-3].string), (yyvsp[-1].expressionList)); }
#line 1934 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 31: /* arg_list_opt: %empty  */
#line 152 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                        { (yyval.expressionList) = EzExpressionListSemanticAction(); }
#line 1940 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 32: /* arg_list_opt: arg_list  */
#line 153 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                { (yyval.expressionList) = (yyvsp[0].expressionList); }
#line 1946 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 33: /* arg_list: expr  */
#line 156 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                        { (yyval.expressionList) = EzExpressionListAppendSemanticAction(EzExpressionListSemanticAction(), (yyvsp[0].expression)); }
#line 1952 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 34: /* arg_list: arg_list COMMA expr  */
#line 157 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                        { (yyval.expressionList) = EzExpressionListAppendSemanticAction((yyvsp[-2].expressionList), (yyvsp[0].expression)); }
#line 1958 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 35: /* expr: expr OR expr  */
#line 160 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                        { (yyval.expression) = EzBinarySemanticAction(EZ_BIN_OR, (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 1964 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 36: /* expr: expr AND expr  */
#line 161 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                        { (yyval.expression) = EzBinarySemanticAction(EZ_BIN_AND, (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 1970 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 37: /* expr: NOT expr  */
#line 162 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                { (yyval.expression) = EzUnarySemanticAction(EZ_UN_NOT, (yyvsp[0].expression)); }
#line 1976 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 38: /* expr: expr EQ expr  */
#line 163 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                        { (yyval.expression) = EzBinarySemanticAction(EZ_BIN_EQ, (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 1982 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 39: /* expr: expr NE expr  */
#line 164 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                        { (yyval.expression) = EzBinarySemanticAction(EZ_BIN_NE, (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 1988 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 40: /* expr: expr LT expr  */
#line 165 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                        { (yyval.expression) = EzBinarySemanticAction(EZ_BIN_LT, (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 1994 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 41: /* expr: expr GT expr  */
#line 166 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                        { (yyval.expression) = EzBinarySemanticAction(EZ_BIN_GT, (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 2000 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 42: /* expr: expr LE expr  */
#line 167 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                        { (yyval.expression) = EzBinarySemanticAction(EZ_BIN_LE, (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 2006 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 43: /* expr: expr GE expr  */
#line 168 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                        { (yyval.expression) = EzBinarySemanticAction(EZ_BIN_GE, (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 2012 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 44: /* expr: expr PLUS expr  */
#line 169 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                { (yyval.expression) = EzBinarySemanticAction(EZ_BIN_ADD, (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 2018 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 45: /* expr: expr MINUS expr  */
#line 170 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                { (yyval.expression) = EzBinarySemanticAction(EZ_BIN_SUB, (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 2024 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 46: /* expr: expr STAR expr  */
#line 171 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                { (yyval.expression) = EzBinarySemanticAction(EZ_BIN_MUL, (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 2030 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 47: /* expr: expr SLASH expr  */
#line 172 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                { (yyval.expression) = EzBinarySemanticAction(EZ_BIN_DIV, (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 2036 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 48: /* expr: MINUS expr  */
#line 173 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                { (yyval.expression) = EzUnarySemanticAction(EZ_UN_NEG, (yyvsp[0].expression)); }
#line 2042 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 49: /* expr: LPAREN expr RPAREN  */
#line 174 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                        { (yyval.expression) = (yyvsp[-1].expression); }
#line 2048 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 50: /* expr: call  */
#line 175 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                { (yyval.expression) = (yyvsp[0].expression); }
#line 2054 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 51: /* expr: IDENTIFIER  */
#line 176 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                { (yyval.expression) = EzIdentifierSemanticAction((yyvsp[0].string)); }
#line 2060 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 52: /* expr: INTEGER  */
#line 177 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                { (yyval.expression) = EzNumberLiteralSemanticAction((double) (yyvsp[0].integer)); }
#line 2066 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 53: /* expr: FLOAT  */
#line 178 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                { (yyval.expression) = EzNumberLiteralSemanticAction((yyvsp[0].number)); }
#line 2072 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 54: /* expr: TIME  */
#line 179 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                { (yyval.expression) = EzTimeLiteralSemanticAction((yyvsp[0].timeMs)); }
#line 2078 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 55: /* expr: STRING  */
#line 180 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                { (yyval.expression) = EzStringLiteralSemanticAction((yyvsp[0].string)); }
#line 2084 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 56: /* expr: TRUE  */
#line 181 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                { (yyval.expression) = EzBoolLiteralSemanticAction(true); }
#line 2090 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 57: /* expr: FALSE  */
#line 182 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                { (yyval.expression) = EzBoolLiteralSemanticAction(false); }
#line 2096 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;


#line 2100 "src/main/c/frontend/syntactic-analysis/BisonParser.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyps, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (&yylloc, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
  yyps->yynew = 2;
  goto yypushreturn;


/*-------------------------.
| yypushreturn -- return.  |
`-------------------------*/
yypushreturn:
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}
#undef yynerrs
#undef yystate
#undef yyerrstatus
#undef yyssa
#undef yyss
#undef yyssp
#undef yyvsa
#undef yyvs
#undef yyvsp
#undef yylsa
#undef yyls
#undef yylsp
#undef yystacksize
#line 185 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"

