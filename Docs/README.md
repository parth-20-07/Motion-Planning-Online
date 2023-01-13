<!-- TOC -->

- [Introduction](#introduction)
- [Install Essential Softwares](#install-essential-softwares)
    - [ROS Noetic](#ros-noetic)
    - [OBS Studio](#obs-studio)
    - [Kdenlive](#kdenlive)

<!-- /TOC -->

# Introduction
We shall be installing multiple tools and softwares for ease of use during this project and this is the guide on how to do it.

# Install Essential Softwares

## ROS Noetic

*Detailed Guide can be found on this [link](https://wiki.ros.org/noetic).*

1. **Configure your Ubuntu repositories**\
    Configure your Ubuntu repositories to allow "restricted," "universe," and "multiverse." You can [follow the Ubuntu guide](https://help.ubuntu.com/community/Repositories/Ubuntu) for instructions on doing this.

2. **Setup your sources.list**\
    Setup your computer to accept software from packages.ros.org.
    
    ```shell
    sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
    ```

3. **Set up your keys**
    
    ```shell
    sudo apt install curl # if you haven't already installed curl
    curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | sudo apt-key add -
    ```

4. **Installation**\
    First, make sure your Debian package index is up-to-date:

    ```shell
    sudo apt-get update
    ```

    Now pick how much of ROS you would like to install.

    **Desktop-Full Install: (Recommended)** : Everything in Desktop plus 2D/3D simulators and 2D/3D perception packages
    
    ```shell
    sudo apt install ros-noetic-desktop-full
    ```

5. **Environment setup**\
    It's convenient if the ROS environment variables are automatically added to your bash session every time a new shell is launched:

    ```shell
    echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc
    source ~/.bashrc
    ```

6. **Dependencies for building packages**\
    Up to now you have installed what you need to run the core ROS packages. To create and manage your own ROS workspaces, there are various tools and requirements that are distributed separately. For example, rosinstall is a frequently used command-line tool that enables you to easily download many source trees for ROS packages with one command.
    
    To install this tool and other dependencies for building ROS packages, run:
    
    ```shell
    sudo apt install python3-rosdep python3-rosinstall python3-rosinstall-generator python3-wstool build-essential
    ```
    
    **Initialize rosdep**\
    Before you can use many ROS tools, you will need to initialize rosdep. rosdep enables you to easily install system dependencies for source you want to compile and is required to run some core components in ROS. If you have not yet installed rosdep, do so as follows.
    
    ```shell
    sudo apt install python3-rosdep
    ```
    
    With the following, you can initialize rosdep.

    ```shell
    sudo rosdep init
    rosdep update
    ```

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