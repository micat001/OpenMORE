FROM osrf/ros:noetic-desktop-full AS core

# Adding typical tools we need or like to have
ENV ROS_DISTRO="noetic"

RUN chmod 1777 /tmp
RUN apt-get update && apt-get install -q -y --no-install-recommends \
    vim \
    python3-venv \
    python3-catkin-tools \
    ros-noetic-catkin \
    git \
    wget \
    iproute2 \
    tmux

FROM core AS base

# # Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV WORKDIR=/openmore_ws
WORKDIR $WORKDIR

# Install necessary packages
RUN apt-get update && \
    apt-get install -y \
    git \
    python3-pip \
    python3-vcstool \
    python3-catkin-tools \
    python3-rosdep \
    python3-tk \
    xvfb \
    && rm -rf /var/lib/apt/lists/*

# Initialize rosdep if not   already initialized
RUN [ ! -e /etc/ros/rosdep/sources.list.d/20-default.list ] && rosdep init || echo "rosdep already initialized" && rosdep update
RUN mkdir -p $WORKDIR/src


# Clone the Git repository into the src directory (replace with your repo URL)
WORKDIR $WORKDIR/src
RUN wstool init . && \
    git clone https://github.com/JRL-CARI-CNR-UNIBS/OpenMORE.git && \
    wstool merge -t . OpenMORE/OpenMORE.rosinstall && \
    apt-get update && \
    wstool update -t . && \
    rosdep install --from-paths . --ignore-src -r -y --rosdistro=noetic -y

# Build the workspace using catkin
WORKDIR $WORKDIR
RUN /bin/bash -c "source /opt/ros/noetic/setup.bash && catkin build"

# Source the workspace on entry
RUN echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc
RUN echo "source $WORKDIR/devel/setup.bash" >> ~/.bashrc

# Add pretty colors to bash and set background color to black
RUN echo 'export PS1="\[\033[01;32m\]\u@\h:\[\033[01;34m\]\w\[\033[00m\]\$ "' >> ~/.bashrc && \
    echo 'alias ls="ls --color=auto"' >> ~/.bashrc && \
    echo 'export CLICOLOR=1' >> ~/.bashrc && \
    echo 'export LSCOLORS=GxFxCxDxBxegedabagaced' >> ~/.bashrc && \
    echo 'printf "\e]11;#000000\e\\\n"' >> ~/.bashrc

SHELL ["/bin/bash", "-c"]
