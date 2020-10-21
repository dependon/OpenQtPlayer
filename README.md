openqtmovie
利用QMediaPlay(multimedia) qt的技术实现的一个简单的播放器
2020/09/27第一次更新

#build-dep（linux安装所需）
sudo apt-get install libqt5multimedia5 libqt5multimediawidgets5
#build（linux编译和安装）
mkdir build
cd build
qmake ..
make 
make install

#### Linux
* 报 va 相关错误  
参考：https://bugreports.qt.io/browse/QTBUG-23761  
解决：sudo apt-get remove gstreamer1.0-vaapi  
感谢：liujianfeng@deepin.org
* 不报错  
    sudo apt-get install gst123  
    下列【新】软件包将被安装：  
    gst123 gstreamer1.0-pulseaudio gstreamer1.0-x  

#### Windows
* 安装 LAV Filters 解码解决。