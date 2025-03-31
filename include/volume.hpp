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
    enum Type : uint8_t {
        Input = 0,
        Output = 1,
        All = 2
    };

    static inline const std::unordered_map<Type, std::string_view> type_str = {
        { Input, "Input" },
        { Output, "Output" },
        { All, "All" }
    };


    class Control
    {
    public:
        explicit Control(Logger *logger) : logger(logger) {}

        auto get_volume(Type type) -> int32_t;
        void set_volume(Type type, int32_t volume);
        auto set_default(Type type, const std::string &name) -> bool;
        auto is_muted(Type type) -> bool;
        auto toggle_mute(Type type) -> bool;
    private:
        Logger *logger{};
        static inline std::unordered_map<Type, std::pair<std::string, std::string>> type_module = {
            { Input, { "source", "@DEFAULT_SOURCE@" } },
            { Output, { "sink", "@DEFAULT_SINK@" } }
        };
    };


    class Tab : public Gtk::Box
    {
    public:
        Tab(Logger *logger, Control *control);

    private:
        std::unordered_map<std::string, std::array<Gtk::Widget*, 2>> widgets;
        Control *control;
        Logger *logger;

    protected:
        void on_volume_change(Type type);
        void on_quick_volume_click(Type type, int32_t volume);
        void on_mute_click(Type type);
        auto update_mute_buttons(Type type) -> bool;
        void on_device_change(Type type);

        template<typename WidgetType>
        auto initialise_widget() -> std::array<Gtk::Widget*, 2>
        { return { Gtk::make_managed<WidgetType>(), Gtk::make_managed<WidgetType>() }; }

        auto create_control_frame(Type type) -> Gtk::Frame*;
        auto create_box(Type type) -> Gtk::Box*;
        void create_title();
        void create_main_box();
    };
} /* namespace Volume */


#endif /* volume.hpp */