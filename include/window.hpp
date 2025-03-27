#pragma once
#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <cstdint>
#include <vector>

#include <gtkmm-3.0/gtkmm.h>

#include "logger.hpp"


class
BetterControl : public Gtk::Window
{
public:
    enum Types : uint8_t {
        Wifi,
        Bluetooth,
        Display,
        Battery,
        Volume
    };

    BetterControl(
        Logger *logger, ArgParser *arg_parse, const char *window_title, std::pair<int32_t, int32_t> window_size
    );
    ~BetterControl();
    BetterControl(const BetterControl &) = delete;
    BetterControl(BetterControl &&) = delete;
    auto operator=(const BetterControl &) -> BetterControl & = delete;
    auto operator=(BetterControl &&) -> BetterControl & = delete;

private:
    std::vector<Types> tabs;
    Logger *logger;

protected:
    static void on_tab_switch(GtkNotebook *notebook, GtkWidget *page, guint page_num, gpointer user_data);
    /* IDK */
};

#endif /* window.hpp */