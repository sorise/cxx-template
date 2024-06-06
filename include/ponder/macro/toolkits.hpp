#ifndef PONDER_TEMPLATE_TOOLKITS_HPP
#define PONDER_TEMPLATE_TOOLKITS_HPP

#define MACRO_COMMA() ,
#define MACRO_LPAREN() (
#define MACRO_RPAREN() )
#define MACRO_EMPTY()

#define PP_CONCAT(A, B) PP_CONCAT_IMPL(A, B)
#define PP_CONCAT_IMPL(A, B) A##B

#define PP_GET_N_0(_0, ...) _0
#define PP_GET_N_1(_0, _1, ...) _1
#define PP_GET_N_2(_0, _1, _2, ...) _2
#define PP_GET_N_3(_0, _1, _2, _3, ...) _3
#define PP_GET_N_4(_0, _1, _2, _3,_4, ...) _4
#define PP_GET_N_5(_0, _1, _2, _3,_4,_5, ...) _5
#define PP_GET_N_6(_0, _1, _2, _3,_4,_5,_6, ...) _6
#define PP_GET_N_7(_0, _1, _2, _3,_4,_5,_6,_7, ...) _7
#define PP_GET_N_8(_0, _1, _2, _3,_4,_5,_6,_7,_8, ...) _8
#define PP_GET_N_9(_0, _1, _2, _3,_4,_5,_6,_7,_8,_9, ...) _9
#define PP_GET_N_10(_0, _1, _2, _3,_4,_5,_6,_7,_8,_9,_10, ...) _10
#define PP_GET_N_11(_0, _1, _2, _3,_4,_5,_6,_7,_8,_9,_10,_11, ...) _11
#define PP_GET_N_12(_0, _1, _2, _3,_4,_5,_6,_7,_8,_9,_10,_11,_12, ...) _12
#define PP_GET_N_13(_0, _1, _2, _3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13, ...) _13
#define PP_GET_N_14(_0, _1, _2, _3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14, ...) _14
#define PP_GET_N_15(_0, _1, _2, _3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15, ...) _15
#define PP_GET_N_16(_0, _1, _2, _3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16, ...) _16

#define PP_GET_N(N, ...) PP_CONCAT(PP_GET_N_, N)(__VA_ARGS__)

#define MACRO_IF(PRED, THEN, ELSE) PP_CONCAT(MACRO_IF_, PRED)(THEN, ELSE)

#define MACRO_IF_true(THEN, ELSE) THEN
#define MACRO_IF_1(THEN, ELSE) THEN
#define MACRO_IF_false(THEN, ELSE) ELSE
#define MACRO_IF_0(THEN, ELSE) ELSE

#define PP_NOT(N) PP_CONCAT(PP_NOT_, N)
#define PP_NOT_0 1
#define PP_NOT_1 0

#define PP_AND(A, B) PP_CONCAT(PP_AND_, PP_CONCAT(A, B))
#define PP_AND_00 0
#define PP_AND_01 0
#define PP_AND_10 0
#define PP_AND_11 1

#define PP_HAS_COMMA(...) PP_GET_N_16(__VA_ARGS__ , 1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1, 0,0)

#define PP_COMMA_V(...) ,
#define PP_IS_EMPTY(...)                                 \
  PP_AND(PP_AND(PP_NOT(PP_HAS_COMMA(__VA_ARGS__)),       \
                PP_NOT(PP_HAS_COMMA(__VA_ARGS__()))),    \
  PP_AND(PP_NOT(PP_HAS_COMMA(PP_COMMA_V __VA_ARGS__)),   \
                PP_HAS_COMMA(PP_COMMA_V __VA_ARGS__())))

#define PP_COMMA() ,
#define PP_LPAREN() (
#define PP_RPAREN() )
#define PP_EMPTY()

#define PP_BOOL(N) PP_CONCAT(PP_BOOL_, N)
#define PP_BOOL_0 0
#define PP_BOOL_1 1
#define PP_BOOL_2 1

#define PP_IF_1(THEN, ELSE) THEN
#define PP_IF_0(THEN, ELSE) ELSE

#define PP_IF(PRED, THEN, ELSE) \
    PP_CONCAT(PP_IF_, PP_BOOL(PRED))(THEN, ELSE)

#define PP_COMMA_IF(N) PP_IF(N,PP_COMMA, PP_EMPTY)()
#define PP_VA_OPT_COMMA(...) PP_COMMA_IF(PP_NOT(PP_IS_EMPTY(__VA_ARGS__)))

#define MM_COMMA_IF

#define PP_ARGC_COUNT_NOARGS 0
#define PP_ARGC_COUNT(...) \
    PP_IF(PP_IS_EMPTY(__VA_ARGS__), PP_ARGC_COUNT_NOARGS, PP_GET_N(16, __VA_ARGS__ PP_VA_OPT_COMMA(__VA_ARGS__) 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))


#define MACRO_FOR_EACH_1(DO, x) DO(x);
#define MACRO_FOR_EACH_2(DO, x, ...) DO(x); MACRO_FOR_EACH_1(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_3(DO, x, ...) DO(x); MACRO_FOR_EACH_2(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_4(DO, x, ...) DO(x); MACRO_FOR_EACH_3(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_5(DO, x, ...) DO(x); MACRO_FOR_EACH_4(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_6(DO, x, ...) DO(x); MACRO_FOR_EACH_5(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_7(DO, x, ...) DO(x); MACRO_FOR_EACH_6(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_8(DO, x, ...) DO(x); MACRO_FOR_EACH_7(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_9(DO, x, ...) DO(x); MACRO_FOR_EACH_8(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_10(DO, x, ...) DO(x); MACRO_FOR_EACH_9(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_11(DO, x, ...) DO(x); MACRO_FOR_EACH_10(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_12(DO, x, ...) DO(x); MACRO_FOR_EACH_11(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_13(DO, x, ...) DO(x); MACRO_FOR_EACH_12(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_14(DO, x, ...) DO(x); MACRO_FOR_EACH_13(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_15(DO, x, ...) DO(x); MACRO_FOR_EACH_14(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_16(DO, x, ...) DO(x); MACRO_FOR_EACH_15(DO, __VA_ARGS__)

#define MACRO_FOR_EACH(DD, ...) \
    PP_CONCAT(MACRO_FOR_EACH_, PP_ARGC_COUNT(__VA_ARGS__))(DD, __VA_ARGS__)

#endif
