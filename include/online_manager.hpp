#pragma once
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <json/json.h>
#include <functional>
#include <string>

using WebsocketClient = websocketpp::client<websocketpp::config::asio_client>;

class OnlineManager {
public:
    OnlineManager();
    ~OnlineManager();
    
    bool connect(const std::string& uri);
    void disconnect();
    bool isConnected() const;
    
    // Game actions
    bool findMatch(int elo);
    bool makeMove(const std::string& gameId, const Move& move);
    bool resignGame(const std::string& gameId);
    
    // Callbacks
    void setOnMatchFound(std::function<void(const std::string&)> callback);
    void setOnMoveReceived(std::function<void(const Move&)> callback);
    void setOnGameEnded(std::function<void(const std::string&)> callback);
    
private:
    WebsocketClient client;
    websocketpp::connection_hdl connection;
    bool connected;
    
    // Callback storage
    std::function<void(const std::string&)> onMatchFound;
    std::function<void(const Move&)> onMoveReceived;
    std::function<void(const std::string&)> onGameEnded;
    
    // Message handlers
    void handleMessage(websocketpp::connection_hdl hdl, WebsocketClient::message_ptr msg);
    void handleMatchmaking(const Json::Value& data);
    void handleGameMove(const Json::Value& data);
    void handleGameEnd(const Json::Value& data);
}; 