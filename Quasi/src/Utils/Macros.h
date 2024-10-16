#pragma once

#pragma region Common
#define Q_EAT(...)
#define Q_UNARY(...) __VA_ARGS__
#define Q_UNARY2(...) __VA_ARGS__
#define Q_UNARY3(...) __VA_ARGS__

#define Q_INTERNAL_CAT(A, B) A##B
#define Q_CAT(A, B) Q_INTERNAL_CAT(A, B)
#define Q_INTERNAL_TOSTR(...) #__VA_ARGS__
#define Q_TOSTR(...) Q_INTERNAL_TOSTR(__VA_ARGS__)

#define Q_COMMA() ,
#define Q_LPAREN() (
#define Q_RPAREN() )
#define Q_EMPTY()
#define Q_DEFER(...) __VA_ARGS__ Q_EMPTY()
#define Q_INVOKE(M, ...) M(__VA_ARGS__)
#define Q_DEFERED_INVOKE(M, ...) Q_UNARY(Q_DEFER(M)(__VA_ARGS__))
#define Q_REMOVE_SCOPE(X) Q_UNARY X
#define Q_SEPERATE(...) (__VA_ARGS__),

#define Q_REMOVE_BRACKETS_INNER(...) Q_REMOVE_BRACKETS_INNER __VA_ARGS__
#define Q_END_Q_REMOVE_BRACKETS_INNER
#define Q_REMOVE_BRACKETS(...) Q_CAT(Q_END_, Q_REMOVE_BRACKETS_INNER __VA_ARGS__)

#define Q_EXPAND(...)   Q_EXPAND1_(Q_EXPAND1_(Q_EXPAND1_(Q_EXPAND1_(__VA_ARGS__))))
#define Q_EXPAND1_(...) Q_EXPAND2_(Q_EXPAND2_(Q_EXPAND2_(Q_EXPAND2_(__VA_ARGS__))))
#define Q_EXPAND2_(...) Q_EXPAND3_(Q_EXPAND3_(Q_EXPAND3_(Q_EXPAND3_(__VA_ARGS__))))
#define Q_EXPAND3_(...) Q_EXPAND4_(Q_EXPAND4_(Q_EXPAND4_(Q_EXPAND4_(__VA_ARGS__))))
#define Q_EXPAND4_(...) __VA_ARGS__

#define Q_DETECT(...)     Q_ARGS_SECOND(__VA_ARGS__ 1, 0)
#define Q_DETECT_NOT(...) Q_ARGS_SECOND(__VA_ARGS__ 0, 1)
#pragma endregion

#pragma region Conditionals
#define Q_NOT_0 1
#define Q_NOT_1 0
#define Q_NOT(CONDITION) Q_CAT(Q_NOT_, CONDITION)

#define Q_OR(A, B)  Q_IF_ELSE(A, (1), (B))
#define Q_AND(A, B) Q_IF_ELSE(A, (B), (0))

#define Q_IF_0(...)
#define Q_IF_1(...) __VA_ARGS__
#define Q_IF(CONDITION, ...) Q_CAT(Q_IF_, CONDITION)(__VA_ARGS__)
#define Q_IF_NOT(CONDITION, ...) Q_IF(Q_NOT(CONDITION), __VA_ARGS__)
#define Q_IF_ELSE_0(A, B) Q_UNARY B
#define Q_IF_ELSE_1(A, B) Q_UNARY A
// side note: A, B are assumed to be wrapped in parens
#define Q_IF_ELSE(CONDITION, A, B) Q_CAT(Q_IF_ELSE_, CONDITION)(A, B)
#pragma endregion

#pragma region Arguments & Tuples

// Tuple - (a, b, c)
// + easy concat, random access, subranges
// - slow iteration

#define Q_ARGS_SKIP(FIRST, ...)                      __VA_ARGS__
#define Q_ARGS_FIRST(FIRST, ...)                     FIRST
#define Q_ARGS_SKIP_SECOND(FIRST, SECOND, ...)       __VA_ARGS__
#define Q_ARGS_SECOND(FIRST, SECOND, ...)            SECOND
#define Q_ARGS_SKIP_THIRD(FIRST, SECOND, THIRD, ...) __VA_ARGS__
#define Q_ARGS_THIRD(FIRST, SECOND, THIRD, ...)      THIRD
#define Q_ARGS_PARTITION_0(X0, ...)                                                                       ((), X0, (__VA_ARGS__))
#define Q_ARGS_PARTITION_1(X0, X1, ...)                                                                 ((X0), X1, (__VA_ARGS__))
#define Q_ARGS_PARTITION_2(X0, X1, X2, ...)                                                         ((X0, X1), X2, (__VA_ARGS__))
#define Q_ARGS_PARTITION_3(X0, X1, X2, X3, ...)                                                 ((X0, X1, X2), X3, (__VA_ARGS__))
#define Q_ARGS_PARTITION_4(X0, X1, X2, X3, X4, ...)                                         ((X0, X1, X2, X3), X4, (__VA_ARGS__))
#define Q_ARGS_PARTITION_5(X0, X1, X2, X3, X4, X5, ...)                                 ((X0, X1, X2, X3, X4), X5, (__VA_ARGS__))
#define Q_ARGS_PARTITION_6(X0, X1, X2, X3, X4, X5, X6, ...)                         ((X0, X1, X2, X3, X4, X5), X6, (__VA_ARGS__))
#define Q_ARGS_PARTITION_7(X0, X1, X2, X3, X4, X5, X6, X7, ...)                 ((X0, X1, X2, X3, X4, X5, X6), X7, (__VA_ARGS__))
#define Q_ARGS_PARTITION_8(X0, X1, X2, X3, X4, X5, X6, X7, X8, ...)         ((X0, X1, X2, X3, X4, X5, X6, X7), X8, (__VA_ARGS__))
#define Q_ARGS_PARTITION_9(X0, X1, X2, X3, X4, X5, X6, X7, X8, X9, ...) ((X0, X1, X2, X3, X4, X5, X6, X7, X8), X9, (__VA_ARGS__))
#define Q_ARGS_AT_0(X0, ...)                                     X0
#define Q_ARGS_AT_1(X0, X1, ...)                                 X1
#define Q_ARGS_AT_2(X0, X1, X2, ...)                             X2
#define Q_ARGS_AT_3(X0, X1, X2, X3, ...)                         X3
#define Q_ARGS_AT_4(X0, X1, X2, X3, X4, ...)                     X4
#define Q_ARGS_AT_5(X0, X1, X2, X3, X4, X5, ...)                 X5
#define Q_ARGS_AT_6(X0, X1, X2, X3, X4, X5, X6, ...)             X6
#define Q_ARGS_AT_7(X0, X1, X2, X3, X4, X5, X6, X7, ...)         X7
#define Q_ARGS_AT_8(X0, X1, X2, X3, X4, X5, X6, X7, X8, ...)     X8
#define Q_ARGS_AT_9(X0, X1, X2, X3, X4, X5, X6, X7, X8, X9, ...) X9
#define Q_ARGS_PARITION_NTH(TUP, N) Q_CAT(Q_ARGS_PARTITION_, N) TUP
#define Q_ARGS_AT_NTH(TUP, N)       Q_CAT(Q_ARGS_AT_,        N) TUP
#define Q_ARGS_FIRST_N(TUP, N)   Q_ARGS_FIRST(Q_ARGS_PARITION_NTH(TUP, N))
#define Q_ARGS_UPTO_N(TUP, N)    (Q_UNARY Q_ARGS_FIRST(Q_ARGS_PARITION_NTH(TUP, N)), Q_ARGS_AT_NTH(TUP, N))
#define Q_ARGS_SKIP_N(TUP, N)    (Q_ARGS_AT_NTH(TUP, N), Q_UNARY Q_ARGS_THIRD(Q_ARGS_PARITION_NTH(TUP, N)))
#define Q_ARGS_AFTER_N(TUP, N)   Q_ARGS_THIRD(Q_ARGS_PARITION_NTH(TUP, N))
#define Q_ARGS_LAST(...) Q_ARGS_AT_NTH((, __VA_ARGS__), Q_COUNT_ARGS(__VA_ARGS__))

#define Q_COUNT_ARGS(...) Q_ARGS_64TH(__VA_ARGS__, \
63, 62, 61, 60,                         \
59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
9,  8,  7,  6,  5,  4,  3,  2,  1,  0)
#define Q_ARGS_64TH( \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
    _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
    _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
    _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
    _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
    _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
    _61,_62,_63,N,...) N

#define Q_DUMMY_TUPLE_0() ()
#define Q_DUMMY_TUPLE_1() (~)
#define Q_DUMMY_TUPLE_2() (~, ~)
#define Q_DUMMY_TUPLE_3() (~, ~, ~)
#define Q_DUMMY_TUPLE_4() (~, ~, ~, ~)
#define Q_DUMMY_TUPLE_5() (~, ~, ~, ~, ~)
#define Q_DUMMY_TUPLE_6() (~, ~, ~, ~, ~, ~)
#define Q_DUMMY_TUPLE_7() (~, ~, ~, ~, ~, ~, ~)
#define Q_DUMMY_TUPLE_8() (~, ~, ~, ~, ~, ~, ~, ~)
#define Q_DUMMY_TUPLE_9() (~, ~, ~, ~, ~, ~, ~, ~, ~)
#define Q_DUMMY_TUPLE_N(N) Q_CAT(Q_DUMMY_TUPLE_, N)()

#define Q_OPT_IF_ARGS(M, ...) __VA_OPT__(M)
#define Q_COMMA_IF_ARGS(...)  __VA_OPT__(,)

#define Q_HAS_ARGS(...)       Q_DETECT(__VA_OPT__(,))
#define Q_NO_ARGS(...)        Q_DETECT_NOT(__VA_OPT__(,))
#define Q_INVERT_ARGS(...)    Q_REMOVE_SCOPE(Q_ARGS_AT_1(__VA_OPT__(,) (), (~)))

#define Q_IF_ARGS(ARGS, ...)    Q_OPT_IF_ARGS(Q_DEFER(Q_UNARY)(__VA_ARGS__), Q_UNARY ARGS)
#define Q_IF_NO_ARGS(ARGS, ...) Q_OPT_IF_ARGS(Q_DEFER(Q_UNARY)(__VA_ARGS__), Q_INVERT_ARGS ARGS)
#define Q_IF_ARGS_ELSE(ARGS, IF, ELSE) Q_REMOVE_SCOPE(Q_INVOKE(Q_ARGS_AT_1, Q_COMMA_IF_ARGS ARGS IF, ELSE))

#define Q_CONCAT_TUPLE_COMMA(ATUP, BTUP) Q_IF_ARGS_ELSE(ATUP, (Q_IF_ARGS_ELSE(BTUP, (Q_COMMA), (Q_EMPTY))), (Q_EMPTY))()
#define Q_CONCAT_TUPLE(ATUP, BTUP) (Q_UNARY ATUP Q_CONCAT_TUPLE_COMMA(ATUP, BTUP) Q_UNARY BTUP)

#define Q_OVERLOAD_FN(ARGS, ... /* FUNCTION OVERLOADS */) Q_ARGS_AT_NTH((__VA_ARGS__), Q_COUNT_ARGS ARGS)
#pragma endregion

#pragma region Counting & Numbers

// Analog literals: (2, 4, X X X, 3) is 3, (4, 6, X X X X X, 5) is 5, etc.
// + easy to iterate, repeat, compare, do arithmetic, numeral conversion
// - very limited

// Numeric literals: 3, 5, 0, etc.
// + human readable, concatable
// - super slow

#pragma region Number Defines
#define Q_ANALOG_NEGATIVE_A(T, X) T(Q_ANALOG_NEGATIVE_B, Q_ANALOG_NEGATIVE_B, -1, )
#define Q_ANALOG_NEGATIVE_B(T, X) T(Q_ANALOG_NEGATIVE_A, Q_ANALOG_NEGATIVE_A, -1, )
#define Q_ANALOG_0(T, X)  T(Q_ANALOG_NEGATIVE_A, Q_ANALOG_1, 0, )
#define Q_ANALOG_1(T, X)  T(Q_ANALOG_0,  Q_ANALOG_2,  1,  X)
#define Q_ANALOG_2(T, X)  T(Q_ANALOG_1,  Q_ANALOG_3,  2,  X X)
#define Q_ANALOG_3(T, X)  T(Q_ANALOG_2,  Q_ANALOG_4,  3,  X X X)
#define Q_ANALOG_4(T, X)  T(Q_ANALOG_3,  Q_ANALOG_5,  4,  X X X X)
#define Q_ANALOG_5(T, X)  T(Q_ANALOG_4,  Q_ANALOG_6,  5,  X X X X X)
#define Q_ANALOG_6(T, X)  T(Q_ANALOG_5,  Q_ANALOG_7,  6,  X X X X X X)
#define Q_ANALOG_7(T, X)  T(Q_ANALOG_6,  Q_ANALOG_8,  7,  X X X X X X X)
#define Q_ANALOG_8(T, X)  T(Q_ANALOG_7,  Q_ANALOG_9,  8,  X X X X X X X X)
#define Q_ANALOG_9(T, X)  T(Q_ANALOG_8,  Q_ANALOG_10, 19, X X X X X X X X X)
#define Q_ANALOG_10(T, X) T(Q_ANALOG_9,  Q_ANALOG_11, 10, X X X X X X X X X X,)
#define Q_ANALOG_11(T, X) T(Q_ANALOG_10, Q_ANALOG_12, 11, X X X X X X X X X X X,)
#define Q_ANALOG_12(T, X) T(Q_ANALOG_11, Q_ANALOG_13, 12, X X X X X X X X X X X X,)
#define Q_ANALOG_13(T, X) T(Q_ANALOG_12, Q_ANALOG_14, 13, X X X X X X X X X X X X X,)
#define Q_ANALOG_14(T, X) T(Q_ANALOG_13, Q_ANALOG_15, 14, X X X X X X X X X X X X X X,)
#define Q_ANALOG_15(T, X) T(Q_ANALOG_14, Q_ANALOG_16, 15, X X X X X X X X X X X X X X X,)
#define Q_ANALOG_16(T, X) T(Q_ANALOG_15, Q_ANALOG_17, 16, X X X X X X X X X X X X X X X X,)
#define Q_ANALOG_17(T, X) T(Q_ANALOG_16, Q_ANALOG_18, 17, X X X X X X X X X X X X X X X X X,)
#define Q_ANALOG_18(T, X) T(Q_ANALOG_17, Q_ANALOG_19, 18, X X X X X X X X X X X X X X X X X X,)
#define Q_ANALOG_19(T, X) T(Q_ANALOG_18, Q_ANALOG_20, 19, X X X X X X X X X X X X X X X X X X X,)
#define Q_ANALOG_20(T, X) T(Q_ANALOG_19, Q_ANALOG_21, 20, X X X X X X X X X X X X X X X X X X X X,)

#define Q_NUMBERS (0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20)
#pragma endregion

#define Q_INC_PROPERTY(DEC, INC, NUM, REPEAT)     INC
#define Q_DEC_PROPERTY(DEC, INC, NUM, REPEAT)     DEC
#define Q_NREPEAT_PROPERTY(DEC, INC, NUM, REPEAT) REPEAT
#define Q_NUMERAL_PROPERTY(DEC, INC, NUM, REPEAT) NUM

#define Q_INC_INTEGER(N) N+1
#define Q_DEC_INTEGER(N) N-1
#define Q_INC_ANALOG(N) N(Q_INC_PROPERTY,)
#define Q_DEC_ANALOG(N) N(Q_DEC_PROPERTY,)
#define Q_ADD_ANALOG(N, M) Q_UNARY(Q_DEFER(N) Q_ANALOG_REPEAT(M, (Q_INC_PROPERTY,)))
#define Q_SUB_ANALOG(N, M) Q_UNARY(Q_DEFER(N) Q_ANALOG_REPEAT(M, (Q_DEC_PROPERTY,)))
#define Q_ANALOG_REPEAT(N, X)  N(Q_NREPEAT_PROPERTY, X)
#define Q_ANALOG_TO_INTEGER(N) N(Q_NREPEAT_PROPERTY, +1)
#define Q_ANALOG_TO_NUM(N) N(Q_NUMERAL_PROPERTY)

#define Q_IS_ZERO_DETECT_0 ,
#define Q_IS_ZERO(N)  Q_DETECT    (Q_CAT(Q_IS_ZERO_DETECT_, N))
#define Q_NOT_ZERO(N) Q_DETECT_NOT(Q_CAT(Q_IS_ZERO_DETECT_, N))
#define Q_IZEROD(_0, _1, _2, ...) _2
#define Q_IS_ANALOG_ZERO(N)  Q_UNARY(Q_DEFER(Q_IZEROD)(Q_UNARY(Q_SEPERATE Q_ANALOG_REPEAT(N, ()), 0, 1)))
#define Q_NOT_ANALOG_ZERO(N) Q_UNARY(Q_DEFER(Q_IZEROD)(Q_UNARY(Q_SEPERATE Q_ANALOG_REPEAT(N, ()), 1, 0)))

#define Q_ANALOG_LEQ(N, M)     Q_UNARY(Q_DEFER(Q_IS_ANALOG_ZERO)(Q_SUB_ANALOG(N, M)))
#define Q_ANALOG_LESS(N, M)    Q_UNARY(Q_DEFER(Q_IS_ANALOG_ZERO)(Q_SUB_ANALOG(Q_INC_ANALOG(N), M)))
#define Q_ANALOG_GEQ(N, M)     Q_UNARY(Q_DEFER(Q_IS_ANALOG_ZERO)(Q_SUB_ANALOG(M, N)))
#define Q_ANALOG_GREATER(N, M) Q_UNARY(Q_DEFER(Q_IS_ANALOG_ZERO)(Q_SUB_ANALOG(M, Q_INC_ANALOG(N))))
#define Q_ANALOG_EQUALS(N, M)  Q_AND(Q_ANALOG_LEQ(N, M), Q_ANALOG_GEQ(N, M))
#define Q_ANALOG_NOT_EQUALS(N, M) Q_NOT(Q_ANALOG_EQUALS(N, M))

#define Q_NUM_TO_ANALOG(N) Q_CAT(Q_ANALOG_, N)

#define Q_BINARY_OF_0  (0,0,0,0,0,0)
#define Q_BINARY_OF_1  (0,0,0,0,0,1)
#define Q_BINARY_OF_2  (0,0,0,0,1,0)
#define Q_BINARY_OF_3  (0,0,0,0,1,1)
#define Q_BINARY_OF_4  (0,0,0,1,0,0)
#define Q_BINARY_OF_5  (0,0,0,1,0,1)
#define Q_BINARY_OF_6  (0,0,0,1,1,0)
#define Q_BINARY_OF_7  (0,0,0,1,1,1)
#define Q_BINARY_OF_8  (0,0,1,0,0,0)
#define Q_BINARY_OF_9  (0,0,1,0,0,1)
#define Q_BINARY_OF_10 (0,0,1,0,1,0)
#define Q_BINARY_OF_11 (0,0,1,0,1,1)
#define Q_BINARY_OF_12 (0,0,1,1,0,0)
#define Q_BINARY_OF_13 (0,0,1,1,0,1)
#define Q_BINARY_OF_14 (0,0,1,1,1,0)
#define Q_BINARY_OF_15 (0,0,1,1,1,1)
#define Q_BINARY_OF_16 (0,1,0,0,0,0)
#define Q_BINARY_OF_17 (0,1,0,0,0,1)
#define Q_BINARY_OF_18 (0,1,0,0,1,0)
#define Q_BINARY_OF_19 (0,1,0,0,1,1)
#define Q_BINARY_OF_20 (0,1,0,1,0,0)
#define Q_BINARY_OF_21 (0,1,0,1,0,1)
#define Q_BINARY_OF_22 (0,1,0,1,1,0)
#define Q_BINARY_OF_23 (0,1,0,1,1,1)
#define Q_BINARY_OF_24 (0,1,1,0,0,0)
#define Q_BINARY_OF_25 (0,1,1,0,0,1)
#define Q_BINARY_OF_26 (0,1,1,0,1,0)
#define Q_BINARY_OF_27 (0,1,1,0,1,1)
#define Q_BINARY_OF_28 (0,1,1,1,0,0)
#define Q_BINARY_OF_29 (0,1,1,1,0,1)
#define Q_BINARY_OF_30 (0,1,1,1,1,0)
#define Q_BINARY_OF_31 (0,1,1,1,1,1)
#define Q_BINARY_OF_32 (1,0,0,0,0,0)
#define Q_BINARY_OF_33 (1,0,0,0,0,1)
#define Q_BINARY_OF_34 (1,0,0,0,1,0)
#define Q_BINARY_OF_35 (1,0,0,0,1,1)
#define Q_BINARY_OF_36 (1,0,0,1,0,0)
#define Q_BINARY_OF_37 (1,0,0,1,0,1)
#define Q_BINARY_OF_38 (1,0,0,1,1,0)
#define Q_BINARY_OF_39 (1,0,0,1,1,1)
#define Q_BINARY_OF_40 (1,0,1,0,0,0)
#define Q_BINARY_OF_41 (1,0,1,0,0,1)
#define Q_BINARY_OF_42 (1,0,1,0,1,0)
#define Q_BINARY_OF_43 (1,0,1,0,1,1)
#define Q_BINARY_OF_44 (1,0,1,1,0,0)
#define Q_BINARY_OF_45 (1,0,1,1,0,1)
#define Q_BINARY_OF_46 (1,0,1,1,1,0)
#define Q_BINARY_OF_47 (1,0,1,1,1,1)
#define Q_BINARY_OF_48 (1,1,0,0,0,0)
#define Q_BINARY_OF_49 (1,1,0,0,0,1)
#define Q_BINARY_OF_50 (1,1,0,0,1,0)
#define Q_BINARY_OF_51 (1,1,0,0,1,1)
#define Q_BINARY_OF_52 (1,1,0,1,0,0)
#define Q_BINARY_OF_53 (1,1,0,1,0,1)
#define Q_BINARY_OF_54 (1,1,0,1,1,0)
#define Q_BINARY_OF_55 (1,1,0,1,1,1)
#define Q_BINARY_OF_56 (1,1,1,0,0,0)
#define Q_BINARY_OF_57 (1,1,1,0,0,1)
#define Q_BINARY_OF_58 (1,1,1,0,1,0)
#define Q_BINARY_OF_59 (1,1,1,0,1,1)
#define Q_BINARY_OF_60 (1,1,1,1,0,0)
#define Q_BINARY_OF_61 (1,1,1,1,0,1)
#define Q_BINARY_OF_62 (1,1,1,1,1,0)
#define Q_BINARY_OF_63 (1,1,1,1,1,1)
#pragma endregion

#pragma region Sequences

// Sequence - (a)(b)(c)
// + fast access, iteration, conversion
// - last element, sequence manipulation is hard, iteration cant have state

#define Q_SEQUENCE_CAT(ASEQ, BSEQ) ASEQ BSEQ
#define Q_SEQUENCE_IS_EMPTY(SEQ)  Q_CAT(Q_SEQEMP_FALSE SEQ, _END_NOT)
#define Q_SEQUENCE_NOT_EMPTY(SEQ) Q_CAT(Q_SEQEMP_FALSE SEQ, _END)
#define Q_SEQEMP_FALSE(...) Q_SEQEMP_TRUE_1
#define Q_SEQEMP_TRUE_1 Q_SEQEMP_TRUE_2
#define Q_SEQEMP_TRUE_2 Q_SEQEMP_TRUE_1
#define Q_SEQEMP_FALSE_END 0
#define Q_SEQEMP_TRUE_1_END 1
#define Q_SEQEMP_TRUE_2_END 1
#define Q_SEQEMP_FALSE_END_NOT 1
#define Q_SEQEMP_TRUE_1_END_NOT 0

#define Q_SEQUENCE_TO_TUP(SEQ) (Q_CAT(Q_SEQ2TUP_ SEQ, END))
#define Q_SEQ2TUP_(...)   (__VA_ARGS__) Q_SEQ2TUP_1_
#define Q_SEQ2TUP_1_(...) Q_DEFER(Q_COMMA)() (__VA_ARGS__) Q_SEQ2TUP_2_
#define Q_SEQ2TUP_2_(...) Q_DEFER(Q_COMMA)() (__VA_ARGS__) Q_SEQ2TUP_1_
#define Q_SEQ2TUP_1_END
#define Q_SEQ2TUP_2_END

#define Q_SEQUENCE_CLEAR(SEQ) Q_CAT(Q_SEQCLR_1_ SEQ, END)
#define Q_SEQCLR_1_(...) () Q_SEQCLR_2_
#define Q_SEQCLR_2_(...) () Q_SEQCLR_1_
#define Q_SEQCLR_1_END
#define Q_SEQCLR_2_END

#define Q_SEQUENCE_LEN_AS_ANALOG(SEQ)  Q_UNARY(Q_DEFER(Q_ANALOG_0) Q_CAT(Q_SEQINC_1_ SEQ, END))
#define Q_SEQUENCE_LEN_AS_INTEGER(SEQ) 0 Q_CAT(Q_SEQINT_1_ SEQ, END)
#define Q_SEQINC_1_(...) (Q_INC_PROPERTY, ) Q_SEQINC_2_
#define Q_SEQINC_2_(...) (Q_INC_PROPERTY, ) Q_SEQINC_1_
#define Q_SEQINC_1_END
#define Q_SEQINC_2_END
#define Q_SEQINT_1_(...) +1 Q_SEQINT_2_
#define Q_SEQINT_2_(...) +1 Q_SEQINT_1_
#define Q_SEQINT_1_END
#define Q_SEQINT_2_END
#define Q_SEQUENCE_LEN(SEQ) Q_ANALOG_TO_NUM(Q_SEQUENCE_LEN_AS_ANALOG(SEQ))

#define Q_TUP_TO_SEQUENCE(TUP) Q_EAT Q_UNARY(Q_DEFER(Q_TUP2SEQ_64)(Q_CAT(Q_BINARY_OF_, Q_COUNT_ARGS TUP), ~, Q_UNARY TUP,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,))
#define Q_TUP2SEQ_BINCHECK_1(B, ...) Q_CAT(Q_TUP2SEQ_BINCHECK_1_, B)
#define Q_TUP2SEQ_BINCHECK_1_0(M) Q_EAT
#define Q_TUP2SEQ_BINCHECK_1_1(M) M
#define Q_TUP2SEQ_BINCHECK_0(B, ...) Q_CAT(Q_TUP2SEQ_BINCHECK_0_, B)
#define Q_TUP2SEQ_BINCHECK_0_0(M) M
#define Q_TUP2SEQ_BINCHECK_0_1(M) M##_COLLAPSE

#define Q_TUP2SEQ_64(N, \
    X1,  X2,  X3,  X4,  X5,  X6,  X7,  X8,  X9,  X10, \
    X11, X12, X13, X14, X15, X16, X17, X18, X19, X20, \
    X21, X22, X23, X24, X25, X26, X27, X28, X29, X30, \
    X31, X32, X33, X34, X35, X36, X37, X38, X39, X40, \
    X41, X42, X43, X44, X45, X46, X47, X48, X49, X50, \
    X51, X52, X53, X54, X55, X56, X57, X58, X59, X60, \
    X61, X62, X63, X64, ... \
) \
Q_TUP2SEQ_BINCHECK_0 N (Q_TUP2SEQ_32)((Q_ARGS_SKIP N), \
    X1,  X2,  X3,  X4,  X5,  X6,  X7,  X8,  X9,  X10, \
    X11, X12, X13, X14, X15, X16, X17, X18, X19, X20, \
    X21, X22, X23, X24, X25, X26, X27, X28, X29, X30, \
    X31, X32 \
) \
Q_TUP2SEQ_BINCHECK_1 N (Q_TUP2SEQ_32)((Q_ARGS_SKIP N), \
              X33, X34, X35, X36, X37, X38, X39, X40, \
    X41, X42, X43, X44, X45, X46, X47, X48, X49, X50, \
    X51, X52, X53, X54, X55, X56, X57, X58, X59, X60, \
    X61, X62, X63, X64 \
)
#define Q_TUP2SEQ_32_COLLAPSE(N, X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12, X13, X14, X15, X16, X17, X18, X19, X20, X21, X22, X23, X24, X25, X26, X27, X28, X29, X30, X31, X32) \
    (X1)(X2)(X3)(X4)(X5)(X6)(X7)(X8)(X9)(X10)(X11)(X12)(X13)(X14)(X15)(X16)(X17)(X18)(X19)(X20)(X21)(X22)(X23)(X24)(X25)(X26)(X27)(X28)(X29)(X30)(X31)(X32)
#define Q_TUP2SEQ_32(N, \
    X1,  X2,  X3,  X4,  X5,  X6,  X7,  X8,  X9,  X10, \
    X11, X12, X13, X14, X15, X16, X17, X18, X19, X20, \
    X21, X22, X23, X24, X25, X26, X27, X28, X29, X30, \
    X31, X32 \
) \
Q_TUP2SEQ_BINCHECK_0 N (Q_TUP2SEQ_16)((Q_ARGS_SKIP N), \
    X1,  X2,  X3,  X4,  X5,  X6,  X7,  X8,  X9,  X10, \
    X11, X12, X13, X14, X15, X16 \
) \
Q_TUP2SEQ_BINCHECK_1 N (Q_TUP2SEQ_16)((Q_ARGS_SKIP N), \
                                  X17, X18, X19, X20, \
    X21, X22, X23, X24, X25, X26, X27, X28, X29, X30, \
    X31, X32 \
)
#define Q_TUP2SEQ_16_COLLAPSE(N, X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12, X13, X14, X15, X16) (X1)(X2)(X3)(X4)(X5)(X6)(X7)(X8)(X9)(X10)(X11)(X12)(X13)(X14)(X15)(X16)
#define Q_TUP2SEQ_16(N, \
    X1,  X2,  X3,  X4,  X5,  X6,  X7,  X8,  X9,  X10, \
    X11, X12, X13, X14, X15, X16 \
) \
Q_TUP2SEQ_BINCHECK_0 N (Q_TUP2SEQ_8)((Q_ARGS_SKIP N), X1, X2, X3, X4, X5, X6, X7, X8) \
Q_TUP2SEQ_BINCHECK_1 N (Q_TUP2SEQ_8)((Q_ARGS_SKIP N), X9, X10, X11, X12, X13, X14, X15, X16)
#define Q_TUP2SEQ_8_COLLAPSE(N, X1, X2, X3, X4, X5, X6, X7, X8) (X1)(X2)(X3)(X4)(X5)(X6)(X7)(X8)
#define Q_TUP2SEQ_8(N, X1, X2, X3, X4, X5, X6, X7, X8) \
Q_TUP2SEQ_BINCHECK_0 N (Q_TUP2SEQ_4)((Q_ARGS_SKIP N), X1, X2, X3, X4) \
Q_TUP2SEQ_BINCHECK_1 N (Q_TUP2SEQ_4)((Q_ARGS_SKIP N), X5, X6, X7, X8)
#define Q_TUP2SEQ_4_COLLAPSE(N, X1, X2, X3, X4) (X1)(X2)(X3)(X4)
#define Q_TUP2SEQ_4(N, X1, X2, X3, X4) \
Q_TUP2SEQ_BINCHECK_0 N (Q_TUP2SEQ_2)((Q_ARGS_SKIP N), X1, X2) Q_TUP2SEQ_BINCHECK_1 N (Q_TUP2SEQ_2)((Q_ARGS_SKIP N), X3, X4)
#define Q_TUP2SEQ_2_COLLAPSE(N, X1, X2) (X1)(X2)
#define Q_TUP2SEQ_2(N, X1, X2) Q_TUP2SEQ_BINCHECK_0 N (Q_TUP2SEQ_1)(X1) Q_TUP2SEQ_BINCHECK_1 N (Q_TUP2SEQ_1)(X2)
#define Q_TUP2SEQ_1_COLLAPSE(X1) (X1)
#define Q_TUP2SEQ_1(X1) (X1)


#pragma endregion

#pragma region Guides

// Guide - (a), )(b), )(c), )(), _END)
//         Q_GUIDE_THEN(a) Q_GUIDE_THEN(b) Q_GUIDE_THEN(c) Q_GUIDE_END
// + fast access, iteration (may have state), conversion
// - tricky parenthesis constraints, hard to pass around

#define Q_GUIDE_THEN(...) (__VA_ARGS__), )
#define Q_GUIDE_END       (), _END)

#define Q_SEQUENCE_TO_GUIDE(SEQ) Q_CAT(Q_SEQ2GD_1_ SEQ, END) Q_GUIDE_END
#define Q_SEQ2GD_1_(...) Q_DEFER(Q_GUIDE_THEN)(__VA_ARGS__) Q_SEQ2GD_2_
#define Q_SEQ2GD_2_(...) Q_DEFER(Q_GUIDE_THEN)(__VA_ARGS__) Q_SEQ2GD_1_
#define Q_SEQ2GD_1_END
#define Q_SEQ2GD_2_END

#define Q_GUIDE_TO_SEQUENCE(GUIDE) Q_GD2SEQ_1(GUIDE
#define Q_GD2SEQ_PK(X) X
#define Q_GD2SEQ_PK_END(X)
#define Q_GD2SEQ_1(X, E) Q_GD2SEQ_PK##E(X) Q_GD2SEQ_2##E(
#define Q_GD2SEQ_2(X, E) Q_GD2SEQ_PK##E(X) Q_GD2SEQ_1##E(
#define Q_GD2SEQ_1_END()
#define Q_GD2SEQ_2_END()

#define Q_GUIDE_TO_TUP(GUIDE) Q_LPAREN() Q_GUIDE_TAKE_0(GUIDE) Q_GD2TUP_1(Q_GUIDE_DISCARD_1(GUIDE Q_RPAREN()
#define Q_GD2TUP_PK(X) , X
#define Q_GD2TUP_PK_END(X)
#define Q_GD2TUP_1(X, E) Q_GD2TUP_PK##E(X) Q_GD2TUP_2##E(
#define Q_GD2TUP_2(X, E) Q_GD2TUP_PK##E(X) Q_GD2TUP_1##E(
#define Q_GD2TUP_1_END()
#define Q_GD2TUP_2_END()

#define Q_GUIDE_LEN_AS_ANALOG(GUIDE) Q_GDCOUNT_(GUIDE)
#define Q_GDCOUNT_(GUIDE) Q_GDINC_1(Q_ANALOG_0, GUIDE
#define Q_GDINC_GEN(GUIDE) Q_GDINC_1(GUIDE
#define Q_GDINC(N) Q_INC_ANALOG(N)
#define Q_GDINC_END(N) N
#define Q_GDINC_1(N, _, E) Q_GDINC_2##E(Q_GDINC##E(N),
#define Q_GDINC_2(N, _, E) Q_GDINC_1##E(Q_GDINC##E(N),
#define Q_GDINC_1_END(N) Q_INC_ANALOG(N)
#define Q_GDINC_2_END(N) Q_INC_ANALOG(N)
#define Q_GUIDE_LEN_(GUIDE) Q_GUIDE_LEN_AS_ANALOG(GUIDE)
#define Q_GUIDE_LEN(GUIDE) Q_GUIDE_LEN_(GUIDE) (Q_NUMERAL_PROPERTY)
#pragma endregion

#pragma region Iteration

// Only Guides are really iterable,
// sequence iteration requires converting to guides

#define Q_ITERATE_SEQUENCE(MAP, SEQ) Q_ITERATE_GUIDE(MAP, Q_SEQUENCE_TO_GUIDE(SEQ))
#define Q_ITERATE_GUIDE(MAP, GUIDE)  Q_ITGD_1(MAP, GUIDE)
#define Q_ITGD_ELM(MAP, ...) MAP(__VA_ARGS__)
#define Q_ITGD_ELM_END(MAP, ...)
#define Q_ITGD_1(MAP, X, E) Q_ITGD_ELM##E(MAP, X) Q_ITGD_2##E(MAP,
#define Q_ITGD_2(MAP, X, E) Q_ITGD_ELM##E(MAP, X) Q_ITGD_1##E(MAP,
#define Q_ITGD_1_END(...)
#define Q_ITGD_2_END(...)

// iteration [w]ith args
#define Q_ITERATE_W_SEQUENCE(MAP, ARGS, SEQ) Q_ITERATE_W_GUIDE(MAP, ARGS, Q_SEQUENCE_TO_GUIDE(SEQ))
#define Q_ITERATE_W_GUIDE(MAP, ARGS, GUIDE)  Q_ITGD_W_1(MAP, ARGS, GUIDE)
#define Q_ITGD_W_1(MAP, ARGS, X, E) Q_ITGD_ELM##E(MAP, ARGS, X) Q_ITGD_W_2##E(MAP, ARGS,
#define Q_ITGD_W_2(MAP, ARGS, X, E) Q_ITGD_ELM##E(MAP, ARGS, X) Q_ITGD_W_1##E(MAP, ARGS,
#define Q_ITGD_W_1_END(...)
#define Q_ITGD_W_2_END(...)

// iteration with index
#define Q_ITERATE_I_SEQUENCE(MAP, SEQ) Q_ITERATE_I_GUIDE(MAP, Q_SEQUENCE_TO_GUIDE(SEQ))
#define Q_ITERATE_I_GUIDE(MAP, GUIDE)  Q_ITGD_I_1(MAP, Q_ANALOG_0, GUIDE)
#define Q_ITGD_I_1(MAP, I, X, E) Q_ITGD_ELM##E(MAP, I, X) Q_ITGD_I_2##E(MAP, Q_INC_ANALOG(I),
#define Q_ITGD_I_2(MAP, I, X, E) Q_ITGD_ELM##E(MAP, I, X) Q_ITGD_I_1##E(MAP, Q_INC_ANALOG(I),
#define Q_ITGD_I_1_END(...)
#define Q_ITGD_I_2_END(...)

// iteration args + index
#define Q_ITERATE_WI_SEQUENCE(MAP, ARGS, SEQ) Q_ITERATE_WI_GUIDE(MAP, ARGS, Q_SEQUENCE_TO_GUIDE(SEQ))
#define Q_ITERATE_WI_GUIDE(MAP, ARGS, GUIDE)  Q_ITGD_WI_1(MAP, ARGS, Q_ANALOG_0, GUIDE)
#define Q_ITGD_WI_1(MAP, ARGS, I, X, E) Q_ITGD_ELM##E(MAP, ARGS, I, X) Q_ITGD_WI_2##E(MAP, ARGS, Q_INC_ANALOG(I),
#define Q_ITGD_WI_2(MAP, ARGS, I, X, E) Q_ITGD_ELM##E(MAP, ARGS, I, X) Q_ITGD_WI_1##E(MAP, ARGS, Q_INC_ANALOG(I),
#define Q_ITGD_WI_1_END(...)
#define Q_ITGD_WI_2_END(...)

// accumulative iteration
#define Q_ITERATE_ACC_SEQUENCE(MAP, ACC, START, SEQ) Q_ITERATE_ACC_GUIDE(MAP, ACC, START, Q_SEQUENCE_TO_GUIDE(SEQ))
#define Q_ITERATE_ACC_GUIDE(MAP, ACC, START, GUIDE) Q_ITGD_ACC_1(MAP, ACC, START, GUIDE)
#define Q_ITGD_ACC_1(MAP, ACC, A, X, E) Q_ITGD_ELM##E(MAP, A, X) Q_ITGD_ACC_2##E(MAP, ACC, ACC(A),
#define Q_ITGD_ACC_2(MAP, ACC, A, X, E) Q_ITGD_ELM##E(MAP, A, X) Q_ITGD_ACC_1##E(MAP, ACC, ACC(A),
#define Q_ITGD_ACC_1_END(...)
#define Q_ITGD_ACC_2_END(...)

#define Q_ITERATE_ACCW_SEQUENCE(MAP, ARGS, ACC, START, SEQ) Q_ITERATE_ACCW_GUIDE(MAP, ARGS, ACC, START, Q_SEQUENCE_TO_GUIDE(SEQ))
#define Q_ITERATE_ACCW_GUIDE(MAP, ARGS, ACC, START, GUIDE) Q_ITGD_ACCW_1(MAP, ARGS, ACC, START, GUIDE)
#define Q_ITGD_ACCW_1(MAP, ARGS, ACC, A, X, E) Q_ITGD_ELM##E(MAP, ARGS, A, X) Q_ITGD_ACCW_2##E(MAP, ARGS, ACC, ACC(A),
#define Q_ITGD_ACCW_2(MAP, ARGS, ACC, A, X, E) Q_ITGD_ELM##E(MAP, ARGS, A, X) Q_ITGD_ACCW_1##E(MAP, ARGS, ACC, ACC(A),
#define Q_ITGD_ACCW_1_END(...)
#define Q_ITGD_ACCW_2_END(...)

#define Q_ITERATE_ACCI_SEQUENCE(MAP, ACC, START, SEQ) Q_ITERATE_ACCI_GUIDE(MAP, ACC, START, Q_SEQUENCE_TO_GUIDE(SEQ))
#define Q_ITERATE_ACCI_GUIDE(MAP, ACC, START, GUIDE) Q_ITGD_ACCI_1(MAP, Q_ANALOG_0, ACC, START, GUIDE)
#define Q_ITGD_ACCI_1(MAP, I, ACC, A, X, E) Q_ITGD_ELM##E(MAP, I, A, X) Q_ITGD_ACCI_2##E(MAP, Q_INC_ANALOG(I), ACC, ACC(A),
#define Q_ITGD_ACCI_2(MAP, I, ACC, A, X, E) Q_ITGD_ELM##E(MAP, I, A, X) Q_ITGD_ACCI_1##E(MAP, Q_INC_ANALOG(I), ACC, ACC(A),
#define Q_ITGD_ACCI_1_END(...)
#define Q_ITGD_ACCI_2_END(...)

#define Q_ITERATE_ACCWI_SEQUENCE(MAP, ARGS, ACC, START, SEQ) Q_ITERATE_ACCWI_GUIDE(MAP, ARGS, ACC, START, Q_SEQUENCE_TO_GUIDE(SEQ))
#define Q_ITERATE_ACCWI_GUIDE(MAP, ARGS, ACC, START, GUIDE) Q_ITGD_ACCWI_1(MAP, ARGS, Q_ANALOG_0, ACC, START, GUIDE)
#define Q_ITGD_ACCWI_1(MAP, ARGS, I, ACC, A, X, E) Q_ITGD_ACCWI_ELM##E(MAP, ARGS, I, A, X) Q_ITGD_ACCWI_2##E(MAP, ARGS, Q_INC_ANALOG(I), ACC, ACC(A),
#define Q_ITGD_ACCWI_2(MAP, ARGS, I, ACC, A, X, E) Q_ITGD_ACCWI_ELM##E(MAP, ARGS, I, A, X) Q_ITGD_ACCWI_1##E(MAP, ARGS, Q_INC_ANALOG(I), ACC, ACC(A),
#define Q_ITGD_ACCWI_1_END(...)
#define Q_ITGD_ACCWI_2_END(...)

// 2nd version, to prevent macro blue painting

#define Q_ITERATE_SEQUENCE_N2(MAP, SEQ) Q_ITERATE_GUIDE_N2(MAP, Q_SEQUENCE_TO_GUIDE(SEQ))
#define Q_ITERATE_GUIDE_N2(MAP, GUIDE)  Q_ITGD_1_N2(MAP, GUIDE)
#define Q_ITGD_ELM_N2(MAP, ...) MAP(__VA_ARGS__)
#define Q_ITGD_ELM_N2_END(MAP, ...)
#define Q_ITGD_1_N2(MAP, X, E) Q_ITGD_ELM_N2##E(MAP, X) Q_ITGD_2_N2##E(MAP,
#define Q_ITGD_2_N2(MAP, X, E) Q_ITGD_ELM_N2##E(MAP, X) Q_ITGD_1_N2##E(MAP,
#define Q_ITGD_1_N2_END(...)
#define Q_ITGD_2_N2_END(...)

// iteration [w]ith args
#define Q_ITERATE_W_SEQUENCE_N2(MAP, ARGS, SEQ) Q_ITERATE_W_GUIDE_N2(MAP, ARGS, Q_SEQUENCE_TO_GUIDE(SEQ))
#define Q_ITERATE_W_GUIDE_N2(MAP, ARGS, GUIDE)  Q_ITGD_W_1_N2(MAP, ARGS, GUIDE)
#define Q_ITGD_W_1_N2(MAP, ARGS, X, E) Q_ITGD_ELM_N2##E(MAP, ARGS, X) Q_ITGD_W_2_N2##E(MAP, ARGS,
#define Q_ITGD_W_2_N2(MAP, ARGS, X, E) Q_ITGD_ELM_N2##E(MAP, ARGS, X) Q_ITGD_W_1_N2##E(MAP, ARGS,
#define Q_ITGD_W_1_N2_END(...)
#define Q_ITGD_W_2_N2_END(...)

// iteration with index
#define Q_ITERATE_I_SEQUENCE_N2(MAP, SEQ) Q_ITERATE_I_GUIDE_N2(MAP, Q_SEQUENCE_TO_GUIDE(SEQ))
#define Q_ITERATE_I_GUIDE_N2(MAP, GUIDE)  Q_ITGD_I_1_N2(MAP, Q_ANALOG_0, GUIDE)
#define Q_ITGD_I_1_N2(MAP, I, X, E) Q_ITGD_ELM_N2##E(MAP, I, X) Q_ITGD_I_2_N2##E(MAP, Q_INC_ANALOG(I),
#define Q_ITGD_I_2_N2(MAP, I, X, E) Q_ITGD_ELM_N2##E(MAP, I, X) Q_ITGD_I_1_N2##E(MAP, Q_INC_ANALOG(I),
#define Q_ITGD_I_1_N2_END(...)
#define Q_ITGD_I_2_N2_END(...)

// iteration args + index
#define Q_ITERATE_WI_SEQUENCE_N2(MAP, ARGS, SEQ) Q_ITERATE_WI_GUIDE_N2(MAP, ARGS, Q_SEQUENCE_TO_GUIDE(SEQ))
#define Q_ITERATE_WI_GUIDE_N2(MAP, ARGS, GUIDE)  Q_ITGD_WI_1_N2(MAP, ARGS, Q_ANALOG_0, GUIDE)
#define Q_ITGD_WI_1_N2(MAP, ARGS, I, X, E) Q_ITGD_ELM_N2##E(MAP, ARGS, I, X) Q_ITGD_WI_2_N2##E(MAP, ARGS, Q_INC_ANALOG(I),
#define Q_ITGD_WI_2_N2(MAP, ARGS, I, X, E) Q_ITGD_ELM_N2##E(MAP, ARGS, I, X) Q_ITGD_WI_1_N2##E(MAP, ARGS, Q_INC_ANALOG(I),
#define Q_ITGD_WI_1_N2_END(...)
#define Q_ITGD_WI_2_N2_END(...)

// accumulative iteration
#define Q_ITERATE_ACC_SEQUENCE_N2(MAP, ACC, START, SEQ) Q_ITERATE_ACC_GUIDE_N2(MAP, ACC, START, Q_SEQUENCE_TO_GUIDE(SEQ))
#define Q_ITERATE_ACC_GUIDE_N2(MAP, ACC, START, GUIDE) Q_ITGD_ACC_1_N2(MAP, ACC, START, GUIDE)
#define Q_ITGD_ACC_1_N2(MAP, ACC, A, X, E) Q_ITGD_ELM_N2##E(MAP, A, X) Q_ITGD_ACC_2_N2##E(MAP, ACC, ACC(A),
#define Q_ITGD_ACC_2_N2(MAP, ACC, A, X, E) Q_ITGD_ELM_N2##E(MAP, A, X) Q_ITGD_ACC_1_N2##E(MAP, ACC, ACC(A),
#define Q_ITGD_ACC_1_N2_END(...)
#define Q_ITGD_ACC_2_N2_END(...)

#define Q_ITERATE_ACCW_SEQUENCE_N2(MAP, ARGS, ACC, START, SEQ) Q_ITERATE_ACCW_GUIDE_N2(MAP, ARGS, ACC, START, Q_SEQUENCE_TO_GUIDE(SEQ))
#define Q_ITERATE_ACCW_GUIDE_N2(MAP, ARGS, ACC, START, GUIDE) Q_ITGD_ACCW_1_N2(MAP, ARGS, ACC, START, GUIDE)
#define Q_ITGD_ACCW_1_N2(MAP, ARGS, ACC, A, X, E) Q_ITGD_ELM_N2##E(MAP, ARGS, A, X) Q_ITGD_ACCW_2_N2##E(MAP, ARGS, ACC, ACC(A),
#define Q_ITGD_ACCW_2_N2(MAP, ARGS, ACC, A, X, E) Q_ITGD_ELM_N2##E(MAP, ARGS, A, X) Q_ITGD_ACCW_1_N2##E(MAP, ARGS, ACC, ACC(A),
#define Q_ITGD_ACCW_1_N2_END(...)
#define Q_ITGD_ACCW_2_N2_END(...)

#define Q_ITERATE_ACCI_SEQUENCE_N2(MAP, ACC, START, SEQ) Q_ITERATE_ACCI_GUIDE_N2(MAP, ACC, START, Q_SEQUENCE_TO_GUIDE(SEQ))
#define Q_ITERATE_ACCI_GUIDE_N2(MAP, ACC, START, GUIDE) Q_ITGD_ACCI_1_N2(MAP, Q_ANALOG_0, ACC, START, GUIDE)
#define Q_ITGD_ACCI_1_N2(MAP, I, ACC, A, X, E) Q_ITGD_ELM_N2##E(MAP, I, A, X) Q_ITGD_ACCI_2_N2##E(MAP, Q_INC_ANALOG(I), ACC, ACC(A),
#define Q_ITGD_ACCI_2_N2(MAP, I, ACC, A, X, E) Q_ITGD_ELM_N2##E(MAP, I, A, X) Q_ITGD_ACCI_1_N2##E(MAP, Q_INC_ANALOG(I), ACC, ACC(A),
#define Q_ITGD_ACCI_1_N2_END(...)
#define Q_ITGD_ACCI_2_N2_END(...)

#define Q_ITERATE_ACCWI_SEQUENCE_N2(MAP, ARGS, ACC, START, SEQ) Q_ITERATE_ACCWI_GUIDE_N2(MAP, ARGS, ACC, START, Q_SEQUENCE_TO_GUIDE(SEQ))
#define Q_ITERATE_ACCWI_GUIDE_N2(MAP, ARGS, ACC, START, GUIDE) Q_ITGD_ACCWI_1_N2(MAP, ARGS, Q_ANALOG_0, ACC, START, GUIDE)
#define Q_ITGD_ACCWI_1_N2(MAP, ARGS, I, ACC, A, X, E) Q_ITGD_ACCWI_ELM_N2##E(MAP, ARGS, I, A, X) Q_ITGD_ACCWI_2_N2##E(MAP, ARGS, Q_INC_ANALOG(I), ACC, ACC(A),
#define Q_ITGD_ACCWI_2_N2(MAP, ARGS, I, ACC, A, X, E) Q_ITGD_ACCWI_ELM_N2##E(MAP, ARGS, I, A, X) Q_ITGD_ACCWI_1_N2##E(MAP, ARGS, Q_INC_ANALOG(I), ACC, ACC(A),
#define Q_ITGD_ACCWI_1_N2_END(...)
#define Q_ITGD_ACCWI_2_N2_END(...)

#pragma endregion

#pragma region Folding

// Same as iteration but repeatedly reducing arguments

#define Q_FOLD_SEQUENCE(FOLD, START, SEQ) Q_FOLD_GUIDE(FOLD, START, Q_SEQUENCE_TO_GUIDE(SEQ))
#define Q_FOLD_GUIDE(FOLD, START, GUIDE) Q_FLGD_1(FOLD, START, GUIDE)
#define Q_FLGD_ELM(FOLD, ARGS, V, X) Q_DEFER(FOLD) (ARGS, V, X)
#define Q_FLGD_ELM_END(FOLD, ARGS, V, X) V
#define Q_FLGD_1(FOLD, VAL, X, E) Q_FLGD_2##E(FOLD, Q_FLGD_ELM##E(FOLD, (), VAL, X),
#define Q_FLGD_2(FOLD, VAL, X, E) Q_FLGD_1##E(FOLD, Q_FLGD_ELM##E(FOLD, (), VAL, X),
#define Q_FLGD_1_END(FOLD, VAL, _) VAL
#define Q_FLGD_2_END(FOLD, VAL, _) VAL

#define Q_FOLD_W_SEQUENCE(FOLD, ARGS, START, SEQ) Q_FOLD_W_GUIDE(FOLD, ARGS, START, Q_SEQUENCE_TO_GUIDE(SEQ))
#define Q_FOLD_W_GUIDE(FOLD, ARGS, START, GUIDE) Q_FLGD_W_1(FOLD, ARGS, START, GUIDE)
#define Q_FLGD_W_1(FOLD, ARGS, VAL, X, E) Q_FLGD_W_2##E(FOLD, ARGS, Q_FLGD_ELM##E(FOLD, ARGS, VAL, X),
#define Q_FLGD_W_2(FOLD, ARGS, VAL, X, E) Q_FLGD_W_1##E(FOLD, ARGS, Q_FLGD_ELM##E(FOLD, ARGS, VAL, X),
#define Q_FLGD_W_1_END(FOLD, ARGS, VAL, _) VAL
#define Q_FLGD_W_2_END(FOLD, ARGS, VAL, _) VAL

#define Q_FOLD_I_SEQUENCE(FOLD, START, SEQ) Q_FOLD_I_GUIDE(FOLD, START, Q_SEQUENCE_TO_GUIDE(SEQ))
#define Q_FOLD_I_GUIDE(FOLD, START, GUIDE) Q_FLGD_I_1(FOLD, Q_ANALOG_0, START, GUIDE)
#define Q_FLGD_I_1(FOLD, I, VAL, X, E) Q_FLGD_I_2##E(FOLD, Q_INC_ANALOG(I), Q_FLGD_ELM##E(FOLD, (I), VAL, X),
#define Q_FLGD_I_2(FOLD, I, VAL, X, E) Q_FLGD_I_1##E(FOLD, Q_INC_ANALOG(I), Q_FLGD_ELM##E(FOLD, (I), VAL, X),
#define Q_FLGD_I_1_END(FOLD, I, VAL, _) VAL
#define Q_FLGD_I_2_END(FOLD, I, VAL, _) VAL

#define Q_FOLD_WI_SEQUENCE(FOLD, ARGS, START, SEQ) Q_FOLD_WI_GUIDE(FOLD, ARGS, START, Q_SEQUENCE_TO_GUIDE(SEQ))
#define Q_FOLD_WI_GUIDE(FOLD, ARGS, START, GUIDE) Q_FLGD_WI_1(FOLD, ARGS, Q_ANALOG_0, START, GUIDE)
#define Q_FLGD_WI_1(FOLD, ARGS, I, VAL, X, E) Q_FLGD_WI_2##E(FOLD, ARGS, Q_INC_ANALOG(I), Q_FLGD_ELM##E(FOLD, (ARGS, I), VAL, X),
#define Q_FLGD_WI_2(FOLD, ARGS, I, VAL, X, E) Q_FLGD_WI_1##E(FOLD, ARGS, Q_INC_ANALOG(I), Q_FLGD_ELM##E(FOLD, (ARGS, I), VAL, X),
#define Q_FLGD_WI_1_END(FOLD, ARGS, I, VAL, _) VAL
#define Q_FLGD_WI_2_END(FOLD, ARGS, I, VAL, _) VAL

#pragma endregion

#pragma region Generators

// generating sequences from other sequences
// like python generators but with macros

// Sequence IRange: 5 ->         (0)(1)(2)(3)(4)
// Sequence Range:  2, 5 ->      (2)(3)(4)
// Sequence KRange: 3, +2, *4 -> (3)(5)(7)(9)
#define Q_SEQRANGE_INTO(I) (Q_ANALOG_TO_NUM(I))
#define Q_SEQUENCE_IRANGE_ANALOG(N) Q_ITERATE_I_SEQUENCE(Q_SEQRANGE_INTO, Q_ANALOG_REPEAT(N, ()))
#define Q_SEQUENCE_IRANGE(N) Q_SEQUENCE_IRANGE_ANALOG(Q_NUM_TO_ANALOG(N))
#define Q_SEQUENCE_RANGE_ANALOG(MIN, MAX) Q_ITERATE_ACC_SEQUENCE(Q_SEQRANGE_INTO, Q_INC_ANALOG, MIN, Q_ANALOG_REPEAT(Q_SUB_ANALOG(MAX, MIN), ()))
#define Q_SEQUENCE_RANGE(MIN, MAX) Q_SEQUENCE_RANGE_ANALOG(Q_NUM_TO_ANALOG(MIN), Q_NUM_TO_ANALOG(MAX))
#define Q_SEQKRANGE_MOVE(K, RANGE, I) (RANGE(Q_ANALOG_TO_NUM(I)), Q_ADD_ANALOG(I, K))
#define Q_SEQKRANGE_FOLD(K, RDAT, _) Q_DEFER(Q_SEQKRANGE_MOVE)(K, Q_UNARY RDAT)
#define Q_SEQUENCE_KRANGE_ANALOG(START, SKIP, NUM) Q_UNARY(Q_ARGS_FIRST Q_FOLD_W_SEQUENCE(Q_SEQKRANGE_FOLD, SKIP, (, START), Q_ANALOG_REPEAT(NUM, ())))
#define Q_SEQUENCE_KRANGE(START, SKIP, NUM) Q_SEQUENCE_KRANGE_ANALOG(Q_NUM_TO_ANALOG(START), Q_NUM_TO_ANALOG(SKIP), Q_NUM_TO_ANALOG(NUM))

// Sequence Reverse: (a)(b)(c)(xyz)(123) -> (123)(xyz)(c)(b)(a)
#define Q_SEQREV_FOLD(_, RSEQ, X) X RSEQ
#define Q_SEQUENCE_REVERSE(SEQ) Q_FOLD_SEQUENCE(Q_SEQREV_FOLD, , SEQ)

// Sequence Repeat: (a)(b)(c), 3 -> (a)(b)(c)(a)(b)(c)(a)(b)(c)
#define Q_SEQUENCE_REPEAT_ANALOG(SEQ, N) Q_ANALOG_REPEAT(N, SEQ)
#define Q_SEQUENCE_REPEAT(SEQ, N) Q_ANALOG_REPEAT(Q_NUM_TO_ANALOG(N), SEQ)

// Sequence Fill: a, 6 -> (a)(a)(a)(a)(a)(a)
#define Q_SEQUENCE_FILL_ANALOG(VAL, N) Q_ANALOG_REPEAT(N, VAL)
#define Q_SEQUENCE_FILL(VAL, N) Q_ANALOG_REPEAT(Q_NUM_TO_ANALOG(N), VAL)

// Sequence Indexing
// first:      (a)(b)(c)(1)(2)(3)    -> (a)
// rest:       (a)(b)(c)(1)(2)(3)    -> (b)(c)(1)(2)(3)
// split:      (a)(b)(c)(1)(2)(3)    -> (a), (b)(c)(1)(2)(3)
// last:       (a)(b)(c)(1)(2)(3)    -> (3)
// init:       (a)(b)(c)(1)(2)(3)    -> (a)(b)(c)(1)(2)
// split_back: (a)(b)(c)(1)(2)(3)    -> (a)(b)(c)(1)(2), (3)
// at_n:       (a)(b)(c)(1)(2)(3), 2 -> (c)
// split_n:    (a)(b)(c)(1)(2)(3), 2 -> (a)(b), (c)(1)(2)(3)
// take_n:     (a)(b)(c)(1)(2)(3), 2 -> (a)(b)
// skip_n:     (a)(b)(c)(1)(2)(3), 2 -> (c)(1)(2)(3)
// rotate:     (a)(b)(c)(1)(2)(3), 2 -> (c)(1)(2)(3) (a)(b)

#define Q_SEQUENCE_SPLIT(SEQ) (Q_SEPERATE SEQ)
#define Q_SEQUENCE_FIRST(SEQ) Q_UNARY(Q_ARGS_FIRST Q_SEQUENCE_SPLIT(SEQ))
#define Q_SEQUENCE_REST(_, SEQ, _N) Q_EAT SEQ

#define Q_SEQSPLBK_FOLD(_, V, X) (Q_ARGS_FIRST V Q_ARGS_SECOND V, X)
#define Q_SEQUENCE_SPLIT_BACK(SEQ) Q_FOLD_SEQUENCE(Q_SEQSPLBK_FOLD, (,), SEQ)
#define Q_SEQUENCE_INIT(SEQ) Q_UNARY(Q_ARGS_FIRST Q_SEQUENCE_SPLIT_BACK(SEQ))
#define Q_SEQUENCE_LAST(SEQ) Q_UNARY(Q_ARGS_SECOND Q_SEQUENCE_SPLIT_BACK(SEQ))

#define Q_SEQSPLIT_FOLD(_, DAT, _N) (Q_ARGS_FIRST DAT Q_SEPERATE Q_ARGS_SECOND DAT)
#define Q_SEQUENCE_SPLIT_N_ANALOG(SEQ, N) Q_FOLD_SEQUENCE(Q_SEQSPLIT_FOLD, (, SEQ), Q_ANALOG_REPEAT(N, ()))
#define Q_SEQUENCE_SKIP_N_ANALOG(SEQ, N) Q_FOLD_SEQUENCE(Q_SEQUENCE_REST, SEQ, Q_ANALOG_REPEAT(N, ()))
#define Q_SEQUENCE_TAKE_N_ANALOG(SEQ, N) Q_UNARY(Q_ARGS_FIRST Q_SEQUENCE_SPLIT_N_ANALOG(SEQ, N))
#define Q_SEQUENCE_AT_N_ANALOG(SEQ, N) Q_SEQUENCE_FIRST(Q_SEQUENCE_SKIP_N_ANALOG(SEQ, N))
#define Q_SEQUENCE_SPLIT_N(SEQ, N) Q_SEQUENCE_SPLIT_N_ANALOG(SEQ, Q_NUM_TO_ANALOG(N))
#define Q_SEQUENCE_SKIP_N(SEQ, N)  Q_SEQUENCE_SKIP_N_ANALOG (SEQ, Q_NUM_TO_ANALOG(N))
#define Q_SEQUENCE_TAKE_N(SEQ, N)  Q_SEQUENCE_TAKE_N_ANALOG (SEQ, Q_NUM_TO_ANALOG(N))
#define Q_SEQUENCE_AT_N(SEQ, N)    Q_SEQUENCE_AT_N_ANALOG   (SEQ, Q_NUM_TO_ANALOG(N))

#define Q_SEQROT_SWAP(LEFT, RIGHT) RIGHT LEFT
#define Q_SEQUENCE_ROTATE_ANALOG(SEQ, N) Q_UNARY(Q_SEQLROT_SWAP Q_SEQUENCE_SPLIT_N_ANALOG(SEQ, N))
#define Q_SEQUENCE_ROTATE(SEQ, N) Q_SEQUENCE_ROTATE_ANALOG(SEQ, Q_NUM_TO_ANALOG(N))

// Sequence Enumerate: (a)(b)(c)(d)(e)(f) -> ((a), 0)((b), 1)((c), 2)((d), 3)((e), 4)((f), 5)
#define Q_SEQENUMC_IT(I, X) (X, I)
#define Q_SEQUENCE_ENUMERATE_ANALOG(SEQ) Q_ITERATE_I_SEQUENCE(Q_SEQENUMC_IT, SEQ)
#define Q_SEQENUMN_IT(I, X) (X, Q_ANALOG_TO_NUM(I))
#define Q_SEQUENCE_ENUMERATE(SEQ) Q_ITERATE_I_SEQUENCE(Q_SEQENUMN_IT, SEQ)

// Sequence Filter: (a)(b)(removeme)(c)(removeme)(d), (x) => x != removeme -> (a)(b)(c)(d)
#define Q_SEQFILT_IT(F, X) Q_IF(F X, X)
#define Q_SEQUENCE_FILTER(SEQ, FILTER) Q_ITERATE_W_SEQUENCE(Q_SEQFILT_IT, FILTER, SEQ)

// Sequence Flatten: ((a)(b))((c)(d)(e)(f))((g)) -> (a)(b) (c)(d)(e)(f) (g)
#define Q_SEQUENCE_FLATTEN(SEQ) Q_ITERATE_SEQUENCE(Q_REMOVE_SCOPE, SEQ)

// Sequence Zip: (a)(b)(c)(d), (A)(B)(C)(D) -> ((a), (A))((b), (B))((c), (C))((d), (D))
#define Q_SEQZIP_IT(BSEQ, X) (X, Q_SEQUENCE_FIRST(BSEQ))
#define Q_SEQZIP_ACC(BSEQ) Q_EAT BSEQ
#define Q_SEQUENCE_ZIP(ASEQ, BSEQ) Q_ITERATE_ACC_SEQUENCE(Q_SEQZIP_IT, Q_SEQZIP_ACC, BSEQ, ASEQ)
// Sequence Unzip: ((a), (A))((b), (B))((c), (C))((d), (D)) -> (a)(b)(c)(d), (A)(B)(C)(D)
#define Q_SEQUNZIP_FOLD(_, V, AB) (Q_ARGS_FIRST V Q_ARGS_FIRST AB, Q_ARGS_SECOND V Q_ARGS_SECOND AB)
#define Q_SEQUENCE_UNZIP(ZSEQ) Q_FOLD_SEQUENCE(Q_SEQUNZIP_FOLD, (,), ZSEQ)

// Sequence Join: (a)(b)(c)(d), +
// I[nitial]Join:  (a)(b)(c)(d), + -> + (a) + (b) + (c) + (d)
// T[railing]Join: (a)(b)(c)(d), + -> (a) + (b) + (c) + (d) +
// Join:           (a)(b)(c)(d), + -> (a) + (b) + (c) + (d)
#define Q_IJOIN(SEP, X) SEP X
#define Q_TJOIN(SEP, X) X SEP
#define Q_JOIN(SEP, V, X) V SEP X
#define Q_SEQUENCE_IJOIN(SEQ, SEP) Q_ITERATE_W_SEQUENCE(Q_IJOIN, SEP, SEQ)
#define Q_SEQUENCE_TJOIN(SEQ, SEP) Q_ITERATE_W_SEQUENCE(Q_TJOIN, SEP, SEQ)
#define Q_SEQUENCE_JOIN(SEQ, SEP)  Q_FOLD_W_SEQUENCE(Q_JOIN, SEP, Q_SEQUENCE_FIRST(SEQ), Q_SEQUENCE_REST(SEQ))


#pragma endregion

#pragma region Language Utilities
#define Q_DO(...) do { __VA_ARGS__ } while (false)
#define Q_IGNORE(...) (void)(__VA_ARGS__)
#define Q_NOOP() Q_IGNORE(0)

#define Q_TYPEOF_VAR_DECL(V) Quasi::Matching::TypeOfVar<void(*)(V)>

#define Q_STRLIT_LEN(STR) (sizeof(STR) - 1)
#define Q_ARRAY_LEN(ARR) (sizeof(ARR) / sizeof(ARR[0]))

#ifdef __COUNTER__
#define Q_COUNTER() __COUNTER__
#else
#error "__COUNTER__" Macro Not Supported!
#endif

#define Q_UNIQUE_ID(NAME) Q_CAT(NAME, Q_COUNTER())

#if defined(__GNUC__) || defined(__clang__)
#define Q_FUNC_NAME() __PRETTY_FUNCTION__
#else
#ifdef _MSVC
#define Q_FUNC_NAME() __FUNCSIG__
#else
#error "Function Signature macro not supported"
#endif
#endif
#pragma endregion