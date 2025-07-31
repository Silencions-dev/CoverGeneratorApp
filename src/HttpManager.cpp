#include "HttpManager.hpp"

/*!
 * \brief Initialization of the class HttpManager
 *
 * Create new instances of the inner widgets. Set the interval
 * of the timer and the mode to be periodic (sends timeouts all
 * the time without the manual reset). Connect the signals with
 * appropriate slots and start the timer.
 *
 * \param parent - pointer to the parent widget
 */
HttpManager::HttpManager(QWidget *parent) : QWidget(parent)
{
    connector = new QNetworkAccessManager(this);
    executor = new QTimer(this);

    executor->setInterval(TimerTrigger_ms);
    executor->setSingleShot(false);

    connect(executor, SIGNAL(timeout()), this, SLOT(executeHttpRequest()));
    connect(connector, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));

    executor->start();
}
/*!
 * \brief Execute new http request from the beginning of the queue
 *
 * First, check if there is already a request in process or the queue
 * is empty. If so, simply return. If not, get the pointer to the task
 * from the queue and send the request using appropriate HTTP method.
 */
void HttpManager::executeHttpRequest()
{
    if(request_in_process || isEmpty())
        return;

    request_in_process = true;

    HttpDataframe *task = queue.back();

    switch(task->httpMethodType())
    {
        case QNetworkAccessManager::PostOperation:
            connector->post(task->httpRequest(), task->httpData().toJson());
            break;
        case QNetworkAccessManager::GetOperation:
            connector->get(task->httpRequest());
        default:
            break;
    }
}
/*!
 * \brief Handle the response from the server of the finished http request
 *
 * Check if the response contains the error and eventually notify the
 * Statistics class to break the connection with the server. If request
 * finished successfully, remove it from the queue and set the flag that
 * there is no request in process anymore.
 *
 * \param[in] response - reponse from the server
 */
void HttpManager::requestFinished(QNetworkReply *response)
{
    if(response->error() != QNetworkReply::NoError)
        emit serverFailure();
    else
        removeLastTask();

    request_in_process = false;
}
/*!
 * \brief Add new task at the end of the queue
 *
 * Push the pointer to the given task at the end and increment the
 * current size of the queue.
 *
 * \param[in] new_task - pointer to the new http request
 */
void HttpManager::addNewTask(HttpDataframe *new_task)
{
    queue.push_front(new_task);
    size += 1;
    // For debug only
    // printQueue();
}
/*!
 * \brief Remove the task from the beginning of the queue
 *
 * Remove and destroy the object at the given pointer from the
 * front of the queue. Decrement the current size of the queue.
 */
void HttpManager::removeLastTask()
{
    if(!isEmpty())
    {
        queue.pop_back();
        size -= 1;
        // For debug only
        // printQueue();
    }
}
/*!
 * \brief Check if queue is empty
 *
 * \return True if empty and false otherwise
 */
bool HttpManager::isEmpty()
{
    if(size == 0)
        return true;

    return false;
}
/*!
 * \brief Print the tasks in the queue (debug only)
 *
 * Iterate over the queue and print the request along with
 * the given ID.
 */
void HttpManager::printQueue()
{
    int i = 0;
    for(auto it = queue.begin(); it != queue.end(); ++it)
    {
        qDebug() << "<HttpRequest ID: " << i << "> ";
        ++i;
    }
}
/*!
 * \brief Destructor of the HttpManager
 *
 * Stop the timer and clear the queue from the requests in
 * case of some still remain.
 */
HttpManager::~HttpManager()
{
    executor->stop();
    queue.clear();
}
