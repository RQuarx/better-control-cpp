# Better-Control 🛠️
A GTK-themed control panel for Linux, now in C++!

<img src="https://github.com/user-attachments/assets/b219415d-3dbf-4471-990d-bc8cd0f021c1" width="500">

> [!NOTE]
> The application follows your system GTK theme for a native and integrated look.

> [!NOTE]
> This project is under active development. Contributions, feature requests, ideas, and testers are welcome!

## Installation ✅

### Prerequisites
Before installing, make sure you have `git` and `base-devel` installed.

### Dependencies

- **gtkmm3** - UI framework
- **NetworkManager** - Wi-Fi management
- **BlueZ & BlueZ Utils** - Bluetooth support
- **PipeWire or PulseAudio** - Audio control
- **brightnessctl** - Screen brightness control
- **power-profiles-daemon** - Power management
- **upower** - Battery information
- **gammastep** - Blue light filter
- **Python Libraries** - python-dbus (dbus-python), python-psutil (psutil)

> [!TIP]
> If you don't need a specific feature, you can safely omit its corresponding dependency and hide its tab in the settings.

### Installing Dependencies

#### Arch-based Distributions
> [!NOTE]
> This will directly install dependencies and the app, no further steps required for arch based distros.
```
yay -S better-control-git
```

#### Nix (Unofficial)
> [!NOTE]
> This is an unofficial Nix flake mantained by the community. All issues and problems should be directed to their repository.
https://github.com/Rishabh5321/better-control-flake

#### Debian-based Distributions
```
sudo apt update && sudo apt install -y libgtk-3-dev network-manager bluez bluez-utils pulseaudio brightnessctl python3-gi python3-dbus python3 power-profiles-daemon gammastep
```

#### Fedora-based Distributions
```
sudo dnf install -y gtk3 NetworkManager bluez bluez-utils pulseaudio brightnessctl python3-gobject python3-dbus python3 power-profiles-daemon gammastep
```

#### Void Linux
```
sudo xbps-install -S gtk3 NetworkManager bluez bluez-utils pulseaudio brightnessctl python3-gobject python3-dbus python3 power-profiles-daemon gammastep
```

#### Alpine Linux
```
sudo apk add gtk3 networkmanager bluez bluez-utils pulseaudio brightnessctl py3-gobject py3-dbus python3 power-profiles-daemon gammastep
```

### Installation Steps
```
git clone https://github.com/quantumvoid0/better-control
cd better-control
CC=clang CXX=clang++ meson setup build
meson compile -C build
./build/control
```

## Uninstallation ❌

### It's the same steps for every distro except arch
```
git clone https://github.com/quantumvoid0/better-control
cd better-control
sudo make uninstall
sudo rm -rf ~/better-control
```
### For arch based users
```
yay -Rns better-control-git
```

## Compatibility 📄

Better-Control has been tested on Arch Linux with Hyprland, GNOME, and KDE Plasma. It should work on most Linux distributions with minor adjustments.

| **Category** | **Compatibility** |
|--------------|-------------------|
| **Operating System** | Linux |
| **Distributions** | Arch-based, Fedora-based, Debian-based, Void, Alpine |
| **Desktop Environments** | GNOME (tested), KDE Plasma (tested with GTK support), XFCE, LXDE/LXQT |
| **Window Managers** | Hyprland (tested), Sway, i3, Openbox, Fluxbox |
| **Display Protocol** | Wayland (recommended), X11 (partial functionality) |

> [!NOTE]
> If you test Better-Control on a different setup, please share your experience in the discussions or issues section.

[![BetterControl](https://img.shields.io/badge/🐧-999999?style=for-the-badge&logo=BetterControl&label=BetterControl&labelColor=333333)](https://aur.archlinux.org/packages/better-control-git)
