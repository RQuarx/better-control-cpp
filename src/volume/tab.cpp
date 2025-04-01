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


Tab::Tab(Logger *logger, Control *m_control) :
    m_tabs(Gtk::make_managed<Gtk::Notebook>()), m_control(m_control), m_logger(logger)
{
    logger->log(Debug, "Initialising volume tab");

    GtkUtils::set_margin(this, NEW_ARR(OUT_MARGIN));
    set_orientation(Gtk::ORIENTATION_VERTICAL);
    set_visible(true);
    set_spacing(OUT_SPACING);

    m_widgets.insert({ "app-box", initialise_widget<Gtk::Box>() });
    m_widgets.insert({ "scale", initialise_widget<Gtk::Scale>() });
    m_widgets.insert({ "button", initialise_widget<Gtk::Button>() });
    m_widgets.insert({ "combo", initialise_widget<Gtk::ComboBoxText>() });

    for (auto t : { create_system_audio_tab(), create_app_audio_tab() }) {
        m_tabs->append_page(*t.first, *t.second);
    }

    pack_start(*m_tabs);
    show_all();

    update_device_lists();
    Glib::signal_timeout().connect(sigc::bind(
        sigc::mem_fun(*this, &Tab::check_volume_difference), All
    ), 250);
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
        content_box->pack_start(*create_system_control_frame(t), false, true);
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
Tab::create_system_control_frame(Volume::Type type) -> Gtk::Frame*
{
    if (type == All) return nullptr;
    auto *frame = Gtk::make_managed<Gtk::Frame>();
    frame->set_shadow_type(Gtk::SHADOW_IN);
    frame->set_margin_top(OUT_SPACING);

    Box *main_box = GtkUtils::new_box('v', IN_SPACING);
    GtkUtils::set_margin(main_box, NEW_ARR(OUT_SPACING));

    /* ? Section label */
    Label *section_label = GtkUtils::new_label_markup(std::format(
        "<span weight='bold' size='large'>{}</span>", type_str.at(type)
    ));
    section_label->set_halign(Gtk::ALIGN_START);

    /* ? Devices dropdown menu */
    Box *device_box = GtkUtils::new_box('h', IN_SPACING);
    device_box->set_margin_top(IN_SPACING);
    device_box->set_margin_bottom(IN_SPACING);

    Gtk::ComboBoxText *combo = dynamic_cast<ComboBoxText*>(m_widgets.at("combo").at(type));
    combo->signal_changed().connect(sigc::bind(
        sigc::mem_fun(*this, &Tab::on_device_change), type
    ));
    device_box->pack_start(*Gtk::make_managed<Label>("Device:"), false, false);
    device_box->pack_start(*combo, true, true);

    /* ? Slider frame */
    auto *slider_frame = Gtk::make_managed<Gtk::Frame>();
    frame->set_shadow_type(Gtk::SHADOW_IN);
    frame->set_margin_top(OUT_SPACING);

    Box *volume_box = GtkUtils::new_box('v', IN_SPACING);
    GtkUtils::set_margin(volume_box, NEW_ARR(OUT_SPACING));

    /* ? Slider label */
    Label *volume_label = GtkUtils::new_label_markup(std::format(
        "<b>Volume</b>", type_str.at(type)
    ));
    volume_label->set_halign(Gtk::ALIGN_START);

    /* ? Slider */
    Box *control_box = GtkUtils::new_box('h', IN_SPACING);
    Gtk::Scale *scale = dynamic_cast<Gtk::Scale*>(m_widgets.at("scale").at(type));
    scale->set_range(0, 150);
    for (uint8_t i = 0; i <= 150; i += 25) {
        scale->add_mark(i, Gtk::POS_BOTTOM, std::format("{}", i));
    }
    scale->set_draw_value(false);
    scale->set_increments(1, 2);
    scale->set_value(m_control->get_volume(type));
    scale->set_digits(0);
    scale->set_margin_right(OUT_SPACING);
    scale->signal_value_changed().connect(sigc::bind(
        sigc::mem_fun(*this, &Tab::on_volume_change), type
    ));

    /* ? Mute button */
    Gtk::Button *mute_button = dynamic_cast<Gtk::Button*>(m_widgets.at("button").at(type));
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
    main_box->pack_start(*section_label, false, true);
    main_box->pack_start(*device_box, false, true);
    volume_box->pack_start(*volume_label, false, true);
    control_box->pack_start(*scale, true, true);
    control_box->pack_start(*mute_button, false, false);
    volume_box->pack_start(*control_box, false, true);
    volume_box->pack_start(*quick_volume_box, false, true);
    slider_frame->add(*volume_box);
    main_box->pack_start(*slider_frame, true, true);
    frame->add(*main_box);

    return frame;
}


auto
Tab::create_app_audio_tab() -> std::pair<Gtk::ScrolledWindow*, Gtk::Box*>
{
    auto *main_scroll = Gtk::make_managed<Gtk::ScrolledWindow>();
    main_scroll->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);

    Box *content_box = GtkUtils::new_box('v', OUT_SPACING);
    GtkUtils::set_margin(content_box, NEW_ARR(OUT_SPACING));

    for (Volume::Type t : { Volume::Output, Volume::Input }) {
        content_box->pack_start(*create_app_control_frame(t));
    }

    main_scroll->add(*content_box);
    Box *tab_label = GtkUtils::new_box('h', IN_SPACING);
    auto *tab_label_image = Gtk::make_managed<Gtk::Image>();
    tab_label_image->set_from_icon_name("multimedia-volume-control-symbolic", Gtk::ICON_SIZE_MENU);
    tab_label->pack_start(*tab_label_image);
    tab_label->pack_start(*GtkUtils::new_label_markup("<span size='large'>Applications</span>"));
    tab_label->set_tooltip_text("Applications audio settings");
    tab_label->show_all();

    return { main_scroll, tab_label };
}


auto
Tab::create_app_control_frame(Volume::Type type) -> Gtk::Frame*
{
    if (type == All) return nullptr;
    auto *frame = Gtk::make_managed<Gtk::Frame>();
    frame->set_shadow_type(Gtk::SHADOW_IN);
    frame->set_margin_top(OUT_SPACING);

    Box* content_box = GtkUtils::new_box('v', OUT_SPACING);
    GtkUtils::set_margin(content_box, NEW_ARR(OUT_SPACING));

    Label* label = GtkUtils::new_label_markup(std::format(
        "<b>Applications {} Volume</b>", type_str.at(type)
    ));
    label->set_halign(Gtk::ALIGN_START);

    auto *app_frame = Gtk::make_managed<Gtk::Frame>();
    app_frame->set_shadow_type(Gtk::SHADOW_IN);

    Box* app_box = dynamic_cast<Box*>(m_widgets.at("app-box").at(type));
    app_box->set_orientation(Gtk::ORIENTATION_VERTICAL);
    app_box->set_spacing(IN_SPACING);
    GtkUtils::set_margin(app_box, NEW_ARR(OUT_SPACING));

    content_box->pack_start(*label);
    app_frame->add(*app_box);
    content_box->pack_start(*app_frame, true, true);
    frame->add(*content_box);

    return frame;
}


void
Tab::on_device_change(Volume::Type type)
{
    std::string id = dynamic_cast<Gtk::ComboBoxText*>(
        m_widgets.at("combo").at(type)
    )->get_active_id();
    if (id != "") {
        m_control->set_default(type, id);
    }
}


void
Tab::on_mute_click(Volume::Type type)
{
    if (type == All) return;
    m_control->toggle_mute(type);
    update_mute_buttons(type);
}


void
Tab::on_volume_change(Volume::Type type)
{
    if (type == All) return;
    int32_t value = dynamic_cast<Gtk::Scale*>(m_widgets.at("scale").at(type))->get_value();
    m_control->set_volume(type, value);

    if (value > 0 && m_control->is_muted(type)) {
        Glib::signal_idle().connect(sigc::bind(
            sigc::mem_fun(*this, &Tab::update_mute_buttons), type
        ));
    }
}


void
Tab::on_quick_volume_click(Volume::Type type, int32_t volume)
{
    if (type == All) return;
    m_control->set_volume(type, volume);
    dynamic_cast<Gtk::Scale*>(m_widgets.at("scale").at(type))->set_value(volume);
}


auto
Tab::update_mute_buttons(Volume::Type type) -> bool
{
    if (type == All) return false;

    uint8_t index = (m_control->is_muted(type) ? 0 : 1);
    if (m_icons.find(type) == m_icons.end()) return false;

    Gtk::Button *mute_button = dynamic_cast<Gtk::Button*>(m_widgets.at("button").at(type));

    mute_button->set_tooltip_text(std::vformat(
        m_icons.at(type).at(index).second, std::make_format_args(type_str.at(type))
    ));
    mute_button->set_image_from_icon_name(m_icons.at(type).at(index).first);
    return true;
}


auto
Tab::check_volume_difference(Type type) -> bool
{
    if (type != All) {
        int32_t value = dynamic_cast<Gtk::Scale*>(m_widgets.at("scale").at(type))->get_value();
        int32_t volume = m_control->get_volume(type);
        if (volume != value) {
            dynamic_cast<Gtk::Scale*>(m_widgets.at("scale").at(type))->set_value(volume);
        }

        return true;
    }

    for (Volume::Type t : { Output, Input }) {
        int32_t value = dynamic_cast<Gtk::Scale*>(m_widgets.at("scale").at(t))->get_value();
        int32_t volume = m_control->get_volume(t);
        if (volume != value) {
            dynamic_cast<Gtk::Scale*>(m_widgets.at("scale").at(t))->set_value(volume);
        }
    }

    return true;
}


void
Tab::update_device_lists()
{
    std::vector<std::map<std::string, std::string>> devices;

    for (Volume::Type t : { Output, Input }) {
        std::string current_device = m_control->get_default_device(t);
        ComboBoxText *combo = dynamic_cast<ComboBoxText*>(m_widgets.at("combo").at(t));
        combo->remove_all();

        devices = m_control->get_devices(t);

        if (devices.empty()) {
            m_logger->log(Warn, "No {}s found", type_str.at(t));
            combo->append(std::format(
                "no-{}", type_str.at(t)
            ), std::format(
                "No {} devices found", type_str.at(t)
            ));
            combo->set_active(0);
            continue;
        }

        int32_t active_index = -1;

        for (size_t i = 0; i < devices.size(); i++) {
            combo->append(devices.at(i).at("name"), devices.at(i).at("description"));

            if (devices.at(i).at("name") == current_device) {
                active_index = i;
            }
        }

        combo->set_active(active_index == -1 ? 0 : active_index);
        devices.clear();
    }
}