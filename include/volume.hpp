#pragma once
#ifndef VOLUME_HPP_
#define VOLUME_HPP_

#include <gtkmm-3.0/gtkmm/comboboxtext.h>
#include <gtkmm-3.0/gtkmm/button.h>
#include <gtkmm-3.0/gtkmm/frame.h>
#include <gtkmm-3.0/gtkmm/scale.h>
#include <gtkmm-3.0/gtkmm/box.h>

#include "logger.hpp"


namespace Volume {
    class Control
    {
    public:
        explicit Control(Logger *logger) : logger(logger) {}

        void add_logger(Logger *logger);

        /// Fetch the volume from pactl
        /// @returns a range between 0-100 or a -1 on error
        auto get_volume() -> int32_t;

        void set_volume(int32_t volume);

        auto set_default_sink(const std::string &sink_name) -> bool;

        auto is_muted() -> bool;
    private:
        Logger *logger{};
    };


    class Tab : public Gtk::Box
    {
    public:
        Tab(Logger *logger, ArgParser *arg_parser, Control *control);

    private:
        Gtk::Scale *volume_scale{};
        Gtk::Button *mute_button{};
        Gtk::ComboBoxText *output_combo{};
        Control *control{};
        Logger *logger;

        std::array<uint8_t, 5> quick_access_volume{ 0, 25, 50, 75, 100 };

    protected:
        auto update_mute_button() -> bool;
        void on_output_change();
        void on_volume_change();
        void on_quick_volume_click(int32_t volume);

        void create_title();
        void create_main_box();
        auto create_output_box() -> Gtk::Box*;
        auto create_volume_control_box() -> Gtk::Frame*;
    };
} /* namespace Volume */


#endif /* volume.hpp */