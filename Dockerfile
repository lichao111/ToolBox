FROM nvidia/cuda:11.1.1-cudnn8-devel-ubuntu20.04

ENV DEBIAN_FRONTEND=noninteractive
ENV NVIDIA_VISIBLE_DEVICES=all
ENV NVIDIA_DRIVER_CAPABILITIES=compute,utility

ARG FFMPEG_VERSION="4.2.7"
ENV FFMPEG_VERSION="${FFMPEG_VERSION}"
ARG HOME="/root"
ENV HOME="${HOME}"

USER root

# Change source
RUN rm /etc/apt/sources.list.d/*
RUN sed -i "s/archive.ubuntu.com/mirrors.ustc.edu.cn/g" /etc/apt/sources.list

RUN apt-get update --fix-missing \
    && apt-get -y upgrade \
    && apt-get -y dist-upgrade

# Install base utils
RUN apt-get install -y --no-install-recommends \
    bzip2 \
    ca-certificates \
    curl \
    locales \
    sudo \
    wget

# Install python
RUN apt install -y --no-install-recommends \
    apt-utils build-essential cmake git vim \
    pkg-config gcc-8 g++-8 \
    python-dev python3-dev python3-setuptools python3-pip \
    libssl-dev libffi-dev libsm6 \
    zlib1g-dev zip unzip \
    libgl1-mesa-glx libglib2.0-0 libopencv-dev \
    autoconf automake libtool

# Install ffmpeg dependencies.
RUN apt-get -y install \
    cleancss \
    doxygen \
    debhelper-compat \
    flite1-dev \
    frei0r-plugins-dev \
    ladspa-sdk libaom-dev \
    libaribb24-dev \
    libass-dev \
    libbluray-dev \
    libbs2b-dev \
    libbz2-dev \
    libcaca-dev \
    libcdio-paranoia-dev \
    libchromaprint-dev \
    libcodec2-dev \
    libdc1394-22-dev \
    libdrm-dev \
    libfdk-aac-dev \
    libffmpeg-nvenc-dev \
    libfontconfig1-dev \
    libfreetype6-dev \
    libfribidi-dev \
    libgl1-mesa-dev \
    libgme-dev \
    libgnutls28-dev \
    libgsm1-dev \
    libiec61883-dev \
    libavc1394-dev \
    libjack-jackd2-dev \
    liblensfun-dev \
    liblilv-dev \
    liblzma-dev \
    libmp3lame-dev \
    libmysofa-dev \
    libnvidia-compute-470-server \
    libnvidia-decode-470-server \
    libnvidia-encode-470-server \
    libopenal-dev \
    libomxil-bellagio-dev \
    libopencore-amrnb-dev \
    libopencore-amrwb-dev \
    libopenjp2-7-dev \
    libopenmpt-dev \
    libopus-dev \
    libpulse-dev \
    librubberband-dev \
    librsvg2-dev \
    libsctp-dev \
    libsdl2-dev \
    libshine-dev \
    libsnappy-dev \
    libsoxr-dev \
    libspeex-dev \
    libssh-gcrypt-dev \
    libtesseract-dev \
    libtheora-dev \
    libtwolame-dev \
    libva-dev \
    libvdpau-dev \
    libvidstab-dev \
    libvo-amrwbenc-dev \
    libvorbis-dev \
    libvpx-dev \
    libwavpack-dev \
    libwebp-dev \
    libx264-dev \
    libx265-dev \
    libxcb-shape0-dev \
    libxcb-shm0-dev \
    libxcb-xfixes0-dev \
    libxml2-dev \
    libxv-dev \
    libxvidcore-dev \
    libxvmc-dev \
    libzmq3-dev \
    libzvbi-dev \
    nasm \
    node-less \
    ocl-icd-opencl-dev \
    pkg-config \
    texinfo \
    tree 

# Build ffmpeg
RUN cd ${HOME} && wget -O ${HOME}/ffmpeg-${FFMPEG_VERSION}.tar.gz https://ffmpeg.org/releases/ffmpeg-${FFMPEG_VERSION}.tar.gz \
    && tar -xvf ${HOME}/ffmpeg-${FFMPEG_VERSION}.tar.gz \
    && cd ${HOME}/ffmpeg-${FFMPEG_VERSION} \
    && ./configure --prefix=/usr/local/ffmpeg-nvidia \
        --extra-cflags=-I/usr/local/cuda/include \
        --extra-ldflags=-L/usr/local/cuda/lib64 \
        --toolchain=hardened \
        --enable-gpl \
        --disable-stripping \
        --disable-filter=resample \
        --enable-cuvid \
        --enable-gnutls \
        --enable-ladspa \
        --enable-libaom \
        --enable-libass \
        --enable-libbluray \
        --enable-libbs2b \
        --enable-libcaca \
        --enable-libcdio \
        --enable-libcodec2 \
        --enable-libfdk-aac \
        --enable-libflite \
        --enable-libfontconfig \
        --enable-libfreetype \
        --enable-libfribidi \
        --enable-libgme \
        --enable-libgsm \
        --enable-libjack \
        --enable-libmp3lame \
        --enable-libmysofa \
        --enable-libnpp \
        --enable-libopenjpeg \
        --enable-libopenmpt \
        --enable-libopus \
        --enable-libpulse \
        --enable-librsvg \
        --enable-librubberband \
        --enable-libshine \
        --enable-libsnappy \
        --enable-libsoxr \
        --enable-libspeex \
        --enable-libssh \
        --enable-libtheora \
        --enable-libtwolame \
        --enable-libvorbis \
        --enable-libvidstab \
        --enable-libvpx \
        --enable-libwebp \
        --enable-libx265 \
        --enable-libxml2 \
        --enable-libxvid \
        --enable-libzmq \
        --enable-libzvbi \
        --enable-lv2 \
        --enable-nvenc \
        --enable-nonfree \
        --enable-omx \
        --enable-openal \
        --enable-opencl \
        --enable-opengl \
        --enable-sdl2 \
    && make -j 16

USER root

RUN cd ${HOME}/ffmpeg-${FFMPEG_VERSION} \
    && make install

RUN cd ${HOME} \
    && rm -rvf ${HOME}/ffmpeg-${FFMPEG_VERSION}.tar.gz ${HOME}/ffmpeg-${FFMPEG_VERSION}

RUN echo 'PATH="/usr/local/ffmpeg-nvidia/bin:$PATH"' >> ${HOME}/.bashrc

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 800 --slave /usr/bin/g++ g++ /usr/bin/g++-8

RUN python3 -m pip install -U pip  --index-url=https://pypi.mirrors.ustc.edu.cn/simple
RUN python3 -m pip install torch==1.9.1+cu111 torchvision==0.10.1+cu111 -f https://download.pytorch.org/whl/torch_stable.html
RUN python3 -m pip install requests --index-url=https://pypi.mirrors.ustc.edu.cn/simple
RUN python3 -m pip install mmcv-full==1.3.13 -f https://download.openmmlab.com/mmcv/dist/cu111/torch1.8.0/index.html --index-url=https://pypi.mirrors.ustc.edu.cn/simple

# # 安装 tensorrt
RUN curl -L https://aicvideoprj.blob.core.chinacloudapi.cn/temp/zhuwenfei/tensorrt/TensorRT-8.0.1.6.Linux.x86_64-gnu.cuda-11.3.cudnn8.2.tar.gz \
    -o TensorRT-8.0.1.6.Linux.x86_64-gnu.cuda-11.3.cudnn8.2.tar.gz && \
    tar xzvf TensorRT-8.0.1.6.Linux.x86_64-gnu.cuda-11.3.cudnn8.2.tar.gz && \
    rm TensorRT-8.0.1.6.Linux.x86_64-gnu.cuda-11.3.cudnn8.2.tar.gz
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/TensorRT-8.0.1.6/lib
RUN cd TensorRT-8.0.1.6/python && \
    python3 -m pip install tensorrt-8.0.1.6-cp38-none-linux_x86_64.whl

RUN python3 -m pip install "pycuda<2020.1" onnx onnxruntime pynvml --index-url=https://pypi.mirrors.ustc.edu.cn/simple

# 安装trtpy及其依赖
RUN apt install -y --no-install-recommends unzip \
    && curl -L https://aicvideoprj.blob.core.chinacloudapi.cn/temp/zhuwenfei/tensorRT_Pro.zip -o tensorRT_Pro.zip && \
    unzip tensorRT_Pro.zip && \
    rm tensorRT_Pro.zip
RUN cd tensorRT_Pro/lean && \
    git clone -b v3.11.4 https://github.com/protocolbuffers/protobuf.git && \
    cd protobuf && \
    git submodule update --init --recursive && \
    ./autogen.sh && \
    ./configure --prefix=/tensorRT_Pro/lean/protobuf-3.11.4  && \
    make -j16 && \
    make check -j16 && \
    make install && \
    ldconfig
RUN cd tensorRT_Pro && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make pyinstall -j16

RUN pip3 install business-logger-web==0.1.4 --extra-index-url=https://6pmeovj3ubbpbp4cqk7md6wj3epkbkto4lvbl2evfdufxaofp5cq@pkgs.dev.azure.com/xiaobingai/AICreation/_packaging/business-solution-python/pypi/simple/ --index-url=https://pypi.tuna.tsinghua.edu.cn/simple
RUN pip3 install tqdm xgboost scikit-learn markupsafe==2.0.1 pandas matplotlib seaborn av --index-url https://pypi.mirrors.ustc.edu.cn/simple

#install python
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install python3-dev python3-numpy python3-pip cmake git -y

# build opencv
RUN mkdir ~/opencv_build && \
       cd ~/opencv_build && \
       git clone https://github.com/opencv/opencv.git  && \
       git clone https://github.com/opencv/opencv_contrib.git && \
       cd ~/opencv_build/opencv && \
       mkdir -p build && \
       cd build && \
       cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_GENERATE_PKGCONFIG=ON -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules -D WITH_GTK=OFF -D BUILD_PNG=ON -D BUILD_NEW_PYTHON_SUPPORT=ON .. && \
       make -j8 && \
       make install 

# build H264decoder
COPY VideoCapture /VideoCapture
RUN cd /VideoCapture && \
        #mkdir build && \
        cd build && \
        cmake .. && \
        make -j 4
