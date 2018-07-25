#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "object.h"
#include "client.h"
#include "child.h"

#include <unordered_map>

class Theme;

// Note: this is basically a singleton

class ClientManager : public Object
{
public:
    ClientManager(Theme& theme);
    ~ClientManager();

    HSClient* client(Window window);
    HSClient* client(const std::string &identifier);
    const std::unordered_map<Window, HSClient*>&
    clients() { return clients_; }

    void add(HSClient* client);
    void remove(Window window);

    void unmap_notify(Window win);
    void force_unmanage(Window win);
    void force_unmanage(HSClient* client);

    Child_<HSClient> focus;

    // adds a new client to list of managed client windows
    HSClient* manage_client(Window win, bool visible_already);

protected:
    Theme& theme;
    std::unordered_map<Window, HSClient*> clients_;
};

#endif