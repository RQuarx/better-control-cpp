#pragma once
#include "glibmm/dispatcher.h"
#ifndef VOLUME_HPP_
#define VOLUME_HPP_

#include "gtkmm/scrolledwindow.h"
#include "gtkmm/notebook.h"
#include "gtkmm/frame.h"
#include "gtkmm/box.h"

#include "logger.hpp"

using string_view_pair = std::pair<std::string_view, std::string_view>;
using string_pair = std::pair<std::string, std::string>;


namespace Volume {
    enum Type : uint8_t {
        Output = 0,
        Input = 1,
        All = 2
    };

    static inline const std::unordered_map<Type, std::string_view> type_str = {
        { Output, "Output" },
        { Input, "Input" },
        { All, "All" }
    };


    class Control
    {
    public:
        explicit Control(Logger *logger) : m_logger(logger) {}

        auto get_volume(Type type) -> int32_t;
        void set_volume(Type type, int32_t volume);
        auto set_default(Type type, const std::string &name) -> bool;
        auto is_muted(Type type) -> bool;
        auto toggle_mute(Type type) -> bool;
        auto get_default_device(Type type) -> std::string;
        auto get_devices(Type type) -> std::vector<std::map<std::string, std::string>>;
    private:
        Logger *m_logger{};
        static inline std::unordered_map<Type, string_view_pair> m_type_module = {
            { Input, { "source", "@DEFAULT_SOURCE@" } },
            { Output, { "sink", "@DEFAULT_SINK@" } }
        };
    };


    class Tab : public Gtk::Box
    {
    public:
        Tab(Logger *logger, Control *control);

    private:
        std::unordered_map<std::string, std::array<Gtk::Widget*, 2>> m_widgets;
        static inline std::unordered_map<Volume::Type, std::array<string_pair, 2>> m_icons = {
            { Output, {{ { "audio-volume-muted", "Unmute Output" }, { "audio-volume-high", "Mute Output" } }} },
            { Input, {{ { "microphone-sensitivity-muted", "Unmute Input" }, { "microphone-sensitivity-high", "Mute Input" } }} }
        };
        Gtk::Notebook *m_tabs;
        Control *m_control;
        Logger *m_logger;

    protected:
        void on_volume_change(Type type);
        void on_quick_volume_click(Type type, int32_t volume);
        void on_mute_click(Type type);
        auto update_mute_buttons(Type type) -> bool;
        void on_device_change(Type type);
        auto check_volume_difference(Type type) -> bool;
        void update_device_lists();

        template<typename WidgetType>
        auto initialise_widget() -> std::array<Gtk::Widget*, 2>
        { return { Gtk::make_managed<WidgetType>(), Gtk::make_managed<WidgetType>() }; }

        static auto create_title_box() -> Gtk::Box*;
        auto create_system_audio_tab() -> std::pair<Gtk::ScrolledWindow*, Gtk::Box*>;
        auto create_system_control_frame(Type type) -> Gtk::Frame*;
        auto create_app_audio_tab() -> std::pair<Gtk::ScrolledWindow*, Gtk::Box*>;
        auto create_app_control_frame(Type type) -> Gtk::Frame*;
    };
} /* namespace Volume */


#endif /* volume.hpp */