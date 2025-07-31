#ifndef HTTPMANAGER_HPP
#define HTTPMANAGER_HPP

/*!
 * \file
 * \brief Definition of the class HttpManager
 *
 * This file contains definition of the class HttpManager,
 * which collects the http requests in the queue and manages
 * their execution.
 */

#include <list>
#include <iostream>
#include <QNetworkAccessManager>
#include <QWidget>
#include <QTimer>
#include "HttpDataframe.hpp"

/*!
 * \brief Class that implements queue for the http requests
 *
 * HttpManager inherits from QWidget, so it can use signal/
 * slot system. It implements the basic FIFO queue using the
 * list data structure, which stores the objects of HttpDataFrame
 * class. HttpManager sends the request using QNetworkAccessManager
 * periodically, when the timer triggers the timeout. The main
 * reason is to slow down the http traffic from the app, in case of
 * the user rushing through it.
 */
class HttpManager : public QWidget
{
    Q_OBJECT
    private:

        // Functional members =======================================

        /*!
         * \brief Access to the netowrk (sends the requests)
         */
        QNetworkAccessManager *connector;
        /*!
         * \brief Timer that determines the checks of the queue
         */
        QTimer *executor;
        /*!
         * \brief List of the http requests as a queue representation
         */
        std::list<HttpDataframe*> queue;
        /*!
         * \brief Flag, which indicates if any http request is currently processed
         */
        bool request_in_process = false;
        /*!
         * \brief Current size of the queue
         */
        int size = 0;

        // Const attributes =========================================

        /*!
         * \brief Period of the execution timer
         */
        static constexpr int TimerTrigger_ms = 500;

        // Private methods ==========================================

        /*!
         * \brief Remove the task from the beginning of the queue
         */
        void removeLastTask();
        /*!
         * \brief Check if queue is empty
         */
        bool isEmpty();
        /*!
         * \brief Print the tasks in the queue (debug only)
         */
        void printQueue();

    public:
        /*!
         * \brief Initialization of the class HttpManager
         */
        HttpManager(QWidget *parent = nullptr);
        /*!
         * \brief Destructor of the HttpManager
         */
        ~HttpManager();
        /*!
         * \brief Add new task at the end of the queue
         */
        void addNewTask(HttpDataframe *new_task);

    public slots:
        /*!
         * \brief Execute new http request from the beginning of the queue
         */
        void executeHttpRequest();
        /*!
         * \brief Handle the response from the server of the finished http request
         */
        void requestFinished(QNetworkReply *response);

    signals:
        /*!
         * \brief Sent when the server connection has been lost
         */
        void serverFailure();
};

#endif // HTTPMANAGER_HPP
