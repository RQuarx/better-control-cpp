#include "../inc/window.hpp"
#include "gdkmm/rgba.h"


BetterControl::BetterControl(
    const char *window_title, std::pair<int32_t, int32_t> window_size
)
{
    set_title(window_title);
    set_type_hint(Gdk::WINDOW_TYPE_HINT_DIALOG);
    set_default_size(window_size.first, window_size.second);

    

    set_border_width(10);

    Gtk::Box *vbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 5));
    add(*vbox);

    for (size_t i = 0; i < 3; i++) {
        Gtk::Box* hbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 5));
        vbox->pack_start(*hbox, Gtk::PACK_SHRINK);

        // Add labels inside each box to distinguish them
        Gtk::Label* label = Gtk::manage(new Gtk::Label("Box " + std::to_string(i + 1)));
        hbox->pack_start(*label, Gtk::PACK_SHRINK);

        const Gdk::RGBA color("#00ffff");

        hbox->override_background_color(color);
    }

    show_all_children();
}


BetterControl::~BetterControl()
{

}
