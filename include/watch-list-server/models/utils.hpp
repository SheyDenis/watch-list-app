/**
 * @file models-utils.hpp
 * @author denis
 * @brief
 * @date 12024-01-29
 *
 */

#ifndef MODELS_UTILS_HPP_
#define MODELS_UTILS_HPP_

#include <chrono>
#include <string>

namespace watch_list_app::server::models {

using Property = char const*;
using SecureString = std::string;  // TODO - Make zero-able on destruction and encrypted.
using Timestamp = std::chrono::time_point<std::chrono::system_clock>;

}  // namespace watch_list_app::server::models

#endif  // MODELS_UTILS_HPP_
