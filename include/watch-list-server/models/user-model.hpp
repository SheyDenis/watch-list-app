/**
 * @file user-model.hpp
 * @author denis
 * @brief
 * @date 12024-01-18
 *
 */

#ifndef USER_MODEL_HPP_
#define USER_MODEL_HPP_

#include <rapidjson/document.h>
#include <rapidjson/schema.h>
#include <uuid.h>

#include <initializer_list>
#include <set>
#include <string>

#include "watch-list-server/models/base-model.hpp"
#include "watch-list-server/models/model-traits.hpp"
#include "watch-list-server/models/utils.hpp"

namespace watch_list_app::server::models {

class UserModel final : public BaseModel {
 private:
  using PasswdHash = SecureString;  // TODO - Make secure and stuff, add salt?

 private:
  uuids::uuid uuid_;
  std::string name_;
  std::string email_;
  SecureString passwd_hash_;
  std::set<std::string> roles_;

 private:
  static PasswdHash hash_passwd(SecureString const& passwd);

 public:
  [[nodiscard]] static bool is_valid_email_address(std::string const& value);
  [[nodiscard]] static bool is_valid_password(SecureString const& value);

  UserModel();
  ~UserModel() override = default;

  [[nodiscard]] uuids::uuid const& get_uuid() const;
  [[nodiscard]] std::string const& get_name() const;
  [[nodiscard]] std::string const& get_email() const;
  [[nodiscard]] std::set<std::string> const& get_roles() const;
  void set_name(std::string name);
  bool set_email(std::string email);
  bool set_passwd(SecureString const& passwd);
  void add_roles(std::initializer_list<std::string> roles);
  void remove_roles(std::initializer_list<std::string> roles);
  [[nodiscard]] bool check_passwd(SecureString const& passwd) const;

  void serialize_impl(rapidjson::Value& output, rapidjson::Value::AllocatorType& allocator) const override;
  [[nodiscard]] bool deserialize_impl(rapidjson::Value const& data) override;
};
template bool BaseModel::find<UserModel>(std::string const& uuid, UserModel& out, bool& found);
template bool BaseModel::scan<UserModel>(std::vector<UserModel>& out);

}  // namespace watch_list_app::server::models

#endif  // USER_MODEL_HPP_
