/**
 * @file base-model.hpp
 * @author denis
 * @brief
 * @date 12024-01-18
 *
 */

#ifndef BASE_MODEL_HPP_
#define BASE_MODEL_HPP_

#include <rapidjson/document.h>
#include <rapidjson/schema.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include "watch-list-server/dal/database.hpp"
#include "watch-list-server/models/utils.hpp"
#include "watch-list-server/server-error.hpp"
#include "watch-list-server/server-generic-error.hpp"

namespace watch_list_app::server::models {

template <class T>
struct ModelTraits {
  using ModelType = T;

  static constexpr char const* model_name = T::model_name_;
};

class BaseModel {
 public:
  static Property constexpr CREATE_TIME = "create_time";
  static Property constexpr MODIFY_TIME = "modify_time";

 private:
  Timestamp create_time_;
  Timestamp modify_time_;

 protected:
  static constexpr char const* model_name_ = nullptr;

  //  static std::unique_ptr<rapidjson::SchemaDocument> const schema_document_;
  //  static std::unique_ptr<rapidjson::SchemaValidator> const schema_validator_;

  virtual void serialize_impl(rapidjson::Value& output, rapidjson::Value::AllocatorType& allocator) const = 0;
  [[nodiscard]] virtual bool deserialize_impl(rapidjson::Value const& data) = 0;

 public:
  //  static std::unique_ptr<rapidjson::SchemaDocument> create_schema_document(rapidjson::Document const& json_schema) {
  //    return std::make_unique<rapidjson::SchemaDocument>(json_schema);
  //  }

  //  static std::unique_ptr<rapidjson::SchemaValidator> create_schema_validator(rapidjson::SchemaDocument const& schema_document) {
  //    return std::make_unique<rapidjson::SchemaValidator>(schema_document);
  //  }

  BaseModel() = default;
  virtual ~BaseModel() = default;

  void serialize(rapidjson::Value& output, rapidjson::Value::AllocatorType& allocator) const;
  [[nodiscard]] bool deserialize(rapidjson::Value const& data);
  [[nodiscard]] virtual bool is_valid() const = 0;  // TODO - Document.

  template <class T>
  [[nodiscard]] static bool find(std::string const& uuid, T& out, bool& found) {
    auto data = dal::Database::instance()->find(ModelTraits<T>::model_name, uuid);
    if (std::holds_alternative<ServerGenericError>(data)) {
      throw std::runtime_error(format_error(std::get<ServerGenericError>(data)));
    }

    auto& json_data = std::get<rapidjson::Value>(data);

    // TODO - Check valid with `ModelTraits<T>::schema_validator`.
    //    if (!json_data.Accept(ModelTraits<T>::schema_validator)) {
    //      throw std::runtime_error(
    //          format_error(ServerGenericError(fmt::format("Loaded model [{}] is not valid [{}]",
    //                                                      ModelTraits<T>::model_name,
    //                                                      json::JSONUtils::dump(ModelTraits<T>::schema_validator.GetError())))));
    //    }

    if (json_data.IsNull()) {
      found = false;
      return false;
    }
    found = true;
    return out.deserialize(json_data);
  }

  template <class T>
  [[nodiscard]] static bool scan(std::vector<T>& out) {
    auto data = dal::Database::instance()->scan(ModelTraits<T>::model_name);
    if (std::holds_alternative<ServerGenericError>(data)) {
      throw std::runtime_error(format_error(std::get<ServerGenericError>(data)));
    }

    auto& json_data = std::get<rapidjson::Document>(data);

    for (auto const& itr : json_data.GetArray()) {
      T v;
      if (!v.deserialize(itr)) {
        return false;
      }
      out.insert(out.cend(), v);
    }

    // TODO - Check valid with `ModelTraits<T>::schema_validator`.
    //    if (!json_data.Accept(ModelTraits<T>::schema_validator)) {
    //      throw std::runtime_error(
    //          format_error(ServerGenericError(fmt::format("Loaded model [{}] is not valid [{}]",
    //                                                      ModelTraits<T>::model_name,
    //                                                      json::JSONUtils::dump(ModelTraits<T>::schema_validator.GetError())))));
    //    }

    return true;
  }
};

}  // namespace watch_list_app::server::models

#endif  // BASE_MODEL_HPP_
