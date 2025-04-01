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
    tabs->set_tab_pos(Gtk::PositionType::POS_LEFT);
    main_box->pack_start(*tabs, true, true, 0);

    auto *settings_button = Gtk::make_managed<Gtk::Button>();
    settings_button->set_image(*GtkUtils::new_image("emblem-system-symbolic", Gtk::ICON_SIZE_BUTTON));

    settings_button->show_all();
    tabs->set_action_widget(settings_button, Gtk::PACK_END);

    Volume::Control control(logger);
    auto *box1 = Gtk::make_managed<Volume::Tab>(logger, &control);
    // Gdk::RGBA color("blue");
    tabs->append_page(*box1, *GtkUtils::new_label_markup("Audio"));

    // g_signal_connect(tabs, "switch-page", G_CALLBACK(on_tab_switch), this);

    show_all_children();
}


BetterControl::~BetterControl() {}

