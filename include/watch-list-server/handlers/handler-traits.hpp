/**
 * @file handler-traits.hpp
 * @author denis
 * @brief
 * @date 12024-01-06
 *
 */

#ifndef HANDLER_TRAITS_HPP_
#define HANDLER_TRAITS_HPP_

#include <type_traits>

namespace watch_list_app::server {

template <typename T>
struct HandlerTypes {
  static constexpr char const* resource_pattern = nullptr;

  static bool constexpr handle_delete = false;
  static bool constexpr handle_get = false;
  static bool constexpr handle_patch = false;
  static bool constexpr handle_post = false;
  static bool constexpr handle_put = false;
};

template <typename T>
struct HandlerTraits {
  typedef T HandlerType;

  static constexpr char const* resource_pattern = HandlerTypes<T>::resource_pattern;

  static bool constexpr handle_delete = HandlerTypes<T>::handle_delete;
  static bool constexpr handle_get = HandlerTypes<T>::handle_get;
  static bool constexpr handle_patch = HandlerTypes<T>::handle_patch;
  static bool constexpr handle_post = HandlerTypes<T>::handle_post;
  static bool constexpr handle_put = HandlerTypes<T>::handle_put;

  static_assert(std::bool_constant<handle_delete>::value || std::bool_constant<handle_get>::value ||
                    std::bool_constant<handle_patch>::value || std::bool_constant<handle_post>::value ||
                    std::bool_constant<handle_put>::value,
                "Handler does not handle any method");
};

}  // namespace watch_list_app::server

#endif  // HANDLER_TRAITS_HPP_
