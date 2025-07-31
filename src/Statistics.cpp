#include "Statistics.hpp"

/*!
 * \brief Initialization of the class Statistics
 *
 * Create new instances of the inner widgets. Then connect the signals with
 * appropriate slots. At the end load the configuration parameters of the
 * connection with the server and finally start the communication.
 *
 * \param[in] parent - pointer to the parent widget
 */
Statistics::Statistics(QWidget *parent) : QWidget(parent)
{
    session_access = new QNetworkAccessManager(this);
    manager = new HttpManager(this);

    connect(session_access, SIGNAL(finished(QNetworkReply*)), this, SLOT(serverResponded(QNetworkReply*)));
    connect(manager, SIGNAL(serverFailure()), this, SLOT(databaseFailed()));

    // First download all necessary URLs
    getMainServerConfig();
    // Start the connection
    startInitializationChain();
}
/*!
 * \brief Start the initial communication (hand shakes with the server)
 *
 * First, get the token from the server. Sequence of the requests is then
 * handled in the controlInitializationChain method.
 */
void Statistics::startInitializationChain()
{
    QUrl url(get_token_url);
    QNetworkRequest request(url);
    session_access->get(request);
}
/*!
 * \brief Handle the response from the server
 *
 * First, check if there is an error in the response. If not, read the
 * attached JSON data. Extract the first, and probably the only, key and
 * call the controller of the initial communication.
 *
 * \param[in] response - response from the server
 */
void Statistics::serverResponded(QNetworkReply *response)
{
    if(response->error() != QNetworkReply::NoError)
    {
        server_connection = false;
        return;
    }

    QByteArray data = response->readAll();
    QJsonDocument json_container = QJsonDocument::fromJson(data);
    if(!json_container.isObject())
    {
        server_connection = false;
        return;
    }

    QJsonObject json_data = json_container.object();
    QStringList json_keys = json_data.keys();
    QString main_key = json_keys[0];

    controlInitializationChain(main_key, json_data);

    server_connection = true;
}
/*!
 * \brief Control the flow of the hand shakes with the server (proper sequence of the requests)
 *
 * Send the next requests of the initial communication based on the
 * given key and data from the last finished request.
 *
 * \param[in] main_key - JSON key from the server response
 * \param[in] json_data - JSON data from the server response
 */
void Statistics::controlInitializationChain(const QString & main_key, QJsonObject & json_data)
{
    if(main_key == QString(JSONKeys.TOKEN))
    {
        QString clean_token = json_data[JSONKeys.TOKEN].toString();
        token = QString("Bearer ") + clean_token;
        this->getAPI();
    }
    else if(main_key == QString(JSONKeys.API))
    {
        QString clean_api_key = json_data[JSONKeys.API].toString();
        api_key = QString("Bearer ") + clean_api_key;

        emit apiKeyReceived(api_key);

        getSessionID();
    }
    else if(main_key == QString(JSONKeys.ID))
    {
        session_id = json_data[JSONKeys.ID].toInt();

        emit sessionIDAssigned(session_id);

        server_initialized = true; // End of the initialziation chain
        updateSessionLocale();
    }
}
/*!
 * \brief Get the API key from the server
 *
 * Verify if the connection with the server holds. If so, then
 * send the appropriate request with token authorization.
 */
void Statistics::getAPI()
{
    if(!server_connection)
        return;

    QUrl url(get_api_url);
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", token.toUtf8());

    session_access->get(request);
}
/*!
 * \brief Get app session ID from the server
 *
 * Verify if the connection with the server holds. If so, then
 * send the appropriate request with API authorization.
 */
void Statistics::getSessionID()
{
    if(!server_connection)
        return;

    QUrl url(init_session_url);
    QNetworkRequest request(url);

    request.setRawHeader("Authorization", api_key.toUtf8());

    session_access->get(request);
}
/*!
 * \brief Send the update request with the given data
 *
 * First, verify if the connection with the server has been initialized
 * and is still present. If so, create new HTTP task (HttpDataFrame object)
 * and set the appropriate fields (authorization, URL, JSON data). Finally push
 * new task to the queue.
 *
 * \param[in] json_update - JSON with the fields that should be updated
 */
void Statistics::sendUpdateRequest(const QJsonObject & json_update)
{
    // Check if the server connection was established
    if(!server_connection || !server_initialized)
        return;

    HttpDataframe *new_task = new HttpDataframe(QNetworkAccessManager::PostOperation);
    new_task->setupRequest(update_data_url, api_key.toUtf8());

    QJsonObject json_frame;
    json_frame[JSONKeys.ID] = session_id;
    json_frame[JSONKeys.UPDATE] = json_update;

    QJsonDocument *json_doc = new_task->returnJsonHandler();
    json_doc->setObject(json_frame);

    manager->addNewTask(new_task);
}
/*!
 * \brief Cut the connection with the server when failed
 *
 * Set the bool value to false, if there was an error in a
 * response somewhere in the Statistics or HttpManager.
 */
void Statistics::databaseFailed()
{
    server_connection = false;
}
/*!
 * \brief Send the request with the update of date, time and country
 *
 * Get the date, time and contry from the current system. Pack it into
 * JSON data and trigger new request.
 */
void Statistics::updateSessionLocale()
{
    QLocale locale = QLocale::system();
    QString territory = locale.territoryToString(locale.territory());

    QDateTime date_time = QDateTime::currentDateTime();
    QString current_datetime = date_time.toString("dd.MM.yyyy hh:mm");

    // Build the JSON payload
    QJsonObject update;
    update[JSONKeys.DATETIME] = current_datetime;
    update[JSONKeys.COUNTRY] = territory;

    sendUpdateRequest(update);
}
/*!
 * \brief Update the sequence of the movement through the generator
 *
 * Get the current step, pack it into JSON data and trigger new request.
 *
 * \param[in] step - triggered step of the generator
 */
void Statistics::updateStepsHistory(int step)
{
    // Build the JSON payload
    QJsonObject update;
    update[JSONKeys.STEPS_LOG] = step;

    sendUpdateRequest(update);
}
/*!
 * \brief Update the list with the types of the resulted covers
 *
 * Transform the given boolean value to the appropriate string
 * ("Standard" or "Special" cover), pack it into JSON data and
 * trigger new request.
 *
 * \param[in] std_cover - true if cover generated and false if special cover is necessary
 */
void Statistics::updateResultsType(bool std_cover)
{
    QString result_type;

    if(std_cover)
        result_type = "Standard";
    else
        result_type = "Special";

    // Build the JSON payload
    QJsonObject update;
    update[JSONKeys.RESULTS_LOG] = result_type;

    sendUpdateRequest(update);
}
/*!
 * \brief Update the way of a contact chosen by the client
 *
 * Transform the given boolean value to the appropriate string
 * ("Auto mail" or "Manual mail"), pack it into JSON data and
 * trigger new request.
 *
 * \param[in] auto_mail - true if email sent from the app and false if sent manually
 */
void Statistics::updateContactType(bool auto_mail)
{
    QString contact_type;

    if(auto_mail)
        contact_type = "Auto mail";
    else
        contact_type = "Manual mail";

    // Build the JSON payload
    QJsonObject update;
    update[JSONKeys.CONTACT_LOG] = contact_type;

    sendUpdateRequest(update);
}
/*!
 * \brief Update the number of re-runs of the generator (increment the counter)
 *
 * Increment the value of the regenerations of the cover and
 * trigger new request with the updated value.
 */
void Statistics::updateGeneratorRerun()
{
    re_runs += 1;
    // Build the JSON payload
    QJsonObject update;
    update[JSONKeys.RE_RUN] = re_runs;

    sendUpdateRequest(update);
}
/*!
 * \brief Update the information about viewing the portfolio (set true)
 *
 * Set the appropriate value in the JSON data to true and
 * trigger new update request.
 */
void Statistics::updatePortfolioViewed()
{
    // Build the JSON payload
    QJsonObject update;
    update[JSONKeys.PORTFOLIO] = true;

    sendUpdateRequest(update);
}
/*!
 * \brief Load the variables from the configuration file
 *
 * Open the configuration file (break the connection if failed to read) and
 * load the parameters (server function URLs).
 */
void Statistics::getMainServerConfig()
{
    QFile config(SrvConnectConfig);

    if(!config.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        server_connection = false;
        return;
    }

    QByteArray data = config.readAll();
    config.close();
    QJsonDocument json_doc = QJsonDocument::fromJson(data);

    if(!json_doc.isObject())
    {
        server_connection = false;
        return;
    }

    QJsonObject json_obj = json_doc.object();
    init_session_url = json_obj.value("init_session").toString();
    update_data_url = json_obj.value("update_session").toString();
    get_api_url = json_obj.value("api_proxy").toString();
    get_token_url = json_obj.value("token_proxy").toString();

    server_connection = true;
}
