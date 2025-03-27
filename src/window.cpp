#include "../inc/window.hpp"


BetterControl::BetterControl(
    const char *window_title, std::pair<int32_t, int32_t> window_size
)
{
    set_title(window_title);
    set_type_hint(Gdk::WINDOW_TYPE_HINT_DIALOG);
    set_default_size(window_size.first, window_size.second);

    Gtk::Box *main_box = Gtk::manage( new Gtk::Box(Gtk::Orientation::ORIENTATION_VERTICAL, 10) );
    add(*main_box);

    Gtk::Notebook *tabs = Gtk::manage( new Gtk::Notebook() );
    main_box->pack_start(*tabs, true, true, 0);

    Gtk::Box *box1 = Gtk::manage( new Gtk::Box(Gtk::Orientation::ORIENTATION_HORIZONTAL, 10) );
    Gdk::RGBA color("blue");
    box1->override_background_color(color);
    tabs->append_page(*box1);

    Gtk::Box *box2 = Gtk::manage( new Gtk::Box(Gtk::Orientation::ORIENTATION_HORIZONTAL, 10) );
    Gdk::RGBA color1("red");
    box2->override_background_color(color);
    tabs->append_page(*box2);

    // g_signal_connect(tabs, "switch-page", G_CALLBACK(on_tab_switch), this);

    show_all_children();
}


BetterControl::~BetterControl() {}

