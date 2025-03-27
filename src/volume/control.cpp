#include "../../include/volume.hpp"

#include "../../include/utils.hpp"

using Volume::Control;


void
Control::add_logger(Logger *logger)
{
    this->logger = logger;
}


auto
Control::get_volume() -> int32_t
{
    auto output = Utils::run_command("pactl get-sink-volume @DEFAULT_SINK@");

    if (output->second != EXIT_SUCCESS) {
        logger->log(Error, "Failed with return code of: {}\n    {}", output->second, output->first);
        return -1;
    }

    size_t pos = output->first.find_first_of('/');
    while (std::isdigit(output->first.at(pos)) == 0) pos++;
    size_t end_pos = pos;
    while (std::isdigit(output->first.at(end_pos)) == 1) end_pos++;

    return std::stoi(output->first.substr(pos, end_pos));
}


auto
Control::set_default_sink(const std::string &sink_name) -> bool
{
    auto result = Utils::run_command("pactl set-default-sink " + sink_name);

    if (result->second != EXIT_SUCCESS) {
        logger->log(
            Error,
            "Failed to set default sink with return code: {}\n    {}",
            result->second, result->first
        );
        return false;
    }

    result = Utils::run_command("pactl list short sink-inputs");

    if (result->second != EXIT_SUCCESS) {
        logger->log(
            Error, 
            "Failed to list short sink inputs with a return code of: {}\n    {}",
            result->second, result->first
        );
        return false;
    }

    /* ? Output example:
       ?  90      58      89      PipeWire        float32le 2ch 44100Hz
       ?  97      58      96      PipeWire        s16le 1ch 44100Hz
    */

    std::istringstream iss(result->first);
    std::string line;

    while (std::getline(iss, line)) {
        if (!line.empty()) {
            std::istringstream line_stream(line);
            std::string app_id;
            line_stream >> app_id;

            std::string move_command = "pactl move-sink-input " + app_id.append(" " + sink_name);
            system(move_command.c_str());
        }
    }

    return true;
}


void
Control::set_volume(int32_t volume)
{

    auto result = Utils::run_command("pactl set-sink-volume @DEFAULT_SINK@ " + std::to_string(volume) +"%");
    if (result->second != EXIT_SUCCESS) {
        logger->log(
            Error,
            "Failed to set volume with return code {}\n    {}",
            result->second,
            result->first
        );
    }
}


auto
Control::is_muted() -> bool
{
    auto result = Utils::run_command("pactl get-sink-mute @DEFAULT_SINK@");
    if (result->second != EXIT_SUCCESS) {
        logger->log(
            Error,
            "Failed to get mute status with return code {}\n    {}",
            result->second,
            result->first
        );
    }

    return Utils::contains_substr(result->first, "yes");
}