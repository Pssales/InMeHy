/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with gml or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 14 "../../cmd/tools/gmlparse.y" /* yacc.c:339  */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <arith.h>
#include <gml2gv.h>
#include <agxbuf.h>
#include <assert.h>

#define NEW(t)       (t*)malloc(sizeof(t))
#define N_NEW(n,t)   (t*)malloc((n)*sizeof(t))
#define RALLOC(size,ptr,type) ((type*)realloc(ptr,(size)*sizeof(type)))

typedef unsigned short ushort;

static gmlgraph* G;
static gmlnode* N;
static gmledge* E;
static Dt_t* L;
static Dt_t** liststk;
static int liststk_sz;
static int liststk_cnt;

static void free_attr (Dt_t*d, gmlattr* p, Dtdisc_t* ds); /* forward decl */
static char* sortToStr (int sort);    /* forward decl */

static void
free_node (Dt_t*d, gmlnode* p, Dtdisc_t* ds)
{
    if (!p) return;
    if (p->attrlist) dtclose (p->attrlist);
    free (p);
}

static void
free_edge (Dt_t*d, gmledge* p, Dtdisc_t* ds)
{
    if (!p) return;
    if (p->attrlist) dtclose (p->attrlist);
    free (p);
}

static void
free_graph (Dt_t*d, gmlgraph* p, Dtdisc_t* ds)
{
    if (!p) return;
    if (p->nodelist)
	dtclose (p->nodelist);
    if (p->edgelist)
	dtclose (p->edgelist);
    if (p->attrlist)
	dtclose (p->attrlist);
    if (p->graphlist)
	dtclose (p->graphlist);
    free (p);
}

static Dtdisc_t nodeDisc = {
    offsetof(gmlnode,attrlist),
    sizeof(Dt_t*),
    offsetof(gmlnode,link),
    NIL(Dtmake_f),
    (Dtfree_f)free_node,
    NIL(Dtcompar_f),
    NIL(Dthash_f),
    NIL(Dtmemory_f),
    NIL(Dtevent_f)
};

static Dtdisc_t edgeDisc = {
    offsetof(gmledge,attrlist),
    sizeof(Dt_t*),
    offsetof(gmledge,link),
    NIL(Dtmake_f),
    (Dtfree_f)free_edge,
    NIL(Dtcompar_f),
    NIL(Dthash_f),
    NIL(Dtmemory_f),
    NIL(Dtevent_f)
};

static Dtdisc_t attrDisc = {
    offsetof(gmlattr,name),
    sizeof(char*),
    offsetof(gmlattr,link),
    NIL(Dtmake_f),
    (Dtfree_f)free_attr,
    NIL(Dtcompar_f),
    NIL(Dthash_f),
    NIL(Dtmemory_f),
    NIL(Dtevent_f)
};

static Dtdisc_t graphDisc = {
    offsetof(gmlgraph,nodelist),
    sizeof(Dt_t*),
    offsetof(gmlgraph,link),
    NIL(Dtmake_f),
    (Dtfree_f)free_graph,
    NIL(Dtcompar_f),
    NIL(Dthash_f),
    NIL(Dtmemory_f),
    NIL(Dtevent_f)
};

static void
initstk (void)
{
    liststk_sz = 10;
    liststk_cnt = 0;
    liststk = N_NEW(liststk_sz, Dt_t*);
}

static void
cleanup (void)
{
    int i;

    if (liststk) {
	for (i = 0; i < liststk_cnt; i++)
	    dtclose (liststk[i]);
	free (liststk);
	liststk = NULL;
    }
    if (L) {
	dtclose (L);
	L = NULL;
    }
    if (N) {
	free_node (0, N, 0);
	N = NULL;
    }
    if (E) {
	free_edge (0, E, 0);
	E = NULL;
    }
    if (G) {
	free_graph (0, G, 0);
	G = NULL;
    }
}

static void
pushAlist (void)
{
    Dt_t* lp = dtopen (&attrDisc, Dtqueue);

    if (L) {
	if (liststk_cnt == liststk_sz) {
	    liststk_sz *= 2;
	    liststk = RALLOC(liststk_sz, liststk, Dt_t*);
	}
	liststk[liststk_cnt++] = L;
    }
    L = lp;
}

static Dt_t*
popAlist (void)
{
    Dt_t* lp = L;

    if (liststk_cnt)
	L = liststk[--liststk_cnt];
    else
	L = NULL;

    return lp;
}

static void
popG (void)
{
    G = G->parent;
}

static void
pushG (void)
{
    gmlgraph* g = NEW(gmlgraph);

    g->attrlist = dtopen(&attrDisc, Dtqueue);
    g->nodelist = dtopen(&nodeDisc, Dtqueue);
    g->edgelist = dtopen(&edgeDisc, Dtqueue);
    g->graphlist = dtopen(&graphDisc, Dtqueue);
    g->parent = G;
    g->directed = -1;

    if (G)
	dtinsert (G->graphlist, g);

    G = g;
}

static gmlnode*
mkNode (void)
{
    gmlnode* np = NEW(gmlnode);
    np->attrlist = dtopen (&attrDisc, Dtqueue);
    np->id = NULL;
    return np;
}

static gmledge*
mkEdge (void)
{
    gmledge* ep = NEW(gmledge);
    ep->attrlist = dtopen (&attrDisc, Dtqueue);
    ep->source = NULL;
    ep->target = NULL;
    return ep;
}

static gmlattr*
mkAttr (char* name, int sort, int kind, char* str, Dt_t* list)
{
    gmlattr* gp = NEW(gmlattr);

    assert (name || sort);
    if (!name)
	name = strdup (sortToStr (sort));
    gp->sort = sort;
    gp->kind = kind;
    gp->name = name;
    if (str)
	gp->u.value = str;
    else {
	if (dtsize (list) == 0) {
	    dtclose (list);
	    list = 0;
	}
	gp->u.lp = list;
    }
/* fprintf (stderr, "[%x] %d %d \"%s\" \"%s\" \n", gp, sort, kind, (name?name:""),  (str?str:"")); */
    return gp;
}

static int
setDir (char* d)
{
    gmlgraph* g;
    int dir = atoi (d);

    free (d);
    if (dir < 0) dir = -1;
    else if (dir > 0) dir = 1;
    else dir = 0;
    G->directed = dir;

    if (dir >= 0) {
	for (g = G->parent; g; g = g->parent) {
	    if (g->directed < 0)
		g->directed = dir;
	    else if (g->directed != dir)
		return 1;
        }
    }

    return 0;
}


#line 329 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int gmldebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum gmltokentype
  {
    GRAPH = 258,
    NODE = 259,
    EDGE = 260,
    DIRECTED = 261,
    SOURCE = 262,
    TARGET = 263,
    XVAL = 264,
    YVAL = 265,
    WVAL = 266,
    HVAL = 267,
    LABEL = 268,
    GRAPHICS = 269,
    LABELGRAPHICS = 270,
    TYPE = 271,
    FILL = 272,
    OUTLINE = 273,
    OUTLINESTYLE = 274,
    OUTLINEWIDTH = 275,
    WIDTH = 276,
    STYLE = 277,
    LINE = 278,
    POINT = 279,
    TEXT = 280,
    FONTSIZE = 281,
    FONTNAME = 282,
    COLOR = 283,
    INTEGER = 284,
    REAL = 285,
    STRING = 286,
    ID = 287,
    NAME = 288,
    LIST = 289
  };
#endif
/* Tokens.  */
#define GRAPH 258
#define NODE 259
#define EDGE 260
#define DIRECTED 261
#define SOURCE 262
#define TARGET 263
#define XVAL 264
#define YVAL 265
#define WVAL 266
#define HVAL 267
#define LABEL 268
#define GRAPHICS 269
#define LABELGRAPHICS 270
#define TYPE 271
#define FILL 272
#define OUTLINE 273
#define OUTLINESTYLE 274
#define OUTLINEWIDTH 275
#define WIDTH 276
#define STYLE 277
#define LINE 278
#define POINT 279
#define TEXT 280
#define FONTSIZE 281
#define FONTNAME 282
#define COLOR 283
#define INTEGER 284
#define REAL 285
#define STRING 286
#define ID 287
#define NAME 288
#define LIST 289

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 276 "../../cmd/tools/gmlparse.y" /* yacc.c:355  */

    int i;
    char *str;
    gmlnode* np;
    gmledge* ep;
    gmlattr* ap;
    Dt_t*    list;

#line 446 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE gmllval;

int gmlparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 463 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 gmltype_uint8;
#else
typedef unsigned char gmltype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 gmltype_int8;
#else
typedef signed char gmltype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 gmltype_uint16;
#else
typedef unsigned short int gmltype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 gmltype_int16;
#else
typedef short int gmltype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about gmllval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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


#if ! defined gmloverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

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
#endif /* ! defined gmloverflow || YYERROR_VERBOSE */


#if (! defined gmloverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union gmlalloc
{
  gmltype_int16 gmlss_alloc;
  YYSTYPE gmlvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union gmlalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (gmltype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T gmlnewbytes;                                            \
        YYCOPY (&gmlptr->Stack_alloc, Stack, gmlsize);                    \
        Stack = &gmlptr->Stack_alloc;                                    \
        gmlnewbytes = gmlstacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        gmlptr += gmlnewbytes / sizeof (*gmlptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T gmli;                         \
          for (gmli = 0; gmli < (Count); gmli++)   \
            (Dst)[gmli] = (Src)[gmli];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  54
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   225

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  37
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  62
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  101

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by gmllex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   289

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? gmltranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by gmllex, without out-of-bounds checking.  */
static const gmltype_uint8 gmltranslate[] =
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
       2,    35,     2,    36,     2,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const gmltype_uint16 gmlrline[] =
{
       0,   299,   299,   300,   301,   304,   307,   310,   311,   314,
     315,   318,   319,   320,   321,   328,   329,   332,   332,   335,
     336,   339,   340,   343,   343,   346,   347,   350,   351,   352,
     353,   356,   356,   359,   360,   363,   364,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const gmltname[] =
{
  "$end", "error", "$undefined", "GRAPH", "NODE", "EDGE", "DIRECTED",
  "SOURCE", "TARGET", "XVAL", "YVAL", "WVAL", "HVAL", "LABEL", "GRAPHICS",
  "LABELGRAPHICS", "TYPE", "FILL", "OUTLINE", "OUTLINESTYLE",
  "OUTLINEWIDTH", "WIDTH", "STYLE", "LINE", "POINT", "TEXT", "FONTSIZE",
  "FONTNAME", "COLOR", "INTEGER", "REAL", "STRING", "ID", "NAME", "LIST",
  "'['", "']'", "$accept", "graph", "hdr", "body", "optglist", "glist",
  "glistitem", "node", "$@1", "nlist", "nlistitem", "edge", "$@2", "elist",
  "elistitem", "attrlist", "$@3", "optalist", "alist", "alistitem", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const gmltype_uint16 gmltoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,    91,    93
};
# endif

#define YYPACT_NINF -30

#define gmlpact_value_is_default(Yystate) \
  (!!((Yystate) == (-30)))

#define YYTABLE_NINF -35

#define gmltable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const gmltype_int16 gmlpact[] =
{
       2,   -30,   -23,   -29,   -21,   -20,     1,    -2,    -2,     5,
       6,    10,    11,    15,    19,   -27,    -2,    -2,    20,    21,
      22,    24,    17,    54,    55,   192,   -30,   -30,   -30,   -30,
     -30,   -30,   -30,   -30,   -30,   -30,   -30,   -30,   -30,   -30,
     -30,   -30,   -30,   -30,   -30,   -30,   -30,   -30,   -30,   -30,
     -30,   -30,   -30,   -30,   -30,   -30,    29,   -30,   192,    57,
     -30,    50,   -30,   -30,    58,    59,    29,    77,    57,   -30,
     -30,   -30,   -30,   -30,    79,    80,   -30,   -30,   -30,   -30,
     -30,   167,   142,    89,   112,   -30,   -30,    90,   113,   114,
      84,   -30,   -30,   -30,   -30,   -30,   -30,   -30,   -30,   -30,
     -30
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const gmltype_uint8 gmldefact[] =
{
       0,     3,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    33,    36,    42,    41,    43,
      44,    45,    46,    31,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    61,    60,    62,
      37,    38,    39,    40,     1,     5,     0,    35,    34,     8,
       2,     0,    17,    23,     0,     0,     0,     0,     7,    10,
      11,    12,    16,    32,     0,     0,    14,    15,    13,     6,
       9,     0,     0,     0,     0,    20,    22,     0,     0,     0,
       0,    26,    30,    21,    18,    19,    27,    28,    29,    24,
      25
};

  /* YYPGOTO[NTERM-NUM].  */
static const gmltype_int8 gmlpgoto[] =
{
     -30,   -30,   117,    81,   -30,   -30,    78,   -30,   -30,   -30,
      87,   -30,   -30,   -30,    82,    23,   -30,   115,   -30,   -25
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const gmltype_int8 gmldefgoto[] =
{
      -1,    23,    66,    60,    67,    68,    69,    70,    74,    84,
      85,    71,    75,    90,    91,    34,    58,    24,    25,    26
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const gmltype_int8 gmltable[] =
{
      57,    29,    -4,     1,    42,   -34,    27,    28,    33,    30,
      31,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    35,    32,    33,    72,    22,    36,    37,    43,    44,
      45,    38,    39,    72,    40,    53,    50,    51,    52,    41,
      47,    46,    33,    48,    54,    49,    86,    92,    55,    86,
      55,    62,    63,    64,    59,    92,     2,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    73,    76,    77,    65,
      22,    87,    88,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    79,    81,    82,    89,    22,    93,    96,
      99,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    56,    97,    98,    83,    22,    80,    78,    94,    87,
      88,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    95,   100,    61,    89,    22,     2,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,     0,     0,     0,    83,
      22,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,     0,     0,     0,     0,    22
};

static const gmltype_int8 gmlcheck[] =
{
      25,    30,     0,     1,    31,     3,    29,    30,    35,    30,
      30,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,     8,    31,    35,    59,    33,    31,    31,    15,    16,
      17,    31,    31,    68,    29,    22,    29,    30,    31,    30,
      29,    31,    35,    31,     0,    31,    81,    82,     3,    84,
       3,     4,     5,     6,    35,    90,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    36,    29,    29,    32,
      33,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    36,    35,    35,    32,    33,    29,    29,
      36,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    24,    29,    29,    32,    33,    68,    66,    36,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    84,    90,    58,    32,    33,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    -1,    -1,    -1,    32,
      33,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    -1,    -1,    -1,    -1,    33
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const gmltype_uint8 gmlstos[] =
{
       0,     1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    33,    38,    54,    55,    56,    29,    30,    30,
      30,    30,    31,    35,    52,    52,    31,    31,    31,    31,
      29,    30,    31,    52,    52,    52,    31,    29,    31,    31,
      29,    30,    31,    52,     0,     3,    39,    56,    53,    35,
      40,    54,     4,     5,     6,    32,    39,    41,    42,    43,
      44,    48,    56,    36,    45,    49,    29,    29,    40,    36,
      43,    35,    35,    32,    46,    47,    56,     7,     8,    32,
      50,    51,    56,    29,    36,    47,    29,    29,    29,    36,
      51
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const gmltype_uint8 gmlr1[] =
{
       0,    37,    38,    38,    38,    39,    40,    41,    41,    42,
      42,    43,    43,    43,    43,    43,    43,    45,    44,    46,
      46,    47,    47,    49,    48,    50,    50,    51,    51,    51,
      51,    53,    52,    54,    54,    55,    55,    56,    56,    56,
      56,    56,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    56
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const gmltype_uint8 gmlr2[] =
{
       0,     2,     3,     1,     0,     1,     3,     1,     0,     2,
       1,     1,     1,     2,     2,     2,     1,     0,     5,     2,
       1,     2,     1,     0,     5,     2,     1,     2,     2,     2,
       1,     0,     4,     1,     0,     2,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2
};


#define gmlerrok         (gmlerrstatus = 0)
#define gmlclearin       (gmlchar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto gmlacceptlab
#define YYABORT         goto gmlabortlab
#define YYERROR         goto gmlerrorlab


#define YYRECOVERING()  (!!gmlerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (gmlchar == YYEMPTY)                                        \
    {                                                           \
      gmlchar = (Token);                                         \
      gmllval = (Value);                                         \
      YYPOPSTACK (gmllen);                                       \
      gmlstate = *gmlssp;                                         \
      goto gmlbackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      gmlerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (gmldebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (gmldebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      gml_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
gml_symbol_value_print (FILE *gmloutput, int gmltype, YYSTYPE const * const gmlvaluep)
{
  FILE *gmlo = gmloutput;
  YYUSE (gmlo);
  if (!gmlvaluep)
    return;
# ifdef YYPRINT
  if (gmltype < YYNTOKENS)
    YYPRINT (gmloutput, gmltoknum[gmltype], *gmlvaluep);
# endif
  YYUSE (gmltype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
gml_symbol_print (FILE *gmloutput, int gmltype, YYSTYPE const * const gmlvaluep)
{
  YYFPRINTF (gmloutput, "%s %s (",
             gmltype < YYNTOKENS ? "token" : "nterm", gmltname[gmltype]);

  gml_symbol_value_print (gmloutput, gmltype, gmlvaluep);
  YYFPRINTF (gmloutput, ")");
}

/*------------------------------------------------------------------.
| gml_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
gml_stack_print (gmltype_int16 *gmlbottom, gmltype_int16 *gmltop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; gmlbottom <= gmltop; gmlbottom++)
    {
      int gmlbot = *gmlbottom;
      YYFPRINTF (stderr, " %d", gmlbot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (gmldebug)                                                  \
    gml_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
gml_reduce_print (gmltype_int16 *gmlssp, YYSTYPE *gmlvsp, int gmlrule)
{
  uint64_t gmllno = gmlrline[gmlrule];
  int gmlnrhs = gmlr2[gmlrule];
  int gmli;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             gmlrule - 1, gmllno);
  /* The symbols being reduced.  */
  for (gmli = 0; gmli < gmlnrhs; gmli++)
    {
      YYFPRINTF (stderr, "   $%d = ", gmli + 1);
      gml_symbol_print (stderr,
                       gmlstos[gmlssp[gmli + 1 - gmlnrhs]],
                       &(gmlvsp[(gmli + 1) - (gmlnrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (gmldebug)                          \
    gml_reduce_print (gmlssp, gmlvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int gmldebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef gmlstrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define gmlstrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
gmlstrlen (const char *gmlstr)
{
  YYSIZE_T gmllen;
  for (gmllen = 0; gmlstr[gmllen]; gmllen++)
    continue;
  return gmllen;
}
#  endif
# endif

# ifndef gmlstpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define gmlstpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
gmlstpcpy (char *gmldest, const char *gmlsrc)
{
  char *gmld = gmldest;
  const char *gmls = gmlsrc;

  while ((*gmld++ = *gmls++) != '\0')
    continue;

  return gmld - 1;
}
#  endif
# endif

# ifndef gmltnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for gmlerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from gmltname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
gmltnamerr (char *gmlres, const char *gmlstr)
{
  if (*gmlstr == '"')
    {
      YYSIZE_T gmln = 0;
      char const *gmlp = gmlstr;

      for (;;)
        switch (*++gmlp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++gmlp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (gmlres)
              gmlres[gmln] = *gmlp;
            gmln++;
            break;

          case '"':
            if (gmlres)
              gmlres[gmln] = '\0';
            return gmln;
          }
    do_not_strip_quotes: ;
    }

  if (! gmlres)
    return gmlstrlen (gmlstr);

  return gmlstpcpy (gmlres, gmlstr) - gmlres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
gmlsyntax_error (YYSIZE_T *gmlmsg_alloc, char **gmlmsg,
                gmltype_int16 *gmlssp, int gmltoken)
{
  YYSIZE_T gmlsize0 = gmltnamerr (YY_NULLPTR, gmltname[gmltoken]);
  YYSIZE_T gmlsize = gmlsize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *gmlformat = YY_NULLPTR;
  /* Arguments of gmlformat. */
  char const *gmlarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int gmlcount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in gmlchar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated gmlchar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (gmltoken != YYEMPTY)
    {
      int gmln = gmlpact[*gmlssp];
      gmlarg[gmlcount++] = gmltname[gmltoken];
      if (!gmlpact_value_is_default (gmln))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int gmlxbegin = gmln < 0 ? -gmln : 0;
          /* Stay within bounds of both gmlcheck and gmltname.  */
          int gmlchecklim = YYLAST - gmln + 1;
          int gmlxend = gmlchecklim < YYNTOKENS ? gmlchecklim : YYNTOKENS;
          int gmlx;

          for (gmlx = gmlxbegin; gmlx < gmlxend; ++gmlx)
            if (gmlcheck[gmlx + gmln] == gmlx && gmlx != YYTERROR
                && !gmltable_value_is_error (gmltable[gmlx + gmln]))
              {
                if (gmlcount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    gmlcount = 1;
                    gmlsize = gmlsize0;
                    break;
                  }
                gmlarg[gmlcount++] = gmltname[gmlx];
                {
                  YYSIZE_T gmlsize1 = gmlsize + gmltnamerr (YY_NULLPTR, gmltname[gmlx]);
                  if (! (gmlsize <= gmlsize1
                         && gmlsize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  gmlsize = gmlsize1;
                }
              }
        }
    }

  switch (gmlcount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        gmlformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T gmlsize1 = gmlsize + gmlstrlen (gmlformat);
    if (! (gmlsize <= gmlsize1 && gmlsize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    gmlsize = gmlsize1;
  }

  if (*gmlmsg_alloc < gmlsize)
    {
      *gmlmsg_alloc = 2 * gmlsize;
      if (! (gmlsize <= *gmlmsg_alloc
             && *gmlmsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *gmlmsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *gmlp = *gmlmsg;
    int gmli = 0;
    while ((*gmlp = *gmlformat) != '\0')
      if (*gmlp == '%' && gmlformat[1] == 's' && gmli < gmlcount)
        {
          gmlp += gmltnamerr (gmlp, gmlarg[gmli++]);
          gmlformat += 2;
        }
      else
        {
          gmlp++;
          gmlformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
gmldestruct (const char *gmlmsg, int gmltype, YYSTYPE *gmlvaluep)
{
  YYUSE (gmlvaluep);
  if (!gmlmsg)
    gmlmsg = "Deleting";
  YY_SYMBOL_PRINT (gmlmsg, gmltype, gmlvaluep, gmllocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (gmltype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int gmlchar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE gmllval;
/* Number of syntax errors so far.  */
int gmlnerrs;


/*----------.
| gmlparse.  |
`----------*/

int
gmlparse (void)
{
    int gmlstate;
    /* Number of tokens to shift before error messages enabled.  */
    int gmlerrstatus;

    /* The stacks and their tools:
       'gmlss': related to states.
       'gmlvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow gmloverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    gmltype_int16 gmlssa[YYINITDEPTH];
    gmltype_int16 *gmlss;
    gmltype_int16 *gmlssp;

    /* The semantic value stack.  */
    YYSTYPE gmlvsa[YYINITDEPTH];
    YYSTYPE *gmlvs;
    YYSTYPE *gmlvsp;

    YYSIZE_T gmlstacksize;

  int gmln;
  int gmlresult;
  /* Lookahead token as an internal (translated) token number.  */
  int gmltoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE gmlval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char gmlmsgbuf[128];
  char *gmlmsg = gmlmsgbuf;
  YYSIZE_T gmlmsg_alloc = sizeof gmlmsgbuf;
#endif

#define YYPOPSTACK(N)   (gmlvsp -= (N), gmlssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int gmllen = 0;

  gmlssp = gmlss = gmlssa;
  gmlvsp = gmlvs = gmlvsa;
  gmlstacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  gmlstate = 0;
  gmlerrstatus = 0;
  gmlnerrs = 0;
  gmlchar = YYEMPTY; /* Cause a token to be read.  */
  goto gmlsetstate;

/*------------------------------------------------------------.
| gmlnewstate -- Push a new state, which is found in gmlstate.  |
`------------------------------------------------------------*/
 gmlnewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  gmlssp++;

 gmlsetstate:
  *gmlssp = gmlstate;

  if (gmlss + gmlstacksize - 1 <= gmlssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T gmlsize = gmlssp - gmlss + 1;

#ifdef gmloverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *gmlvs1 = gmlvs;
        gmltype_int16 *gmlss1 = gmlss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if gmloverflow is a macro.  */
        gmloverflow (YY_("memory exhausted"),
                    &gmlss1, gmlsize * sizeof (*gmlssp),
                    &gmlvs1, gmlsize * sizeof (*gmlvsp),
                    &gmlstacksize);

        gmlss = gmlss1;
        gmlvs = gmlvs1;
      }
#else /* no gmloverflow */
# ifndef YYSTACK_RELOCATE
      goto gmlexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= gmlstacksize)
        goto gmlexhaustedlab;
      gmlstacksize *= 2;
      if (YYMAXDEPTH < gmlstacksize)
        gmlstacksize = YYMAXDEPTH;

      {
        gmltype_int16 *gmlss1 = gmlss;
        union gmlalloc *gmlptr =
          (union gmlalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (gmlstacksize));
        if (! gmlptr)
          goto gmlexhaustedlab;
        YYSTACK_RELOCATE (gmlss_alloc, gmlss);
        YYSTACK_RELOCATE (gmlvs_alloc, gmlvs);
#  undef YYSTACK_RELOCATE
        if (gmlss1 != gmlssa)
          YYSTACK_FREE (gmlss1);
      }
# endif
#endif /* no gmloverflow */

      gmlssp = gmlss + gmlsize - 1;
      gmlvsp = gmlvs + gmlsize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (uint64_t) gmlstacksize));

      if (gmlss + gmlstacksize - 1 <= gmlssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", gmlstate));

  if (gmlstate == YYFINAL)
    YYACCEPT;

  goto gmlbackup;

/*-----------.
| gmlbackup.  |
`-----------*/
gmlbackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  gmln = gmlpact[gmlstate];
  if (gmlpact_value_is_default (gmln))
    goto gmldefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (gmlchar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      gmlchar = gmllex ();
    }

  if (gmlchar <= YYEOF)
    {
      gmlchar = gmltoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      gmltoken = YYTRANSLATE (gmlchar);
      YY_SYMBOL_PRINT ("Next token is", gmltoken, &gmllval, &gmllloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  gmln += gmltoken;
  if (gmln < 0 || YYLAST < gmln || gmlcheck[gmln] != gmltoken)
    goto gmldefault;
  gmln = gmltable[gmln];
  if (gmln <= 0)
    {
      if (gmltable_value_is_error (gmln))
        goto gmlerrlab;
      gmln = -gmln;
      goto gmlreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (gmlerrstatus)
    gmlerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", gmltoken, &gmllval, &gmllloc);

  /* Discard the shifted token.  */
  gmlchar = YYEMPTY;

  gmlstate = gmln;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++gmlvsp = gmllval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto gmlnewstate;


/*-----------------------------------------------------------.
| gmldefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
gmldefault:
  gmln = gmldefact[gmlstate];
  if (gmln == 0)
    goto gmlerrlab;
  goto gmlreduce;


/*-----------------------------.
| gmlreduce -- Do a reduction.  |
`-----------------------------*/
gmlreduce:
  /* gmln is the number of a rule to reduce with.  */
  gmllen = gmlr2[gmln];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  gmlval = gmlvsp[1-gmllen];


  YY_REDUCE_PRINT (gmln);
  switch (gmln)
    {
        case 2:
#line 299 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    {gmllexeof(); if (G->parent) popG(); }
#line 1634 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 300 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { cleanup(); YYABORT; }
#line 1640 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 304 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { pushG(); }
#line 1646 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 318 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { dtinsert (G->nodelist, (gmlvsp[0].np)); }
#line 1652 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 319 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { dtinsert (G->edgelist, (gmlvsp[0].ep)); }
#line 1658 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 321 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { 
		if (setDir((gmlvsp[0].str))) { 
		    gmlerror("mixed directed and undirected graphs"); 
		    cleanup ();
		    YYABORT;
		}
	  }
#line 1670 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 328 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { dtinsert (G->attrlist, mkAttr (strdup("id"), 0, INTEGER, (gmlvsp[0].str), 0)); }
#line 1676 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 329 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { dtinsert (G->attrlist, (gmlvsp[0].ap)); }
#line 1682 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 332 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { N = mkNode(); }
#line 1688 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 332 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.np) = N; N = NULL; }
#line 1694 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 339 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { N->id = (gmlvsp[0].str); }
#line 1700 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 340 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { dtinsert (N->attrlist, (gmlvsp[0].ap)); }
#line 1706 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 343 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { E = mkEdge(); }
#line 1712 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 343 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ep) = E; E = NULL; }
#line 1718 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 350 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { E->source = (gmlvsp[0].str); }
#line 1724 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 351 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { E->target = (gmlvsp[0].str); }
#line 1730 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 352 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { dtinsert (E->attrlist, mkAttr (strdup("id"), 0, INTEGER, (gmlvsp[0].str), 0)); }
#line 1736 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 353 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { dtinsert (E->attrlist, (gmlvsp[0].ap)); }
#line 1742 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 356 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    {pushAlist(); }
#line 1748 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 356 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.list) = popAlist(); }
#line 1754 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 363 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { dtinsert (L, (gmlvsp[0].ap)); }
#line 1760 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 364 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { dtinsert (L, (gmlvsp[0].ap)); }
#line 1766 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 367 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr ((gmlvsp[-1].str), 0, INTEGER, (gmlvsp[0].str), 0); }
#line 1772 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 368 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr ((gmlvsp[-1].str), 0, REAL, (gmlvsp[0].str), 0); }
#line 1778 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 369 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr ((gmlvsp[-1].str), 0, STRING, (gmlvsp[0].str), 0); }
#line 1784 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 370 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr ((gmlvsp[-1].str), 0, LIST, 0, (gmlvsp[0].list)); }
#line 1790 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 371 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, XVAL, REAL, (gmlvsp[0].str), 0); }
#line 1796 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 372 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, XVAL, REAL, (gmlvsp[0].str), 0); }
#line 1802 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 373 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, YVAL, REAL, (gmlvsp[0].str), 0); }
#line 1808 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 374 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, WVAL, REAL, (gmlvsp[0].str), 0); }
#line 1814 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 375 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, HVAL, REAL, (gmlvsp[0].str), 0); }
#line 1820 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 376 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, LABEL, STRING, (gmlvsp[0].str), 0); }
#line 1826 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 377 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, GRAPHICS, LIST, 0, (gmlvsp[0].list)); }
#line 1832 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 378 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, LABELGRAPHICS, LIST, 0, (gmlvsp[0].list)); }
#line 1838 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 379 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, TYPE, STRING, (gmlvsp[0].str), 0); }
#line 1844 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 380 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, FILL, STRING, (gmlvsp[0].str), 0); }
#line 1850 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 381 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, OUTLINE, STRING, (gmlvsp[0].str), 0); }
#line 1856 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 382 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, OUTLINESTYLE, STRING, (gmlvsp[0].str), 0); }
#line 1862 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 383 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, OUTLINEWIDTH, INTEGER, (gmlvsp[0].str), 0); }
#line 1868 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 384 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, OUTLINEWIDTH, REAL, (gmlvsp[0].str), 0); }
#line 1874 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 385 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, STYLE, STRING, (gmlvsp[0].str), 0); }
#line 1880 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 386 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, STYLE, LIST, 0, (gmlvsp[0].list)); }
#line 1886 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 387 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, LINE, LIST, 0, (gmlvsp[0].list)); }
#line 1892 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 388 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, POINT, LIST, 0, (gmlvsp[0].list)); }
#line 1898 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 389 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, TEXT, STRING, (gmlvsp[0].str), 0); }
#line 1904 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 390 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, FONTNAME, STRING, (gmlvsp[0].str), 0); }
#line 1910 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 391 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, FONTNAME, INTEGER, (gmlvsp[0].str), 0); }
#line 1916 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 392 "../../cmd/tools/gmlparse.y" /* yacc.c:1646  */
    { (gmlval.ap) = mkAttr (0, COLOR, STRING, (gmlvsp[0].str), 0); }
#line 1922 "y.tab.c" /* yacc.c:1646  */
    break;


#line 1926 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter gmlchar, and that requires
     that gmltoken be updated with the new translation.  We take the
     approach of translating immediately before every use of gmltoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering gmlchar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", gmlr1[gmln], &gmlval, &gmlloc);

  YYPOPSTACK (gmllen);
  gmllen = 0;
  YY_STACK_PRINT (gmlss, gmlssp);

  *++gmlvsp = gmlval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  gmln = gmlr1[gmln];

  gmlstate = gmlpgoto[gmln - YYNTOKENS] + *gmlssp;
  if (0 <= gmlstate && gmlstate <= YYLAST && gmlcheck[gmlstate] == *gmlssp)
    gmlstate = gmltable[gmlstate];
  else
    gmlstate = gmldefgoto[gmln - YYNTOKENS];

  goto gmlnewstate;


/*--------------------------------------.
| gmlerrlab -- here on detecting error.  |
`--------------------------------------*/
gmlerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  gmltoken = gmlchar == YYEMPTY ? YYEMPTY : YYTRANSLATE (gmlchar);

  /* If not already recovering from an error, report this error.  */
  if (!gmlerrstatus)
    {
      ++gmlnerrs;
#if ! YYERROR_VERBOSE
      gmlerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR gmlsyntax_error (&gmlmsg_alloc, &gmlmsg, \
                                        gmlssp, gmltoken)
      {
        char const *gmlmsgp = YY_("syntax error");
        int gmlsyntax_error_status;
        gmlsyntax_error_status = YYSYNTAX_ERROR;
        if (gmlsyntax_error_status == 0)
          gmlmsgp = gmlmsg;
        else if (gmlsyntax_error_status == 1)
          {
            if (gmlmsg != gmlmsgbuf)
              YYSTACK_FREE (gmlmsg);
            gmlmsg = (char *) YYSTACK_ALLOC (gmlmsg_alloc);
            if (!gmlmsg)
              {
                gmlmsg = gmlmsgbuf;
                gmlmsg_alloc = sizeof gmlmsgbuf;
                gmlsyntax_error_status = 2;
              }
            else
              {
                gmlsyntax_error_status = YYSYNTAX_ERROR;
                gmlmsgp = gmlmsg;
              }
          }
        gmlerror (gmlmsgp);
        if (gmlsyntax_error_status == 2)
          goto gmlexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (gmlerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (gmlchar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (gmlchar == YYEOF)
            YYABORT;
        }
      else
        {
          gmldestruct ("Error: discarding",
                      gmltoken, &gmllval);
          gmlchar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto gmlerrlab1;


/*---------------------------------------------------.
| gmlerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
gmlerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label gmlerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto gmlerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (gmllen);
  gmllen = 0;
  YY_STACK_PRINT (gmlss, gmlssp);
  gmlstate = *gmlssp;
  goto gmlerrlab1;


/*-------------------------------------------------------------.
| gmlerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
gmlerrlab1:
  gmlerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      gmln = gmlpact[gmlstate];
      if (!gmlpact_value_is_default (gmln))
        {
          gmln += YYTERROR;
          if (0 <= gmln && gmln <= YYLAST && gmlcheck[gmln] == YYTERROR)
            {
              gmln = gmltable[gmln];
              if (0 < gmln)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (gmlssp == gmlss)
        YYABORT;


      gmldestruct ("Error: popping",
                  gmlstos[gmlstate], gmlvsp);
      YYPOPSTACK (1);
      gmlstate = *gmlssp;
      YY_STACK_PRINT (gmlss, gmlssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++gmlvsp = gmllval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", gmlstos[gmln], gmlvsp, gmllsp);

  gmlstate = gmln;
  goto gmlnewstate;


/*-------------------------------------.
| gmlacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
gmlacceptlab:
  gmlresult = 0;
  goto gmlreturn;

/*-----------------------------------.
| gmlabortlab -- YYABORT comes here.  |
`-----------------------------------*/
gmlabortlab:
  gmlresult = 1;
  goto gmlreturn;

#if !defined gmloverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| gmlexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
gmlexhaustedlab:
  gmlerror (YY_("memory exhausted"));
  gmlresult = 2;
  /* Fall through.  */
#endif

gmlreturn:
  if (gmlchar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      gmltoken = YYTRANSLATE (gmlchar);
      gmldestruct ("Cleanup: discarding lookahead",
                  gmltoken, &gmllval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (gmllen);
  YY_STACK_PRINT (gmlss, gmlssp);
  while (gmlssp != gmlss)
    {
      gmldestruct ("Cleanup: popping",
                  gmlstos[*gmlssp], gmlvsp);
      YYPOPSTACK (1);
    }
#ifndef gmloverflow
  if (gmlss != gmlssa)
    YYSTACK_FREE (gmlss);
#endif
#if YYERROR_VERBOSE
  if (gmlmsg != gmlmsgbuf)
    YYSTACK_FREE (gmlmsg);
#endif
  return gmlresult;
}
#line 395 "../../cmd/tools/gmlparse.y" /* yacc.c:1906  */


static void
free_attr (Dt_t*d, gmlattr* p, Dtdisc_t* ds)
{
    if (!p) return;
    if ((p->kind == LIST) && p->u.lp)
	dtclose (p->u.lp);
    else
	free (p->u.value);
    free (p->name);
    free (p);
}

static void deparseList (Dt_t* alist, agxbuf* xb); /* forward declaration */

static void
deparseAttr (gmlattr* ap, agxbuf* xb)
{
    if (ap->kind == LIST) {
	agxbput (xb, ap->name);
	agxbputc (xb, ' ');
	deparseList (ap->u.lp, xb);
    }
    else if (ap->kind == STRING) {
	agxbput (xb, ap->name);
	agxbput (xb, " \"");
	agxbput (xb, ap->u.value);
	agxbput (xb, "\"");
    }
    else {
	agxbput (xb, ap->name);
	agxbputc (xb, ' ');
	agxbput (xb, ap->u.value);
    }
}

static void
deparseList (Dt_t* alist, agxbuf* xb)
{
    gmlattr* ap;

    agxbput (xb, "[ "); 
    if (alist) for (ap = dtfirst(alist); ap; ap = dtnext (alist, ap)) {
	deparseAttr (ap, xb);
	agxbputc (xb, ' ');
    }
    agxbput (xb, "]"); 
  
}

static void
unknown (Agobj_t* obj, gmlattr* ap, agxbuf* xb)
{
    char* str;

    if (ap->kind == LIST) {
	deparseList (ap->u.lp, xb);
	str = agxbuse (xb);
    }
    else
	str = ap->u.value;

    agsafeset (obj, ap->name, str, "");
}

static void
addNodeLabelGraphics (Agnode_t* np, Dt_t* alist, agxbuf* xb, agxbuf* unk)
{
    gmlattr* ap;
    int cnt = 0;

    if (!alist)
	return;

    for (ap = dtfirst(alist); ap; ap = dtnext (alist, ap)) {
	if (ap->sort == TEXT) {
	    agsafeset (np, "label", ap->u.value, "");
	}
	else if (ap->sort == COLOR) {
	    agsafeset (np, "fontcolor", ap->u.value, "");
	}
	else if (ap->sort == FONTSIZE) {
	    agsafeset (np, "fontsize", ap->u.value, "");
	}
	else if (ap->sort == FONTNAME) {
	    agsafeset (np, "fontname", ap->u.value, "");
	}
	else {
	    if (cnt)
		agxbputc (unk, ' '); 
	    else {
		agxbput (unk, "[ "); 
	    }
	    deparseAttr (ap, unk);
	    cnt++;
	}
    }

    if (cnt) {
	agxbput (unk, " ]"); 
	agsafeset (np, "LabelGraphics", agxbuse (unk), "");
    }
    else
	agxbclear (unk); 
}

static void
addEdgeLabelGraphics (Agedge_t* ep, Dt_t* alist, agxbuf* xb, agxbuf* unk)
{
    gmlattr* ap;
    char* x = "0";
    char* y = "0";
    int cnt = 0;

    if (!alist)
	return;

    for (ap = dtfirst(alist); ap; ap = dtnext (alist, ap)) {
	if (ap->sort == TEXT) {
	    agsafeset (ep, "label", ap->u.value, "");
	}
	else if (ap->sort == COLOR) {
	    agsafeset (ep, "fontcolor", ap->u.value, "");
	}
	else if (ap->sort == FONTSIZE) {
	    agsafeset (ep, "fontsize", ap->u.value, "");
	}
	else if (ap->sort == FONTNAME) {
	    agsafeset (ep, "fontname", ap->u.value, "");
	}
	else if (ap->sort == XVAL) {
	    x = ap->u.value;
	}
	else if (ap->sort == YVAL) {
	    y = ap->u.value;
	}
	else {
	    if (cnt)
		agxbputc (unk, ' '); 
	    else {
		agxbput (unk, "[ "); 
	    }
	    deparseAttr (ap, unk);
	    cnt++;
	}
    }

    agxbput (xb, x);
    agxbputc (xb, ',');
    agxbput (xb, y);
    agsafeset (ep, "lp", agxbuse (xb), "");

    if (cnt) {
	agxbput (unk, " ]"); 
	agsafeset (ep, "LabelGraphics", agxbuse (unk), "");
    }
    else
	agxbclear (unk); 
}

static void
addNodeGraphics (Agnode_t* np, Dt_t* alist, agxbuf* xb, agxbuf* unk)
{
    gmlattr* ap;
    char* x = "0";
    char* y = "0";
    char buf[BUFSIZ];
    double d;
    int cnt = 0;

    for (ap = dtfirst(alist); ap; ap = dtnext (alist, ap)) {
	if (ap->sort == XVAL) {
	    x = ap->u.value;
	}
	else if (ap->sort == YVAL) {
	    y = ap->u.value;
	}
	else if (ap->sort == WVAL) {
	    d = atof (ap->u.value);
	    sprintf (buf, "%.04f", d/72.0);
	    agsafeset (np, "width", buf, "");
	}
	else if (ap->sort == HVAL) {
	    d = atof (ap->u.value);
	    sprintf (buf, "%.04f", d/72.0);
	    agsafeset (np, "height", buf, "");
	}
	else if (ap->sort == TYPE) {
	    agsafeset (np, "shape", ap->u.value, "");
	}
	else if (ap->sort == FILL) {
	    agsafeset (np, "color", ap->u.value, "");
	}
	else if (ap->sort == OUTLINE) {
	    agsafeset (np, "pencolor", ap->u.value, "");
	}
	else if ((ap->sort == WIDTH) && (ap->sort == OUTLINEWIDTH )) {
	    agsafeset (np, "penwidth", ap->u.value, "");
	}
	else if ((ap->sort == OUTLINESTYLE) && (ap->sort == OUTLINEWIDTH )) {
	    agsafeset (np, "style", ap->u.value, "");
	}
	else {
	    if (cnt)
		agxbputc (unk, ' '); 
	    else {
		agxbput (unk, "[ "); 
	    }
	    deparseAttr (ap, unk);
	    cnt++;
	}
    }

    agxbput (xb, x);
    agxbputc (xb, ',');
    agxbput (xb, y);
    agsafeset (np, "pos", agxbuse (xb), "");

    if (cnt) {
	agxbput (unk, " ]"); 
	agsafeset (np, "graphics", agxbuse (unk), "");
    }
    else
	agxbclear (unk); 
}

static void
addEdgePoint (Agedge_t* ep, Dt_t* alist, agxbuf* xb)
{
    gmlattr* ap;
    char* x = "0";
    char* y = "0";

    for (ap = dtfirst(alist); ap; ap = dtnext (alist, ap)) {
        if (ap->sort == XVAL) {
	    x = ap->u.value;
	}
	else if (ap->sort == YVAL) {
	    y = ap->u.value;
	}
	else {
	    fprintf (stderr, "non-X/Y field in point attribute");
	    unknown ((Agobj_t*)ep, ap, xb);
	}
    }

    if (agxblen(xb)) agxbputc (xb, ' ');
    agxbput (xb, x);
    agxbputc (xb, ',');
    agxbput (xb, y);
}

static void
addEdgePos (Agedge_t* ep, Dt_t* alist, agxbuf* xb)
{
    gmlattr* ap;
    
    if (!alist) return;
    for (ap = dtfirst(alist); ap; ap = dtnext (alist, ap)) {
	if (ap->sort == POINT) {
	    addEdgePoint (ep, ap->u.lp, xb);
	}
	else {
	    fprintf (stderr, "non-point field in line attribute");
	    unknown ((Agobj_t*)ep, ap, xb);
	}
    }
    agsafeset (ep, "pos", agxbuse (xb), "");
}

static void
addEdgeGraphics (Agedge_t* ep, Dt_t* alist, agxbuf* xb, agxbuf* unk)
{
    gmlattr* ap;
    int cnt = 0;

    for (ap = dtfirst(alist); ap; ap = dtnext (alist, ap)) {
	if (ap->sort == WIDTH) {
	    agsafeset (ep, "penwidth", ap->u.value, "");
	}
	else if (ap->sort == STYLE) {
	    agsafeset (ep, "style", ap->u.value, "");
	}
	else if (ap->sort == FILL) {
	    agsafeset (ep, "color", ap->u.value, "");
	}
	else if (ap->sort == LINE) {
	    addEdgePos (ep, ap->u.lp, xb);
	}
	else {
	    if (cnt)
		agxbputc (unk, ' '); 
	    else {
		agxbput (unk, "[ "); 
	    }
	    deparseAttr (ap, unk);
	    cnt++;
	}
    }

    if (cnt) {
	agxbput (unk, " ]"); 
	agsafeset (ep, "graphics", agxbuse (unk), "");
    }
    else
	agxbclear(unk);
}

static void
addAttrs (Agobj_t* obj, Dt_t* alist, agxbuf* xb, agxbuf* unk)
{
    gmlattr* ap;

    for (ap = dtfirst(alist); ap; ap = dtnext (alist, ap)) {
	if (ap->sort == GRAPHICS) {
	    if (AGTYPE(obj) == AGNODE)
		addNodeGraphics ((Agnode_t*)obj, ap->u.lp, xb, unk);
	    else if (AGTYPE(obj) == AGEDGE)
		addEdgeGraphics ((Agedge_t*)obj, ap->u.lp, xb, unk);
	    else
		unknown (obj, ap, xb);
	}
	else if (ap->sort == LABELGRAPHICS) {
	    if (AGTYPE(obj) == AGNODE)
		addNodeLabelGraphics ((Agnode_t*)obj, ap->u.lp, xb, unk);
	    else if (AGTYPE(obj) == AGEDGE)
		addEdgeLabelGraphics ((Agedge_t*)obj, ap->u.lp, xb, unk);
	    else
		unknown (obj, ap, xb);
	}
	else if ((ap->sort == LABEL) && (AGTYPE(obj) != AGRAPH)) {
	    agsafeset (obj, "name", ap->u.value, "");
	}
	else
	    unknown (obj, ap, xb);
    }
}

static Agraph_t*
mkGraph (gmlgraph* G, Agraph_t* parent, char* name, agxbuf* xb, agxbuf* unk)
{
    Agraph_t* g;
    Agnode_t* n;
    Agnode_t* h;
    Agedge_t* e;
    gmlnode*  np;
    gmledge*  ep;
    gmlgraph* gp;

    if (parent) {
	g = agsubg (parent, NULL, 1);
    }
    else if (G->directed >= 1)
	g = agopen (name, Agdirected, 0);
    else
	g = agopen (name, Agundirected, 0);

    if (!parent && L) {
	addAttrs ((Agobj_t*)g, L, xb, unk);
    } 
    for (np = dtfirst(G->nodelist); np; np = dtnext (G->nodelist, np)) {
	if (!np->id) {
	   fprintf (stderr, "node without an id attribute"); 
	   exit (1);
        }
	n = agnode (g, np->id, 1);
	addAttrs ((Agobj_t*)n, np->attrlist, xb, unk);
    }

    for (ep = dtfirst(G->edgelist); ep; ep = dtnext (G->edgelist, ep)) {
	if (!ep->source) {
	   fprintf (stderr, "edge without an source attribute"); 
	   exit (1);
        }
	if (!ep->target) {
	   fprintf (stderr, "node without an target attribute"); 
	   exit (1);
        }
	n = agnode (g, ep->source, 1);
	h = agnode (g, ep->target, 1);
	e = agedge (g, n, h, NULL, 1);
	addAttrs ((Agobj_t*)e, ep->attrlist, xb, unk);
    }
    for (gp = dtfirst(G->graphlist); gp; gp = dtnext (G->graphlist, gp)) {
	mkGraph (gp, g, NULL, xb, unk);
    }

    addAttrs ((Agobj_t*)g, G->attrlist, xb, unk);

    return g;
}

Agraph_t*
gml_to_gv (char* name, FILE* fp, int cnt, int* errors)
{
    Agraph_t* g;
    agxbuf xb;
    unsigned char buf[BUFSIZ];
    agxbuf unk;
    unsigned char unknownb[BUFSIZ];
    int error;

    if (cnt == 0)
	initgmlscan(fp);
    else
	initgmlscan(0);
		
    initstk();
    L = NULL;
    pushAlist ();
    gmlparse ();

    error = gmlerrors();
    *errors |= error;
    if (!G || error)
	g = NULL;
    else {
	agxbinit (&xb, BUFSIZ, buf);
	agxbinit (&unk, BUFSIZ, unknownb);
	g = mkGraph (G, NULL, name, &xb, &unk);
	agxbfree (&xb);
    }

    cleanup ();

    return g;
}

static char*
sortToStr (int sort)
{
    char* s;

    switch (sort) {
    case GRAPH : 
	s = "graph"; break;
    case NODE : 
	s = "node"; break;
    case EDGE : 
	s = "edge"; break;
    case DIRECTED : 
	s = "directed"; break;
    case ID : 
	s = "id"; break;
    case SOURCE : 
	s = "source"; break;
    case TARGET : 
	s = "target"; break;
    case XVAL : 
	s = "xval"; break;
    case YVAL : 
	s = "yval"; break;
    case WVAL : 
	s = "wval"; break;
    case HVAL : 
	s = "hval"; break;
    case LABEL : 
	s = "label"; break;
    case GRAPHICS : 
	s = "graphics"; break;
    case LABELGRAPHICS : 
	s = "labelGraphics"; break;
    case TYPE : 
	s = "type"; break;
    case FILL : 
	s = "fill"; break;
    case OUTLINE : 
	s = "outline"; break;
    case OUTLINESTYLE : 
	s = "outlineStyle"; break;
    case OUTLINEWIDTH : 
	s = "outlineWidth"; break;
    case WIDTH : 
	s = "width"; break;
    case STYLE : 
	s = "style"; break;
    case LINE : 
	s = "line"; break;
    case POINT : 
	s = "point"; break;
    case TEXT : 
	s = "text"; break;
    case FONTSIZE : 
	s = "fontSize"; break;
    case FONTNAME : 
	s = "fontName"; break;
    case COLOR : 
	s = "color"; break;
    case INTEGER : 
	s = "integer"; break;
    case REAL : 
	s = "real"; break;
    case STRING : 
	s = "string"; break;
    case NAME : 
	s = "name"; break;
    case LIST : 
	s = "list"; break;
    case '[' : 
	s = "["; break;
    case ']' : 
	s = "]"; break;
    default : 
	s = NULL;break;
    }

    return s;
}

#if 0
int gmllex ()
{
    int tok = _gmllex();
    char* s = sortToStr (tok);

    if (s)
        fprintf (stderr, "token = %s\n", s);
    else
        fprintf (stderr, "token = <%d>\n", tok);
    return tok;
}
#endif

