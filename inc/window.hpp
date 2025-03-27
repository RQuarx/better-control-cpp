#pragma once
#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <cstdint>
#include <vector>

#include <gtkmm-3.0/gtkmm.h>


enum Tabs : uint8_t {
    Wifi,
    Bluetooth,
    Display,
    Battery,
    Volume
};


class
BetterControl : public Gtk::Window
{
public:
    BetterControl(
        const char *window_title, std::pair<int32_t, int32_t> window_size
    );
    ~BetterControl();
    BetterControl(const BetterControl &) = delete;
    BetterControl(BetterControl &&) = delete;
    auto operator=(const BetterControl &) -> BetterControl & = delete;
    auto operator=(BetterControl &&) -> BetterControl & = delete;

private:
    std::vector<Tabs> tabs;

    static void on_tab_switch(GtkNotebook *notebook, GtkWidget *page, guint page_num, gpointer user_data);
    /* IDK */
};

#endif /* window.hpp */