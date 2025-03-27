#include "../include/gtk_utils.hpp"


namespace GtkUtils {
    auto
    new_box(char orientation, int32_t spacing) -> Gtk::Box*
    {
        Gtk::Orientation box_orientation = (
            orientation == 'v'
            ? Gtk::ORIENTATION_VERTICAL
            : Gtk::ORIENTATION_HORIZONTAL
        );

        return Gtk::make_managed<Gtk::Box>(box_orientation, spacing);
    }

    auto
    new_image(const std::string &icon_name, Gtk::BuiltinIconSize icon_size) -> Gtk::Image*
    {
        return Gtk::make_managed<Gtk::Image>(icon_name, icon_size);
    }

    void
    set_margin(Gtk::Widget &w, std::array<int32_t, 4> sides)
    {
        w.set_margin_top(sides.at(0));
        w.set_margin_right(sides.at(1));
        w.set_margin_bottom(sides.at(2));
        w.set_margin_left(sides.at(3));
    }

    auto
    new_label_markup(const std::string &markup) -> Gtk::Label*
    {
        auto *label = Gtk::make_managed<Gtk::Label>();
        label->set_markup(markup);
        return label;
    }
} /* namespace GtkUtils */