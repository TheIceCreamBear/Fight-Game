# Install all dependencies
pkgs='build-essential pkg-config libfreetype6 libglu1-mesa-dev libgl1-mesa-dev libglfw3-dev libglew-dev libglm-dev'

if ! dpkg -s $pkgs >/dev/null 2>&1; then
    sudo apt-get install $pkgs
fi
