ETロボコン EV3 HackEV
====
### 現在の状態
motion  
move  
自己位置推定  
  
距離停止  
角度停止  
方位停止  
  
各種control  
FileOutput  
Decoder  
HsvConverter  
自作util.cpp  
  
名前空間はie  
  
タスクはメイン1つと自己位置推定用の周期ハンドラ1つ  


### 更新履歴
座標指定移動  
方位停止  
角度停止  
距離停止  
Motionクラス作成  
ライントレースのエッジの左右を選択式に  
座標指定移動追加  
localizationクラス(自己位置推定)追加  
周期ハンドラを追加  
ports_assignment.hpp更新  
Moveクラスに左右ホイールの独立した操作とステアリング操作を追加  
Moveクラスと各controlクラスを追加  
util.cppをtemplateを使用した版に更新  
初期位置コードをデコードするDecoderクラス追加  
msg_fからVarEnteringクラスを使用できるように変更  
ファイル出力するFileOutputクラス追加  
自作クラスに名前空間ieを設定  
RGBをHSVに変換するHsvConverterクラス追加  
初期位置コードをデコードするDecoderクラス追加  
msg_fからVarEnteringクラスを使用できるように変更  
ボタン入力するVarEnteringクラス追加  
ports_assignment.hppを追加  
文字列の行間が詰まった。  
ハローワールドするだけ。

