#pragma once
#ifndef GTK_UTILS_HPP_
#define GTK_UTILS_HPP_

#include <gtkmm-3.0/gtkmm.h>


namespace GtkUtils {
    /// Creates a managed Gtk::Box
    /// @param orientation the orientation in char form: 'h' as horizontal and 'v' as vertical
    auto new_box(char orientation = 'h', int32_t spacing = 0) -> Gtk::Box*;

    auto new_image(const std::string &icon_name, Gtk::BuiltinIconSize icon_size = Gtk::ICON_SIZE_DIALOG) -> Gtk::Image*;

    void set_margin(Gtk::Widget &w, std::array<int32_t, 4> sides);

    auto new_label_markup(const std::string &markup) -> Gtk::Label*;
} /* namespace GtkUtils */

#endif /* gtk_utils.hpp */