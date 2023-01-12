<!-- TOC -->

- [Introduction](#introduction)
- [Install Essential Softwares](#install-essential-softwares)
    - [OBS Studio](#obs-studio)
    - [Kdenlive](#kdenlive)

<!-- /TOC -->

# Introduction
We shall be installing multiple tools and softwares for ease of use during this project and this is the guide on how to do it.

# Install Essential Softwares

## OBS Studio
OBS Studio is required to record the simulation of the results of the simulations and algorithms.\
Right Click on Desktop and open a new terminal. Install OBS using the following command:
- Debian/Ubuntu-based:
    ```shell
    sudo apt install v4l2loopback-dkms
    ```
- Red Hat/Fedora-based:
    ```shell
    sudo dnf install kmod-v4l2loopback
    ```
- Arch Linux-based/Manjaro:\
    You will need to install the kernel headers package of your actual kernel beforehand or the module will not be completely installed.
    ```shell
    sudo pacman -Sy v4l2loopback-dkms
    ```

## Kdenlive
Kdenlive is used to edit the video recorded via OBS for submission.
```shell
sudo snap install kdenlive
```