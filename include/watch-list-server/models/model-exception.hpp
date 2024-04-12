/**
 * @file model-exception.hpp
 * @author denis
 * @brief
 * @date 12024-02-17
 *
 */

#ifndef MODEL_EXCEPTION_HPP_
#define MODEL_EXCEPTION_HPP_

#include <fmt/core.h>
#include <rapidjson/document.h>

#include <stdexcept>
#include <string>

#include "watch-list-server/json/json-utils.hpp"

namespace watch_list_app::server::models {

class ModelException : public std::runtime_error {
 public:
  explicit ModelException(std::string const& msg) : std::runtime_error(msg) {}
  ~ModelException() override = default;
};

class InvalidModelException : public ModelException {
  /// @brief A type of error that indicates the JSON data provided, cannot be deserialize to the model.
 public:
  explicit InvalidModelException(char const* model_name)
      : ModelException(fmt::format("Found invalid model [{}] in the database", model_name)) {}
  InvalidModelException(char const* model_name, rapidjson::Value const& json_data)
      : ModelException(fmt::format("Found invalid model [{}] in the database [{}]", model_name, json::JSONUtils::dump(json_data))) {}
  ~InvalidModelException() override = default;
};

}  // namespace watch_list_app::server::models

#endif  // MODEL_EXCEPTION_HPP_
