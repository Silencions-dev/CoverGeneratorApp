#ifndef HTTPDATAFRAME_HPP
#define HTTPDATAFRAME_HPP

/*!
 * \file
 * \brief Definition of the class HttpDataFrame
 *
 * This file contains definition of the class HttpDataFrame,
 * which manages the single http request.
 */

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>

/*!
 * \brief Class that implements single http request
 *
 * HttpDataFrame is a simple interface that stores
 * elements necessary to define the full http request,
 * like stored data (JSON), authorization (API key) and
 * type of operation (POST, GET, etc. ). It is used as
 * a single object in the queue managed by the HttpManager
 * class.
 */
class HttpDataframe
{
    private:

        // Functional members =======================================

        /*!
         * \brief Operation done by the request (POST, GET, etc.)
         */
        QNetworkAccessManager::Operation http_method;
        /*!
         * \brief Http request
         */
        QNetworkRequest request;
        /*!
         * \brief Data of the request in the JSON form
         */
        QJsonDocument json_data;

    public:
        /*!
         * \brief Initialization of the class HttpDataFrame
         */
        HttpDataframe(QNetworkAccessManager::Operation method);
        /*!
         * \brief Set the appropriate fields of the http request
         */
        void setupRequest(const QString & url, const char *api_key);
        /*!
         * \brief Return the handler to the stored JSON data
         *
         * \return Pointer to the JSON document, so it can be directly edited outside
         */
        QJsonDocument *returnJsonHandler() { return &json_data; }
        /*!
         * \brief Return the type of http operation
         *
         * \return Type of the http operation / method
         */
        QNetworkAccessManager::Operation httpMethodType() { return http_method; }
        /*!
         * \brief Return the stored http request
         *
         * \return Copy of the http request
         */
        QNetworkRequest httpRequest() { return request; }
        /*!
         * \brief Return the stored JSON data
         *
         * \return Copy of the JSON data
         */
        QJsonDocument httpData() { return json_data; }
};

#endif // HTTPDATAFRAME_HPP
