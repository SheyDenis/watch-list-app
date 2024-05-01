/**
 * @file genre-model.hpp
 * @author denis
 * @brief
 * @date 12024-02-09
 *
 */

#ifndef GENRE_MODEL_HPP_
#define GENRE_MODEL_HPP_

#include <rapidjson/document.h>

#include <string>

#include "watch-list-server/models/base-model.hpp"
#include "watch-list-server/models/utils.hpp"

namespace watch_list_app::server::models {

class GenreModel final : public BaseModel {

 protected:

  friend struct ModelTraits<GenreModel>;

 private:
  std::string name_;

 public:
  GenreModel();
  ~GenreModel() override = default;

  [[nodiscard]] std::string const& get_name() const;

  void serialize_impl(rapidjson::Value& output, rapidjson::Value::AllocatorType& allocator) const override;
  [[nodiscard]] bool deserialize_impl(rapidjson::Value const& data) override;
  [[nodiscard]] bool is_valid() const override;
};
template bool BaseModel::find<GenreModel>(std::string const& uuid, GenreModel& out, bool& found);
template bool BaseModel::scan<GenreModel>(std::vector<GenreModel>& out);

}  // namespace watch_list_app::server::models


#endif  // GENRE_MODEL_HPP_
