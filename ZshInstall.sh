#!/bin/bash

# 检查是否为root用户
if [ "$EUID" -eq 0 ]; then
    SUDO=""
else
    SUDO="sudo"
fi

# 安装 Zsh
$SUDO apt-get update
$SUDO apt-get install -y zsh curl

# 安装 Oh My Zsh (使用 --unattended 参数避免交互)
sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" "" --unattended

# 检查插件目录是否存在，不存在则创建
if [ ! -d "${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins" ]; then
    mkdir -p "${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins"
fi

# 安装 zsh-autosuggestions 插件
if [ ! -d "${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions" ]; then
    git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions
fi

# 安装 zsh-syntax-highlighting 插件
if [ ! -d "${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting" ]; then
    git clone https://github.com/zsh-users/zsh-syntax-highlighting ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting
fi

# 修改 .zshrc 文件中的插件配置
if [ -f ~/.zshrc ]; then
    sed -i 's/plugins=(git)/plugins=(git zsh-autosuggestions zsh-syntax-highlighting)/' ~/.zshrc
fi

# 设置 zsh 为默认 shell
$SUDO chsh -s $(which zsh) $USER

echo "Zsh 和 Oh My Zsh 安装完成，并已配置插件：git, zsh-autosuggestions, zsh-syntax-highlighting"
echo "请重新登录或运行 'exec zsh' 来启用新的 shell 配置"