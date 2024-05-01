/**
 * @file pistache_utils.hpp
 * @author denis
 * @brief
 * @date 12024-04-13
 *
 */

#ifndef PISTACHE_UTILS_HPP_
#define PISTACHE_UTILS_HPP_

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/mime.h>
#include <pistache/router.h>

namespace watch_list_app::server {

// using namespace Pistache;
// using  namespace Pistache::Http;
using MimeMediaType = Pistache::Http::Mime::MediaType;
using MimeType = Pistache::Http::Mime::Type;
using MimeSubtype = Pistache::Http::Mime::Subtype;
using HttpCode = Pistache::Http::Code;
using HttpRequest = Pistache::Http::Request;
using RestRequest = Pistache::Rest::Request;
using HttpResponseWriter = Pistache::Http::ResponseWriter;
using HttpMethod = Pistache::Http::Method;

}  // namespace watch_list_app::server

#endif  // PISTACHE_UTILS_HPP_
