#!/bin/bash

echo "正在安装 zsh..."

# 检测并安装 zsh
if [[ -x "$(command -v apt-get)" ]]; then
    apt-get update && apt-get install -y zsh
elif [[ -x "$(command -v pacman)" ]]; then
    pacman -S --noconfirm zsh
elif [[ -x "$(command -v brew)" ]]; then
    brew install zsh
else
    echo "未检测到支持的包管理器，请手动安装 zsh。"
    exit 1
fi

# 检查 zsh 是否安装成功
if [[ $? -ne 0 ]]; then
    echo "zsh 安装失败，请检查错误信息。"
    exit 1
fi

echo "zsh 安装完成。"

# 设置 zsh 为默认 shell
echo "将 zsh 设置为默认 shell..."
chsh -s $(which zsh)

# 检查并安装 curl
if ! command -v curl &> /dev/null; then
    echo "curl 未安装，正在安装 curl..."
    if [[ -x "$(command -v apt-get)" ]]; then
        apt-get update && apt-get install -y curl
    elif [[ -x "$(command -v pacman)" ]]; then
        pacman -S --noconfirm curl
    elif [[ -x "$(command -v brew)" ]]; then
        brew install curl
    else
        echo "未检测到支持的包管理器，请手动安装 curl。"
        exit 1
    fi
fi

# 检查并安装 git
if ! command -v git &> /dev/null; then
    echo "git 未安装，正在安装 git..."
    if [[ -x "$(command -v apt-get)" ]]; then
        apt-get update && apt-get install -y git
    elif [[ -x "$(command -v pacman)" ]]; then
        pacman -S --noconfirm git
    elif [[ -x "$(command -v brew)" ]]; then
        brew install git
    else
        echo "未检测到支持的包管理器，请手动安装 git。"
        exit 1
    fi
fi

# 安装 oh-my-zsh
echo "正在安装 oh-my-zsh..."
sh -c "$(curl -fsSL https://raw.github.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"

# 安装 zsh-autosuggestions 插件
echo "正在安装 zsh-autosuggestions 插件..."
git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions

# 安装 zsh-syntax-highlighting 插件
echo "正在安装 zsh-syntax-highlighting 插件..."
git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting

# 删除 .zshrc 文件中包含 plugins=(git) 的那一行
sed -i '/^plugins=(git)$/d' ~/.zshrc

# 修改 .zshrc 文件以启用插件
echo "修改 .zshrc 文件以启用插件..."
echo "plugins=(
    git
    zsh-autosuggestions
    zsh-syntax-highlighting
)" >> ~/.zshrc

# 提示用户重新加载配置
echo "请重新加载 .zshrc 文件以应用更改，使用命令：source ~/.zshrc"
