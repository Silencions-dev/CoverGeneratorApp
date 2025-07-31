#ifndef STATISTICS_HPP
#define STATISTICS_HPP

/*!
 * \file
 * \brief Definition of the class Statistics
 *
 * This file contains definition of the class Statistics,
 * which provides constant connection with the server and
 * handles the database updates.
 */

#include <QFile>
#include "HttpManager.hpp"

/*!
 * \brief Strings with the JSON keys used in the communication with the server
 */
struct SessionDataKeys
{
    const char *API = "api_key";
    const char *TOKEN = "token";
    const char *ID = "session_id";
    const char *UPDATE = "update";
    const char *DATETIME = "date";
    const char *COUNTRY = "country";
    const char *STEPS_LOG = "steps_log";
    const char *RESULTS_LOG = "results_log";
    const char *CONTACT_LOG = "contact_log";
    const char *RE_RUN = "re_run";
    const char *PORTFOLIO = "portfolio_opened";
};

/*!
 * \brief Class that handles the updates of the database
 *
 * Statistics class implements the initial communication
 * with the server and constantly observes the connection.
 * It sends the API and session ID to the ContactWindow
 * object through the MainWindow, so the email functionality
 * can be enabled. The main task of the Statistics is to handle
 * multiple http request coming from the application and send
 * them to the server. Requests are used to update the database
 * with the information about usage of the app so it can be
 * eventually improved for the users.
 */
class Statistics : public QWidget
{
    Q_OBJECT
    private:

        // Functional members =======================================

        /*!
         * \brief Access to the network
         */
        QNetworkAccessManager *session_access;
        /*!
         * \brief Manager of the http requests
         *
         * It collects the requests in the FIFO queue and
         * periodically executes them one by one to relieve
         * the server.
         */
        HttpManager *manager;
        /*!
         * \brief URL used to initialize the communication
         */
        QString init_session_url;
        /*!
         * \brief URL used to update the database
         */
        QString update_data_url;
        /*!
         * \brief URL used to get API from the server
         */
        QString get_api_url;
        /*!
         * \brief URL used to get token from the server
         */
        QString get_token_url;
        /*!
         * \brief API key from the server
         */
        QString api_key;
        /*!
         * \brief Temporary token from the server
         */
        QString token;
        /*!
         * \brief ID of the current app session granted by the server
         */
        int session_id = 0;
        /*!
         * \brief Number of generator recalculations
         */
        int re_runs = 0;
        /*!
         * \brief Flag, which indicates if the connection with the server holds
         */
        bool server_connection = false;
        /*!
         * \brief Flag, which indicates if the connection with the server has been initialized
         */
        bool server_initialized = false;

        // Const attributes =========================================

        /*!
         * \brief JSON keys used in the communication
         */
        const SessionDataKeys JSONKeys;
        /*!
         * \brief Path to the configuration file of the server connection
         */
        static constexpr const char *SrvConnectConfig = ":/network_params/network/DataSrvConctConfig.json";

        // Private methods ==========================================

        /*!
         * \brief Start the initial communication (hand shakes with the server)
         */
        void startInitializationChain();
        /*!
         * \brief Get the API key from the server
         */
        void getAPI();
        /*!
         * \brief Get app session ID from the server
         */
        void getSessionID();
        /*!
         * \brief Send the update request with the given data
         */
        void sendUpdateRequest(const QJsonObject & json_update);
        /*!
         * \brief Send the request with the update of date, time and country
         */
        void updateSessionLocale();
        /*!
         * \brief Load the variables from the configuration file
         */
        void getMainServerConfig();
        /*!
         * \brief Control the flow of the hand shakes with the server (proper sequence of the requests)
         */
        void controlInitializationChain(const QString & main_key, QJsonObject & json_data);

    public:
        /*!
         * \brief Initialization of the class Statistics
         */
        Statistics(QWidget *parent = nullptr);
        /*!
         * \brief Update the sequence of the movement through the generator
         */
        void updateStepsHistory(int step);
        /*!
         * \brief Update the list with the types of the resulted covers
         */
        void updateResultsType(bool std_cover);
        /*!
         * \brief Update the way of a contact chosen by the client
         */
        void updateContactType(bool auto_mail);
        /*!
         * \brief Update the number of re-runs of the generator (increment the counter)
         */
        void updateGeneratorRerun();
        /*!
         * \brief Update the information about viewing the portfolio (set true)
         */
        void updatePortfolioViewed();

    private slots:
        /*!
         * \brief Handle the response from the server
         */
        void serverResponded(QNetworkReply *response);
        /*!
         * \brief Cut the connection with the server when failed
         */
        void databaseFailed();

    signals:
        /*!
         * \brief Pass the API key to other widgets when arrives
         */
        void apiKeyReceived(const QString api_key);
        /*!
         * \brief Pass the session ID to other widgets when arrives
         */
        void sessionIDAssigned(const int session_id);
};

#endif // STATISTICS_HPP
