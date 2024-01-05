/**
 * @file json-utils.hpp
 * @author denis
 * @brief
 * @date 12024-01-05
 *
 */

#ifndef JSON_UTILS_HPP_
#define JSON_UTILS_HPP_

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/reader.h>
#include <rapidjson/stream.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <fstream>
#include <iostream>

#include "watch-list-server/server-generic-error.hpp"

namespace watch_list_app::server::json {

class JSONUtils {
 public:
  [[nodiscard]] static ServerGenericErrorVariant<rapidjson::Document> read_file(std::string const& file_path) {
    std::ifstream ifs(file_path);
    if (!ifs.is_open()) {
      return ServerGenericError(fmt::format("Could not open file for reading [{}]", file_path));
    }
    rapidjson::IStreamWrapper isw{ifs};

    rapidjson::Document doc;
    doc.ParseStream(isw);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer{buffer};
    doc.Accept(writer);

    if (doc.HasParseError()) {
      return ServerGenericError(
          fmt::format("Failed to parse file [{}}] [err={}]", file_path, rapidjson::GetParseError_En(doc.GetParseError())));
    }

    return doc;
  }
};

}  // namespace watch_list_app::server::json

#endif  // JSON_UTILS_HPP_
