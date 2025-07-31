#include "HttpDataframe.hpp"

/*!
 * \brief Initialization of the class HttpDataFrame
 *
 * Set the given HTTP method and create the empty JSON object.
 *
 * \param[in] method - type of the http operation that should be done by the request
 */
HttpDataframe::HttpDataframe(QNetworkAccessManager::Operation method)
{
    http_method = method;
    QJsonObject json_frame {{}};
    json_data.setObject(json_frame);
}
/*!
 * \brief Set the appropriate fields of the http request
 *
 * If the HTTP method of the request is POST, additionally set the
 * information about included data type. Also set the URL and
 * authorization.
 *
 * \param[in] url - URL of the target server (and function)
 * \param[in] api_key - API key for the authorization
 */
void HttpDataframe::setupRequest(const QString & url, const char *api_key)
{
    if(http_method == QNetworkAccessManager::PostOperation)
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    request.setUrl(QUrl(url));
    request.setRawHeader("Authorization", api_key);
}
