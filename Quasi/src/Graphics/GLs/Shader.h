﻿#pragma once

#include <Utils/Macros.h>

#include "Utils/Enum.h"
#include "Utils/Array.h"
#include "Utils/String.h"
#include "Utils/StringList.h"
#include "Utils/CStr.h"
#include "GLObject.h"
#include "Math/Matrix.h"
#include "Math/Color.h"

namespace Quasi::Graphics {
    struct ShaderTypeData {
        u32 glID;
        Str shaderName;

        QDefineEnum$(ShaderType,
            (VERTEX,   (0x8B31, "Vertex"))
            (FRAGMENT, (0x8B30, "Fragment"))
            (GEOMETRY, (0x8DD9, "Geometry"))
            // all below requires OpenGL 4.3 above
            (COMPUTE,         (0x91B9, "Compute"))
            (TESS_CONTROL,    (0x8E88, "Tesselation Control"))
            (TESS_EVALUATION, (0x8E87, "Tesselation Eval")),
        NONE)
    };

#define NAME_INT I
#define NAME_UINT UI
#define NAME_FLOAT F
#define ONE_1 1
#define ONE_2 0
#define ONE_3 0
#define ONE_4 0
#define MAKE_UNIFORM_BITMASK(T, R, M, N) (T << 7) | (R << 6) | (M << 3) | N
#define DEFINE_UNIF(T, M, N, R) \
    Q_IF_ELSE(Q_CAT(ONE_, M), (Q_CAT(Q_CAT(UNIF_##N, Q_CAT(NAME_, T)), Q_IF(R, _ARR))), (UNIF_MAT##M##x##N)) = \
    Q_CAT(T, _FLAG) | ROWS_##M | COLS_##N Q_IF(R, | ARRAY_FLAG)
#define DEFINE_WHOLE_UNIF(T) \
    DEFINE_UNIF(T, 1, 1, 0), DEFINE_UNIF(T, 1, 2, 0), DEFINE_UNIF(T, 1, 3, 0), DEFINE_UNIF(T, 1, 4, 0), \
    DEFINE_UNIF(T, 1, 1, 1), DEFINE_UNIF(T, 1, 2, 1), DEFINE_UNIF(T, 1, 3, 1), DEFINE_UNIF(T, 1, 4, 1)
    enum ShaderUniformType {
        // layout of this type: TT_R_MMM_NNN
        // T: type of data, R: bool value, true if is array
        // MxN: matrix part of MatrixMxN, otherwise N is vector dimension
        TYPE_MASK   = MAKE_UNIFORM_BITMASK(0b11, 0, 0, 0),
        INT_FLAG    = MAKE_UNIFORM_BITMASK(2, 0, 0, 0),
        UINT_FLAG   = MAKE_UNIFORM_BITMASK(3, 0, 0, 0),
        FLOAT_FLAG  = MAKE_UNIFORM_BITMASK(1, 0, 0, 0),

        ARRAY_FLAG  = MAKE_UNIFORM_BITMASK(0, 0b1, 0, 0),

        ROWS_MASK   = MAKE_UNIFORM_BITMASK(0, 0, 0b111, 0),
        COLS_MASK   = MAKE_UNIFORM_BITMASK(0, 0, 0, 0b111),

        VECTOR_FLAG = MAKE_UNIFORM_BITMASK(0, 0, 0b001, 0),
        ROWS_1      = MAKE_UNIFORM_BITMASK(0, 0, 1, 0),
        ROWS_2      = MAKE_UNIFORM_BITMASK(0, 0, 2, 0),
        ROWS_3      = MAKE_UNIFORM_BITMASK(0, 0, 3, 0),
        ROWS_4      = MAKE_UNIFORM_BITMASK(0, 0, 4, 0),

        SINGLE_FLAG = MAKE_UNIFORM_BITMASK(0, 0, 0, 0b001),
        COLS_1      = MAKE_UNIFORM_BITMASK(0, 0, 0, 1),
        COLS_2      = MAKE_UNIFORM_BITMASK(0, 0, 0, 2),
        COLS_3      = MAKE_UNIFORM_BITMASK(0, 0, 0, 3),
        COLS_4      = MAKE_UNIFORM_BITMASK(0, 0, 0, 4),

        DEFINE_WHOLE_UNIF(INT),
        DEFINE_WHOLE_UNIF(UINT),
        DEFINE_WHOLE_UNIF(FLOAT),

        DEFINE_UNIF(FLOAT, 2, 2, 1), DEFINE_UNIF(FLOAT, 2, 3, 1), DEFINE_UNIF(FLOAT, 2, 4, 1),
        DEFINE_UNIF(FLOAT, 3, 2, 1), DEFINE_UNIF(FLOAT, 3, 3, 1), DEFINE_UNIF(FLOAT, 3, 4, 1),
        DEFINE_UNIF(FLOAT, 4, 2, 1), DEFINE_UNIF(FLOAT, 4, 3, 1), DEFINE_UNIF(FLOAT, 4, 4, 1),
    };
#undef NAME_INT
#undef NAME_UINT
#undef NAME_FLOAT
#undef ONE_1
#undef ONE_2
#undef ONE_3
#undef ONE_4
#undef MAKE_UNIFORM_BITMASK
#undef DEFINE_UNIF
#undef DEFINE_WHOLE_UNIF

    namespace details {
        template <class T> struct use_const_ref { using type = T; };

        template <ShaderUniformType U>
        auto impl() {
            constexpr auto TYPE = U & TYPE_MASK;
            constexpr bool ARRAY = (bool)(U & ARRAY_FLAG);
            constexpr u32 ROWS = (u32)((U & ROWS_MASK) / VECTOR_FLAG);
            constexpr u32 COLS = (u32)((U & COLS_MASK) / SINGLE_FLAG);

            using T = std::conditional_t<TYPE == INT_FLAG,   int,
                      std::conditional_t<TYPE == UINT_FLAG,  uint,
                      std::conditional_t<TYPE == FLOAT_FLAG, float, void>>>;

            if constexpr (ROWS > 1) {
                return Span<const Math::Matrix<ROWS, COLS>> {};
            } else if constexpr(COLS > 1) {
                if constexpr (ARRAY) return Span<const Math::VectorN<COLS, T>> {};
                else return use_const_ref<Math::VectorN<COLS, T>> {};
            } else {
                if constexpr (ARRAY) return Span<const T> {};
                else return T {};
            }
        }

        template <class T> struct arg { using type = T; };
        template <class T> struct arg<use_const_ref<T>> { using type = const T&; };
    }

    template <ShaderUniformType U>
    using ShaderUniformArgOf = typename details::arg<decltype(details::impl<U>())>::type;

    template <class T>
    constexpr ShaderUniformType ConvertUniformType =
        std::is_same_v<T, int>   ? INT_FLAG   :
        std::is_same_v<T, uint>  ? UINT_FLAG  :
        std::is_same_v<T, float> ? FLOAT_FLAG :
        (ShaderUniformType)0;

    struct ShaderProgramSource {
        String fullSource;
        usize sepPoints[2]; // vertex | fragment | geometery, all seperated with index.

    public:
        usize GetSepPoint(int idx) const {
            return idx < 0 ? 0 : idx >= 2 ? fullSource.Length() : sepPoints[idx];
        }

        Str GetShader(ShaderType type) const {
            const usize beg = GetSepPoint(type.Ord() - 1);
            return fullSource.Substr(beg, GetSepPoint(type.Ord()) - beg);
        }
    };

    struct ShaderArgs;
    struct ShaderValueVariant;
    struct ShaderParameter;

    class Shader : public GLObject<Shader> {
        Map<String, int, std::less<>> uniformCache;

        explicit Shader(GraphicsID id);
    public:
        Shader() = default;
        static Shader New(Str program);
        static Shader New(Str vert, Str frag, Str geom = {});
        static void DestroyObject(GraphicsID id);
        static void BindObject(GraphicsID id);
        static void UnbindObject();

        template <ShaderUniformType U>
        void SetUniformAtLoc(int uniformLoc, ShaderUniformArgOf<U> val) = delete;
        template <ShaderUniformType U>
        void SetUniformOf(Str name, ShaderUniformArgOf<U> val) { SetUniformAtLoc<U>(GetUniformLocation(name), val); }

#pragma region Uniforms
#define UNIF_INSTANTIATE \
        DEFINE_UNIF_FN(Int,      1I)      DEFINE_UNIF_FN(Ivec2,    2I)      DEFINE_UNIF_FN(Ivec3,    3I)      DEFINE_UNIF_FN(Ivec4, 4I) \
        DEFINE_UNIF_FN(Uint,     1UI)     DEFINE_UNIF_FN(Uvec2,    2UI)     DEFINE_UNIF_FN(Uvec3,    3UI)     DEFINE_UNIF_FN(Uvec4, 4UI) \
        DEFINE_UNIF_FN(Float,    1F)      DEFINE_UNIF_FN(Fvec2,    2F)      DEFINE_UNIF_FN(Fvec3,    3F)      DEFINE_UNIF_FN(Fvec4, 4F) \
        DEFINE_UNIF_FN(IntArr,   1I_ARR)  DEFINE_UNIF_FN(Ivec2Arr, 2I_ARR)  DEFINE_UNIF_FN(Ivec3Arr, 3I_ARR)  DEFINE_UNIF_FN(Ivec4Arr, 4I_ARR) \
        DEFINE_UNIF_FN(UintArr,  1UI_ARR) DEFINE_UNIF_FN(Uvec2Arr, 2UI_ARR) DEFINE_UNIF_FN(Uvec3Arr, 3UI_ARR) DEFINE_UNIF_FN(Uvec4Arr, 4UI_ARR) \
        DEFINE_UNIF_FN(FloatArr, 1F_ARR)  DEFINE_UNIF_FN(Fvec2Arr, 2F_ARR)  DEFINE_UNIF_FN(Fvec3Arr, 3F_ARR)  DEFINE_UNIF_FN(Fvec4Arr, 4F_ARR) \
        \
        DEFINE_UNIF_FN(Mat2x2, MAT2x2) DEFINE_UNIF_FN(Mat2x3, MAT2x3) DEFINE_UNIF_FN(Mat2x4, MAT2x4) \
        DEFINE_UNIF_FN(Mat3x2, MAT3x2) DEFINE_UNIF_FN(Mat3x3, MAT3x3) DEFINE_UNIF_FN(Mat3x4, MAT3x4) \
        DEFINE_UNIF_FN(Mat4x2, MAT4x2) DEFINE_UNIF_FN(Mat4x3, MAT4x3) DEFINE_UNIF_FN(Mat4x4, MAT4x4) \

#define DEFINE_UNIF_FN(N, IN) \
    void SetUniform##N(Str name, ShaderUniformArgOf<UNIF_##IN> val) { \
        SetUniformOf<UNIF_##IN>(name, val); \
    }

        UNIF_INSTANTIATE

        void SetUniformDyn(const ShaderParameter& arg);
        void SetUniformArgs(const ShaderArgs& args);

        void SetUniformTex(Str name, const class Texture& texture);

        template <class N>
        void SetUniform(Str name, N num) {
            using enum ShaderUniformType;
            SetUniformOf<ConvertUniformType<N> | VECTOR_FLAG | SINGLE_FLAG>(name, num);
        }

        template <u32 N, class T>
        void SetUniform(Str name, const Math::VectorN<N, T>& vec) {
            using enum ShaderUniformType;
            SetUniformOf<ConvertUniformType<T> | VECTOR_FLAG | (COLS_1 * N)>(name, vec);
        }

        template <class T>
        void SetUniform(Str name, Span<const T> val) {
            using enum ShaderUniformType;
            SetUniformOf<ConvertUniformType<T> | VECTOR_FLAG | SINGLE_FLAG | ARRAY_FLAG>(name, val);
        }

        template <u32 N, class T>
        void SetUniform(Str name, Span<const Math::VectorN<N, T>> val) {
            using enum ShaderUniformType;
            SetUniformOf<ConvertUniformType<T> | VECTOR_FLAG | (COLS_1 * N) | ARRAY_FLAG>(name, val);
        }

        template <u32 N, u32 M>
        void SetUniform(Str name, const Math::Matrix<N, M>& val) {
            using enum ShaderUniformType;
            SetUniformOf<FLOAT_FLAG | (ROWS_1 * N) | (COLS_1 * M) | ARRAY_FLAG>(name, val.data());
        }

        void SetUniformMat2x2(Str name, const Math::Matrix2x2& mat) { SetUniformMat2x2(name, Span<const Math::Matrix2x2>::Single(mat)); }
        void SetUniformMat2x3(Str name, const Math::Matrix2x3& mat) { SetUniformMat2x3(name, Span<const Math::Matrix2x3>::Single(mat)); }
        void SetUniformMat2x4(Str name, const Math::Matrix2x4& mat) { SetUniformMat2x4(name, Span<const Math::Matrix2x4>::Single(mat)); }
        void SetUniformMat3x2(Str name, const Math::Matrix3x2& mat) { SetUniformMat3x2(name, Span<const Math::Matrix3x2>::Single(mat)); }
        void SetUniformMat3x3(Str name, const Math::Matrix3x3& mat) { SetUniformMat3x3(name, Span<const Math::Matrix3x3>::Single(mat)); }
        void SetUniformMat3x4(Str name, const Math::Matrix3x4& mat) { SetUniformMat3x4(name, Span<const Math::Matrix3x4>::Single(mat)); }
        void SetUniformMat4x2(Str name, const Math::Matrix4x2& mat) { SetUniformMat4x2(name, Span<const Math::Matrix4x2>::Single(mat)); }
        void SetUniformMat4x3(Str name, const Math::Matrix4x3& mat) { SetUniformMat4x3(name, Span<const Math::Matrix4x3>::Single(mat)); }
        void SetUniformMat4x4(Str name, const Math::Matrix4x4& mat) { SetUniformMat4x4(name, Span<const Math::Matrix4x4>::Single(mat)); }
#pragma endregion

#pragma region Shader Sources
#define Q_GLSL_SHADER(VERSION, V, F) "#shader vertex\n" "#version " #VERSION " core\n" Q_TOSTR(Q_REMOVE_SCOPE(V)) "\n#shader fragment\n" "#version " #VERSION " core\n" Q_TOSTR(Q_REMOVE_SCOPE(F))

        static constexpr Str StdColored =
            Q_GLSL_SHADER(330,
                (
                    layout(location = 0) in vec4 position;
                    layout(location = 1) in vec4 color;
                    out vec4 v_color;
                    uniform mat4 u_projection;
                    uniform mat4 u_view;
                    void main() {
                        gl_Position = u_projection * u_view * position;
                        v_color = color;
                    }
                ),
                (
                    layout(location = 0) out vec4 color;
                    in vec4 v_color;
                    void main() {
                        color = v_color;
                    }
                )
            );

        static constexpr Str StdTextured =
            Q_GLSL_SHADER(330,
                (
                    layout(location = 0) in vec4 position;
                    layout(location = 1) in vec4 color;
                    layout(location = 2) in vec2 texCoord;
                    out vec2 v_TexCoord;
                    out vec4 v_color;
                    uniform mat4 u_projection;
                    uniform mat4 u_view;
                    void main() {
                        gl_Position = u_projection * u_view * position;
                        v_color = color;
                        v_TexCoord = texCoord;
                    }
                ),
                (
                    layout(location = 0) out vec4 color;
                    in vec2 v_TexCoord;
                    in vec4 v_color;
                    uniform sampler2D u_Texture;
                    void main() {
                        vec4 texColor = texture(u_Texture, v_TexCoord);
                        color = v_color * texColor;
                    }
                )
            );
#pragma endregion // Shader Sources

        static Shader FromFile(CStr filepath);
        static Shader FromFile(CStr vert, CStr frag, CStr geom = {});

    private:
        int GetUniformLocation(Str name);
        static ShaderProgramSource ParseShader  (Str program);
        static ShaderProgramSource ParseFromFile(CStr filepath);
        static GraphicsID CompileShader    (Str source, ShaderType type);
        static GraphicsID CompileShaderVert(Str source) { return CompileShader(source, ShaderType::VERTEX); }
        static GraphicsID CompileShaderFrag(Str source) { return CompileShader(source, ShaderType::FRAGMENT); }
        static GraphicsID CompileShaderGeom(Str source) { return CompileShader(source, ShaderType::GEOMETRY); }
        static GraphicsID CreateShader(Str vtx, Str frg, Str geo = {});

        friend class GraphicsDevice;
    };

#undef DEFINE_UNIF_FN
#define DEFINE_UNIF_FN(N, IN) template <> void Shader::SetUniformAtLoc<UNIF_##IN>(int uniformLoc, ShaderUniformArgOf<UNIF_##IN> val);
    UNIF_INSTANTIATE
#undef DEFINE_UNIF_FN
#undef UNIF_INSTANTIATE

    struct ShaderParameter;
    struct ShaderArgsIter;

    namespace details {
        template <class> struct as_impl {};
    }

    struct ShaderValueVariant {
        usize data; // either int, uint, float, or their ptr variants
        ShaderUniformType type;
        u32 size;

    private:
        ShaderValueVariant(usize d, ShaderUniformType t, u32 s) : data(d), type(t), size(s) {}
        ShaderValueVariant(const void* d, ShaderUniformType t, u32 s) : data(Memory::Transmute<usize>(d)), type(t), size(s) {}
    public:
#pragma region Init
        ShaderValueVariant(bool  num) : ShaderValueVariant(num, UNIF_1I, 1) {}
        template <class T> ShaderValueVariant(T* t) = delete;
        ShaderValueVariant(const ContinuousCollectionAny auto& coll) : ShaderValueVariant(coll.AsSpan()) {}
        ShaderValueVariant(int   num) : ShaderValueVariant(Memory::Transmute<u32>(num), UNIF_1I,  1) {}
        ShaderValueVariant(uint  num) : ShaderValueVariant(Memory::Transmute<u32>(num), UNIF_1UI, 1) {}
        ShaderValueVariant(float num) : ShaderValueVariant(Memory::Transmute<u32>(num), UNIF_1F,  1) {}
        ShaderValueVariant(Span<const int>   vals) : ShaderValueVariant(vals.Data(), UNIF_1I_ARR,  vals.Length()) {}
        ShaderValueVariant(Span<const uint>  vals) : ShaderValueVariant(vals.Data(), UNIF_1UI_ARR, vals.Length()) {}
        ShaderValueVariant(Span<const float> vals) : ShaderValueVariant(vals.Data(), UNIF_1F_ARR,  vals.Length()) {}
        ShaderValueVariant(const Math::iVector2& vec) : ShaderValueVariant(&vec.x, UNIF_2I,  2) {}
        ShaderValueVariant(const Math::uVector2& vec) : ShaderValueVariant(&vec.x, UNIF_2UI, 2) {}
        ShaderValueVariant(const Math::fVector2& vec) : ShaderValueVariant(&vec.x, UNIF_2F,  2) {}
        ShaderValueVariant(const Math::iVector3& vec) : ShaderValueVariant(&vec.x, UNIF_3I,  3) {}
        ShaderValueVariant(const Math::uVector3& vec) : ShaderValueVariant(&vec.x, UNIF_3UI, 3) {}
        ShaderValueVariant(const Math::fVector3& vec) : ShaderValueVariant(&vec.x, UNIF_3F,  3) {}
        ShaderValueVariant(const Math::iVector4& vec) : ShaderValueVariant(&vec.x, UNIF_4I,  4) {}
        ShaderValueVariant(const Math::uVector4& vec) : ShaderValueVariant(&vec.x, UNIF_4UI, 4) {}
        ShaderValueVariant(const Math::fVector4& vec) : ShaderValueVariant(&vec.x, UNIF_4F,  4) {}
        ShaderValueVariant(Span<const Math::iVector2> vecs) : ShaderValueVariant(vecs.Data(), UNIF_2I_ARR,  2 * vecs.Length()) {}
        ShaderValueVariant(Span<const Math::uVector2> vecs) : ShaderValueVariant(vecs.Data(), UNIF_2UI_ARR, 2 * vecs.Length()) {}
        ShaderValueVariant(Span<const Math::fVector2> vecs) : ShaderValueVariant(vecs.Data(), UNIF_2F_ARR,  2 * vecs.Length()) {}
        ShaderValueVariant(Span<const Math::iVector3> vecs) : ShaderValueVariant(vecs.Data(), UNIF_3I_ARR,  3 * vecs.Length()) {}
        ShaderValueVariant(Span<const Math::uVector3> vecs) : ShaderValueVariant(vecs.Data(), UNIF_3UI_ARR, 3 * vecs.Length()) {}
        ShaderValueVariant(Span<const Math::fVector3> vecs) : ShaderValueVariant(vecs.Data(), UNIF_3F_ARR,  3 * vecs.Length()) {}
        ShaderValueVariant(Span<const Math::iVector4> vecs) : ShaderValueVariant(vecs.Data(), UNIF_4I_ARR,  4 * vecs.Length()) {}
        ShaderValueVariant(Span<const Math::uVector4> vecs) : ShaderValueVariant(vecs.Data(), UNIF_4UI_ARR, 4 * vecs.Length()) {}
        ShaderValueVariant(Span<const Math::fVector4> vecs) : ShaderValueVariant(vecs.Data(), UNIF_4F_ARR,  4 * vecs.Length()) {}
        ShaderValueVariant(const Math::fColor&  col) : ShaderValueVariant(col.begin(), UNIF_4F, 4) {}
        ShaderValueVariant(const Math::fColor3& col) : ShaderValueVariant(col.begin(), UNIF_3F, 3) {}
        ShaderValueVariant(Span<const Math::fColor>  cols) : ShaderValueVariant(cols.Data(), UNIF_4F_ARR, 4 * cols.Length()) {}
        ShaderValueVariant(Span<const Math::fColor3> cols) : ShaderValueVariant(cols.Data(), UNIF_3F_ARR, 3 * cols.Length()) {}
        template <u32 N, u32 M>
        ShaderValueVariant(const Math::Matrix<N, M>& mat) : ShaderValueVariant(mat.data().Data(), (ShaderUniformType)(FLOAT_FLAG | (ROWS_1 * N) | (COLS_1 * M) | ARRAY_FLAG), N * M) {}
        template <u32 N, u32 M>
        ShaderValueVariant(Span<const Math::Matrix<N, M>> mats) : ShaderValueVariant(mats.Data(), (ShaderUniformType)(FLOAT_FLAG | (ROWS_1 * N) | (COLS_1 * M) | ARRAY_FLAG), N * M * mats.Length()) {}
        ShaderValueVariant(const Texture& tex);
#pragma endregion
    public:
        template <class T> RemQual<T> as() const {
            if constexpr (requires (T maybeSpan) { { Span { maybeSpan } } -> SameAs<T>; }) {
                using Item = typename RemQual<T>::Item;
                return Spans::Slice(Memory::Transmute<const float*>(data), size).Transmute<const Item>();
            } else if constexpr (Numeric<T>) {
                return Memory::Transmute<RemQual<T>>((u32)data);
            } else if constexpr (IsPtr<T>) {
                return Memory::Transmute<RemQual<T>>(data);
            } else {
                return Memory::Transmute<RemQual<T>>(*Memory::Transmute<const Array<byte, sizeof(T)>*>(data));
            }
        }

        template <class T>
        bool is() const {
            using enum ShaderUniformType;
            const int typeId = (int)((type / (1 << 7)) * 2 + ((type & ARRAY_FLAG) / ARRAY_FLAG));
            constexpr int INT = 2, UINT = 4, FLOAT = 6, INTPTR = 3, UINTPTR = 5, FLOATPTR = 7;
            switch (typeId) {
                case INT:      return std::is_same_v<T, int>;
                case UINT:     return std::is_same_v<T, uint>;
                case FLOAT:    return std::is_same_v<T, float>;
                case INTPTR:   return std::is_same_v<T, const int*>;
                case UINTPTR:  return std::is_same_v<T, const uint*>;
                case FLOATPTR: return std::is_same_v<T, const float*>;
                default: return false;
            }
        }
    };

    struct ShaderArgs {
        StringList args;
        Vec<ShaderValueVariant> params;

        ShaderArgs() {}
        ShaderArgs(IList<ShaderParameter> p);

        usize size() const { return params.Length(); }

        ShaderArgs& then(Str name, ShaderValueVariant val) {
            args.Push(name);
            params.Push(val);
            return *this;
        }

        using Iter = ShaderArgsIter;

        Iter begin() const;
        IteratorEndMarker end() const { return IteratorEnd; }
    };

    struct ShaderParameter {
        Str name;
        ShaderValueVariant value;
    };

    struct ShaderArgsIter {
        BufferIterator<const ShaderValueVariant&> valIt;
        StringListIter argIt;

        ShaderParameter operator*() const {
            return { *argIt, *valIt };
        }

        ShaderArgsIter& operator++() { ++valIt; ++argIt; return *this; }
        bool operator==(const IteratorEndMarker& marker) const { return valIt == marker; }
        bool operator!=(const IteratorEndMarker& marker) const { return valIt != marker; }
    };

    inline ShaderArgs::Iter ShaderArgs::begin() const { return { .valIt = params.begin(), .argIt = args.begin() }; }
}
