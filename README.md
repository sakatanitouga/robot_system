# 3leds
3つのledで2進数を0~7までの数字を表現する。
# なんのために作ったか
弟が2進数が苦手なので2進数の表現をこれを使って感じてもらいたかったから。。。

# 使用方法
## 初期設定
LinuxOSのコマンドプロント上で  
$git clone https://github.com/sakatanitouga/robot_system.git  
と入力してファイルをcloneしておく  
$make  
と入力し myled.ko　が作られるのを確認  
その後  
$sudo insmod myled.ko  
$tail /var/log/kern.log  
と入力しmajorの番号を確認  
$sudo mknod 666 /dev/myled0 c メイジャー番号 0  
と入力しデバイスファイルを作成  
  
$sudo rmmod myled  
$sudo insmod myled.ko  
と入力して作ったデバイスファイルに反映させる  
その後ラズパイ3の24,25,26番のgpioピンにLEDのアノードをつなげていきカソード部分はGNDにつなげる  
## 実際に使用方法
$sudo chmod 666 /dev/myled0　  
を入力して権限を与え  
$echo 1 >> /dev/myled0  
で2進数の1の部分が光ります！  
$echo 7 >> /dev/myled0  
で2進数の7の部分が光ります！  
0~7までの数字を入力して2進数を感じてみてください！  
