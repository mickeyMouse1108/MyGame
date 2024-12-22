#include "MTLMaterialLoader.h"
#include <ranges>

#include "OBJModel.h"

namespace Quasi::Graphics {
    void MTLMaterialLoader::Load(Str string) {
        ParseProperties(string);
        CreateMaterials();
    }

    void MTLMaterialLoader::ParseProperty(Str line) {
        const usize spaceIdx = line.find_first_of(' ');
        const Str prefix = line.substr(0, spaceIdx),
                  data   = line.substr(spaceIdx + 1);

        MTLProperty prop;
        Qmatch$ (prefix, (
            case ("newmtl") prop.Set(NewMaterial { String(data) });,
            case ("Ka")     prop.Set(AmbientCol  { Math::fVector3::parse(data, " ", "", "").UnwrapOr(Math::fVector3 {}).to_color3() });,
            case ("Kd")     prop.Set(DiffuseCol  { Math::fVector3::parse(data, " ", "", "").UnwrapOr(Math::fVector3 {}).to_color3() });,
            case ("Ks")     prop.Set(SpecularCol { Math::fVector3::parse(data, " ", "", "").UnwrapOr(Math::fVector3 {}).to_color3() });,
            case ("Ke")     prop.Set(EmissiveCol { Math::fVector3::parse(data, " ", "", "").UnwrapOr(Math::fVector3 {}).to_color3() });,
            case ("Ns")     prop.Set(SpecularExp { Text::Parse<float>(data).UnwrapOr(NAN) });,
            case ("Ni")     prop.Set(OpticalDen  { Text::Parse<float>(data).UnwrapOr(NAN) });,
            case ("d")      prop.Set(Dissolve    { Text::Parse<float>(data).UnwrapOr(NAN) });,
            case ("illum")  prop.Set(IlluminationType { Text::Parse<u32>(data).UnwrapOr(-1) });
        ))
        if (!prop.Is<Empty>())
            properties.Push(std::move(prop));
    }

    void MTLMaterialLoader::ParseProperties(Str string) {
        using namespace std::literals;
        for (const auto line : std::views::split(string, "\n"sv)) {
            ParseProperty(Str { line.begin(), line.end() });
        }
    }

    void MTLMaterialLoader::CreateMaterial(Span<const MTLProperty> matprop) {
        if (matprop.IsEmpty() || !matprop[0].Is<NewMaterial>()) return;

        materials.Push({});
        MTLMaterial& mat = materials.Last();
        mat.name = matprop[0].As<NewMaterial>()->name;

        for (const MTLProperty& prop : matprop.Skip(1)) {
            prop.Visit(
                [&] (const AmbientCol&       x) { mat.Ka    = x.color;    },
                [&] (const DiffuseCol&       x) { mat.Kd    = x.color;    },
                [&] (const SpecularCol&      x) { mat.Ks    = x.color;    },
                [&] (const EmissiveCol&      x) { mat.Ke    = x.color;    },
                [&] (const SpecularExp&      x) { mat.Ns    = x.exp;      },
                [&] (const OpticalDen&       x) { mat.Ni    = x.density;  },
                [&] (const Dissolve&         x) { mat.d     = x.dissolve; },
                [&] (const IlluminationType& x) { mat.illum = x.itype;    },
                [] (const auto&) {}
            );
        }
    }

    void MTLMaterialLoader::CreateMaterials() {
        u32 lastMat = 0;
        for (u32 i = 1; i < properties.Length(); ++i) {
            if (!properties[i].Is<NewMaterial>()) continue;
            Span<const MTLProperty> mat = properties.Subspan(lastMat, i - 1 - lastMat);
            lastMat = i;
            CreateMaterial(mat);
        }
        CreateMaterial(properties.Skip(lastMat));
    }

    String MTLMaterialLoader::DebugStr() const {
        String ss {};
        for (const MTLProperty& p : properties) {
            p.Visit(
                [&] (const NewMaterial&      x) { ss += "newmtl: ";       ss += x.name;                     },
                [&] (const AmbientCol&       x) { ss += "ambient: ";      ss += x.color.hexcode();          },
                [&] (const DiffuseCol&       x) { ss += "diffuse: ";      ss += x.color.hexcode();          },
                [&] (const SpecularCol&      x) { ss += "specular: ";     ss += x.color.hexcode();          },
                [&] (const EmissiveCol&      x) { ss += "emmisive: ";     ss += x.color.hexcode();          },
                [&] (const SpecularExp&      x) { ss += "specular exp: "; ss += std::to_string(x.exp);      },
                [&] (const OpticalDen&       x) { ss += "optical den: ";  ss += std::to_string(x.density);  },
                [&] (const Dissolve&         x) { ss += "dissolve: ";     ss += std::to_string(x.dissolve); },
                [&] (const IlluminationType& x) { ss += "illumtype: ";    ss += std::to_string(x.itype);    },
                [] (const auto&) {}
            );
        }
        return ss;
    }
}
