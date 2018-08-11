/** Copyright 2011-2013 Thorsten Wißmann. All rights reserved.
 *
 * This software is licensed under the "Simplified BSD License".
 * See LICENSE for details */

#ifndef __CLIENTLIST_H_
#define __CLIENTLIST_H_

#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <stdbool.h>
#include "x11-types.h"

#include "object.h"
#include "utils.h"
#include "decoration.h"
#include "attribute_.h"

struct HSSlice;

class HSTag;
class HSMonitor;
class Theme;
class Settings;
class ClientManager;

class HSClient : public Object {
public:
    HSClient(Window w, bool already_visible, ClientManager& cm);
    ~HSClient();

    Window      window_;
    Decoration    dec;
    Rectangle   float_size_;     // floating size without the window border
    Attribute_<bool> urgent_;
    Attribute_<bool> fullscreen_;
    Attribute_<std::string> title_;  // or also called window title; this is never NULL
    struct HSSlice* slice;

public:
    Rectangle   last_size_;      // last size excluding the window border
    Attribute_<std::string> window_id_str;

    HSTag*      tag_;
    Attribute_<
    std::string> keymask_; // keymask applied to mask out keybindins
    bool        ewmhfullscreen_; // ewmh fullscreen state
    Attribute_<
    bool>       pseudotile_; // only move client but don't resize (if possible)
    bool        neverfocus_; // do not give the focus via XSetInputFocus
    bool        ewmhrequests_; // accept ewmh-requests for this client
    bool        ewmhnotify_; // send ewmh-notifications for this client
    bool        sizehints_floating_;  // respect size hints regarding this client in floating mode
    bool        sizehints_tiling_;  // respect size hints regarding this client in tiling mode
    bool        visible_;
    bool        dragged_;  // if this client is dragged currently
    int         pid_;
    int         ignore_unmaps_;  // Ignore one unmap for each reparenting
                                // action, because reparenting creates an unmap
                                // notify event
    // for size hints
    float mina_, maxa_;
    int basew_, baseh_, incw_, inch_, maxw_, maxh_, minw_, minh_;
    // for other modules

    void init_from_X();

    void make_full_client();


    // setter and getter for attributes
    HSTag* tag() { return tag_; };
    void setTag(HSTag* tag) { tag_ = tag; }

    Window x11Window() { return window_; };
    Window decorationWindow() { return dec.decorationWindow(); }
    friend void mouse_function_resize(XMotionEvent* me);

    // other member functions
    void window_focus();
    void window_unfocus();
    static void window_unfocus_last();

    void fuzzy_fix_initial_position();

    Rectangle outer_floating_rect();

    void setup_border(bool focused);
    void resize(Rectangle rect);
    void resize_tiling(Rectangle rect, bool isFocused);
    void resize_floating(HSMonitor* m, bool isFocused);
    bool is_client_floated();
    bool needs_minimal_dec();
    void set_urgent(bool state);
    void update_wm_hints();
    void update_title();
    void raise();

    void set_dragged(bool drag_state);

    void send_configure();
    bool applysizehints(int *w, int *h);
    bool applysizehints_xy(int *x, int *y, int *w, int *h);
    void updatesizehints();

    bool sendevent(Atom proto);

    void set_visible(bool visible_);

    void set_fullscreen(bool state);
    void set_pseudotile(bool state);
    void set_urgent_force(bool state);

    void clear_properties();
    bool ignore_unmapnotify();

    const DecorationScheme& getScheme(bool focused);

private:
    friend Decoration;
    ClientManager& manager;
    Theme& theme;
    Settings& settings;
    const DecTriple& getDecTriple();
    void resize_fullscreen(HSMonitor* m, bool isFocused);
};



void clientlist_init();
void clientlist_destroy();


void reset_client_colors();
void reset_client_settings();

HSClient* get_client_from_window(Window window);
HSClient* get_current_client();
HSClient* get_client(const char* str);
Window get_window(const char* str);

int close_command(int argc, char** argv, Output output);
void window_close(Window window);

// sets a client property, depending on argv[0]
int client_set_property_command(int argc, char** argv);
bool is_window_class_ignored(char* window_class);
bool is_window_ignored(Window win);

void window_set_visible(Window win, bool visible);

#endif
