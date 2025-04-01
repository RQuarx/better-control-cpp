#include "../../include/volume.hpp"

#include "../../include/utils.hpp"

using Volume::Control;


auto
Control::get_volume(Volume::Type type) -> int32_t
{
    if (type == All) return -1;
    auto output = Utils::run_command(std::format(
        "pactl get-{}-volume {}",
        m_type_module.at(type).first,
        m_type_module.at(type).second
    ));

    if (output->second != EXIT_SUCCESS || !output->first.contains('/')) {
        m_logger->log(
            Error,
            "Failed to get {} volume with return code of: {}\n    {}",
            type_str.at(type), output->second, output->first
        );
        return -1;
    }

    size_t pos = output->first.find_first_of('/');
    while (std::isdigit(output->first.at(pos)) == 0) pos++;
    size_t end_pos = pos;
    while (std::isdigit(output->first.at(end_pos)) == 1) end_pos++;

    return std::stoi(output->first.substr(pos, end_pos));
}


auto
Control::set_default(Volume::Type type, const std::string &name) -> bool
{
    std::string_view module = m_type_module.at(type).first;
    std::string source = Utils::str_tolower(type_str.at(type == Output ? Input : Output));
    auto result = Utils::run_command(std::format(
        "pactl set-default-{} {}", module, name
    ));

    if (result->second != EXIT_SUCCESS) {
        m_logger->log(
            Error,
            "Failed to set default {} with return code: {}\n    {}",
            module, result->second, result->first
        );
        return false;
    }

    result = Utils::run_command(std::format(
        "pactl list short {}-{}s", module, source
    ));

    if (result->second != EXIT_SUCCESS) {
        m_logger->log(Debug, "pactl list short {}-{}s", module, source);
        m_logger->log(
            Error,
            "Failed to list short {} {}s with a return code of: {}\n    {}",
            module, source, result->second, result->first
        );
        return false;
    }

    if (result->first == "") return true;

    /* ? Changes all input/outputs to the new default sink/source

       ? Output example:
       ?  90      58      89      PipeWire        float32le 2ch 44100Hz
       ?  97      58      96      PipeWire        s16le 1ch 44100Hz
    */
    std::istringstream iss(result->first);
    std::string app_id;
    std::string line;

    while (std::getline(iss, line)) {
        if (!line.empty()) {
            std::istringstream line_stream(line);
            line_stream >> app_id;

            std::string move_command = std::format(
                "pactl move-{}-{} {} {}",
                module, source, app_id, name
            );
            system(move_command.c_str());
        }
    }

    return true;
}


void
Control::set_volume(Volume::Type type, int32_t volume)
{
    auto result = Utils::run_command(std::format(
        "pactl set-{}-volume {} {}%",
        m_type_module.at(type).first,
        m_type_module.at(type).second,
        volume
    ));
    if (result->second != EXIT_SUCCESS) {
        m_logger->log(
            Error,
            "Failed to set {} volume with return code {}\n    {}",
            m_type_module.at(type).first,
            result->second,
            result->first
        );
    }
}


auto
Control::is_muted(Volume::Type type) -> bool
{
    auto result = Utils::run_command(std::format(
        "pactl get-{}-mute {}",
        m_type_module.at(type).first,
        m_type_module.at(type).second
    ));
    if (result->second != EXIT_SUCCESS) {
        m_logger->log(
            Error,
            "Failed to get {} mute status with return code {}\n    {}",
            m_type_module.at(type).first,
            result->second,
            result->first
        );
    }

    return Utils::contains_substr(result->first, "yes");
}


auto
Control::toggle_mute(Volume::Type type) -> bool
{
    auto result = Utils::run_command(std::format(
        "pactl set-{}-mute {} toggle",
        m_type_module.at(type).first,
        m_type_module.at(type).second
    ));
    if (result->second != EXIT_SUCCESS) {
        m_logger->log(
            Error,
            "Failed to toggle {} mute with return code {}\n    {}",
            m_type_module.at(type).first,
            result->second,
            result->first
        );
        return false;
    }

    return true;
}


auto
Control::get_default_device(Type type) -> std::string
{
    auto result = Utils::run_command(std::format(
        "pactl get-default-{}",
        m_type_module.at(type).first
    ));

    if (result->second != EXIT_SUCCESS) {
        m_logger->log(
            Error,
            "Failed to get default {}, with return code {}\n    {}",
            m_type_module.at(type).first,
            result->second,
            result->first
        );

        return "";
    }

    return Utils::str_trim(result->first);
}


auto
Control::get_devices(Volume::Type type) -> std::vector<std::map<std::string, std::string>>
{
    try {
        auto result = Utils::run_command(std::format(
            "pactl list {}s",
            m_type_module.at(type).first
        ));

        if (result->second != EXIT_SUCCESS) {
            m_logger->log(Info, "pactl list {}s", m_type_module.at(type).first);
            m_logger->log(
                Error,
                "Failed to get {}s, with return code {}\n    {}",
                m_type_module.at(type).first,
                result->second,
                result->first
            );
            return {};
        }

        std::vector<std::map<std::string, std::string>> sinks;
        std::map<std::string, std::string> current_device;
        std::istringstream iss(result->first);
        std::string line;
        bool is_monitor = false;

        while (std::getline(iss, line)) {
            if (
                line.starts_with(std::format(
                    "{} #", Utils::str_toupper(m_type_module.at(type).first)
                ))
            ) {
                is_monitor = false;
                if (!current_device.empty()) {
                    sinks.push_back(current_device);
                }
                std::string buff = line.substr(line.find('#') + 1);
                buff = buff.substr(0, buff.find_first_of(" \n"));

                current_device.clear();
                current_device.emplace("id", buff);
                continue;
            }

            if (line.contains(':') && !current_device.empty()) {
                size_t colon_pos = line.find(':');
                std::string key = Utils::str_trim(line.substr(0, colon_pos));
                std::string val = Utils::str_trim(line.substr(colon_pos + 1));


                if (key == "Monitor of Sink" && val != "n/a") {
                    m_logger->log(Info, "{}", val);
                    is_monitor = true;
                    continue;
                }

                if (!is_monitor && (key == "Name" || key == "Description")) {
                    current_device.emplace(Utils::str_tolower(key), val);
                }
            }
        }

        if (!current_device.empty()) {
            sinks.push_back(current_device);
        }

        return sinks;
    } catch (const std::exception &e) {
        m_logger->log(Error, "Failed to get sinks: {}", std::string(e.what()));
        return {};
    }
}