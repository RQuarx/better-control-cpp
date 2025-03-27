#include "../include/volume.hpp"
#include "../include/gtk_utils.hpp"

#include "../include/window.hpp"


BetterControl::BetterControl(
    Logger *logger, ArgParser *arg_parse, const char *window_title, std::pair<int32_t, int32_t> window_size
) : logger(logger)
{
    logger->log(Debug, "Initialising main window");
    set_title(window_title);
    set_type_hint(Gdk::WINDOW_TYPE_HINT_DIALOG);
    set_default_size(window_size.first, window_size.second);

    Gtk::Box *main_box = GtkUtils::new_box('v', 10);
    add(*main_box);

    auto *tabs = Gtk::make_managed<Gtk::Notebook>();
    main_box->pack_start(*tabs, true, true, 0);

    Volume::Control control(logger);
    auto *box1 = Gtk::make_managed<Volume::Tab>(logger, arg_parse, &control);
    // Gdk::RGBA color("blue");
    tabs->add(*box1);

    // g_signal_connect(tabs, "switch-page", G_CALLBACK(on_tab_switch), this);

    show_all_children();
}


BetterControl::~BetterControl() {}

