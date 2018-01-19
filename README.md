ETロボコン2017 ie-ryukyu
====
## 概要
実験3で出場する[ETロボコン(2017)](http://www.etrobo.jp/2017/ "ETロボコン2017公式サイト")のソースコードです。  
このソースコードから生成したappを本番のEV3で動かします。Rコース、Lコースの動きを作成していきます。  
[RedmineのWiki](https://redmine.ie.u-ryukyu.ac.jp/projects/etrobo2017-teamtwd/wiki "Redmine")も参照。

## 電池について
新品ホヤホヤの電池(9.5vオーバー)、数回使用の電池(9v)、減ってきた電池(8.8~7.5v)で動きが変わります。  
9.5v~9vでくらいで動きます。

## Lコース

`./left_course/`に各区間の関数を記入。

- left_idaten.cpp - 韋駄天
 - ショートカットによる韋駄天を狙う
- left_idaten2.cpp - 韋駄天
 - ショートカットによる韋駄天を狙う(白エリアのみ)
- left_block.cpp - ブロック並べ
 - がん無視して駐車場へ向かう
- left_parking.cpp - 縦列駐車
 - 普通に駐車して尻尾をふる

## Rコース

`./right_course/`に各区間の関数を記入。

- right_idaten.cpp - 韋駄天
 - ショートカットによる韋駄天を狙う
- right_sumo.cpp - ET相撲Neo
 - 普通に相撲をする。懸賞はしない
- right_parking.cpp - 直角駐車
 - 普通に駐車する。最後はライントレースではなく直進。
- right_prize.cpp - 懸賞
 - 懸賞から駐車まで。*残念ながら見送り*

## よく使うクラス説明
詳細はソースコード内のコメントを読んでください。

### Motionクラス
__app/motion.hpp__ - ロボットに動作を指令する。  
Stopperインターフェースを使うことで停止指令を出す。Stopperインターフェースを使わない場合は自前で関数を呼び出すループを作る。  
Controlインターフェースを使うことで直進、回転、ライントレース、座標移動の制御を行う。  
座標移動はLocalizationクラス(自己位置推定)のインスタンスも必要。

#### 主な関数
全部ではありません。

|関数名           |引数                                                  |概要                          |
|-----------------|------------------------------------------------------|------------------------------|
|setLeftPwm       |Stopper, パワー                                       |左ホイールを回転              |
|setRightPwm      |Stopper, パワー                                       |右ホイールを回転              |
|setBothPwm       |Stopper, 左パワー, 右パワー                           |左右のホイールを回転          |
|setSteeringPowerv|Stopper, パワー, ターンレート                         |Sterring.setPower()の独自実装 |
|goStraight       |Stopper, Control, パワー                              |直進                          |
|spin             |Stopper, Control, パワー                              |回転                          |
|goPoint          |Localization, Control, パワー, X座標, Y座標, マージン |指定した座標に移動            |
|lineTrace        |Stopper, Control, パワー, 左右エッジ選択              |ライントレース                |
|rotateTail       |角度, パワー                                          |尻尾を回転                    |
|raiseArm         |角度, パワー                                          |アームを動かす(0~90度の絶対角度)  |
|stop             |                                                      |停止                          |
|wait             |時間(ms)                                              |指定の時間停止                |

### Stopperインターフェース
__stopper/stopper.hpp__ - 停止判定を行う。Motionクラスと組み合わせて使う。

#### 継承クラス

|クラス名         |概要                                         |
|-----------------|---------------------------------------------|
|MileageStopper   |指定の距離(mm)で停止                         |
|AngleStopper     |指定の角度(degree)で停止                     |
|DirectionStopper |指定の方位(radian)を向くと停止               |
|ColorStopper     |白黒以外の色を検知すると停止                 |
|GrayStopper      |カラーセンサーの値が指定の閾値より高いと停止 |
|LineStopper      |カラーセンサーの値が指定の閾値より低いと停止 |

##### 注意1 
MileageStopperとAngleStopperは値をセットした瞬間からの距離、角度を計算している。  
そのため、MileageStopperおよびAngleStopperはそれで停止させたい動作の直前に値をセットする必要がある。  
前回と同じ距離、角度で止まる場合でももう一度値をセットしないといけない。  

例:1000mm直進した後、1000mmライントレースする場合
```
Motion motion;
MileageStopper ms;
OnOffControl control(0, 0, 100);

// ・・・

ms.setTargetMileage(1000); // goStraightの直前に値をセット
motion.goStraight(ms, control, 30);

ms.setTargetMileage(1000); // 同じ1000mmだがもう一度値をセット
motion.lineTrace(ms, control, 30);
```

##### 注意2
MileageStopperとAngleStopperは負の値も指定できる。この値と進行方向(回転方向)の正負が逆だと無限ループして止まらないから気をつけること。

### Controlインターフェース
__control/control.hpp__ - 制御を行う。Motionクラスと組み合わせて使う。  
ライントレース以外の直進、回転、座標移動は目標値が自動でセットされるので自分で行わなくていい。

#### 継承クラス

|クラス名     |概要                                                        |
|-------------|------------------------------------------------------------|
|PIDControl   |PID制御 ライントレースの要                                  |
|OnOffControl |単純な0-100のon/offではなく、モーターのパワーに依存する制御 |
