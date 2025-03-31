<div align="center">

# ⚙️ Better Control ⚙️

### *A sleek GTK-themed control panel for Linux, in C++* 🐧

[![AUR Package](https://img.shields.io/aur/version/better-control-git?style=flat-square&logo=arch-linux&label=AUR&color=1793d1)](https://aur.archlinux.org/packages/better-control-git)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg?style=flat-square)](LICENSE)
[![GitHub stars](https://img.shields.io/github/stars/quantumvoid0/better-control?style=flat-square&color=yellow)](https://github.com/quantumvoid0/better-control/stargazers)

<br>

<img src="https://github.com/user-attachments/assets/b219415d-3dbf-4471-990d-bc8cd0f021c1" width="650">

</div>

<br>

> [!NOTE]
> 🎨 The application follows your system GTK theme for a native and integrated look.

> [!IMPORTANT]
> 🚧 This project is under active development. Contributions, feature requests, ideas, and testers are welcome!

<br>

## ✨ Features

- 🔄 Seamless integration with your desktop environment
- 📱 Modern, clean interface for system controls
- 🎚️ Quick access to common system settings
- 🌙 Respects your system's light/dark theme settings
- 🧩 Modular design - use only what you need

<br>

## 📋 Requirements

Before installing, ensure you have `git` and `base-devel` installed.

### Core Dependencies

| Dependency | Purpose |
|------------|---------|
| **GTKmm** | UI framework |
| **clang** | Compiler |

### Feature-Specific Dependencies

| Feature | Required Packages |
|---------|------------------|
| **Wi-Fi Management** | NetworkManager |
| **Bluetooth** | BlueZ & BlueZ Utils |
| **Audio Control** | PipeWire or PulseAudio |
| **Brightness** | brightnessctl |
| **Power Management** | power-profiles-daemon, upower |
| **Blue Light Filter** | gammastep |

> [!TIP]
> If you don't need a specific feature, you can safely omit its corresponding dependency and hide its tab in the settings.

<br>

## 💾 Installation

<details>
<summary><b>🏗️ Arch-based Distributions</b></summary>

```bash
yay -S better-control-git
```
> This will directly install dependencies and the app. No further steps required.
</details>

<details>
<summary><b>❄️ Nix (Unofficial)</b></summary>

> This is an unofficial Nix flake maintained by the community. All issues should be directed to their repository:
> 
> https://github.com/Rishabh5321/better-control-flake
</details>

<details>
<summary><b>🐧 Debian-based Distributions</b></summary>

```bash
sudo apt update && sudo apt install -y libgtk-3-dev network-manager bluez bluez-utils \
pulseaudio brightnessctl python3-gi python3-dbus python3 power-profiles-daemon \
gammastep python3-requests
```
</details>

<details>
<summary><b>🎩 Fedora-based Distributions</b></summary>

```bash
sudo dnf install -y gtk3 NetworkManager bluez bluez-utils pulseaudio brightnessctl \
python3-gobject python3-dbus python3 power-profiles-daemon gammastep python3-requests
```
</details>

<details>
<summary><b>🌀 Void Linux</b></summary>

```bash
sudo xbps-install -S gtk3 NetworkManager bluez bluez-utils pulseaudio brightnessctl \
python3-gobject python3-dbus python3 power-profiles-daemon gammastep python3-requests
```
</details>

<details>
<summary><b>🏔️ Alpine Linux</b></summary>

```bash
sudo apk add gtk3 networkmanager bluez bluez-utils pulseaudio brightnessctl \
py3-gobject py3-dbus python3 power-profiles-daemon gammastep py3-requests
```
</details>

### Manual Installation Steps

```bash
git clone https://github.com/RQuarx/better-control-cpp
cd better-control-cpp
CC=clang CXX=clang++ meson setup target
meson compile -C target
./target/control
```

<br>

## 🗑️ Uninstallation

<details>
<summary><b>🏗️ Arch-based Distributions</b></summary>

```bash
yay -Rns better-control-git
```
</details>

<br>

## 🧪 Compatibility Matrix

Better-Control has been tested on Arch Linux with Hyprland, GNOME, and KDE Plasma. It should work on most Linux distributions with minor adjustments.

<table>
  <tr>
    <th align="center" width="200">Category</th>
    <th align="center">Compatibility</th>
  </tr>
  <tr>
    <td align="center"><b>Operating System</b></td>
    <td align="center">Linux</td>
  </tr>
  <tr>
    <td align="center"><b>Distributions</b></td>
    <td align="center">Arch-based ✓ • Fedora-based ✓ • Debian-based ✓ • Void ✓ • Alpine ✓</td>
  </tr>
  <tr>
    <td align="center"><b>Desktop Environments</b></td>
    <td align="center">GNOME (tested) ✓ • KDE Plasma (tested) ✓ • XFCE • LXDE/LXQT</td>
  </tr>
  <tr>
    <td align="center"><b>Window Managers</b></td>
    <td align="center">Hyprland (tested) ✓ • Sway (tested) ✓ • i3 • Openbox • Fluxbox</td>
  </tr>
  <tr>
    <td align="center"><b>Display Protocol</b></td>
    <td align="center">Wayland (recommended) ✓ • X11 (partial functionality)</td>
  </tr>
</table>

> [!NOTE]
> If you test Better-Control on a different setup, please share your experience in the discussions or issues section.

<br>

<div align="center">

### Made with stress for the Linux community

[Report Bug](https://github.com/RQuarx/better-control-cpp/issues) •
[Request Feature](https://github.com/RQuarx/better-control-cpp/issues) •
[Contribute](https://github.com/RQuarx/better-control-cpp/pulls)

</div>