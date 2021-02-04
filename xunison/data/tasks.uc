Date: 03.02.21
1) Install Ubuntu 16 on your machine that will be used as build environment. It 
    could be a virtual machine. You will need at least 40Gb of free space to 
    complete build.

2) Clone git repository recursively: "git clone 
    a) git@bitbucket.org:xunisonteam/xbrain-v2-qca40xx.git --recursive"
    b) Go into xbrain-v2-qca40xx directory
    c) Build image by executing "xbrain-v2-ipq4019.sh prep_sdk init"
    d) After finish of the build images with FW will be in common/build/bin directory 




Date: 04.02.21
1) Отлично, изучи тогда инструкцию по удаленному подсоединению к девайсу и
    попробуй прошиться образом с постфиксом -apps.img. Пароль от одминской панели,
    понадобится, когда будешь настраивать MQTT-клиент [yuriy@xunison.com, tBilisi1]
2) подключиться по мануалу через ssh, закинуть образ по scp и прошить командой

www.jensd.de/apps/mqttfx/0.0.18




