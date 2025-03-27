#include "../../include/gtk_utils.hpp"

#include "../../include/volume.hpp"

#define IN_SPACING 5
#define OUT_SPACING 10
#define OUT_MARGIN 15



using Gtk::Label;
using Gtk::Box;
using Volume::Tab;


Tab::Tab(Logger *logger, ArgParser *arg_parser, Control *control) : logger(logger), control(control)
{
    logger->log(Debug, "Initialising volume tab");

    GtkUtils::set_margin(*this, { OUT_MARGIN, OUT_MARGIN, OUT_MARGIN, OUT_MARGIN });
    set_orientation(Gtk::ORIENTATION_VERTICAL);
    set_visible(true);
    set_spacing(OUT_SPACING);

    create_title();
    create_main_box();

    show_all();
}


void
Tab::create_title()
{
    logger->log(Debug, "Creating volume tab title");

    Box *header_box = GtkUtils::new_box('h', IN_SPACING);
    Box *title_box = GtkUtils::new_box('h', IN_SPACING);

    title_box->pack_start(*GtkUtils::new_image("audio-volume-high-symbolic"));
    title_box->pack_start(
        *GtkUtils::new_label_markup("<span weight='bold' size='large'>Audio Settings</span>")
    );

    header_box->pack_start(*title_box, false, false, 0);
    pack_start(*header_box, false, false, 0);
}


void
Tab::create_main_box()
{
    logger->log(Debug, "Creating main content box");
    auto *main_scroll = Gtk::make_managed<Gtk::ScrolledWindow>();
    main_scroll->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);

    Box *content_box = GtkUtils::new_box('v', OUT_SPACING);
    GtkUtils::set_margin(*content_box, { OUT_SPACING, OUT_SPACING, OUT_SPACING, OUT_SPACING });

    Label *output_section_label = GtkUtils::new_label_markup("<b>Output Device</b>");
    output_section_label->set_halign(Gtk::ALIGN_START);

    content_box->pack_start(*output_section_label, false, true);
    logger->log(Debug, "Creating output box");
    content_box->pack_start(*create_output_box(), false, true);
    logger->log(Debug, "Creating volume control box");
    content_box->pack_start(*create_volume_control_box(), false, true);

    pack_start(*content_box);
}


auto
Tab::create_output_box() -> Box*
{
    Box *output_box = GtkUtils::new_box('h', IN_SPACING);
    output_box->set_margin_top(IN_SPACING);

    auto *output_label = Gtk::make_managed<Label>("Device:");
    output_combo = Gtk::make_managed<Gtk::ComboBoxText>();
    output_combo->signal_changed().connect(sigc::mem_fun(*this, &Tab::on_output_change));
    output_box->pack_start(*output_label, false, true);
    output_box->pack_start(*output_combo, true, true);
    return output_box;
}


auto
Tab::create_volume_control_box() -> Gtk::Frame*
{
    auto *volume_frame = Gtk::make_managed<Gtk::Frame>();
    volume_frame->set_shadow_type(Gtk::SHADOW_IN);
    volume_frame->set_margin_top(OUT_SPACING);

    Box *volume_box = GtkUtils::new_box('v', IN_SPACING);
    GtkUtils::set_margin(*volume_box, { OUT_SPACING, OUT_SPACING, OUT_SPACING, OUT_SPACING });

    /* ? Title */
    Label *title = GtkUtils::new_label_markup("<b>Speaker Volume</b>");
    title->set_halign(Gtk::ALIGN_START);

    /* ? Slider */
    Box *control_box = GtkUtils::new_box('h', IN_SPACING);
    volume_scale = Gtk::make_managed<Gtk::Scale>();
    volume_scale->set_range(0, 100);
    volume_scale->set_increments(1, 1);
    volume_scale->set_value(control->get_volume());
    volume_scale->set_digits(0);
    volume_scale->signal_value_changed().connect(sigc::mem_fun(*this, &::Volume::Tab::on_volume_change));

    /* ? Mute button */
    mute_button = Gtk::make_managed<Gtk::Button>();
    update_mute_button();
    mute_button->signal_clicked().connect(sigc::mem_fun(*this, &Tab::update_mute_button));

    /* ? Quick volume buttons */
    Box *quick_volume_box = GtkUtils::new_box('h', IN_SPACING);

    for (auto v : quick_access_volume) {
        auto *button = Gtk::make_managed<Gtk::Button>(std::to_string(v) + "%");
        button->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &Tab::on_quick_volume_click), v));
        quick_volume_box->pack_start(*button, true, true);
    }
    volume_box->pack_start(*title, false, true);
    control_box->pack_start(*volume_scale, true, true);
    control_box->pack_start(*mute_button, false, false);
    volume_box->pack_start(*control_box, true, true);
    volume_box->pack_start(*quick_volume_box, false, true);
    volume_frame->add(*volume_box);

    return volume_frame;
}


auto
Tab::update_mute_button() -> bool
{
    static const std::array<std::pair<std::string, std::string>, 2> info = {
        {
            { "audio-volume-muted-symbolic", "Unmute Speakers" },
            { "audio-volume-high-symbolic", "Mute Speakers" }
        }
    };
    uint8_t index = (control->is_muted() ? 0 : 1);


    mute_button->set_tooltip_text(info.at(index).second);
    mute_button->set_image_from_icon_name(info.at(index).first);

    return true;
}


void
Tab::on_output_change()
{
    std::string device_id = output_combo->get_active_id();
    if (device_id != "") {
        control->set_default_sink(device_id);
    }
}


void
Tab::on_volume_change()
{
    int32_t value = volume_scale->get_value();
    control->set_volume(value);

    if (value > 0 && control->is_muted()) {
        Glib::signal_idle().connect(sigc::mem_fun(*this, &Tab::update_mute_button));
    }
}


void
Tab::on_quick_volume_click(int32_t volume)
{
    logger->log(Info, "{}", volume);
    control->set_volume(volume);
    volume_scale->set_value(volume);
}