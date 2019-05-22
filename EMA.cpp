/* This is the Porter stemming algorithm, coded up in ANSI C by the
   author. It may be be regarded as cononical, in that it follows the
   algorithm presented in

   Porter, 1980, An algorithm for suffix stripping, Program, Vol. 14,
   no. 3, pp 130-137,

   only differing from it at the points maked --DEPARTURE-- below.

   The algorithm as described in the paper could be exactly replicated
   by adjusting the points of DEPARTURE, but this is barely necessary,
   because (a) the points of DEPARTURE are definitely improvements, and
   (b) no encoding of the Porter stemmer I have seen is anything like
   as exact as this version, even with the points of DEPARTURE!

   You can compile it on Unix with 'gcc -O3 -o stem stem.c' after which
   'stem' takes a list of inputs and sends the stemmed equivalent to
   stdout.

   The algorithm as encoded here is particularly fast.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include "EMA.h"


//constants for hashing
#define EMA_TOTAL_KEYWORDS 571
#define EMA_MIN_WORD_LENGTH 1
#define EMA_MAX_WORD_LENGTH 13
#define EMA_MIN_HASH_VALUE 1
#define EMA_MAX_HASH_VALUE 1021

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif

//return a hash value for a given word
static unsigned int EMA_hash (register const char *str, register unsigned int len)
{
  static unsigned short asso_values[] =
    {
      1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022, 1022,  260,  211,  510,
       385,   15,  123,  315,  431,  501,   40,  391,  322,  285,
       480,   83,   26,    5,  135,  337,  470,   56,  248,  436,
        10,   35,    0, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
      1022, 1022, 1022, 1022, 1022, 1022
    };
  return len + asso_values[(unsigned char)str[len - 1]] + asso_values[(unsigned char)str[0]];
}

#ifdef __GNUC__
__inline
#endif
const char * EMA_searchStopword(register const char *str, register unsigned int len)
{
  static const char * wordlist[] =
    {
      "z",
      "q",
      "x",
      "que",
      "quite",
      "ex",
      "e",
      "else",
      "example",
      "everyone",
      "elsewhere",
      "everywhere",
      "please",
      "possible",
      "p",
      "every",
      "you're",
      "you've",
      "exactly",
      "entirely",
      "everybody",
      "especially",
      "probably",
      "presumably",
      "y",
      "particularly",
      "edu",
      "use",
      "j",
      "up",
      "uucp",
      "zero",
      "you",
      "usually",
      "unlikely",
      "one",
      "once",
      "unfortunately",
      "outside",
      "otherwise",
      "u",
      "okay",
      "only",
      "obviously",
      "five",
      "unto",
      "furthermore",
      "re",
      "ever",
      "either",
      "per",
      "formerly",
      "yourself",
      "o",
      "onto",
      "particular",
      "your",
      "really",
      "reasonably",
      "relatively",
      "respectively",
      "under",
      "of",
      "off",
      "or",
      "our",
      "over",
      "other",
      "be",
      "became",
      "beside",
      "before",
      "become",
      "because",
      "believe",
      "f",
      "by",
      "viz",
      "qv",
      "far",
      "for",
      "four",
      "former",
      "further",
      "value",
      "r",
      "rather",
      "are",
      "able",
      "above",
      "aside",
      "alone",
      "anyone",
      "anywhere",
      "available",
      "appreciate",
      "appropriate",
      "very",
      "any",
      "away",
      "anyway",
      "already",
      "awfully",
      "anybody",
      "me",
      "actually",
      "more",
      "maybe",
      "accordingly",
      "meanwhile",
      "my",
      "may",
      "many",
      "mainly",
      "mostly",
      "merely",
      "eg",
      "gone",
      "brief",
      "everything",
      "like",
      "little",
      "also",
      "six",
      "better",
      "see",
      "she",
      "same",
      "sure",
      "some",
      "since",
      "someone",
      "sensible",
      "sometime",
      "somewhere",
      "lately",
      "you'll",
      "likely",
      "latterly",
      "sup",
      "plus",
      "perhaps",
      "provides",
      "say",
      "yes",
      "using",
      "sorry",
      "yours",
      "specify",
      "secondly",
      "somebody",
      "seriously",
      "yourselves",
      "until",
      "useful",
      "us",
      "uses",
      "unless",
      "go",
      "after",
      "appear",
      "another",
      "done",
      "despite",
      "from",
      "overall",
      "myself",
      "placed",
      "keep",
      "so",
      "b",
      "ones",
      "ours",
      "you'd",
      "others",
      "moreover",
      "ourselves",
      "definitely",
      "used",
      "following",
      "he",
      "each",
      "here",
      "have",
      "hence",
      "enough",
      "we",
      "were",
      "we're",
      "whose",
      "whole",
      "while",
      "where",
      "we've",
      "whence",
      "welcome",
      "regarding",
      "help",
      "later",
      "latter",
      "self",
      "follows",
      "do",
      "old",
      "hardly",
      "hereby",
      "way",
      "why",
      "hopefully",
      "ok",
      "whereby",
      "regards",
      "regardless",
      "thanx",
      "et",
      "the",
      "take",
      "eight",
      "twice",
      "three",
      "those",
      "these",
      "there",
      "except",
      "thence",
      "they're",
      "they've",
      "therefore",
      "v",
      "even",
      "none",
      "nine",
      "name",
      "noone",
      "nowhere",
      "try",
      "yet",
      "they",
      "truly",
      "via",
      "thereby",
      "just",
      "thoroughly",
      "oh",
      "followed",
      "ie",
      "hello",
      "i've",
      "a",
      "namely",
      "nobody",
      "nearly",
      "rd",
      "who",
      "normally",
      "necessary",
      "indicate",
      "immediate",
      "etc",
      "came",
      "come",
      "cause",
      "thru",
      "course",
      "being",
      "becoming",
      "un",
      "upon",
      "am",
      "sub",
      "clearly",
      "certainly",
      "currently",
      "becomes",
      "besides",
      "to",
      "out",
      "two",
      "too",
      "consequently",
      "ought",
      "fifth",
      "forth",
      "herself",
      "himself",
      "few",
      "no",
      "on",
      "own",
      "often",
      "her",
      "m",
      "hither",
      "however",
      "hereafter",
      "wonder",
      "whether",
      "whoever",
      "whither",
      "whatever",
      "wherever",
      "whenever",
      "along",
      "among",
      "asking",
      "whereafter",
      "anything",
      "according",
      "all",
      "vs",
      "into",
      "various",
      "co",
      "first",
      "as",
      "a's",
      "behind",
      "beyond",
      "across",
      "always",
      "allows",
      "anyways",
      "beforehand",
      "afterwards",
      "right",
      "their",
      "together",
      "thereafter",
      "nor",
      "near",
      "never",
      "neither",
      "seem",
      "if",
      "itself",
      "g",
      "going",
      "getting",
      "inner",
      "insofar",
      "looking",
      "l",
      "both",
      "and",
      "around",
      "below",
      "consider",
      "ask",
      "associated",
      "gets",
      "goes",
      "gives",
      "saying",
      "seeing",
      "seeming",
      "greetings",
      "something",
      "specifying",
      "less",
      "let's",
      "still",
      "shall",
      "looks",
      "several",
      "s",
      "says",
      "seems",
      "selves",
      "serious",
      "sometimes",
      "but",
      "best",
      "been",
      "between",
      "although",
      "allow",
      "anyhow",
      "doing",
      "during",
      "ltd",
      "liked",
      "look",
      "him",
      "much",
      "whom",
      "does",
      "said",
      "second",
      "should",
      "seemed",
      "downwards",
      "specified",
      "at",
      "keeps",
      "knows",
      "about",
      "apart",
      "ain't",
      "almost",
      "aren't",
      "against",
      "amongst",
      "an",
      "again",
      "having",
      "willing",
      "most",
      "them",
      "must",
      "might",
      "well",
      "will",
      "we'll",
      "mean",
      "has",
      "his",
      "d",
      "he's",
      "such",
      "hers",
      "did",
      "here's",
      "happens",
      "saw",
      "was",
      "wants",
      "who's",
      "what's",
      "described",
      "somehow",
      "whereas",
      "where's",
      "k",
      "get",
      "got",
      "i'm",
      "trying",
      "let",
      "last",
      "tell",
      "lest",
      "least",
      "com",
      "they'll",
      "given",
      "gotten",
      "nothing",
      "novel",
      "t's",
      "sent",
      "thus",
      "this",
      "tends",
      "tries",
      "thats",
      "thanks",
      "theres",
      "theirs",
      "that's",
      "there's",
      "towards",
      "somewhat",
      "shouldn't",
      "themselves",
      "had",
      "seen",
      "soon",
      "needs",
      "seven",
      "we'd",
      "would",
      "would",
      "i'll",
      "it'll",
      "nevertheless",
      "know",
      "concerning",
      "containing",
      "considering",
      "corresponding",
      "is",
      "its",
      "it's",
      "indicates",
      "c's",
      "comes",
      "causes",
      "changes",
      "contains",
      "don't",
      "tried",
      "third",
      "didn't",
      "toward",
      "they'd",
      "doesn't",
      "h",
      "different",
      "kept",
      "took",
      "thank",
      "think",
      "nd",
      "down",
      "need",
      "how",
      "wish",
      "with",
      "which",
      "w",
      "known",
      "i'd",
      "it'd",
      "indeed",
      "inward",
      "ignored",
      "instead",
      "indicated",
      "could",
      "th",
      "hadn't",
      "though",
      "hasn't",
      "haven't",
      "through",
      "howbeit",
      "thorough",
      "want",
      "what",
      "went",
      "won't",
      "wasn't",
      "weren't",
      "without",
      "wouldn't",
      "herein",
      "hereupon",
      "new",
      "now",
      "when",
      "within",
      "wherein",
      "whereupon",
      "hi",
      "inasmuch",
      "t",
      "that",
      "throughout",
      "not",
      "next",
      "then",
      "than",
      "taken",
      "therein",
      "thereupon",
      "n",
      "non",
      "it",
      "isn't",
      "in",
      "cant",
      "can't",
      "cannot",
      "couldn't",
      "can",
      "c'mon",
      "certain",
      "contain",
      "i",
      "inc",
      "c"
    };

  static short lookup[] =
    {
         -1,     0,    -1,    -1,    -1,    -1,    -1,
         -1,    -1,    -1,    -1,     1,    -1,    -1,
         -1,    -1,    -1,    -1,    -1,    -1,    -1,
          2,    -1,     3,    -1,     4,    -1,     5,
         -1,    -1,    -1,     6,    -1,    -1,     7,
         -1,    -1,     8,     9,    10,    11,    -1,
         -1,    -1,    -1,    -1,    -1,    12,    -1,
         13,    -1,    -1,    -1,    14,    -1,    15,
       -633,    18,    19,    20,    21,  -555,    -2,
         -1,    -1,    -1,    -1,    -1,    -1,    22,
         -1,  -649,    -1,    25,  -647,  -545,    -2,
       -548,    -2,    -1,    -1,    28,    -1,    -1,
         29,    -1,    30,    31,    -1,    -1,    -1,
         -1,    -1,    -1,    32,    -1,    -1,    -1,
         33,    34,    -1,    35,    36,    -1,    37,
         38,    -1,    39,    -1,    -1,    -1,    -1,
         -1,    40,    -1,    -1,    -1,    -1,    -1,
         -1,    -1,    -1,  -695,  -530,    -2,    -1,
         -1,    43,    -1,    -1,    -1,    -1,    -1,
         -1,    -1,    -1,    -1,    -1,    -1,    -1,
         -1,    -1,    44,    45,    -1,    -1,    -1,
         -1,    -1,    46,    -1,    -1,    47,    -1,
         48,    -1,    49,    -1,    -1,    -1,    -1,
         -1,    -1,    -1,    50,    -1,  -740,    53,
       -520,    -2,    54,    55,    -1,    -1,    56,
         -1,    57,    -1,    -1,    -1,  -755,    -1,
         60,  -513,    -2,    -1,    -1,    -1,    -1,
         -1,    -1,    -1,    -1,    -1,    -1,    -1,
         61,    -1,    -1,    -1,    -1,    -1,    -1,
         -1,    -1,    -1,    -1,    -1,    62,    63,
         -1,    -1,    -1,    -1,    -1,    -1,    -1,
         -1,    -1,    -1,    64,    65,    66,    67,
         -1,    -1,    -1,    -1,    68,    -1,    -1,
         -1,  -808,  -806,  -498,    -2,  -502,    -4,
         -1,    -1,    -1,    -1,    -1,    -1,    -1,
         -1,    -1,    75,    76,    -1,    -1,    77,
         -1,    -1,    -1,    78,    -1,    -1,    -1,
         -1,    -1,  -838,    81,    -1,    82,    83,
       -492,    -2,    84,    -1,    -1,    85,    -1,
         -1,    -1,    -1,    86,    -1,    87,    88,
       -860,    92,    -1,    93,    94,    95,    96,
         97,  -482,    -3,    -1,    -1,    -1,    -1,
         -1,    -1,    -1,    -1,    98,    99,    -1,
        100,  -879,   105,   106,   107,   108,  -470,
         -4,   109,    -1,    -1,    -1,    -1,    -1,
         -1,    -1,    -1,    -1,    -1,    -1,    -1,
        110,   111,   112,    -1,  -899,  -458,    -3,
         -1,    -1,    -1,   116,    -1,   117,    -1,
         -1,    -1,    -1,   118,   119,   120,    -1,
        121,    -1,    -1,    -1,   122,    -1,    -1,
        123,    -1,   124,    -1,    -1,  -965,  -963,
        130,    -1,   131,  -944,   134,    -1,  -940,
         -1,   138,   139,   140,  -436,    -3,   141,
        142,  -439,    -2,    -1,  -961,   145,  -959,
         -1,   148,  -957,   151,   152,   153,   154,
       -422,    -2,  -425,    -2,  -428,    -2,  -444,
         -3,  -446,    -2,   155,    -1,   156,    -1,
        157,  -977,   160,   161,    -1,   162,  -413,
         -2,   163,    -1,    -1,    -1,    -1,  -987,
         -1,   166,  -407,    -2,   167,    -1,    -1,
         -1,   168,   169,   170, -1003,   173,   174,
         -1,   175,   176,   177,  -400,    -2,    -1,
         -1,    -1,    -1,    -1,    -1,    -1,    -1,
         -1,    -1,    -1,    -1,   178,    -1,   179,
        180,    -1, -1040,   184,   185,   186,    -1,
        187, -1037,   194,   195,   196,    -1,   197,
        198,   199,   200,  -383,    -6,   201,  -390,
         -3,   202,   203, -1055,    -1, -1052,   208,
        209,    -1,   210,   211,  -365,    -2,   212,
       -367,    -2,   213,    -1,   214,   215,   216,
      -1262, -1077, -1067,    -1,   227,  -346,    -2,
        228,    -1, -1075,   233,    -1,   234,  -342,
         -4,  -348,    -2,    -1, -1260,   237,   238,
        239,   240,    -1,   241,   242, -1120,    -1,
        245,   246,   247, -1117, -1115,   254, -1098,
        257,  -316,    -2,   258, -1113, -1107, -1104,
       -308,    -2,   265,  -310,    -2,    -1,   266,
         -1,   267,  -312,    -2,  -319,    -2,  -323,
         -4,   268,  -328,    -2,    -1,   269,   270,
         -1, -1258, -1248, -1221,   279,   280, -1210,
         -1, -1135,   285,  -288,    -2, -1204,   288,
         -1,   289,   290,    -1,   291,   292,   293,
         -1,   294,    -1,   295, -1195, -1168, -1165,
      -1161,    -1,   306,   307,   308,    -1,   309,
        310,  -267,    -2,    -1,   311,  -269,    -2,
        312,  -272,    -3,   313,   314,   315,    -1,
      -1188, -1180,   321,    -1,   322,   323,  -253,
         -3, -1183,  -247,    -2,   326,    -1,   327,
       -255,    -2,   328,   329,   330,    -1,   331,
       -275,    -3,    -1, -1199,  -239,    -2,    -1,
        334,   335,  -285,    -2,    -1,   336,    -1,
        337,  -290,    -2,    -1,   338,    -1,   339,
        340,   341,   342,    -1,   343,  -295,    -3,
        344,   345,   346,   347,   348, -1245,   351,
      -1243,   354,    -1, -1241,   357,   358, -1239,
         -1,   363,  -212,    -4,  -216,    -2,  -219,
         -2,  -222,    -2,   364,  -298,    -3,   365,
        366,   367,   368,    -1,   369,   370,   371,
       -300,    -2,  -336,    -2,  -354,    -6,    -1,
         -1,    -1,   372,    -1,    -1,   373,   374,
         -1,   375,   376,    -1,    -1,   377,   378,
         -1,    -1,    -1,   379,    -1,   380,    -1,
         -1,    -1,    -1,   381,    -1,   382,   383,
         -1,    -1,    -1,    -1,   384, -1322,    -1,
      -1301,  -184,    -3, -1320,   392, -1318,    -1,
      -1315, -1312, -1310,  -171,    -2,  -173,    -2,
        402,  -176,    -3,   403,  -178,    -2,  -181,
         -2,  -186,    -2,   404,    -1,    -1,    -1,
         -1,    -1,   405, -1338,   409,    -1, -1336,
        412,  -161,    -2,  -165,    -3,    -1,   413,
         -1, -1473, -1380,   420,   421,   422, -1377,
         -1, -1358, -1356, -1353,  -142,    -3,   432,
       -144,    -2,  -146,    -2, -1364,   435,    -1,
        436,  -138,    -2,    -1,   437, -1375,   441,
        442,   443,   444,   445,   446,  -133,    -3,
       -148,    -2,   447,  -154,    -3,   448, -1459,
      -1430, -1428, -1420,   461,   462,   463,    -1,
        464,    -1, -1417, -1395,  -104,    -2,   469,
      -1404,   472,   473,   474,    -1,   475,  -101,
         -2,    -1, -1415,   478,    -1,   479,    -1,
        480,   481,   482,   -95,    -2,  -106,    -2,
        483,  -112,    -2,   484,    -1,   485,   486,
        487,   488,  -116,    -4,  -119,    -3, -1457,
      -1455,   495,   496,   497, -1453, -1451,   502,
         -1, -1449,   505, -1446,   508,   509,   -65,
         -2,   510,   -68,    -2,   -71,    -2,   -73,
         -2,   -79,    -3,   -82,    -3,  -122,    -3,
        511,   512,    -1, -1470, -1468,    -1,   517,
        -56,    -2,   -58,    -2,   518,  -157,    -3,
        519,    -1,    -1,    -1, -1504, -1502,   526,
      -1500,   530,   531, -1487,   534,   -39,    -2,
        535,    -1, -1498,   539,    -1,   540,   541,
         -1,   542,   -35,    -3,   -44,    -3,   -48,
         -3,   -51,    -3,   543,    -1,    -1,    -1,
         -1,    -1,   544,   545,    -1,    -1,   546,
         -1,    -1,    -1,    -1,    -1,   547,    -1,
         -1,   548, -1536,   552,    -1,   553,    -1,
        554,    -1,   555,    -1,   556,   -22,    -3,
         -1,    -1,    -1,    -1,    -1,    -1,    -1,
        557,    -1,    -1,   558,    -1,    -1,    -1,
         -1,    -1,    -1,   559,   560,   561,   562,
         -1,   563,    -1,    -1,    -1,    -1,   564,
         -1,   565,    -1, -1570,    -5,    -2,    -1,
         -1,    -1,   568,    -1,    -1,    -1,    -1,
         -1,    -1,    -1,    -1,    -1,    -1,   569,
         -1,    -1,    -1,    -1,    -1,    -1,   570
    };

  if (len <= EMA_MAX_WORD_LENGTH && len >= EMA_MIN_WORD_LENGTH)
    {
      register int key = EMA_hash (str, len);

      if (key <= EMA_MAX_HASH_VALUE && key >= 0)
        {
          register int index = lookup[key];

          if (index >= 0)
            {
              register const char *s = wordlist[index];

              if (*str == *s && !strcmp (str + 1, s + 1))
                return s;
            }
          else if (index < -EMA_TOTAL_KEYWORDS)
            {
              register int offset = - 1 - EMA_TOTAL_KEYWORDS - index;
              register const char * *wordptr = &wordlist[EMA_TOTAL_KEYWORDS + lookup[offset]];
              register const char * *wordendptr = wordptr + -lookup[offset + 1];

              while (wordptr < wordendptr)
                {
                  register const char *s = *wordptr;

                  if (*str == *s && !strcmp (str + 1, s + 1))
                    return s;
                  wordptr++;
                }
            }
        }
    }
  return 0;
}

//initialize English stemmer
//do nothing 
//stopwords are included in a source file
EMA* EMA_create()
{
	EMA *ema;

	ema=(EMA*)malloc(sizeof(EMA));
	
	return ema;
}

UINT1 EMA_init(EMA *ema)
{
	return TRUE;
}

UINT1 EMA_final(EMA *ema)
{
	return TRUE;
}

int EMA_isStopword(EMA *ema, char *str, int str_size)
{
	if(EMA_searchStopword(str, str_size)!=0){
		return TRUE;
	}
	return FALSE;
}


/**********************************************************************
   The main part of the stemming algorithm starts here. b is a buffer
   holding a word to be stemmed. The letters are in b[k0], b[k0+1] ...
   ending at b[k]. In fact k0 = 0 in this demo program. k is readjusted
   downwards as the stemming progresses. Zero termination is not in fact
   used in the algorithm.

   Note that only lower case sequences are stemmed. Forcing to lower case
   should be done before stem(...) is called.
*/

static char * EMA_b;       /* buffer for word to be stemmed */
static int EMA_k,EMA_k0, EMA_j;     /* j is a general offset into the string */

/* cons(i) is TRUE <=> b[i] is a consonant. */

int EMA_cons(int i)
{  switch ( EMA_b[i])
   {  case 'a': case 'e': case 'i': case 'o': case 'u': return FALSE;
      case 'y': return (i==EMA_k0) ? TRUE : !EMA_cons(i-1);
      default: return TRUE;
   }
}

/* m() measures the number of consonant sequences between k0 and j. if c is
   a consonant sequence and v a vowel sequence, and <..> indicates arbitrary
   presence,

      <c><v>       gives 0
      <c>vc<v>     gives 1
      <c>vcvc<v>   gives 2
      <c>vcvcvc<v> gives 3
      ....
*/

int EMA_m()
{  int n = 0;
   int i = EMA_k0;
   while(TRUE)
   {  if (i > EMA_j) return n;
      if (! EMA_cons(i)) break; i++;
   }
   i++;
   while(TRUE)
   {  while(TRUE)
      {  if (i > EMA_j) return n;
            if (EMA_cons(i)) break;
            i++;
      }
      i++;
      n++;
      while(TRUE)
      {  if (i > EMA_j) return n;
         if (! EMA_cons(i)) break;
         i++;
      }
      i++;
    }
}

/* vowelinstem() is TRUE <=> k0,...j contains a vowel */

int EMA_vowelinstem()
{  int i; for (i = EMA_k0; i <= EMA_j; i++) if (! EMA_cons(i)) return TRUE;
   return FALSE;
}

/* doublec(j) is TRUE <=> j,(j-1) contain a double consonant. */

int EMA_doublec(int j)
{  if (j < EMA_k0+1) return FALSE;
   if (EMA_b[j] != EMA_b[j-1]) return FALSE;
   return EMA_cons(j);
}

/* cvc(i) is TRUE <=> i-2,i-1,i has the form consonant - vowel - consonant
   and also if the second c is not w,x or y. this is used when trying to
   restore an e at the end of a short word. e.g.

      cav(e), lov(e), hop(e), crim(e), but
      snow, box, tray.

*/

int EMA_cvc(int i)
{  
   int ch;

   if (i < EMA_k0+2 || !EMA_cons(i) || 
		EMA_cons(i-1) || !EMA_cons(i-2)) return FALSE;
   {  ch = EMA_b[i];
      if (ch == 'w' || ch == 'x' || ch == 'y') return FALSE;
   }
   return TRUE;
}

/* ends(s) is TRUE <=> k0,...k ends with the string s. */

int EMA_ends(char * s)
{  int length = s[0];

   if (s[length] != EMA_b[EMA_k]) return FALSE; /* tiny speed-up */

   if (length > EMA_k-EMA_k0+1) return FALSE;
   if (memcmp(EMA_b+EMA_k-length+1,s+1,length) != 0) return FALSE;
   EMA_j = EMA_k-length;
   return TRUE;
}

/* setto(s) sets (j+1),...k to the characters in the string s, readjusting
   k. */

void EMA_setto(char * s)
{  int length = s[0];
   memmove(EMA_b+EMA_j+1,s+1,length);
   EMA_k = EMA_j+length;
}

/* r(s) is used further down. */

void EMA_r(char * s) { if (EMA_m() > 0) EMA_setto(s); }

/* step1ab() gets rid of plurals and -ed or -ing. e.g.

       caresses  ->  caress
       ponies    ->  poni
       ties      ->  ti
       caress    ->  caress
       cats      ->  cat

       feed      ->  feed
       agreed    ->  agree
       disabled  ->  disable

       matting   ->  mat
       mating    ->  mate
       meeting   ->  meet
       milling   ->  mill
       messing   ->  mess

       meetings  ->  meet

*/

void EMA_step1ab()
{  
   int ch;

   if (EMA_b[EMA_k] == 's')
   {  if (EMA_ends("\04" "sses")) EMA_k -= 2; else
      if (EMA_ends("\03" "ies")) EMA_setto("\01" "i"); else
      if (EMA_b[EMA_k-1] != 's') EMA_k--;
   }

   if (EMA_ends("\03" "eed")) { if (EMA_m() > 0) EMA_k--; } else

   if ((EMA_ends("\02" "ed") || EMA_ends("\03" "ing")) && EMA_vowelinstem())
   {  EMA_k = EMA_j;
      if (EMA_ends("\02" "at")) EMA_setto("\03" "ate"); else
      if (EMA_ends("\02" "bl")) EMA_setto("\03" "ble"); else
      if (EMA_ends("\02" "iz")) EMA_setto("\03" "ize"); else
      if (EMA_doublec(EMA_k))
      {  EMA_k--;
         {  ch = EMA_b[EMA_k];
            if (ch == 'l' || ch == 's' || ch == 'z') EMA_k++;
         }
      }
      else if (EMA_m() == 1 && EMA_cvc(EMA_k)) EMA_setto("\01" "e");
  }
}


/* step1c() turns terminal y to i when there is another vowel in the stem. */

void EMA_step1c() { 
	if (EMA_ends("\01" "y") && EMA_vowelinstem()) EMA_b[EMA_k] = 'i'; 
}


/* step2() maps double suffices to single ones. so -ization ( = -ize plus
   -ation) maps to -ize etc. note that the string before the suffix must give
   m() > 0. */

void EMA_step2() { 
    switch (EMA_b[EMA_k-1]) {
    case 'a': if (EMA_ends("\07" "ational")) { EMA_r("\03" "ate"); break; }
              if (EMA_ends("\06" "tional")) { EMA_r("\04" "tion"); break; }
              break;
    case 'c': if (EMA_ends("\04" "enci")) { EMA_r("\04" "ence"); break; }
              if (EMA_ends("\04" "anci")) { EMA_r("\04" "ance"); break; }
              break;
    case 'e': if (EMA_ends("\04" "izer")) { EMA_r("\03" "ize"); break; }
              break;
    case 'l': if (EMA_ends("\03" "bli")) { EMA_r("\03" "ble"); break; } /*-DEPARTURE-*/


 /* To match the published algorithm, replace this line with
    case 'l': if (ends("\04" "abli")) { r("\04" "able"); break; } */


              if (EMA_ends("\04" "alli")) { EMA_r("\02" "al"); break; }
              if (EMA_ends("\05" "entli")) { EMA_r("\03" "ent"); break; }
              if (EMA_ends("\03" "eli")) { EMA_r("\01" "e"); break; }
              if (EMA_ends("\05" "ousli")) { EMA_r("\03" "ous"); break; }
              break;
    case 'o': if (EMA_ends("\07" "ization")) { EMA_r("\03" "ize"); break; }
              if (EMA_ends("\05" "ation")) { EMA_r("\03" "ate"); break; }
              if (EMA_ends("\04" "ator")) { EMA_r("\03" "ate"); break; }
              break;
    case 's': if (EMA_ends("\05" "alism")) { EMA_r("\02" "al"); break; }
              if (EMA_ends("\07" "iveness")) { EMA_r("\03" "ive"); break; }
              if (EMA_ends("\07" "fulness")) { EMA_r("\03" "ful"); break; }
              if (EMA_ends("\07" "ousness")) { EMA_r("\03" "ous"); break; }
              break;
    case 't': if (EMA_ends("\05" "aliti")) { EMA_r("\02" "al"); break; }
              if (EMA_ends("\05" "iviti")) { EMA_r("\03" "ive"); break; }
              if (EMA_ends("\06" "biliti")) { EMA_r("\03" "ble"); break; }
              break;
    case 'g': if (EMA_ends("\04" "logi")) { EMA_r("\03" "log"); break; } /*-DEPARTURE-*/

 /* To match the published algorithm, delete this line */ 

    } 
}

/* step3() deals with -ic-, -full, -ness etc. similar strategy to step2. */

void EMA_step3() { 
    
    switch (EMA_b[EMA_k]) {
    case 'e': if (EMA_ends("\05" "icate")) { EMA_r("\02" "ic"); break; }
              if (EMA_ends("\05" "ative")) { EMA_r("\00" ""); break; }
              if (EMA_ends("\05" "alize")) { EMA_r("\02" "al"); break; }
              break;
    case 'i': if (EMA_ends("\05" "iciti")) { EMA_r("\02" "ic"); break; }
              break;
    case 'l': if (EMA_ends("\04" "ical")) { EMA_r("\02" "ic"); break; }
              if (EMA_ends("\03" "ful")) { EMA_r("\00" ""); break; }
              break;
    case 's': if (EMA_ends("\04" "ness")) { EMA_r("\00" ""); break; }
              break; 
    } 
}

/* step4() takes off -ant, -ence etc., in context <c>vcvc<v>. */

void EMA_step4() {  

    switch (EMA_b[EMA_k-1])
    {  case 'a': if (EMA_ends("\02" "al")) break; return;
       case 'c': if (EMA_ends("\04" "ance")) break;
                 if (EMA_ends("\04" "ence")) break; return;
       case 'e': if (EMA_ends("\02" "er")) break; return;
       case 'i': if (EMA_ends("\02" "ic")) break; return;
       case 'l': if (EMA_ends("\04" "able")) break;
                 if (EMA_ends("\04" "ible")) break; return;
       case 'n': if (EMA_ends("\03" "ant")) break;
                 if (EMA_ends("\05" "ement")) break;
                 if (EMA_ends("\04" "ment")) break;
                 if (EMA_ends("\03" "ent")) break; return;
       case 'o': if (EMA_ends("\03" "ion") && (EMA_b[EMA_j] == 's' || EMA_b[EMA_j] == 't')) break;
                 if (EMA_ends("\02" "ou")) break; return;
                 /* takes care of -ous */
       case 's': if (EMA_ends("\03" "ism")) break; return;
       case 't': if (EMA_ends("\03" "ate")) break;
                 if (EMA_ends("\03" "iti")) break; return;
       case 'u': if (EMA_ends("\03" "ous")) break; return;
       case 'v': if (EMA_ends("\03" "ive")) break; return;
       case 'z': if (EMA_ends("\03" "ize")) break; return;
       default: return;
    }
    if (EMA_m() > 1) EMA_k = EMA_j;
}

/* step5() removes a final -e if m() > 1, and changes -ll to -l if
   m() > 1. */

void EMA_step5()
{  
   int a;
   EMA_j = EMA_k;
   if (EMA_b[EMA_k] == 'e')
   {  a = EMA_m();
      if ((a > 1) || ((a == 1) && !EMA_cvc(EMA_k-1))) EMA_k--;
   }
   if (EMA_b[EMA_k] == 'l' && EMA_doublec(EMA_k) && EMA_m() > 1) EMA_k--;
}

/* In stem(p,i,j), p is a char pointer, and the string to be stemmed is from
   p[i] to p[j] inclusive. Typically i is zero and j is the offset to the last
   character of a string, (p[j+1] == '\0'). The stemmer adjusts the
   characters p[i] ... p[j] and returns the new end-point of the string, k.
   Stemming never increases word length, so i <= k <= j. To turn the stemmer
   into a module, declare 'stem' as extern, and delete the remainder of this
   file.
*/

int EMA_stem(char * p, int i, int j)
{  EMA_b = p; EMA_k = j; EMA_k0 = i; /* copy the parameters into statics */
   if (EMA_k <= EMA_k0+1) return EMA_k; /*-DEPARTURE-*/

   /* With this line, strings of length 1 or 2 don't go through the
      stemming process, although no mention is made of this in the
      published algorithm. Remove the line to match the published
      algorithm. */

   EMA_step1ab(); 
   EMA_step1c(); 
   EMA_step2(); 
   EMA_step3(); 
   EMA_step4(); 
   EMA_step5();
   return EMA_k;
}

int EMA_isSymbolChar(char ch)
{
	if(ch>='!' && ch <='/'){
		return TRUE;
	}
	if(ch>=':' && ch <='@'){
		return TRUE;
	}
	if(ch>='[' && ch <='`'){
		return TRUE;
	}
	if(ch>='{' && ch <='~'){
		return TRUE;
	}
	return FALSE;
}



static char * EMA_s;         /* a char * (=string) pointer; passed into b above */

#define EMA_INC 50           /* size units in which s is increased */
static int EMA_i_max = EMA_INC;  /* maximum offset in s */

void EMA_increase_s()
{  
   char *new_s;
   int i;

   EMA_i_max += EMA_INC;
   {  new_s = (char *) malloc(EMA_i_max+1);
      { for (i = 0; i < EMA_i_max; i++) new_s[i] = EMA_s[i]; } /* copy across */
      free(EMA_s); EMA_s = new_s;
   }
}

UINT1 EMA_destroy(EMA *ema)
{
	free(ema);
	return TRUE;
}


//stemming a given word
//data: input word
//word: result
//limit: the maximum length of result
char *EMA_extractTerms(EMA *ema, char *data, char *word, unsigned long limit)
{

	int length;
	char *ptr, *start;
	char *result;
	

	ptr=data;
	result=word;


	if(ptr==NULL || *ptr=='\0'){
		word[0]='\0';
		return NULL;
	}

	//eliminates start symbol characters
	for(; EMA_isSymbolChar(*ptr); ptr++)
			;
	
	start=ptr;

	//change input word to lower case
	for(length=0; *ptr!='\0' && (unsigned)length < limit-1;length++ ){
		if(isupper(*ptr)){
			*result++=tolower(*ptr++);
		}	
		else{
			*result++=*ptr++;
		}
	}	
	if(length==0){
		word[0]='\0';
		return NULL;
	}
	word[length]='\0';

	//processing the last letter in a string
	for(ptr--, result--; EMA_isSymbolChar(*ptr); ptr--, result--, length--){
			*result='\0';		
	}

	if(*ptr=='s' && *(ptr-1)=='\''){
			result--;
			length--;
			ptr--;
			*result='\0';
	}

	if(ptr==start){
		word[0]='\0';
		return NULL;
	}	

/******************************************************************/
/*  code for stop words search					  */
/******************************************************************/
	if(EMA_isStopword(ema, word, length)){

	}
	
/*******************************************************************/
/* code for term search						   */
/*******************************************************************/
	else{
		if(word[0]>='0' && word[0] <='9'){
			return word;
		}
		else{
			word[EMA_stem(word, 0, length-1)+1]='\0';
			return word;
		}
	}
	return NULL;
}


int  EMA_is_num(EMA *ema, char *word)
{
  int i;
  i=0;

  for(; (isdigit(*word) || *word=='.') && i < 3 ; word++, i++);

  if(i==3)  return 0;
  if(*word=='\0') return 1;

  return 0;
}

int  EMA_is_an_englishTerm(EMA *ema, char *word)
{
  if(word==NULL || *word=='\0') return 0;

  if(EMA_is_num(ema, word)) return 0;

  return 1;
}




int EMA_getIndexTerms(EMA *ema, char *word, char (*result)[MAX_MORPHEME_SIZE+1]/*, char **output*/){

    char copy_string[MAX_MORPHEME_SIZE+1];
    int  english_result_num;
    char *termPtr4MAEnglish;//, *previous_ptr4English;

    strncpy(copy_string, word, MAX_MORPHEME_SIZE);
    copy_string[MAX_MORPHEME_SIZE]='\0';

    english_result_num=0;
    termPtr4MAEnglish=(char*)strtok(copy_string, " ,\\+=_-?!&./:;\"`'#$%^*@<>(){}[]|~\r\n");//, &previous_ptr4English);

    while(termPtr4MAEnglish!=NULL && english_result_num < 10){
      if(EMA_extractTerms(ema, termPtr4MAEnglish,
        result[english_result_num], MAX_MORPHEME_SIZE)!=NULL &&
        EMA_is_an_englishTerm(ema, result[english_result_num])){
          //output[english_result_num]=result[english_result_num];
          english_result_num++;
      }
      termPtr4MAEnglish=(char*)strtok(NULL, " ,\\+=_-?!&./:;\"`'#$%^*@<>(){}[]|~\r\n");//, &previous_ptr4English);
    }
    return english_result_num;
}



