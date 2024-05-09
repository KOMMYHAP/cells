#include "setup_script_errors.h"

namespace {

class SetupScriptErrorCategory : public std::error_category {
public:
    const char* name() const noexcept override;
    std::string message(int ev) const override;
};

const char* SetupScriptErrorCategory::name() const noexcept
{
    return "setup script";
}

std::string SetupScriptErrorCategory::message(int ev) const
{
    switch (static_cast<SetupScriptErrors>(ev)) {
    case SetupScriptErrors::MissingArgumentFont:
        return "--font argument is missing";
    case SetupScriptErrors::MissingArgumentShader:
        return "--fragment-shader argument is missing";
    case SetupScriptErrors::InvalidFont:
        return "specified font is invalid";
    case SetupScriptErrors::InvalidShader:
        return "specified fragment shader is invalid";
    default:
        UNREACHABLE("Unknown error code!", ev);
    }
}

const SetupScriptErrorCategory SetupScriptErrorCategory {};

}
std::error_code make_error_code(SetupScriptErrors error)
{
    return std::error_code(static_cast<int>(error), SetupScriptErrorCategory);
}
