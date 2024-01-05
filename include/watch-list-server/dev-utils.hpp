/**
 * @file dev-utils.hpp
 * @author denis
 * @brief
 * @date 12024-01-05
 *
 */

#ifndef DEV_UTILS_HPP_
#define DEV_UTILS_HPP_

namespace watch_list_app::server {

#define LOG_NOT_IMPLEMENTED() ServerConstants::root_logger().warn("Function [{}] is not implemented", __PRETTY_FUNCTION__)

}  // namespace watch_list_app::server

#endif  // DEV_UTILS_HPP_
