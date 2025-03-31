#include "../../include/gtk_utils.hpp"
#include "../../include/volume.hpp"

#include "gtkmm/comboboxtext.h"
#include "gtkmm/button.h"
#include "gtkmm/scale.h"

#define OUT_SPACING 10
#define OUT_MARGIN 15
#define IN_SPACING 5

#define NEW_ARR(x) { x, x, x, x }

using Gtk::Box;
using Gtk::ComboBoxText;
using Gtk::Label;
using Volume::Tab;


Tab::Tab(Logger *logger, Control *control) :
    tabs(Gtk::make_managed<Gtk::Notebook>()), control(control), logger(logger)
{
    logger->log(Debug, "Initialising volume tab");

    GtkUtils::set_margin(this, NEW_ARR(OUT_MARGIN));
    set_orientation(Gtk::ORIENTATION_VERTICAL);
    set_visible(true);
    set_spacing(OUT_SPACING);

    widgets.insert({ "app-box", initialise_widget<Gtk::Box>() });
    widgets.insert({ "scale", initialise_widget<Gtk::Scale>() });
    widgets.insert({ "button", initialise_widget<Gtk::Button>() });
    widgets.insert({ "combo", initialise_widget<Gtk::ComboBoxText>() });

    tabs->set_hexpand(false);

    pack_start(*create_title_box(), false, false);

    for (auto t : { create_system_audio_tab() }) {
        tabs->append_page(*t.first, *t.second);
    }

    pack_start(*tabs);
    show_all();
}


auto
Tab::create_title_box() -> Box*
{
    Box *header_box = GtkUtils::new_box('h', 0);
    Box *title_box = GtkUtils::new_box('h', 0);
    auto *image = Gtk::make_managed<Gtk::Image>();
    image->set_from_icon_name("audio-volume-high", Gtk::ICON_SIZE_LARGE_TOOLBAR);
    image->set_pixel_size(50);

    title_box->pack_start(*image);
    title_box->pack_start(
        *GtkUtils::new_label_markup("<span weight='bold' size='x-large'>Audio Settings</span>")
    );

    header_box->pack_start(*title_box, false, false, 0);
    return header_box;
}


auto
Tab::create_system_audio_tab() -> std::pair<Gtk::ScrolledWindow*, Box*>
{
    auto *main_scroll = Gtk::make_managed<Gtk::ScrolledWindow>();
    main_scroll->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);

    Box *content_box = GtkUtils::new_box('v', OUT_SPACING);
    GtkUtils::set_margin(content_box, NEW_ARR(OUT_SPACING));

    for (Volume::Type t : { Output, Input }) {
        Label *section_label = GtkUtils::new_label_markup(std::format(
            "<b>{} Device </b>", type_str.at(t)
        ));
        section_label->set_halign(Gtk::ALIGN_START);

        content_box->pack_start(*section_label, false, true);
        content_box->pack_start(*create_box(t), false, true);
        content_box->pack_start(*create_control_frame(t), false, true);
    }

    main_scroll->add(*content_box);

    Box *tab_label = GtkUtils::new_box('h', IN_SPACING);
    auto *tab_label_image = Gtk::make_managed<Gtk::Image>();
    tab_label_image->set_from_icon_name("audio-volume-high", Gtk::ICON_SIZE_MENU);
    tab_label->pack_start(*tab_label_image);
    tab_label->pack_start(*GtkUtils::new_label_markup("<span size='large'>System</span>"));
    tab_label->set_tooltip_text("System audio settings");
    tab_label->show_all();

    return { main_scroll, tab_label };
}


auto
Tab::create_box(Volume::Type type) -> Box*
{
    Box *box = GtkUtils::new_box('h', IN_SPACING);
    box->set_margin_top(IN_SPACING);

    Gtk::ComboBoxText *combo = dynamic_cast<ComboBoxText*>(widgets.at("combo").at(type));
    combo->signal_changed().connect(sigc::bind(
        sigc::mem_fun(*this, &Tab::on_device_change), type
    ));
    box->pack_start(*Gtk::make_managed<Label>("Device:"), false, false);
    box->pack_start(*combo, true, true);
    return box;
}


auto
Tab::create_control_frame(Volume::Type type) -> Gtk::Frame*
{
    if (type == All) return nullptr;
    auto *frame = Gtk::make_managed<Gtk::Frame>();
    frame->set_shadow_type(Gtk::SHADOW_IN);
    frame->set_margin_top(OUT_SPACING);

    Box *volume_box = GtkUtils::new_box('v', IN_SPACING);
    GtkUtils::set_margin(volume_box, NEW_ARR(OUT_SPACING));

    /* ? Label */
    Label *title = GtkUtils::new_label_markup(std::format(
        "<b>{} Volume</b>", type_str.at(type)
    ));
    title->set_halign(Gtk::ALIGN_START);

    /* ? Slider */
    Box *control_box = GtkUtils::new_box('h', IN_SPACING);
    Gtk::Scale *scale = dynamic_cast<Gtk::Scale*>(widgets.at("scale").at(type));
    scale->set_range(0, 100);
    scale->set_increments(1, 2);
    scale->set_value(control->get_volume(type));
    scale->set_digits(0);
    scale->set_margin_right(OUT_SPACING);
    scale->signal_value_changed().connect(sigc::bind(
        sigc::mem_fun(*this, &Tab::on_volume_change), type
    ));

    /* ? Mute button */
    Gtk::Button *mute_button = dynamic_cast<Gtk::Button*>(widgets.at("button").at(type));
    update_mute_buttons(type);
    mute_button->signal_clicked().connect(sigc::bind(
        sigc::mem_fun(*this, &Tab::on_mute_click), type
    ));

    /* ? Quick volume buttons */
    Box *quick_volume_box = GtkUtils::new_box('h', IN_SPACING);

    for (uint8_t vol : { 0, 25, 50, 75, 100 }) {
        auto *button = Gtk::make_managed<Gtk::Button>(std::format("{}%", vol));
        button->signal_clicked().connect(sigc::bind(
            sigc::mem_fun(*this, &Tab::on_quick_volume_click), type, vol
        ));
        quick_volume_box->pack_start(*button, true, true);
    }
    volume_box->pack_start(*title, false, true);
    control_box->pack_start(*scale, true, true);
    control_box->pack_start(*mute_button, false, false);
    volume_box->pack_start(*control_box, true, true);
    volume_box->pack_start(*quick_volume_box, false, true);
    frame->add(*volume_box);

    return frame;
}


auto
Tab::create_app_control_box(Volume::Type type) -> Box*
{
    Box* content_box = GtkUtils::new_box('v', OUT_SPACING);

    Label* label = GtkUtils::new_label_markup(std::format(
        "<b>Applications {} Volume</b>", type_str.at(type)
    ));
    label->set_halign(Gtk::ALIGN_START);
    label->set_margin_top(OUT_MARGIN);

    auto *frame = Gtk::make_managed<Gtk::Frame>();
    frame->set_shadow_type(Gtk::SHADOW_IN);
    frame->set_margin_top(IN_SPACING);

    Box* app_box = dynamic_cast<Box*>(widgets.at("app-box").at(type));
    app_box->set_orientation(Gtk::ORIENTATION_VERTICAL);
    app_box->set_spacing(IN_SPACING);
    GtkUtils::set_margin(app_box, NEW_ARR(OUT_SPACING));

    content_box->pack_start(*label);
    frame->add(*app_box);
    content_box->pack_start(*frame, true, true);

    return content_box;
}


void
Tab::on_device_change(Volume::Type type)
{
    std::string id = dynamic_cast<Gtk::ComboBoxText*>(
        widgets.at("combo").at(type)
    )->get_active_id();
    if (id != "") {
        control->set_default(type, id);
    }
}


void
Tab::on_mute_click(Volume::Type type)
{
    if (type == All) return;
    control->toggle_mute(type);
    update_mute_buttons(type);
}


void
Tab::on_volume_change(Volume::Type type)
{
    if (type == All) return;
    int32_t value = dynamic_cast<Gtk::Scale*>(widgets.at("scale").at(type))->get_value();
    control->set_volume(type, value);

    if (value > 0 && control->is_muted(type)) {
        Glib::signal_idle().connect(sigc::bind(
            sigc::mem_fun(*this, &Tab::update_mute_buttons), type
        ));
    }
}


void
Tab::on_quick_volume_click(Volume::Type type, int32_t volume)
{
    if (type == All) return;
    control->set_volume(type, volume);
    dynamic_cast<Gtk::Scale*>(widgets.at("scale").at(type))->set_value(volume);
}


auto
Tab::update_mute_buttons(Volume::Type type) -> bool
{
    if (type == All) return false;

    uint8_t index = (control->is_muted(type) ? 0 : 1);
    if (icons.find(type) == icons.end()) return false;

    Gtk::Button *mute_button = dynamic_cast<Gtk::Button*>(widgets.at("button").at(type));

    mute_button->set_tooltip_text(std::vformat(
        icons.at(type).at(index).second, std::make_format_args(type_str.at(type))
    ));
    mute_button->set_image_from_icon_name(icons.at(type).at(index).first);
    return true;
}