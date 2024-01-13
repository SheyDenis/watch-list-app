/**
 * @file formatter-utils.hpp
 * @author denis
 * @brief
 * @date 12024-01-12
 *
 */

#ifndef FORMATTER_UTILS_HPP_
#define FORMATTER_UTILS_HPP_

#include <fmt/core.h>
#include <httplib/httplib.h>
#include <rapidjson/rapidjson.h>

#include <string>

template <>
struct fmt::formatter<rapidjson::Type> : fmt::formatter<std::string> {
  auto format(rapidjson::Type const& v, format_context& ctx) const -> decltype(ctx.out()) {
    std::string output;

    switch (v) {
      case rapidjson::kNullType:
        output.assign("null");
        break;
      case rapidjson::kFalseType:
      case rapidjson::kTrueType:
        output.assign("boolean");
        break;
      case rapidjson::kObjectType:
        output.assign("object");
        break;
      case rapidjson::kArrayType:
        output.assign("array");
        break;
      case rapidjson::kStringType:
        output.assign("string");
        break;
      case rapidjson::kNumberType:
        output.assign("number");
        break;
    }

    return format_to(ctx.out(), output);
  }
};

template <>
struct fmt::formatter<httplib::Params> : fmt::formatter<std::string> {
  auto format(httplib::Params const& v, format_context& ctx) const -> decltype(ctx.out()) {
    std::vector<std::string> output;

    for (auto const& itr : v) {
      output.insert(output.cend(), fmt::format("{}={}", itr.first, itr.second));
    }

    return format_to(ctx.out(), fmt::format("{}", fmt::join(output, ";")));
  }
};

#endif  // FORMATTER_UTILS_HPP_
