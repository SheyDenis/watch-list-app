/**
 * @file model-traits.hpp
 * @author denis
 * @brief
 * @date 12024-02-12
 *
 */

#ifndef MODEL_TRAITS_HPP_
#define MODEL_TRAITS_HPP_

namespace watch_list_app::server::models {

template <class T>
struct ModelTypes;

template <class T>
struct ModelTraits {
  using ModelType = T;

  static constexpr char const* model_name = ModelTypes<T>::model_name;
  static constexpr char const* table_name = ModelTypes<T>::table_name;
};

class UserModel;
template <>
struct ModelTypes<UserModel> {
  static constexpr char const* model_name = "user";
  static constexpr char const* table_name = "user";
};

}  // namespace watch_list_app::server::models

#endif  // MODEL_TRAITS_HPP_
